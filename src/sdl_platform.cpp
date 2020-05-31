#include <assert.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "os_platform.h"
#include "sdl_platform.h"

// Platform Layer
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#ifndef SDL_JOYSTICK_AXIS_MAX
#define SDL_JOYSTICK_AXIS_MAX 32767
#endif

#define MAX_CONTROLLERS 4
#define CONTROLLER_AXIS_LEFT_DEADZONE 7849

global_var SDL_GameController *controller_handles[MAX_CONTROLLERS];
global_var SDL_Haptic *rumble_handles[MAX_CONTROLLERS];
global_var u64 g_perf_counter;
global_var bool g_running;

internal void SDLx_ProcessKeyboardEvent(GameButtonState *new_state, b32 is_down)
{
    if (new_state->ended_down != is_down) {
        new_state->ended_down = is_down;
        ++new_state->half_transition_count;
    }
}


internal void SDLx_ProcessEvents(SDLx_State &state, GameControllerInput *keyboard)
{
    SDL_Event event;
    for (;;) {
        int pending_events = 0;
        pending_events = SDL_PollEvent(&event);
        if (!pending_events) {
            break;
        }

        switch (event.type) {
            case SDL_QUIT:
                g_running = false;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_Keycode key_code = event.key.keysym.sym;
                bool was_down = (event.key.state == SDL_RELEASED);
                bool is_down = (event.key.state == SDL_PRESSED);
                if (event.key.repeat == 0) {
                if (key_code == SDLK_w) {
                    SDLx_ProcessKeyboardEvent(&keyboard->move_up, is_down);
                } else if (key_code == SDLK_a) {
                    SDLx_ProcessKeyboardEvent(&keyboard->move_left, is_down);
                } else if (key_code == SDLK_s) {
                    SDLx_ProcessKeyboardEvent(&keyboard->move_down, is_down);
                } else if (key_code == SDLK_d) {
                    SDLx_ProcessKeyboardEvent(&keyboard->move_right, is_down);
                } else if (key_code == SDLK_q) {
                    SDLx_ProcessKeyboardEvent(&keyboard->left_shoulder, is_down);
                } else if (key_code == SDLK_e) {
                    SDLx_ProcessKeyboardEvent(&keyboard->right_shoulder, is_down);
                } else if (key_code == SDLK_UP || key_code == SDLK_h) {
                    SDLx_ProcessKeyboardEvent(&keyboard->action_up, is_down);
                } else if (key_code == SDLK_LEFT) {
                    SDLx_ProcessKeyboardEvent(&keyboard->action_left, is_down);
                } else if (key_code == SDLK_DOWN) {
                    SDLx_ProcessKeyboardEvent(&keyboard->action_down, is_down);
                } else if (key_code == SDLK_RIGHT) {
                    SDLx_ProcessKeyboardEvent(&keyboard->action_right, is_down);
                }  else if (key_code == SDLK_ESCAPE) {
                    SDLx_ProcessKeyboardEvent(&keyboard->back, is_down);
                } else if (key_code == SDLK_RETURN) {
                    SDLx_ProcessKeyboardEvent(&keyboard->start, is_down);
                } else if ((key_code == SDLK_ESCAPE) ||
                            (key_code == SDLK_LSHIFT) || (key_code == SDLK_RSHIFT)) {
                    b32 either_down = (SDL_GetKeyboardState(0)[SDL_SCANCODE_SPACE] ||
                                      (SDL_GetModState() & KMOD_SHIFT));
                    keyboard->clutch_max = (either_down ? 1.0f : 0.0f);
                }
                }
                break;
            }

        }
    }
}


internal void SDLx_OpenGameControllers()
{
    int max_joysticks = SDL_NumJoysticks();
    int controller_index = 0;
    for (int joystick_index = 0; joystick_index < max_joysticks; ++joystick_index) {
        if (!SDL_IsGameController(joystick_index)) {
            continue;
        }

        if (controller_index >= MAX_CONTROLLERS) {
            break;
        }
        
        controller_handles[controller_index] = SDL_GameControllerOpen(joystick_index);

        SDL_Joystick *joystick_handle = SDL_GameControllerGetJoystick(controller_handles[controller_index]);
        rumble_handles[controller_index] = SDL_HapticOpenFromJoystick(joystick_handle);
        if (SDL_HapticRumbleInit(rumble_handles[controller_index]) != 0) {
            SDL_HapticClose(rumble_handles[controller_index]);
            rumble_handles[controller_index] = 0;
        }

        controller_index++;
    }
}

internal void SDLx_CloseGameControllers()
{
    for (int controller_index = 0; controller_index < MAX_CONTROLLERS; ++controller_index) {
        if (controller_handles[controller_index]) {
            if (rumble_handles[controller_index]) {
                SDL_HapticClose(rumble_handles[controller_index]);
            }

            SDL_GameControllerClose(controller_handles[controller_index]);
        }
    }
}

internal void initializeSystems(GameRoot &game_root, SDLx_State &state)
{
    state.total_size = GB(1) + MB(160);
    void *game_memory_block = mmap(0, state.total_size, 
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);    
    assert(game_memory_block != MAP_FAILED);
    
    state.game_memory_block = game_memory_block;

    //TODO: allocate these in the some partition instead of new

    auto game_partition = new LinearAllocator(MB(16), memory::add(game_memory_block, MB(16)));
    auto resource_partition = new StackAllocator(GB(1), game_memory_block);
    
    MemoryStorage memory_storage = {};
    memory_storage.resource_partition = resource_partition;
    memory_storage.game_partition = game_partition;
    game_root.memory_storage = memory_storage;
    game_root.renderer_api = RendererAPI::instance();
//  game_root.filesystem_api = FileSystemAPI::instance();
//  game_root.resource_manager = ResourceManager::instance();
     
}


inline time_t SDLx_GetLastWriteTime(char *filename)
{
    time_t last_write_time = 0;
    struct stat file_status;
    if (stat(filename, &file_status) == 0) {
        last_write_time = file_status.st_mtime;
    }

    return last_write_time;
}

void SDLx_LoadedCode::unloadCode()
{
    if (dll) {
        dlclose(dll);
        dll = 0;
    }

    is_valid = false;
    zeroArray(function_count, functions); 
}

b32 SDLx_LoadedCode::isCodeChanged()
{
    time_t new_dll_write_time = SDLx_GetLastWriteTime(dll_fullname_path);
    b32 result = new_dll_write_time != dll_last_write_time;

    return result;
}

void SDLx_LoadedCode::reloadCode()
{
    unloadCode();
    for (u32 i = 0; !is_valid && (i < 100); ++i) {
        loadCode();
        SDL_Delay(100);
    }
}

void SDLx_LoadedCode::loadCode()
{
    char *source_dll_name = dll_fullname_path;
    dll_last_write_time = SDLx_GetLastWriteTime(source_dll_name);
    if (dll_last_write_time) {
        if (!dll) {
            dll = dlopen(source_dll_name, RTLD_LAZY);
        }
        if (dll) {
            is_valid = true;
            for (u32 i = 0; i < function_count; ++i) {
                void *fn = dlsym(dll, function_names[i]);
                if (fn) {
                    functions[i] = fn;
                } else {
                    fprintf(stderr, "DLL Function Err: %s\n", function_names[i]);
                    fprintf(stderr, "DLL ALERT: %s\n", dlerror());
                    is_valid = false;
                }
            }
        } else {
            fprintf(stderr, "ERROR: Cannot load game code. %s\n", dlerror());
        }
    }

    if (!is_valid) {
        unloadCode();
    }
}


int main()
{
    SDLx_State state = {};
    g_perf_counter = SDL_GetPerformanceFrequency();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    
    state.setEXEPath();
    
    SDLx_OpenGameControllers();

    char src_game_dll_fullpath[SDL_PATH_MAX];
    state.buildEXEFileName("game.so", sizeof(src_game_dll_fullpath), src_game_dll_fullpath);
    SDL_Window *window = SDL_CreateWindow("stuff",                                           
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          960,
                                          540,
                                          SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIDDEN|SDL_WINDOW_OPENGL);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow error: %s", SDL_GetError());
    } 
    
    SDL_ShowWindow(window); 
    //SDL_GLContext context; 
    g_running = true;
    
    SDLx_GameFunctionTable game = {};
    SDLx_LoadedCode game_code = {};
    game_code.dll_fullname_path = src_game_dll_fullpath;
    game_code.function_count = ARRAY_LEN(SDLx_GameFunctionTableNames);
    game_code.function_names = SDLx_GameFunctionTableNames;
    game_code.functions = reinterpret_cast<void **>(&game);
    game_code.loadCode();
    
    i32 monitor_refresh_hz = 60;
    i32 display_index = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode = {};
    i32 display_mode_result = SDL_GetDesktopDisplayMode(display_index, &mode);
    if (display_mode_result == 0 && mode.refresh_rate > 1) {
        monitor_refresh_hz = mode.refresh_rate;
    }

    f32 game_update_hz = static_cast<f32>(monitor_refresh_hz);


    GameInput input[2] = {};
    auto new_input = &input[0];
    auto old_input = &input[1];
    
    
    GameRoot game_root = {};
    initializeSystems(game_root, state);
    game_root.renderer_api->init(window);
    
    
    
    f32 target_seconds_per_frame = 1 / game_update_hz;
    //*********** GAME LOOP *********************//
    while (g_running) {
        new_input->dt_for_frame = target_seconds_per_frame;
        GameControllerInput *old_keyboard_controller = getController(old_input, 0);
        GameControllerInput *new_keyboard_controller = getController(new_input, 0);        
        *new_keyboard_controller = {};
        new_keyboard_controller->is_connected = true;
        for (int button_index = 0; button_index < ARRAY_LEN(new_keyboard_controller->buttons); ++button_index) {
            new_keyboard_controller->buttons[button_index].ended_down =
                old_keyboard_controller->buttons[button_index].ended_down;
        }   

        new_keyboard_controller->clutch_max = old_keyboard_controller->clutch_max;

        SDLx_ProcessEvents(state, new_keyboard_controller);

        if (game.updateAndRenderer) {
            game.updateAndRenderer(new_input, game_root);
        }

        b32 should_be_reloaded = game_code.isCodeChanged();
        if (should_be_reloaded) {
            game_code.reloadCode();
        }

        swapInput(&new_input, &old_input);
        SDL_GL_SwapWindow(window);
    }
   
    state.freeMemoryBlock();
    OpenGL *context =  reinterpret_cast<OpenGL *>(game_root.renderer_api->getContext());
    SDL_GL_DeleteContext(context->gl_context);
    SDLx_CloseGameControllers();
    SDL_Quit();
    return 0;
}

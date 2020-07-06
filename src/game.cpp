#include "os_platform.h"
#include "game.h"

global_var v3 camera_pos;
global_var f32 camera_rot = 0.0f;
global_var int i;

#ifdef GAME_INTERNAL
DebugTable g_debug_table;
#endif

extern "C" UPDATE_AND_RENDER(updateAndRender) {
  MemoryStorage memory = game_root.memory_storage;

#ifdef GAME_INTERNAL
    g_debug_table = *game_root.debug_table;    
#endif

    if (!game_root.game_state) {
    game_root.game_state = alloc<GameState>(memory.game_partition);
  }

  GameState *game_state = game_root.game_state;
  if (!game_state->renderer && !game_state->running) {
    game_state->running = true;


    RendererCommands commands{game_root.renderer_api};
    Renderer2D renderer_2d;
    renderer_2d.init(game_root.renderer_api, memory);
    renderer_2d.commands = commands;

    game_state->renderer = alloc<Renderer>(memory.game_partition);
    game_state->renderer->commands = commands;
    game_state->renderer->renderer_2d = renderer_2d;
    
    game_state->material_texture = Texture::instance(game_root.renderer_api, memory);
    game_state->material_texture->create("./assets/container.png");
  }

  f32 camera_move_speed = 5.0f;
  for (int controller_index = 0; controller_index < ARRAY_LEN(input->controllers); ++controller_index) {
    GameControllerInput *controller_input = getController(input, controller_index);

    if (controller_input->move_up.ended_down) {
      camera_pos.y -= camera_move_speed * input->dt_for_frame;
    }

    if (controller_input->move_down.ended_down) {
      camera_pos.y += camera_move_speed * input->dt_for_frame;
    }

    if (controller_input->move_right.ended_down) {
      camera_pos.x -= camera_move_speed * input->dt_for_frame;
    }

    if (controller_input->move_left.ended_down) {
      camera_pos.x += camera_move_speed * input->dt_for_frame;
    }

    if (controller_input->action_up.ended_down) {
      camera_rot += 1.0f;
    }

    if (controller_input->action_down.ended_down) {
      camera_rot -= 1.0f;
    }
  }

  Renderer *renderer = game_state->renderer;
  Renderer2D renderer_2d = renderer->renderer_2d;


  BEGIN_PROFILE("Renderer clear"); 
  renderer->commands.clear({0.1f, 0.1f, 0.1f});
  END_PROFILE();
  
  game_state->camera.setPosition(camera_pos);
  game_state->camera.setRotation(camera_rot);


  BEGIN_PROFILE("Renderer draw");
  renderer_2d.beginScene(game_state->camera);
  renderer_2d.drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
  renderer_2d.drawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
  renderer_2d.drawQuad({0.0f, 0.0f, 0.1f}, {0.5f, 0.5f}, game_state->material_texture);
  renderer_2d.endScene();
  END_PROFILE();

#ifdef GAME_INTERNAL
   DEBUG_PlainConsolePrint(g_debug_table);
#endif

  return 0;
}

#include "os_platform.h"
#include "game.h"


#ifdef FIREWOOD_INTERNAL
DebugTable g_debug_table;
#endif

extern "C" UPDATE_AND_RENDER(updateAndRender) {

#ifdef FIREWOOD_INTERNAL
  g_debug_table = *game_root.debug_table;
#endif
    
  MemoryStorage memory = game_root.memory_storage;


  if (!game_root.game_state) {
    game_root.game_state = alloc<GameState>(memory.game_partition);
  }

  GameState *game_state = game_root.game_state;
  if (!game_state->renderer && !game_state->running) {
    game_state->running = true;

    game_state->camera_controller = CameraController(960.0f / 540.0f);
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

  for (int controller_index = 0; controller_index < ARRAY_LEN(input->controllers); ++controller_index) {
    GameControllerInput *controller_input = getController(input, controller_index);
    game_state->camera_controller.update(controller_input, input->dt_for_frame);
  }

  Renderer *renderer = game_state->renderer;
  Renderer2D renderer_2d = renderer->renderer_2d;


  BEGIN_PROFILE("Renderer clear");
  renderer->commands.clear({0.1f, 0.1f, 0.1f});
  END_PROFILE();


  BEGIN_PROFILE("Renderer draw");
  renderer_2d.beginScene(game_state->camera_controller.camera);
  renderer_2d.drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, 45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
  renderer_2d.drawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, 0.0f, {0.2f, 0.3f, 0.8f, 1.0f});
  renderer_2d.drawQuad({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, 0.0f, game_state->material_texture);
  renderer_2d.endScene();
  END_PROFILE();

  MEMORY_USAGE(memory);

#ifdef FIREWOOD_INTERNAL
  DEBUG_PlainConsolePrint(g_debug_table);
#endif

  return 0;
}

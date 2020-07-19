#include "game_events.cpp"
#include "camera.cpp"
#include "renderer.cpp"
#include "renderer2D.cpp"

struct GameState {
  Renderer *renderer;
  Texture *material_texture;
  b32 running;
  CameraController camera_controller;
};

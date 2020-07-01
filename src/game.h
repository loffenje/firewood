#include "game_events.cpp"
#include "camera.cpp"
#include "renderer.cpp"
#include "renderer2D.cpp"

struct GameState {
  Renderer *renderer;
  b32 running;
  Camera camera{-1.6f, 1.6f, -0.9f, 0.9f};
};

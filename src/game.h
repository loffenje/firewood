#include "game_events.cpp"
#include "renderer.cpp"
#include "camera.cpp"

struct GameState
{
    RendererData *renderer_data;
    Camera camera{-1.6f, 1.6f, -0.9f, 0.9f};
};

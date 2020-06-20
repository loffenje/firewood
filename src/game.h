#include "game_events.cpp"
#include "camera.cpp"
#include "renderer.cpp"

struct GameState
{
    std::vector<RendererData *> renderer_data;
    Renderer *renderer;
    Camera camera{-1.6f, 1.6f, -0.9f, 0.9f};
};

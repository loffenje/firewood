#include "game_events.cpp"
#include "camera.cpp"
#include "renderer.cpp"
#include "renderer2D.cpp"
#include "tile_map.cpp"


struct World {
  TileMap *tile_map;
};

struct GameState {
  Renderer *renderer;
  Texture *material_texture;
  b32 running;
  CameraController camera_controller;
};

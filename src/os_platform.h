#ifndef OS_PLATFORM_H
#define OS_PLATFORM_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef int32_t b32;

#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std::chrono;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <sys/mman.h>

#define internal static
#define global_var static
#define local_var static

#define GAME_INTERNAL 1

#define STB_IMAGE_IMPLEMENTATION

#include "libs/stb_image.h"
#include "math.h"
#include "utils.h"
#include "input.h"
#include "event.h"
#include "game_memory.cpp"
#include "debug_service.h"

enum class RendererType { OpenGL_API };

struct MemoryStorage {
    StackAllocator *resource_partition;
    LinearAllocator *game_partition;
};

#define GRAPHICS_PLATFORM_API OPEN_GL

#include "renderer_types.h"

global_var RendererType renderer_type = RendererType::OpenGL_API;

struct GameState;

struct GameRoot {
    MemoryStorage memory_storage;
    RendererAPI *renderer_api;
    GameState *game_state;
    DebugTable *debug_table;
};

#include "opengl_platform.cpp"

struct RendererCommands;
#define UPDATE_AND_RENDER(name) int name(GameInput *input, GameRoot &game_root)

typedef UPDATE_AND_RENDER(update_and_render);

struct SDLx_GameFunctionTable {
    update_and_render *updateAndRenderer;
};

global_var const char *SDLx_GameFunctionTableNames[] = {"updateAndRender"};

#endif

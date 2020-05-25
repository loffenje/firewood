#ifndef OS_PLATFORM_H
#define OS_PLATFORM_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef int32_t b32;

#define internal static
#define global_var static
#define local_var static

#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL_opengl.h>

#include "math.h"
#include "utils.h"
#include "input.h"

enum class RendererAPI 
{
    OpenGL_API
};

global_var RendererAPI renderer_api = RendererAPI::OpenGL_API;

//TODO: split renderer api
#include "renderer_types.h"
#include "opengl_platform.cpp"

struct RendererCommands;
#define UPDATE_AND_RENDER(name) int name(GameInput *input)
typedef UPDATE_AND_RENDER(update_and_render);

struct SDLx_GameFunctionTable {
    update_and_render *updateAndRenderer;
};

global_var const char *SDLx_GameFunctionTableNames[] = {
    "updateAndRender"
};

#endif

#include "renderer.h"

internal void beginScene()
{

}

internal void endScene()
{

}

internal void submit(RendererCommands &commands, const std::shared_ptr<VertexArray > &vertex_array)
{
    vertex_array->bind();
    commands.drawIndexed(vertex_array);
}



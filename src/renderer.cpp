#include "renderer.h"

inline void RendererCommands::clear(v3 color) { renderer_api->clear(color); }

inline void RendererCommands::drawIndexed(VertexArray *vertex_array) {
    renderer_api->drawIndexed(vertex_array);
}

void Renderer::beginScene(Camera &camera) {
  scene.view_projection_mat = camera.view_projection_mat;
}

void Renderer::endScene() {}

void Renderer::submit(VertexArray *vertex_array, Shader *shader,
                      const Mat4x4 &model) {
  shader->bind();
  shader->uploadUniformMat4("u_ViewProjection", scene.view_projection_mat);
  shader->uploadUniformMat4("u_Model", model);

  vertex_array->bind();
  commands.drawIndexed(vertex_array);
}


#include "renderer.h"


void Renderer::beginScene(Camera &camera)
{
    scene.view_projection_mat = camera.view_projection_mat;
}

void Renderer::endScene()
{

}

void Renderer::submit(const std::shared_ptr<VertexArray > &vertex_array, const std::shared_ptr<Shader> &shader, const Mat4x4 &model)
{
    shader->bind();
    shader->uploadUniformMat4("u_ViewProjection", scene.view_projection_mat);
    shader->uploadUniformMat4("u_Model", model);

    vertex_array->bind();
    commands.drawIndexed(vertex_array);
}



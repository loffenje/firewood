
struct RendererCommands
{
    RendererAPI *renderer_api;

    void clear(v3 color); 
    void drawIndexed(const std::shared_ptr<VertexArray> &vertex_array);
};

inline void RendererCommands::clear(v3 color)
{
    renderer_api->clear(color);
}

inline void RendererCommands::drawIndexed(const std::shared_ptr<VertexArray> &vertex_array)
{
    renderer_api->drawIndexed(vertex_array);
}

struct Scene
{
    Mat4x4 view_projection_mat;
};

struct Renderer
{
    void beginScene(Camera &camera);
    void endScene();
    void submit(const std::shared_ptr<VertexArray > &vertex_array, const std::shared_ptr<Shader> &shader, const Mat4x4 &model);
    RendererCommands commands;
    Scene scene;
};

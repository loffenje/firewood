
struct RendererCommands
{
    RendererAPI *renderer_api;
    
    inline void clear(v3 color); 
    inline void drawIndexed(const std::shared_ptr<VertexArray> &vertex_array);
};

inline void RendererCommands::clear(v3 color)
{
    renderer_api->clear(color);
}

inline void RendererCommands::drawIndexed(const std::shared_ptr<VertexArray> &vertex_array)
{
    renderer_api->drawIndexed(vertex_array);
}



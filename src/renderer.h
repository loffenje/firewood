
struct RendererCommands {
  RendererAPI *renderer_api;

  void clear(v3 color);
  void drawIndexed(VertexArray *vertex_array);
};

struct Renderer2D_Data {
  VertexArray *quad_va;
  Shader *color_shader;
  Shader *texture_shader;
};

struct Renderer2D {
  void init(RendererAPI *renderer_api, const MemoryStorage &memory);
  void destroy(const MemoryStorage &memory);
  void beginScene(Camera &camera);
  void endScene();
  void drawQuad(const v2 &pos, const v2 &size, const v4 &color);
  void drawQuad(const v3 &pos, const v2 &size, const v4 &color);
  void drawQuad(const v2 &pos, const v2 &size, Texture *texture);
  void drawQuad(const v3 &pos, const v2 &size, Texture *Texture);
  
  RendererCommands commands;
  Renderer2D_Data *data;
};

struct Scene {
  Mat4x4 view_projection_mat;
};

struct Renderer {
  void beginScene(Camera &camera);
  void endScene();
  void submit(VertexArray *vertex_array, Shader *shader, const Mat4x4 &model);
  RendererCommands commands;
  Renderer2D renderer_2d;
  Scene scene;
};

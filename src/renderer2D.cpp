
namespace {
constexpr u32 max_quads = 2000;
constexpr u32 max_vertices = max_quads * 4;
constexpr u32 max_indices = max_quads * 6;
}; // namespace

void Renderer2D::init(RendererAPI *renderer_api, const MemoryStorage &memory) {
  data.quad_va = VertexArray::instance(renderer_api, memory);
  data.quad_va->create();


  data.quad_vbo = VertexBuffer::instance(renderer_api, memory);
  data.quad_vbo->create(max_vertices * sizeof(QuadVertex));

  std::vector<Element> layout = {{Float3, "a_Position"}, {Float4, "a_Color"}, {Float2, "a_TexCoord"}};

  data.quad_vbo->setLayout(layout);
  data.quad_va->addBuffer(data.quad_vbo);

  data.quad_buffer_base = alloc_array<QuadVertex, max_vertices>(memory.resource_partition);

  u32 *quad_indices = alloc_array<u32, max_indices>(memory.resource_partition);

  u32 offset = 0;
  for (u32 i = 0; i < max_indices; i += 6) {
    quad_indices[i + 0] = offset + 0;
    quad_indices[i + 1] = offset + 1;
    quad_indices[i + 2] = offset + 2;

    quad_indices[i + 3] = offset + 2;
    quad_indices[i + 4] = offset + 3;
    quad_indices[i + 5] = offset + 0;

    offset += 4;
  }

  IndexBuffer *quad_ibo = IndexBuffer::instance(renderer_api, memory);
  quad_ibo->create(quad_indices, max_indices);
  data.quad_va->setIndexBuffer(quad_ibo);
  dealloc_array<u32>(memory.resource_partition, quad_indices);

  const char *color_vertex = "#version 410 core\n"
                             "layout(location = 0) in vec3 a_Position;\n"
                             "uniform mat4 u_ViewProjection;\n"
                             "uniform mat4 u_Model;\n"
                             "void main()\n"
                             "{\n"
                             "gl_Position =  u_ViewProjection * u_Model * vec4(a_Position, 1.0);\n"
                             "}\n\0";

  const char *color_fragment = "#version 410 core\n"
                               "layout (location = 0) out vec4 color;\n"
                               "uniform vec4 u_Color;\n"
                               "void main()\n"
                               "{\n"
                               "color = u_Color;\n"
                               "}\n\0";
  Shader *color_shader = Shader::instance(renderer_api, memory);
  color_shader->createProgram(color_vertex, color_fragment);

  data.color_shader = color_shader;

  const char *texture_vertex = "#version 410 core\n"
                               "layout (location = 0) in vec3 a_Position;\n"
                               "layout (location = 1) in vec4 a_Color;\n"
                               "layout (location = 2) in vec2 a_TexCoord;\n"
                               "uniform mat4 u_ViewProjection;\n"
                               "out vec4 v_Color;\n"
                               "out vec2 v_TexCoord;\n"
                               "void main()\n"
                               "{\n"
                               "v_Color = a_Color;\n"
                               "v_TexCoord = a_TexCoord;\n"
                               "gl_Position = u_ViewProjection * vec4(a_Position, 1.0);\n"
                               "}\n\0";

  const char *texture_fragment = "#version 410 core\n"
                                 "layout (location = 0) out vec4 color;\n"
                                 "in vec4 v_Color;\n"
                                 "in vec2 v_TexCoord;\n"
                                 "uniform sampler2D u_Texture;\n"
                                 "void main()\n"
                                 "{\n"
                                 "color = v_Color;\n"
                                 "}\n\0";

  Shader *texture_shader = Shader::instance(renderer_api, memory);
  texture_shader->createProgram(texture_vertex, texture_fragment);

  data.texture_shader = texture_shader;
  data.texture_shader->bind();
  data.texture_shader->uploadUniformInt("u_Texture", 0);
}

void Renderer2D::beginScene(Camera &camera) {
  TIMED_BLOCK("Renderer2D::beginScene");

  data.color_shader->bind();
  data.color_shader->uploadUniformMat4("u_ViewProjection", camera.view_projection_mat);

  data.texture_shader->bind();
  data.texture_shader->uploadUniformMat4("u_ViewProjection", camera.view_projection_mat);

  data.quad_index_count = 0;
  data.quad_buffer_ptr = data.quad_buffer_base;
}

void Renderer2D::endScene() {
  TIMED_BLOCK("Renderer2D::endScene");

  u32 data_size = reinterpret_cast<u8 *>(data.quad_buffer_ptr) - reinterpret_cast<u8 *>(data.quad_buffer_base);
  data.quad_vbo->setData(data.quad_buffer_base, data_size);

  commands.drawIndexed(data.quad_va, data.quad_index_count);
}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, f32 angle, const v4 &color) {
  drawQuad({pos.x, pos.y, 0.0f}, size, angle, color);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, f32 angle, const v4 &color) {
  data.quad_buffer_ptr->position = pos;
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x + size.x, pos.y, 0.0f};
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x + size.x, pos.y + size.y, 0.0f};
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x, pos.y + size.y, 0.0f};
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr++;

  data.quad_index_count += 6;
}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, f32 angle, Texture *texture) {
  drawQuad({pos.x, pos.y, 0.0f}, size, angle, texture);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, f32 angle, Texture *texture) {

  data.quad_buffer_ptr->position = pos;
  data.quad_buffer_ptr->tex_coord = {0.0f, 0.0f};
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x + size.x, pos.y, 0.0f};
  data.quad_buffer_ptr->tex_coord = {1.0f, 0.0f};
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x + size.x, pos.y + size.y, 0.0f};
  data.quad_buffer_ptr->tex_coord = {1.0f, 1.0f};
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = {pos.x, pos.y + size.y, 0.0f};
  data.quad_buffer_ptr->tex_coord = {0.0f, 1.0f};
  data.quad_buffer_ptr++;

  data.quad_index_count += 6;
}

void Renderer2D::destroy(const MemoryStorage &memory) {
  dealloc<VertexArray>(memory.resource_partition, data.quad_va);
  dealloc<Shader>(memory.resource_partition, data.color_shader);
}

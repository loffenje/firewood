
void Renderer2D::init(RendererAPI *renderer_api, const MemoryStorage &memory) {
  data = alloc<Renderer2D_Data>(memory.resource_partition);
  data->quad_va = VertexArray::instance(renderer_api, memory);
  data->quad_va->create();

  f32 square_vertices[5 * 4] = {
      -0.5f, -0.5f, 0.0f, 0.5f,
      -0.5f, 0.0f, 0.5f, 0.5f,
      0.0f, -0.5f, 0.5f, 0.0f
  };

  VertexBuffer *square_vbo = VertexBuffer::instance(renderer_api, memory);
  square_vbo->create(square_vertices, sizeof(square_vertices));

  std::vector<Element> layout = {{Float3, "a_Position"}};

  square_vbo->setLayout(layout);
  data->quad_va->addBuffer(square_vbo);

  u32 square_indices[6] = {0, 1, 2, 2, 3, 0};
  IndexBuffer *square_ibo = IndexBuffer::instance(renderer_api, memory);
  square_ibo->create(square_indices, ARRAY_LEN(square_indices));
  data->quad_va->setIndexBuffer(square_ibo);

  const char *vertex = "#version 410 core\n"
                       "layout(location = 0) in vec3 a_Position;\n"
                       "uniform mat4 u_ViewProjection;\n"
                       "uniform mat4 u_Model;\n"
                       "void main()\n"
                       "{\n"
                       "gl_Position =  u_ViewProjection * u_Model * vec4(a_Position, 1.0);\n"
                       "}\n\0";

  const char *fragment = "#version 410 core\n"
                         "layout (location = 0) out vec4 color;\n"
                         "uniform vec4 u_Color;\n"
                         "void main()\n"
                         "{\n"
                         "color = u_Color;\n"
                         "}\n\0";

  Shader *square_shader = Shader::instance(renderer_api, memory);
  square_shader->createProgram(vertex, fragment);

  data->color_shader = square_shader;
}

void Renderer2D::beginScene(Camera &camera) {
  data->color_shader->bind();
  data->color_shader->uploadUniformMat4("u_ViewProjection", camera.view_projection_mat);
  data->color_shader->uploadUniformMat4("u_Model", Mat4x4());
}

void Renderer2D::endScene() {}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, const v4 &color) {
  drawQuad({pos.x, pos.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, const v4 &color) {
  data->color_shader->bind();
  data->color_shader->uploadUniformFloat4("u_Color", color);

  data->quad_va->bind();
  commands.drawIndexed(data->quad_va);
}

void Renderer2D::destroy(const MemoryStorage &memory) {
  dealloc<VertexArray>(memory.resource_partition, data->quad_va);
  dealloc<Shader>(memory.resource_partition, data->color_shader);
  dealloc<Renderer2D_Data>(memory.resource_partition, data);
}

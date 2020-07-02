
void Renderer2D::init(RendererAPI *renderer_api, const MemoryStorage &memory) {
  data = alloc<Renderer2D_Data>(memory.resource_partition);
  data->quad_va = VertexArray::instance(renderer_api, memory);
  data->quad_va->create();

  f32 square_vertices[5 * 4] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
  };


  VertexBuffer *square_vbo = VertexBuffer::instance(renderer_api, memory);
  square_vbo->create(square_vertices, sizeof(square_vertices));

  std::vector<Element> layout = {
      {Float3, "a_Position"},
      {Float2, "a_TexCoord"} 
  };

  square_vbo->setLayout(layout);
  data->quad_va->addBuffer(square_vbo);

  u32 square_indices[6] = {0, 1, 2, 2, 3, 0};
  IndexBuffer *square_ibo = IndexBuffer::instance(renderer_api, memory);
  square_ibo->create(square_indices, ARRAY_LEN(square_indices));
  data->quad_va->setIndexBuffer(square_ibo);

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

  data->color_shader = color_shader;
  
  const char *texture_vertex = "#version 410 core\n"
      "layout (location = 0) in vec3 a_Position;\n"
      "layout (location = 1) in vec2 a_TexCoord;\n"
      "uniform mat4 u_ViewProjection;\n"
      "uniform mat4 u_Model;\n"
      "out vec2 v_TexCoord;\n"
      "void main()\n"
      "{\n"
       "v_TexCoord = a_TexCoord;\n"
       "gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);\n"
      "}\n\0";

  const char *texture_fragment = "#version 410 core\n"
      "layout (location = 0) out vec4 color;\n"
      "in vec2 v_TexCoord;\n"
      "uniform sampler2D u_Texture;\n"
      "void main()\n"
      "{\n"
      "color = texture(u_Texture, v_TexCoord);\n"
      "}\n\0";
    
  Shader *texture_shader = Shader::instance(renderer_api, memory);
  texture_shader->createProgram(texture_vertex, texture_fragment);

  data->texture_shader = texture_shader;
  data->texture_shader->bind();
  data->texture_shader->uploadUniformInt("u_Texture", 0);
}

void Renderer2D::beginScene(Camera &camera) {
  data->color_shader->bind();
  data->color_shader->uploadUniformMat4("u_ViewProjection", camera.view_projection_mat);
  
  data->texture_shader->bind();
  data->texture_shader->uploadUniformMat4("u_ViewProjection", camera.view_projection_mat);
}

void Renderer2D::endScene() {}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, const v4 &color) {
  drawQuad({pos.x, pos.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, const v4 &color) {
  data->color_shader->bind();
  data->color_shader->uploadUniformFloat4("u_Color", color);

  Transform model = translate(pos) * scale(size.x, size.y, 1.0f);
  data->color_shader->uploadUniformMat4("u_Model", model.matrix);

  data->quad_va->bind();
  commands.drawIndexed(data->quad_va);
}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, Texture *texture) {
  drawQuad({pos.x, pos.y, 0.0f}, size, texture);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, Texture *texture) {
  data->texture_shader->bind();

  Transform model = translate(pos) * scale(size.x, size.y, 1.0f);
  data->texture_shader->uploadUniformMat4("u_Model", model.matrix);

  texture->bind();
  
  data->quad_va->bind();
  commands.drawIndexed(data->quad_va);
}

void Renderer2D::destroy(const MemoryStorage &memory) {
  dealloc<VertexArray>(memory.resource_partition, data->quad_va);
  dealloc<Shader>(memory.resource_partition, data->color_shader);
  dealloc<Renderer2D_Data>(memory.resource_partition, data);
}

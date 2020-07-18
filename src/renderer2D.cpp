
void Renderer2D::init(RendererAPI *renderer_api, const MemoryStorage &memory) {
  data.quad_va = VertexArray::instance(renderer_api, memory);
  data.quad_va->create();


  data.quad_vbo = VertexBuffer::instance(renderer_api, memory);
  data.quad_vbo->create(max_vertices * sizeof(QuadVertex));

  std::vector<Element> layout = {{Float3, "a_Position"}, 
                                {Float4, "a_Color"},
                                {Float2, "a_TexCoord"},
                                {Float, "a_TexIndex"}};

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

  const char *texture_vertex = "#version 410 core\n"
                               "layout (location = 0) in vec3 a_Position;\n"
                               "layout (location = 1) in vec4 a_Color;\n"
                               "layout (location = 2) in vec2 a_TexCoord;\n"
                               "layout (location = 3) in float a_TexIndex;\n"
                               "uniform mat4 u_ViewProjection;\n"
                               "out vec4 v_Color;\n"
                               "out vec2 v_TexCoord;\n"
                               "out float v_TexIndex;\n"
                               "void main()\n"
                               "{\n"
                               "v_Color = a_Color;\n"
                               "v_TexCoord = a_TexCoord;\n"
                               "v_TexIndex = a_TexIndex;\n"
                               "gl_Position = u_ViewProjection * vec4(a_Position, 1.0);\n"
                               "}\n\0";

  const char *texture_fragment = "#version 410 core\n"
                                 "layout (location = 0) out vec4 color;\n"
                                 "in vec4 v_Color;\n"
                                 "in vec2 v_TexCoord;\n"
                                 "in float v_TexIndex;\n"
                                 "uniform sampler2D u_Textures[16];\n"
                                 "void main()\n"
                                 "{\n"
                                 "color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;\n"
                                 "}\n\0";

  data.white_texture = Texture::instance(renderer_api, memory);
  data.white_texture->create(1,1);

  u32 white_texture_data = 0xFFFFFFFF;
  data.white_texture->setData(&white_texture_data, sizeof(u32));

  i32 samplers[max_texture_slots];
  for (u32 i = 0; i < max_texture_slots; i++) {
    samplers[i] = i;
  }
  
  data.texture_shader = Shader::instance(renderer_api, memory);
  data.texture_shader->createProgram(texture_vertex, texture_fragment);
  data.texture_shader->bind();
  data.texture_shader->uploadArrayi("u_Textures", samplers, max_texture_slots);

  data.texture_slots[0] = data.white_texture;
  
  data.quad_vertices[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  data.quad_vertices[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  data.quad_vertices[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  data.quad_vertices[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::beginScene(Camera &camera) {
  TIMED_BLOCK("Renderer2D::beginScene");

  data.texture_shader->bind();
  data.texture_shader->uploadMat4("u_ViewProjection", camera.view_projection_mat);

  data.quad_index_count = 0;
  
  data.quad_buffer_ptr = data.quad_buffer_base;
  data.texture_slot_index = 1;
}

void Renderer2D::endScene() {
  TIMED_BLOCK("Renderer2D::endScene");

  u32 data_size = reinterpret_cast<u8 *>(data.quad_buffer_ptr) - reinterpret_cast<u8 *>(data.quad_buffer_base);
  data.quad_vbo->setData(data.quad_buffer_base, data_size);

  for (u32 i = 0; i < data.texture_slot_index; i++) {
    data.texture_slots[i]->bind(i);
  }

  commands.drawIndexed(data.quad_va, data.quad_index_count);
}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, f32 angle, const v4 &color) {
  drawQuad({pos.x, pos.y, 0.0f}, size, angle, color);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, f32 angle, const v4 &color) {
 
  f32 tex_index = 0.0f; //white texture
 
  Mat4x4 tran = identity(); 
  if (angle != 0.0f) {
    tran = translate(pos) * rotZ(angle) * scale({size.x, size.y, 1.0f});
  } else {
    tran = translate(pos)  * scale({size.x, size.y, 1.0f});
  }

  data.quad_buffer_ptr->position = tran * data.quad_vertices[0];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {0.0f, 0.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[1];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {1.0f, 0.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[2];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {1.0f, 1.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[3];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {0.0f, 1.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_index_count += 6;
}

void Renderer2D::drawQuad(const v2 &pos, const v2 &size, f32 angle, Texture *texture) {
  drawQuad({pos.x, pos.y, 0.0f}, size, angle, texture);
}

void Renderer2D::drawQuad(const v3 &pos, const v2 &size, f32 angle, Texture *texture) {
  v4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  f32 tex_index = 0.0f;
  for (u32 i = 1; i < data.texture_slot_index; i++) {
    if (*(data.texture_slots[i]) == *texture) {
        tex_index = static_cast<f32>(i);
        break;
    }
  }

  if (tex_index == 0.0f) {
    tex_index = static_cast<f32>(data.texture_slot_index);
    data.texture_slots[data.texture_slot_index] = texture;
    data.texture_slot_index++;
  }
   
  Mat4x4 tran = identity(); 
  tran = translate(pos)  * scale(v3{size.x, size.y, 1.0f});
  
  data.quad_buffer_ptr->position = tran * data.quad_vertices[0];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {0.0f, 0.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[1];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {1.0f, 0.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[2];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {1.0f, 1.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_buffer_ptr->position = tran * data.quad_vertices[3];
  data.quad_buffer_ptr->color = color;
  data.quad_buffer_ptr->tex_coord = {0.0f, 1.0f};
  data.quad_buffer_ptr->tex_index = tex_index;
  data.quad_buffer_ptr++;

  data.quad_index_count += 6;
}

//TODO: Determine when it needs to be called
void Renderer2D::destroy(const MemoryStorage &memory) {
  dealloc<VertexArray>(memory.resource_partition, data.quad_va);
  dealloc<Shader>(memory.resource_partition, data.texture_shader);
}

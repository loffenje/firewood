#include "renderer_types.h"
#include "opengl_def.h"

internal GLenum mapElemToShaderType(ShaderDataType type)
{
    switch (type) {
        case Float:
        case Float2:
        case Float3:
        case Float4:
        case Mat3:
        case Mat4:
            return GL_FLOAT;
        case Int:
        case Int2:
        case Int3:
        case Int4:
            return GL_INT;
        case Bool:
            return GL_BOOL;
    }

    return None;
}

struct OpenGLShader : public Shader
{
    OpenGLShader() {
        open_gl = reinterpret_cast<OpenGL *>(RendererAPI::instance()->getContext());
    }

    ~OpenGLShader() {}

    OpenGLShader(OpenGL *open_gl): open_gl{open_gl} {}
      
    OpenGL *open_gl;
    GLint program_id;

    void createProgram(const char *vertex_shader_src, const char *fragment_shader_src) override;
    void bind() override;
};

void OpenGLShader::bind()
{
    open_gl->glUseProgram(program_id);
}

void OpenGLShader::createProgram(const char *vertex_shader_src, const char *fragment_shader_src)
{
    int vertex_id = open_gl->glCreateShader(GL_VERTEX_SHADER);
    open_gl->glShaderSource(vertex_id, 1, &vertex_shader_src, NULL);
    open_gl->glCompileShader(vertex_id);

    int fragment_id = open_gl->glCreateShader(GL_FRAGMENT_SHADER);
    open_gl->glShaderSource(fragment_id, 1, &fragment_shader_src, NULL);
    open_gl->glCompileShader(fragment_id);

    int shader_program = open_gl->glCreateProgram();
    open_gl->glAttachShader(shader_program, vertex_id);
    open_gl->glAttachShader(shader_program, fragment_id);
    open_gl->glLinkProgram(shader_program);
    open_gl->glUseProgram(shader_program);
    open_gl->glValidateProgram(shader_program);
    GLint linked = false;
    open_gl->glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
    if (!linked) {
        fprintf(stderr,"Not linked\n");
        GLsizei ignored;
        char vertex_errors[4096];
        char fragment_errors[4096];
        char program_errors[4096];
        open_gl->glGetShaderInfoLog(vertex_id, sizeof(vertex_errors), &ignored, vertex_errors);
        open_gl->glGetShaderInfoLog(fragment_id, sizeof(fragment_errors), &ignored, fragment_errors);
        open_gl->glGetProgramInfoLog(shader_program, sizeof(program_errors), &ignored, program_errors);
        
        fprintf(stderr, "shader::error::%s\n",vertex_errors);
        fprintf(stderr, "shader::error::%s\n",fragment_errors);
        fprintf(stderr, "shader::error::%s\n",program_errors); 
    }

    open_gl->glDeleteShader(vertex_id);
    open_gl->glDeleteShader(fragment_id);
    
    program_id = shader_program;
}

struct OpenGLVertexBuffer : public VertexBuffer
{
    OpenGLVertexBuffer() {
        open_gl = reinterpret_cast<OpenGL *>(RendererAPI::instance()->getContext());
    }

    ~OpenGLVertexBuffer() {}

    OpenGLVertexBuffer(OpenGL *open_gl): open_gl{open_gl} {}

    unsigned int vbo;
    OpenGL *open_gl;
    std::vector<Element> elements;

    u32 getStride() override;
    void create(u32 size) override; 
    void create(f32 *vertices, u32 size) override;
    inline void bind() override;
    inline void unbind() override;
    void setData(const void *data, u32 size) override;
    void setLayout(const std::vector<Element> &elements) override;
};

void OpenGLVertexBuffer::setData(const void *data, u32 size)
{
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    open_gl->glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void OpenGLVertexBuffer::setLayout(const std::vector<Element> &elements)
{
    this->elements = elements;
}

u32 OpenGLVertexBuffer::getStride()
{
    size_t offset = 0;
    u32 stride = 0;
    for (auto &elem: elements) {
        elem.offset = offset;
        offset += elem.size;
        stride += elem.size;
    }

    return stride;
}

void OpenGLVertexBuffer::create(u32 size)
{
    open_gl->glGenBuffers(1, &vbo);
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    open_gl->glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void OpenGLVertexBuffer::create(f32 *vertices, u32 size)
{
    open_gl->glGenBuffers(1, &vbo);
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    open_gl->glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

inline void OpenGLVertexBuffer::bind()
{
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

inline void OpenGLVertexBuffer::unbind()
{
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

struct OpenGLIndexBuffer : public IndexBuffer
{
    OpenGLIndexBuffer() {
        open_gl = reinterpret_cast<OpenGL *>(RendererAPI::instance()->getContext());
    }

    ~OpenGLIndexBuffer() {}

    OpenGLIndexBuffer(OpenGL *open_gl): open_gl{open_gl} {}
    
    unsigned int ibo;
    u32 count;
    OpenGL *open_gl;
    void create(u32 *indices, u32 size) override;
    inline void bind() override;
    inline void unbind() override;
};

void OpenGLIndexBuffer::create(u32 *indices, u32 size)
{
    count = size;
    open_gl->glGenBuffers(1, &ibo);
    open_gl->glBindBuffer(GL_ARRAY_BUFFER, ibo);
    open_gl->glBufferData(GL_ARRAY_BUFFER, size * sizeof(u32), indices, GL_STATIC_DRAW);
}

inline void OpenGLIndexBuffer::bind()
{
    open_gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

inline void OpenGLIndexBuffer::unbind()
{
    open_gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

struct OpenGLVertexArray : public VertexArray
{
    OpenGLVertexArray() {
        open_gl = reinterpret_cast<OpenGL *>(RendererAPI::instance()->getContext());
    }

    OpenGLVertexArray(OpenGL *open_gl): open_gl{open_gl} {}
    
    OpenGL *open_gl;
    u32 vertex_buffer_index;
    u32 vao;
    int component_count;
    std::shared_ptr<IndexBuffer> index_buffer;
    std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;

    void create() override;
    void setIndexBuffer(std::shared_ptr<IndexBuffer> &buffer) override;
    void addBuffer(std::shared_ptr<VertexBuffer> &buffer) override;

    inline void bind() override;
    inline void unbind() override;
};

void OpenGLVertexArray::create()
{
    open_gl->glGenVertexArrays(1, &vao);
}

void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer> &buffer)
{
    open_gl->glBindVertexArray(vao);
    buffer->bind();

    index_buffer = buffer;
}

void OpenGLVertexArray::addBuffer(std::shared_ptr<VertexBuffer> &buffer)
{
    open_gl->glBindVertexArray(vao);
    buffer->bind();
    
    std::vector<Element> elements;
    elements.assign(buffer->elements.begin(), buffer->elements.end());

    for (const auto &elem : elements) {
        open_gl->glEnableVertexAttribArray(vertex_buffer_index);
        open_gl->glVertexAttribPointer(0, elem.getComponentCount(), 
                                      mapElemToShaderType(elem.type), elem.normalized ? GL_TRUE : GL_FALSE,
                                      buffer->getStride(),
                                      reinterpret_cast<void *>(elem.offset));
        vertex_buffer_index++;   
    }

    vertex_buffers.push_back(buffer);
}

inline void OpenGLVertexArray::bind()
{
    open_gl->glBindVertexArray(vao);
}

inline void OpenGLVertexArray::unbind()
{
    open_gl->glBindVertexArray(0);
}

std::shared_ptr<VertexBuffer> VertexBuffer::instance()
{
    switch (renderer_type) {
        case RendererType::OpenGL_API:
            return std::make_shared<OpenGLVertexBuffer>();
    }

    return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::instance()
{
    switch (renderer_type) {
        case RendererType::OpenGL_API:
            return std::make_shared<OpenGLIndexBuffer>();
    }

    return nullptr;
}

std::shared_ptr<VertexArray> VertexArray::instance()
{
    switch (renderer_type) {
        case RendererType::OpenGL_API:
            return std::make_shared<OpenGLVertexArray>();
    }

    return nullptr;
}

std::shared_ptr<Shader> Shader::instance()
{
    switch (renderer_type) {
        case RendererType::OpenGL_API:
            return std::make_shared<OpenGLShader>();
    }

    return nullptr;
}

RendererAPI *RendererAPI::instance()
{
#if GRAPHICS_PLATFORM_API == OPEN_GL
    global_var RendererAPI *instance = new OpenGLRendererAPI();
    return instance;
#else
    std::cerr << "Not supported graphics api\n";
    return nullptr;
#endif
}

#ifndef RENDERER_TYPES_H
#define RENDERER_TYPES_H


enum ShaderDataType
{
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

internal u32 mapShaderTypeToSize(ShaderDataType type)
{
    switch (type) {
        case Float:
        case Int:
            return 4;
        case Float2:
        case Int2:
            return 4 * 2;
        case Float3:
        case Int3:
            return 4 * 3;
        case Float4:
        case Int4:
            return 4 * 4;
        case Mat3:
            return 4 * 3 * 3;
        case Mat4:
            return 4 * 4 * 4;
        case Bool:
            return 1;
    }

    return None;
}

class RendererAPI
{
public:
    static RendererAPI *instance();
    
    virtual void init(SDL_Window *window) = 0;
    virtual void *getContext() = 0; 
    virtual ~RendererAPI() {}
protected:
    RendererAPI() {}
};


struct Element
{
    Element() = default;

    Element(ShaderDataType type, const std::string &name, bool normalized = false) :
        name{name},
        type{type},
        normalized{normalized},
        size{mapShaderTypeToSize(type)},
        offset{0} {}
    std::string name;
    b32 normalized;
    u32 size;
    size_t offset;
    ShaderDataType type;

    u32 getComponentCount() const;
};

u32 Element::getComponentCount() const
{
    switch (type) {
        case Bool: 
        case Int:  
        case Float:   
            return 1;
        case Int2: case Float2:  return 2;
        case Int3: case Float3:  return 3;
        case Int4: case Float4:  return 4;
        case Mat3: return 3 * 3;
        case Mat4: return 4 * 4;
    }
    
    return 0;
}

struct VertexBuffer
{
    std::vector<Element> elements;
    
    static std::shared_ptr<VertexBuffer> instance();

    virtual void create(u32 size) = 0; 
    virtual void create(f32 *vertices, u32 size) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    virtual void setData(const void *data, u32 size) = 0;
    virtual void setLayout(const std::vector<Element> &elements) = 0;
    virtual u32 getStride() = 0;

    virtual ~VertexBuffer() = default;
};


struct IndexBuffer
{
    static std::shared_ptr<IndexBuffer> instance();

    virtual void create(u32 *indices, u32 size) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    
    virtual ~IndexBuffer() = default;
};

struct VertexArray
{
    static std::shared_ptr<VertexArray> instance();

    virtual void create() = 0;
    virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> &buffer) = 0;
    virtual void addBuffer(std::shared_ptr<VertexBuffer> &buffer) = 0;

    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;

    virtual ~VertexArray() = default;
};

struct Shader
{
    static std::shared_ptr<Shader> instance();
    
    virtual void createProgram(const char *vertex_shader_src, const char *fragment_shader_src) = 0;
    virtual void bind() = 0;
    
    virtual ~Shader() = default;
};

struct RendererData
{
    Shader *shader;
    VertexArray *vao;
};
#endif

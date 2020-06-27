#ifndef RENDERER_TYPES_H
#define RENDERER_TYPES_H

struct RendererAPI;

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
    
    static std::shared_ptr<VertexBuffer> instance(RendererAPI *renderer_api);

    virtual void create(u32 size) = 0; 
    virtual void create(f32 *vertices, u32 size) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    virtual void setData(const void *data, u32 size) = 0;
    virtual void setLayout(std::vector<Element> elements) = 0;
    virtual u32 getStride() = 0;

    virtual ~VertexBuffer() = default;
};


struct IndexBuffer
{
    static std::shared_ptr<IndexBuffer> instance(RendererAPI *renderer_api);

    virtual void create(u32 *indices, u32 size) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    virtual inline u32 getCount() = 0; 
    virtual ~IndexBuffer() = default;
};

struct VertexArray
{
    static std::shared_ptr<VertexArray> instance(RendererAPI *renderer_api);

    virtual void create() = 0;
    virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> &buffer) = 0;
    virtual void addBuffer(std::shared_ptr<VertexBuffer> &buffer) = 0;

    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;

    virtual ~VertexArray() = default;
    
    std::shared_ptr<IndexBuffer> index_buffer;
};

struct Shader
{
    static std::shared_ptr<Shader> instance(RendererAPI *renderer_api);
    
    virtual void uploadUniformMat4(const char *name, const Mat4x4 &matrix) = 0;
    virtual void uploadUniformInt(const char *name, i32 integer) = 0;
	virtual void createProgram(const char *vertex_shader_src, const char *fragment_shader_src) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;    
    virtual ~Shader() = default;
};

struct Texture
{
	static Texture *instance(RendererAPI *renderer_api, const MemoryStorage &memory);

	virtual void create(const char *path) = 0;
	virtual void getDimension(u32 &width, u32 &height) = 0;
	virtual void bind() = 0;
	virtual ~Texture() = default;
};

class RendererAPI 
{
public:
    static RendererAPI *instance();
    
    virtual void init(SDL_Window *window) = 0;
    virtual void *getContext() = 0; 
    virtual void clear(v3 color) = 0;
    virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertex_array) = 0;
    virtual ~RendererAPI() {}
protected:
    RendererAPI() {}
};

struct RendererData
{
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexArray> vao;
    Texture *texture;
   	u32 indices_count;
};
#endif

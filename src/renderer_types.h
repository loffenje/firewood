#ifndef RENDERER_TYPES_H
#define RENDERER_TYPES_H

struct RendererAPI;

enum ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

internal u32 mapShaderTypeToSize(ShaderDataType type) {
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

struct Element {
    Element() = default;

    Element(ShaderDataType type, const std::string &name,
	    bool normalized = false)
	: name{name},
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

u32 Element::getComponentCount() const {
    switch (type) {
	case Bool:
	case Int:
	case Float:
	    return 1;
	case Int2:
	case Float2:
	    return 2;
	case Int3:
	case Float3:
	    return 3;
	case Int4:
	case Float4:
	    return 4;
	case Mat3:
	    return 3 * 3;
	case Mat4:
	    return 4 * 4;
    }

    return 0;
}

struct VertexBuffer {
    std::vector<Element> elements;

    static VertexBuffer *instance(RendererAPI *renderer_api,
				  const MemoryStorage &memory);

    virtual void create(u32 size) = 0;
    virtual void create(f32 *vertices, u32 size) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    virtual void setData(const void *data, u32 size) = 0;
    virtual void setLayout(std::vector<Element> elements) = 0;
    virtual u32 getStride() = 0;

    virtual ~VertexBuffer() = default;
};

struct IndexBuffer {
    static IndexBuffer *instance(RendererAPI *renderer_api,
				 const MemoryStorage &memory);

    virtual void create(u32 *indices, u32 count = 0) = 0;
    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;
    virtual inline u32 getCount() = 0;
    virtual ~IndexBuffer() = default;
};

struct VertexArray {
    static VertexArray *instance(RendererAPI *renderer_api,
				 const MemoryStorage &memory);

    virtual void create() = 0;
    virtual void setIndexBuffer(IndexBuffer *buffer) = 0;
    virtual void addBuffer(VertexBuffer *buffer) = 0;

    virtual inline void bind() = 0;
    virtual inline void unbind() = 0;

    virtual ~VertexArray() = default;

    IndexBuffer *index_buffer;
};

struct Shader {
    static Shader *instance(RendererAPI *renderer_api,
			    const MemoryStorage &memory);

    virtual void uploadArrayi(const char *name, i32 *values, u32 count) = 0;
    virtual void uploadMat4(const char *name, const Mat4x4 &value) = 0;
    virtual void uploadFloat4(const char *name, const v4 &value) = 0;
    virtual void uploadInt(const char *name, i32 value) = 0;
    virtual void createProgram(const char *vertex_shader_src,
			       const char *fragment_shader_src) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual ~Shader() = default;
};

struct Texture {
    static Texture *instance(RendererAPI *renderer_api,
			     const MemoryStorage &memory);

    virtual void create(const char *path) = 0;
    virtual void create(u32 width, u32 height) = 0;
    virtual void getDimension(u32 &width, u32 &height) = 0;
    virtual void bind(u32 slot = 0) = 0;
    virtual void setData(void *data, u32 size) = 0;
    virtual bool operator==(const Texture &other) = 0;
    virtual ~Texture() = default;
};

class RendererAPI {
   public:
    static RendererAPI *instance();

    virtual void init(SDL_Window *window) = 0;
    virtual void *getContext() = 0;
    virtual void clear(v3 color) = 0;
    virtual void drawIndexed(VertexArray *vertex_array, u32 count = 0) = 0;
    virtual ~RendererAPI() {}

   protected:
    RendererAPI() {}
};

struct RendererData {
    Shader *shader;
    VertexArray *vao;
    Texture *texture;
    Texture *overlay_texture;
    u32 indices_count;
};
#endif

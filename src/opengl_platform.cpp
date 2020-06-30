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
	OpenGLShader(RendererAPI *renderer_api) {
		open_gl = reinterpret_cast<OpenGL *>(renderer_api->getContext());
	}

	~OpenGLShader() {
		open_gl->glDeleteProgram(program_id);
	}

	OpenGLShader(OpenGL *open_gl): open_gl{open_gl} {}
	  
	OpenGL *open_gl;
	GLint program_id;

	void createProgram(const char *vertex_shader_src, const char *fragment_shader_src) override;
	void bind() override;
	void unbind() override;
	void uploadUniformMat4(const char *name, const Mat4x4 &matrix) override;
	void uploadUniformInt(const char *name, i32 integer) override;
};

void OpenGLShader::bind()
{
	open_gl->glUseProgram(program_id);
}

void OpenGLShader::unbind()
{
	open_gl->glUseProgram(0);
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

void OpenGLShader::uploadUniformMat4(const char *name, const Mat4x4 &matrix)
{
   GLint location = open_gl->glGetUniformLocation(program_id, name);
   open_gl->glUniformMatrix4fv(location, 1, GL_FALSE, matrix.n[0]); 
}

void OpenGLShader::uploadUniformInt(const char *name, i32 integer)
{
	GLint location = open_gl->glGetUniformLocation(program_id, name);
	open_gl->glUniform1i(location, integer);
}

struct OpenGLVertexBuffer : public VertexBuffer
{
	OpenGLVertexBuffer(RendererAPI *renderer_api) {
		open_gl = reinterpret_cast<OpenGL *>(renderer_api->getContext());
	}

	~OpenGLVertexBuffer() {
		open_gl->glDeleteBuffers(1, &vbo);
	}

	OpenGLVertexBuffer(OpenGL *open_gl): open_gl{open_gl} {}

	unsigned int vbo;
	u32 stride;

	OpenGL *open_gl;
	std::vector<Element> elements_data;

	u32 getStride() override;
	void create(u32 size) override; 
	void create(f32 *vertices, u32 size) override;
	inline void bind() override;
	inline void unbind() override;
	void setData(const void *data, u32 size) override;
	void setLayout(std::vector<Element> elements) override;
	void calcOffsetAndStride();
};

void OpenGLVertexBuffer::setData(const void *data, u32 size)
{
	open_gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	open_gl->glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void OpenGLVertexBuffer::setLayout(std::vector<Element> elements)
{
	this->elements = elements;

	calcOffsetAndStride();
}

void OpenGLVertexBuffer::calcOffsetAndStride()
{
	size_t offset = 0;
	stride = 0;
	for (auto &elem: elements) {
		elem.offset = offset;
		offset += elem.size;
		stride += elem.size;
	}
}

u32 OpenGLVertexBuffer::getStride()
{
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
	OpenGLIndexBuffer(RendererAPI *renderer_api) {
		open_gl = reinterpret_cast<OpenGL *>(renderer_api->getContext());
	}

	~OpenGLIndexBuffer() {
		open_gl->glDeleteBuffers(1, &ibo);
	}

	OpenGLIndexBuffer(OpenGL *open_gl): open_gl{open_gl} {}
	
	unsigned int ibo;
	u32 count;
	OpenGL *open_gl;
	void create(u32 *indices, u32 size) override;
	inline void bind() override;
	inline void unbind() override;
	inline u32 getCount() override;
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


inline u32 OpenGLIndexBuffer::getCount()
{
	return count;
}

struct OpenGLTexture : public Texture
{
	OpenGLTexture(RendererAPI *renderer_api) {
		open_gl = reinterpret_cast<OpenGL *>(renderer_api->getContext());
	}
   
	OpenGLTexture(OpenGL *open_gl): open_gl{open_gl} {}
	
	~OpenGLTexture() {
		glDeleteTextures(1, &texture);
	}

	void create(const char *path) override;
	void getDimension(u32 &width, u32 &height) override;
	void bind() override;
	
	unsigned int texture;

	OpenGL *open_gl;
	u32 width;
	u32 height;
	const char *path;
};

void OpenGLTexture::create(const char *path) 
{
	i32 img_width;
	i32 img_height;
	i32 channels;
	
	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data = stbi_load(path, &img_width, &img_height, &channels, 0);
	assert(data && "Failed to load image");	

	width = img_width;
	height = img_height;

	GLenum data_format = 0;
	if (channels == 4) {
		data_format = GL_RGBA;
	} else if (channels == 3) {
		data_format = GL_RGB;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, data_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
	open_gl->glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void OpenGLTexture::getDimension(u32 &width, u32 &height)
{
	width = this->width;
	height = this->height;
}

void OpenGLTexture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

struct OpenGLVertexArray : public VertexArray
{
	OpenGLVertexArray(RendererAPI *renderer_api) {
		open_gl = reinterpret_cast<OpenGL *>(renderer_api->getContext());
	}
	
	~OpenGLVertexArray() {
		open_gl->glDeleteVertexArrays(1, &vao);
	}

	OpenGLVertexArray(OpenGL *open_gl): open_gl{open_gl} {}
	
	OpenGL *open_gl;
	u32 vertex_buffer_index;
	u32 vao;
	int component_count;
	std::vector<VertexBuffer *> vertex_buffers;

	void create() override;
	void setIndexBuffer(IndexBuffer *buffer) override;
	void addBuffer(VertexBuffer *buffer) override;

	inline void bind() override;
	inline void unbind() override;
};

void OpenGLVertexArray::create()
{
	open_gl->glGenVertexArrays(1, &vao);
}

void OpenGLVertexArray::setIndexBuffer(IndexBuffer *buffer)
{
	open_gl->glBindVertexArray(vao);
	buffer->bind();

	index_buffer = buffer;
}

void OpenGLVertexArray::addBuffer(VertexBuffer *buffer)
{
	open_gl->glBindVertexArray(vao);
	buffer->bind();
	
	std::vector<Element> elements;
	elements.assign(buffer->elements.begin(), buffer->elements.end());

	for (const auto &elem : elements) {
		open_gl->glEnableVertexAttribArray(vertex_buffer_index);
		open_gl->glVertexAttribPointer(vertex_buffer_index, elem.getComponentCount(), 
									  mapElemToShaderType(elem.type), elem.normalized ? GL_TRUE : GL_FALSE,
									  buffer->getStride(),
									  reinterpret_cast<const void *>(elem.offset));
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

VertexBuffer *VertexBuffer::instance(RendererAPI *renderer_api, const MemoryStorage &memory)
{
	switch (renderer_type) {
		case RendererType::OpenGL_API:
			return alloc<OpenGLVertexBuffer>(memory.resource_partition, renderer_api);
	}

	return nullptr;
}

IndexBuffer *IndexBuffer::instance(RendererAPI *renderer_api, const MemoryStorage &memory)
{
	switch (renderer_type) {
		case RendererType::OpenGL_API:
			return alloc<OpenGLIndexBuffer>(memory.resource_partition, renderer_api);
	}

	return nullptr;
}

Texture *Texture::instance(RendererAPI *renderer_api, const MemoryStorage &memory)
{
	switch (renderer_type) {
		case RendererType::OpenGL_API:
			return alloc<OpenGLTexture>(memory.resource_partition, renderer_api);
	}

	return nullptr;
}

VertexArray *VertexArray::instance(RendererAPI *renderer_api, const MemoryStorage &memory)
{
	switch (renderer_type) {
		case RendererType::OpenGL_API:
			return alloc<OpenGLVertexArray>(memory.resource_partition, renderer_api);
	}

	return nullptr;
}

Shader *Shader::instance(RendererAPI *renderer_api, const MemoryStorage &memory)
{
	switch (renderer_type) {
		case RendererType::OpenGL_API:
			return alloc<OpenGLShader>(memory.resource_partition, renderer_api);
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

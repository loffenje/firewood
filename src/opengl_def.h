#ifndef OPENGL_DEF_H
#define OPENGL_DEF_H

typedef void APIENTRY type_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,
    GLsizei height, GLboolean fixedsamplelocations);
typedef void APIENTRY type_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void APIENTRY type_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void APIENTRY type_glFramebufferTexture2D(
    GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum APIENTRY type_glCheckFramebufferStatus(GLenum target);
typedef void APIENTRY type_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0,
    GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void APIENTRY type_glAttachShader(GLuint program, GLuint shader);
typedef void APIENTRY type_glCompileShader(GLuint shader);
typedef GLuint APIENTRY type_glCreateProgram(void);
typedef GLuint APIENTRY type_glCreateShader(GLenum type);
typedef void APIENTRY type_glLinkProgram(GLuint program);
typedef void APIENTRY type_glShaderSource(GLuint shader, GLsizei count, const GLchar **string, GLint *length);
typedef void APIENTRY type_glUseProgram(GLuint program);
typedef void APIENTRY type_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY type_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY type_glValidateProgram(GLuint program);
typedef void APIENTRY type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef GLint APIENTRY type_glGetUniformLocation(GLuint program, const GLchar *name);
typedef void APIENTRY type_glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY type_glUniform1i(GLint location, GLint v0);
typedef void APIENTRY type_glUniform1iv(GLint location, GLsizei count, const GLint *value);
typedef void APIENTRY type_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void APIENTRY type_glUniform1f(GLint location, GLfloat v0);
typedef void APIENTRY type_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);

typedef void APIENTRY type_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void APIENTRY type_glEnableVertexAttribArray(GLuint index);
typedef void APIENTRY type_glDisableVertexAttribArray(GLuint index);
typedef GLint APIENTRY type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void APIENTRY type_glVertexAttribPointer(
    GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void APIENTRY type_glVertexAttribIPointer(
    GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);

typedef void APIENTRY type_glDeleteBuffers(GLsizei n, const GLuint *buffers);
typedef void APIENTRY type_glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
typedef void APIENTRY type_glBindVertexArray(GLuint array);
typedef void APIENTRY type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void APIENTRY type_glBindBuffer(GLenum target, GLuint buffer);
typedef void APIENTRY type_glGenBuffers(GLsizei n, GLuint *buffers);
typedef void APIENTRY type_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY type_glActiveTexture(GLenum texture);
typedef void APIENTRY type_glDeleteProgram(GLuint program);
typedef void APIENTRY type_glDeleteShader(GLuint shader);
typedef void APIENTRY type_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
typedef void APIENTRY type_glDrawBuffers(GLsizei n, const GLenum *bufs);
typedef void APIENTRY type_glCreateTextures(GLenum target, GLsizei n, GLuint *textures);
typedef void APIENTRY type_glTexStorage2D(
    GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void APIENTRY type_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
    GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void APIENTRY type_glBindTextureUnit(GLuint unit, GLuint texture);
typedef void APIENTRY type_glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
    GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void APIENTRY type_glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
    GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
#define GL_DEBUG_CALLBACK(Name)                                                                                        \
  void APIENTRY Name(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,    \
      const void *userParam)
typedef GL_DEBUG_CALLBACK(GL_DEBUG_PROC);
typedef void APIENTRY type_glDebugMessageCallbackARB(GL_DEBUG_PROC *callback, const void *userParam);
typedef const GLubyte *APIENTRY type_glGetStringi(GLenum name, GLuint index);
typedef void APIENTRY type_glDrawElementsBaseVertex(
    GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void APIENTRY type_glGenerateMipmap(GLenum target);
#define openGLFunction(name) type_##name *name

struct OpenGL {
  SDL_GLContext gl_context;

  openGLFunction(glTexImage2DMultisample);
  openGLFunction(glBindFramebuffer);
  openGLFunction(glGenFramebuffers);
  openGLFunction(glFramebufferTexture2D);
  openGLFunction(glCheckFramebufferStatus);
  openGLFunction(glBlitFramebuffer);
  openGLFunction(glAttachShader);
  openGLFunction(glCompileShader);
  openGLFunction(glCreateProgram);
  openGLFunction(glCreateShader);
  openGLFunction(glLinkProgram);
  openGLFunction(glShaderSource);
  openGLFunction(glUseProgram);
  openGLFunction(glGetProgramInfoLog);
  openGLFunction(glGetShaderInfoLog);
  openGLFunction(glValidateProgram);
  openGLFunction(glGetProgramiv);
  openGLFunction(glGetUniformLocation);
  openGLFunction(glUniform4fv);
  openGLFunction(glUniformMatrix4fv);
  openGLFunction(glUniform1i);
  openGLFunction(glUniform1iv);
  openGLFunction(glUniform1f);
  openGLFunction(glUniform4f);
  openGLFunction(glUniform2fv);
  openGLFunction(glUniform3fv);
  openGLFunction(glEnableVertexAttribArray);
  openGLFunction(glDisableVertexAttribArray);
  openGLFunction(glGetAttribLocation);
  openGLFunction(glVertexAttribPointer);
  openGLFunction(glVertexAttribIPointer);
  openGLFunction(glDebugMessageCallbackARB);
  openGLFunction(glBindVertexArray);
  openGLFunction(glGenVertexArrays);
  openGLFunction(glBindBuffer);
  openGLFunction(glGenBuffers);
  openGLFunction(glBufferData);
  openGLFunction(glBufferSubData);
  openGLFunction(glActiveTexture);
  openGLFunction(glGetStringi);
  openGLFunction(glDeleteProgram);
  openGLFunction(glDeleteShader);
  openGLFunction(glDeleteFramebuffers);
  openGLFunction(glDrawBuffers);
  openGLFunction(glBindTextureUnit);
  openGLFunction(glCreateTextures);
  openGLFunction(glTexStorage2D);
  openGLFunction(glTexSubImage2D);
  openGLFunction(glTexImage3D);
  openGLFunction(glTexSubImage3D);
  openGLFunction(glDrawElementsBaseVertex);
  openGLFunction(glGenerateMipmap);
  openGLFunction(glDeleteBuffers);
  openGLFunction(glDeleteVertexArrays);
};

class OpenGLRendererAPI : public RendererAPI {
  OpenGL *context;

  void *getContext() override;
  void init(SDL_Window *window) override;
  void drawIndexed(VertexArray *vertex_array, u32 index_count = 0) override;
  void setAttributes();
  OpenGL *rendererAlloc(size_t size);
  void clear(v3 color) override;
};

void OpenGLRendererAPI::clear(v3 color) {
  glClearColor(color.x, color.y, color.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void *OpenGLRendererAPI::getContext() {
  if (!context) {
    std::cerr << "Initialize renderer first!\n";
    exit(1);
  }

  return context;
}

void OpenGLRendererAPI::drawIndexed(VertexArray *vertex_array, u32 index_count) {
  u32 count = index_count ? index_count : vertex_array->index_buffer->getCount();
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGL *OpenGLRendererAPI::rendererAlloc(size_t size) {
  void *memory = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  OpenGL *result = reinterpret_cast<OpenGL *>(memory != MAP_FAILED ? memory : 0);

  return result;
}

void OpenGLRendererAPI::setAttributes() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
}

void OpenGLRendererAPI::init(SDL_Window *window) {
  context = rendererAlloc(sizeof(OpenGL));
  setAttributes();

  context->gl_context = SDL_GL_CreateContext(window);

  if (context->gl_context) {
    SDL_GL_MakeCurrent(window, context->gl_context);
#define SDL_GetOpenGLFunction(name) context->name = (type_##name *)SDL_GL_GetProcAddress(#name)

    SDL_GetOpenGLFunction(glTexImage2DMultisample);
    SDL_GetOpenGLFunction(glBlitFramebuffer);
    SDL_GetOpenGLFunction(glAttachShader);
    SDL_GetOpenGLFunction(glCompileShader);
    SDL_GetOpenGLFunction(glCreateProgram);
    SDL_GetOpenGLFunction(glCreateShader);
    SDL_GetOpenGLFunction(glLinkProgram);
    SDL_GetOpenGLFunction(glShaderSource);
    SDL_GetOpenGLFunction(glUseProgram);
    SDL_GetOpenGLFunction(glGetProgramInfoLog);
    SDL_GetOpenGLFunction(glGetShaderInfoLog);
    SDL_GetOpenGLFunction(glValidateProgram);
    SDL_GetOpenGLFunction(glGetProgramiv);
    SDL_GetOpenGLFunction(glGetUniformLocation);
    SDL_GetOpenGLFunction(glUniform4fv);
    SDL_GetOpenGLFunction(glUniformMatrix4fv);
    SDL_GetOpenGLFunction(glUniform1i);
    SDL_GetOpenGLFunction(glUniform1iv);
    SDL_GetOpenGLFunction(glBufferSubData);
    SDL_GetOpenGLFunction(glEnableVertexAttribArray);
    SDL_GetOpenGLFunction(glDisableVertexAttribArray);
    SDL_GetOpenGLFunction(glGetAttribLocation);
    SDL_GetOpenGLFunction(glVertexAttribPointer);
    SDL_GetOpenGLFunction(glVertexAttribIPointer);
    SDL_GetOpenGLFunction(glDebugMessageCallbackARB);
    SDL_GetOpenGLFunction(glBindVertexArray);
    SDL_GetOpenGLFunction(glGenVertexArrays);
    SDL_GetOpenGLFunction(glBindBuffer);
    SDL_GetOpenGLFunction(glGenBuffers);
    SDL_GetOpenGLFunction(glBufferData);
    SDL_GetOpenGLFunction(glActiveTexture);
    SDL_GetOpenGLFunction(glGetStringi);
    SDL_GetOpenGLFunction(glDeleteProgram);
    SDL_GetOpenGLFunction(glDeleteShader);
    SDL_GetOpenGLFunction(glDeleteFramebuffers);
    SDL_GetOpenGLFunction(glDrawBuffers);
    SDL_GetOpenGLFunction(glBindTextureUnit);
    SDL_GetOpenGLFunction(glCreateTextures);
    SDL_GetOpenGLFunction(glTexStorage2D);
    SDL_GetOpenGLFunction(glTexSubImage2D);
    SDL_GetOpenGLFunction(glTexImage3D);
    SDL_GetOpenGLFunction(glTexSubImage3D);
    SDL_GetOpenGLFunction(glDrawElementsBaseVertex);
    SDL_GetOpenGLFunction(glUniform1f);
    SDL_GetOpenGLFunction(glUniform4f);
    SDL_GetOpenGLFunction(glUniform2fv);
    SDL_GetOpenGLFunction(glUniform3fv);
    SDL_GetOpenGLFunction(glGenerateMipmap);
    SDL_GetOpenGLFunction(glDeleteBuffers);
    SDL_GetOpenGLFunction(glDeleteVertexArrays);

  } else {
    fprintf(stderr, "Fail to create context %s", SDL_GetError());
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  SDL_GL_SetSwapInterval(1);
}

#endif

#ifndef OPENGL_DEF_H
#define OPENGL_DEF_H


typedef void APIENTRY type_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void APIENTRY type_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void APIENTRY type_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void APIENTRY type_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum APIENTRY type_glCheckFramebufferStatus(GLenum target);
typedef void APIENTRY type_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
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
typedef GLint APIENTRY type_glGetUniformLocation (GLuint program, const GLchar *name);
typedef void APIENTRY type_glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY type_glUniform1i(GLint location, GLint v0);
typedef void APIENTRY type_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void APIENTRY type_glUniform1f(GLint location, GLfloat v0);
typedef void APIENTRY type_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);

typedef void APIENTRY type_glBufferSubData(GLenum target,GLintptr offset,GLsizeiptr size, const void *data);
typedef void APIENTRY type_glEnableVertexAttribArray(GLuint index);
typedef void APIENTRY type_glDisableVertexAttribArray(GLuint index);
typedef GLint APIENTRY type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void APIENTRY type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void APIENTRY type_glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void APIENTRY type_glBindVertexArray(GLuint array);
typedef void APIENTRY type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void APIENTRY type_glBindBuffer (GLenum target, GLuint buffer);
typedef void APIENTRY type_glGenBuffers (GLsizei n, GLuint *buffers);
typedef void APIENTRY type_glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY type_glActiveTexture (GLenum texture);
typedef void APIENTRY type_glDeleteProgram (GLuint program);
typedef void APIENTRY type_glDeleteShader (GLuint shader);
typedef void APIENTRY type_glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers);
typedef void APIENTRY type_glDrawBuffers (GLsizei n, const GLenum *bufs);
typedef void APIENTRY type_glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void APIENTRY type_glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
#define GL_DEBUG_CALLBACK(Name) void APIENTRY Name(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam)
typedef GL_DEBUG_CALLBACK(GL_DEBUG_PROC);
typedef void APIENTRY type_glDebugMessageCallbackARB(GL_DEBUG_PROC *callback, const void *userParam);
typedef const GLubyte * APIENTRY type_glGetStringi(GLenum name, GLuint index);
typedef void APIENTRY type_glDrawElementsBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void APIENTRY type_glGenerateMipmap(GLenum target);
#define openGLFunction(name) type_##name *name


struct OpenGL {
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
    openGLFunction(glTexImage3D);
    openGLFunction(glTexSubImage3D);
    openGLFunction(glDrawElementsBaseVertex);
    openGLFunction(glGenerateMipmap);
};



#endif

/* Copyright (c) 2011 Wildfire Games
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * OpenGL extension function declarations (X macros).
 */

#if OS_MACOSX
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif
#if OS_WIN
# include <GL/wglext.h>
#endif

/*

FUNC is used for functions that are only extensions.
FUNC2 is used for functions that have been promoted to core features.
FUNC3 is used for functions that have been promoted to core features
but have slightly changed semantics and need to be referred to by their
core name instead of extension name.

The FUNC2/FUNC3 calls include the version of OpenGL in which the extension was promoted,
and the pre- and post-promotion names (e.g. "glBindBufferARB" vs "glBindBuffer").

If the GL driver is advertising a sufficiently high version, we load the promoted
name; otherwise we use the *ARB name. (The spec says:
	"GL implementations of such later revisions should continue to export the name
	 strings of promoted extensions in the EXTENSIONS string, and continue to support
	 the ARB-affixed versions of functions and enumerants as a transition aid."
but some drivers might be stupid/buggy and fail to do that, so we don't just use
the ARB names unconditionally.)

The names are made accessible to engine code only via the ARB name, to make it
obvious that care must be taken (i.e. by being certain that the extension is
actually supported).

*/

// were these defined as real functions in gl.h already?

// GL_EXT_draw_range_elements / GL1.2:
FUNC2(void, glDrawRangeElementsEXT, glDrawRangeElements, "1.2", (GLenum, GLuint, GLuint, GLsizei, GLenum, GLvoid*))

// GL_ARB_multitexture / GL1.3:
FUNC2(void, glMultiTexCoord2fARB, glMultiTexCoord2f, "1.3", (int, float, float))
FUNC2(void, glMultiTexCoord3fARB, glMultiTexCoord3f, "1.3", (int, float, float, float))
FUNC2(void, glActiveTextureARB, glActiveTexture, "1.3", (int))
FUNC2(void, glClientActiveTextureARB, glClientActiveTexture, "1.3", (int))

// GL_EXT_blend_color / GL1.4 (optional in 1.2):
FUNC2(void, glBlendColorEXT, glBlendColor, "1.4", (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha))

// GL_EXT_blend_minmax / GL1.4 (optional in 1.2):
FUNC2(void, glBlendEquationEXT, glBlendEquation, "1.4", (GLenum mode))

// GL_ARB_vertex_buffer_object / GL1.5:
FUNC2(void, glBindBufferARB, glBindBuffer, "1.5", (int target, GLuint buffer))
FUNC2(void, glDeleteBuffersARB, glDeleteBuffers, "1.5", (GLsizei n, const GLuint* buffers))
FUNC2(void, glGenBuffersARB, glGenBuffers, "1.5", (GLsizei n, GLuint* buffers))
FUNC2(bool, glIsBufferARB, glIsBuffer, "1.5", (GLuint buffer))
FUNC2(void, glBufferDataARB, glBufferData, "1.5", (int target, GLsizeiptrARB size, const void* data, int usage))
FUNC2(void, glBufferSubDataARB, glBufferSubData, "1.5", (int target, GLintptrARB offset, GLsizeiptrARB size, const void* data))
FUNC2(void, glGetBufferSubDataARB, glGetBufferSubData, "1.5", (int target, GLintptrARB offset, GLsizeiptrARB size, void* data))
FUNC2(void*, glMapBufferARB, glMapBuffer, "1.5", (int target, int access))
FUNC2(bool, glUnmapBufferARB, glUnmapBuffer, "1.5", (int target))
FUNC2(void, glGetBufferParameterivARB, glGetBufferParameteriv, "1.5", (int target, int pname, int* params))
FUNC2(void, glGetBufferPointervARB, glGetBufferPointerv, "1.5", (int target, int pname, void** params))

// GL_ARB_texture_compression / GL1.3
FUNC2(void, glCompressedTexImage3DARB, glCompressedTexImage3D, "1.3", (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*))
FUNC2(void, glCompressedTexImage2DARB, glCompressedTexImage2D, "1.3", (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*))
FUNC2(void, glCompressedTexImage1DARB, glCompressedTexImage1D, "1.3", (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*))
FUNC2(void, glCompressedTexSubImage3DARB, glCompressedTexSubImage3D, "1.3", (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*))
FUNC2(void, glCompressedTexSubImage2DARB, glCompressedTexSubImage2D, "1.3", (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*))
FUNC2(void, glCompressedTexSubImage1DARB, glCompressedTexSubImage1D, "1.3", (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*))
FUNC2(void, glGetCompressedTexImageARB, glGetCompressedTexImage, "1.3", (GLenum, GLint, GLvoid*))

// GL_EXT_framebuffer_object
FUNC(GLboolean, glIsRenderbufferEXT, (GLuint renderbuffer))
FUNC(void, glBindRenderbufferEXT, (GLenum target, GLuint renderbuffer))
FUNC(void, glDeleteRenderbuffersEXT, (GLsizei n, const GLuint *renderbuffers))
FUNC(void, glGenRenderbuffersEXT, (GLsizei n, GLuint *renderbuffers))
FUNC(void, glRenderbufferStorageEXT, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height))
FUNC(void, glGetRenderbufferParameterivEXT, (GLenum target, GLenum pname, GLint *params))
FUNC(GLboolean, glIsFramebufferEXT, (GLuint framebuffer))
FUNC(void, glBindFramebufferEXT, (GLenum target, GLuint framebuffer))
FUNC(void, glDeleteFramebuffersEXT, (GLsizei n, const GLuint *framebuffers))
FUNC(void, glGenFramebuffersEXT, (GLsizei n, GLuint *framebuffers))
FUNC(GLenum, glCheckFramebufferStatusEXT, (GLenum target))
FUNC(void, glFramebufferTexture1DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))
FUNC(void, glFramebufferTexture2DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))
FUNC(void, glFramebufferTexture3DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset))
FUNC(void, glFramebufferRenderbufferEXT, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))
FUNC(void, glGetFramebufferAttachmentParameterivEXT, (GLenum target, GLenum attachment, GLenum pname, GLint *params))
FUNC(void, glGenerateMipmapEXT, (GLenum target))

// GL_ARB_vertex_program, GL_ARB_fragment_program
FUNC(void, glProgramStringARB, (GLenum target, GLenum format, GLsizei len, const GLvoid *string))
FUNC(void, glBindProgramARB, (GLenum target, GLuint program))
FUNC(void, glDeleteProgramsARB, (GLsizei n, const GLuint *programs))
FUNC(void, glGenProgramsARB, (GLsizei n, GLuint *programs))
FUNC(void, glProgramEnvParameter4dARB, (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))
FUNC(void, glProgramEnvParameter4dvARB, (GLenum target, GLuint index, const GLdouble *params))
FUNC(void, glProgramEnvParameter4fARB, (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))
FUNC(void, glProgramEnvParameter4fvARB, (GLenum target, GLuint index, const GLfloat *params))
FUNC(void, glProgramLocalParameter4dARB, (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))
FUNC(void, glProgramLocalParameter4dvARB, (GLenum target, GLuint index, const GLdouble *params))
FUNC(void, glProgramLocalParameter4fARB, (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))
FUNC(void, glProgramLocalParameter4fvARB, (GLenum target, GLuint index, const GLfloat *params))
FUNC(void, glGetProgramEnvParameterdvARB, (GLenum target, GLuint index, GLdouble *params))
FUNC(void, glGetProgramEnvParameterfvARB, (GLenum target, GLuint index, GLfloat *params))
FUNC(void, glGetProgramLocalParameterdvARB, (GLenum target, GLuint index, GLdouble *params))
FUNC(void, glGetProgramLocalParameterfvARB, (GLenum target, GLuint index, GLfloat *params))
FUNC(void, glGetProgramivARB, (GLenum target, GLenum pname, GLint *params))
FUNC(void, glGetProgramStringARB, (GLenum target, GLenum pname, GLvoid *string))
FUNC(GLboolean, glIsProgramARB, (GLuint program))

// GL_ARB_shader_objects
// (NOTE: Many of these have "Object" in their ARB names, but "Program" or "Shader" in their core names.
// When both Program and Shader versions exist, we use FUNC3 here and the engine must call the specific
// core name instead of the generic ARB name.)
FUNC3(void, glDeleteObjectARB, glDeleteShader, "2.0", (GLhandleARB obj))
FUNC3(void, glDeleteObjectARB, glDeleteProgram, "2.0", (GLhandleARB obj))
// FUNC2(GLhandleARB, glGetHandleARB, glGetHandle, "2.0", (GLenum pname))
  // there is no analog to the ARB function in GL 2.0 (the functionality is probably moved into glGetIntegerv(GL_CURRENT_PROGRAM))
  // so we can't represent it in this FUNC2 system, so just pretend it doesn't exist
FUNC2(void, glDetachObjectARB, glDetachShader, "2.0", (GLhandleARB containerObj, GLhandleARB attachedObj))
FUNC2(GLhandleARB, glCreateShaderObjectARB, glCreateShader, "2.0", (GLenum shaderType))
FUNC2(void, glShaderSourceARB, glShaderSource, "2.0", (GLhandleARB shaderObj, GLsizei count, const char **string, const GLint *length))
FUNC2(void, glCompileShaderARB, glCompileShader, "2.0", (GLhandleARB shaderObj))
FUNC2(GLhandleARB, glCreateProgramObjectARB, glCreateProgram, "2.0", (void))
FUNC2(void, glAttachObjectARB, glAttachShader, "2.0", (GLhandleARB containerObj, GLhandleARB obj))
FUNC2(void, glLinkProgramARB, glLinkProgram, "2.0", (GLhandleARB programObj))
FUNC2(void, glUseProgramObjectARB, glUseProgram, "2.0", (GLhandleARB programObj))
FUNC2(void, glValidateProgramARB, glValidateProgram, "2.0", (GLhandleARB programObj))
FUNC2(void, glUniform1fARB, glUniform1f, "2.0", (GLint location, GLfloat v0))
FUNC2(void, glUniform2fARB, glUniform2f, "2.0", (GLint location, GLfloat v0, GLfloat v1))
FUNC2(void, glUniform3fARB, glUniform3f, "2.0", (GLint location, GLfloat v0, GLfloat v1, GLfloat v2))
FUNC2(void, glUniform4fARB, glUniform4f, "2.0", (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3))
FUNC2(void, glUniform1iARB, glUniform1i, "2.0", (GLint location, GLint v0))
FUNC2(void, glUniform2iARB, glUniform2i, "2.0", (GLint location, GLint v0, GLint v1))
FUNC2(void, glUniform3iARB, glUniform3i, "2.0", (GLint location, GLint v0, GLint v1, GLint v2))
FUNC2(void, glUniform4iARB, glUniform4i, "2.0", (GLint location, GLint v0, GLint v1, GLint v2, GLint v3))
FUNC2(void, glUniform1fvARB, glUniform1fv, "2.0", (GLint location, GLsizei count, const GLfloat *value))
FUNC2(void, glUniform2fvARB, glUniform2fv, "2.0", (GLint location, GLsizei count, const GLfloat *value))
FUNC2(void, glUniform3fvARB, glUniform3fv, "2.0", (GLint location, GLsizei count, const GLfloat *value))
FUNC2(void, glUniform4fvARB, glUniform4fv, "2.0", (GLint location, GLsizei count, const GLfloat *value))
FUNC2(void, glUniform1ivARB, glUniform1iv, "2.0", (GLint location, GLsizei count, const GLint *value))
FUNC2(void, glUniform2ivARB, glUniform2iv, "2.0", (GLint location, GLsizei count, const GLint *value))
FUNC2(void, glUniform3ivARB, glUniform3iv, "2.0", (GLint location, GLsizei count, const GLint *value))
FUNC2(void, glUniform4ivARB, glUniform4iv, "2.0", (GLint location, GLsizei count, const GLint *value))
FUNC2(void, glUniformMatrix2fvARB, glUniformMatrix2fv, "2.0", (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
FUNC2(void, glUniformMatrix3fvARB, glUniformMatrix3fv, "2.0", (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
FUNC2(void, glUniformMatrix4fvARB, glUniformMatrix4fv, "2.0", (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))
FUNC3(void, glGetObjectParameterfvARB, glGetProgramfv, "2.0", (GLhandleARB obj, GLenum pname, GLfloat *params))
FUNC3(void, glGetObjectParameterfvARB, glGetShaderfv, "2.0", (GLhandleARB obj, GLenum pname, GLfloat *params))
FUNC3(void, glGetObjectParameterivARB, glGetProgramiv, "2.0", (GLhandleARB obj, GLenum pname, GLint *params))
FUNC3(void, glGetObjectParameterivARB, glGetShaderiv, "2.0", (GLhandleARB obj, GLenum pname, GLint *params))
FUNC3(void, glGetInfoLogARB, glGetProgramInfoLog, "2.0", (GLhandleARB obj, GLsizei maxLength, GLsizei *length, char *infoLog))
FUNC3(void, glGetInfoLogARB, glGetShaderInfoLog, "2.0", (GLhandleARB obj, GLsizei maxLength, GLsizei *length, char *infoLog))
FUNC2(void, glGetAttachedObjectsARB, glGetAttachedShaders, "2.0", (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj))
FUNC2(GLint, glGetUniformLocationARB, glGetUniformLocation, "2.0", (GLhandleARB programObj, const char *name))
FUNC2(void, glGetActiveUniformARB, glGetActiveUniform, "2.0", (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, char *name))
FUNC2(void, glGetUniformfvARB, glGetUniformfv, "2.0", (GLhandleARB programObj, GLint location, GLfloat *params))
FUNC2(void, glGetUniformivARB, glGetUniformiv, "2.0", (GLhandleARB programObj, GLint location, GLint *params))
FUNC2(void, glGetShaderSourceARB, glGetShaderSource, "2.0", (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source))

// GL_ARB_vertex_shader
FUNC2(void, glVertexAttrib1fARB, glVertexAttrib1f, "2.0", (GLuint index, GLfloat v0))
FUNC2(void, glVertexAttrib1sARB, glVertexAttrib1s, "2.0", (GLuint index, GLshort v0))
FUNC2(void, glVertexAttrib1dARB, glVertexAttrib1d, "2.0", (GLuint index, GLdouble v0))
FUNC2(void, glVertexAttrib2fARB, glVertexAttrib2f, "2.0", (GLuint index, GLfloat v0, GLfloat v1))
FUNC2(void, glVertexAttrib2sARB, glVertexAttrib2s, "2.0", (GLuint index, GLshort v0, GLshort v1))
FUNC2(void, glVertexAttrib2dARB, glVertexAttrib2d, "2.0", (GLuint index, GLdouble v0, GLdouble v1))
FUNC2(void, glVertexAttrib3fARB, glVertexAttrib3f, "2.0", (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2))
FUNC2(void, glVertexAttrib3sARB, glVertexAttrib3s, "2.0", (GLuint index, GLshort v0, GLshort v1, GLshort v2))
FUNC2(void, glVertexAttrib3dARB, glVertexAttrib3d, "2.0", (GLuint index, GLdouble v0, GLdouble v1, GLdouble v2))
FUNC2(void, glVertexAttrib4fARB, glVertexAttrib4f, "2.0", (GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3))
FUNC2(void, glVertexAttrib4sARB, glVertexAttrib4s, "2.0", (GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3))
FUNC2(void, glVertexAttrib4dARB, glVertexAttrib4d, "2.0", (GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3))
FUNC2(void, glVertexAttrib4NubARB, glVertexAttrib4Nub, "2.0", (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w))
FUNC2(void, glVertexAttrib1fvARB, glVertexAttrib1fv, "2.0", (GLuint index, const GLfloat *v))
FUNC2(void, glVertexAttrib1svARB, glVertexAttrib1sv, "2.0", (GLuint index, const GLshort *v))
FUNC2(void, glVertexAttrib1dvARB, glVertexAttrib1dv, "2.0", (GLuint index, const GLdouble *v))
FUNC2(void, glVertexAttrib2fvARB, glVertexAttrib2fv, "2.0", (GLuint index, const GLfloat *v))
FUNC2(void, glVertexAttrib2svARB, glVertexAttrib2sv, "2.0", (GLuint index, const GLshort *v))
FUNC2(void, glVertexAttrib2dvARB, glVertexAttrib2dv, "2.0", (GLuint index, const GLdouble *v))
FUNC2(void, glVertexAttrib3fvARB, glVertexAttrib3fv, "2.0", (GLuint index, const GLfloat *v))
FUNC2(void, glVertexAttrib3svARB, glVertexAttrib3sv, "2.0", (GLuint index, const GLshort *v))
FUNC2(void, glVertexAttrib3dvARB, glVertexAttrib3dv, "2.0", (GLuint index, const GLdouble *v))
FUNC2(void, glVertexAttrib4fvARB, glVertexAttrib4fv, "2.0", (GLuint index, const GLfloat *v))
FUNC2(void, glVertexAttrib4svARB, glVertexAttrib4sv, "2.0", (GLuint index, const GLshort *v))
FUNC2(void, glVertexAttrib4dvARB, glVertexAttrib4dv, "2.0", (GLuint index, const GLdouble *v))
FUNC2(void, glVertexAttrib4ivARB, glVertexAttrib4iv, "2.0", (GLuint index, const GLint *v))
FUNC2(void, glVertexAttrib4bvARB, glVertexAttrib4bv, "2.0", (GLuint index, const GLbyte *v))
FUNC2(void, glVertexAttrib4ubvARB, glVertexAttrib4ubv, "2.0", (GLuint index, const GLubyte *v))
FUNC2(void, glVertexAttrib4usvARB, glVertexAttrib4usv, "2.0", (GLuint index, const GLushort *v))
FUNC2(void, glVertexAttrib4uivARB, glVertexAttrib4uiv, "2.0", (GLuint index, const GLuint *v))
FUNC2(void, glVertexAttrib4NbvARB, glVertexAttrib4Nbv, "2.0", (GLuint index, const GLbyte *v))
FUNC2(void, glVertexAttrib4NsvARB, glVertexAttrib4Nsv, "2.0", (GLuint index, const GLshort *v))
FUNC2(void, glVertexAttrib4NivARB, glVertexAttrib4Niv, "2.0", (GLuint index, const GLint *v))
FUNC2(void, glVertexAttrib4NubvARB, glVertexAttrib4Nubv, "2.0", (GLuint index, const GLubyte *v))
FUNC2(void, glVertexAttrib4NusvARB, glVertexAttrib4Nusv, "2.0", (GLuint index, const GLushort *v))
FUNC2(void, glVertexAttrib4NuivARB, glVertexAttrib4Nuiv, "2.0", (GLuint index, const GLuint *v))
FUNC2(void, glVertexAttribPointerARB, glVertexAttribPointer, "2.0", (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer))
FUNC2(void, glEnableVertexAttribArrayARB, glEnableVertexAttribArray, "2.0", (GLuint index))
FUNC2(void, glDisableVertexAttribArrayARB, glDisableVertexAttribArray, "2.0", (GLuint index))
FUNC2(void, glBindAttribLocationARB, glBindAttribLocation, "2.0", (GLhandleARB programObj, GLuint index, const char *name))
FUNC2(void, glGetActiveAttribARB, glGetActiveAttrib, "2.0", (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, int *size, GLenum *type, char *name))
FUNC2(GLint, glGetAttribLocationARB, glGetAttribLocation, "2.0", (GLhandleARB programObj, const char *name))
FUNC2(void, glGetVertexAttribdvARB, glGetVertexAttribdv, "2.0", (GLuint index, GLenum pname, GLdouble *params))
FUNC2(void, glGetVertexAttribfvARB, glGetVertexAttribfv, "2.0", (GLuint index, GLenum pname, GLfloat *params))
FUNC2(void, glGetVertexAttribivARB, glGetVertexAttribiv, "2.0", (GLuint index, GLenum pname, GLint *params))
FUNC2(void, glGetVertexAttribPointervARB, glGetVertexAttribPointerv, "2.0", (GLuint index, GLenum pname, void **pointer))

#if OS_WIN
// WGL_EXT_swap_control
FUNC(int, wglSwapIntervalEXT, (int))

// WGL_ARB_pbuffer
FUNC(HPBUFFERARB, wglCreatePbufferARB, (HDC, int, int, int, const int*))
FUNC(HDC, wglGetPbufferDCARB, (HPBUFFERARB))
FUNC(int, wglReleasePbufferDCARB, (HPBUFFERARB, HDC))
FUNC(int, wglDestroyPbufferARB, (HPBUFFERARB))
FUNC(int, wglQueryPbufferARB, (HPBUFFERARB, int, int*))

// GL_ARB_pixel_format
FUNC(int, wglGetPixelFormatAttribivARB, (HDC, int, int, unsigned int, const int*, int*))
FUNC(int, wglGetPixelFormatAttribfvARB, (HDC, int, int, unsigned int, const int*, float*))
FUNC(int, wglChoosePixelFormatARB, (HDC, const int *, const float*, unsigned int, int*, unsigned int*))

// GL_GREMEDY_string_marker (from gDEBugger)
FUNC(int, glStringMarkerGREMEDY, (GLsizei len, const GLvoid *string))
#endif // OS_WIN

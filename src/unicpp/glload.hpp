#pragma once

#include "type.hpp"
#ifdef PROG_SYS_WIN
    #include "macro-off.hpp"
    #include <windows.h>
    #include "macro-on.hpp"
#endif

#ifdef PROG_SYS_WIN
    #define GLAPI APIENTRY
#else
    #define GLAPI
#endif

// GL variable types
using GLboolean = U1;
using GLuint = U4;
using GLint = S4;
using GLintptr = SA;
using GLsizei = S4;
using GLsizeiptr = SA;
using GLenum = U4;
using GLbitfield = U4;
using GLfloat = F4;
using GLdouble = F8;
using GLchar = CS;

// GL constants
cxex GLenum GL_UNSIGNED_BYTE = 0x1401;
cxex GLenum GL_FLOAT = 0x1406;
cxex GLenum GL_RGB8 = 0x8051;
cxex GLenum GL_RGBA8 = 0x8058;
cxex GLenum GL_RGB = 0x1907;
cxex GLenum GL_RGBA = 0x1908;
cxex GLenum GL_VALIDATE_STATUS = 0x8B83;
cxex GLenum GL_LINK_STATUS = 0x8B82;
cxex GLenum GL_COMPILE_STATUS = 0x8B81;
cxex GLenum GL_ARRAY_BUFFER = 0x8892;
cxex GLenum GL_ELEMENT_ARRAY_BUFFER = 0x8893;
cxex GLenum GL_SHADER_STORAGE_BUFFER = 0x90D2;
cxex GLenum GL_TEXTURE_2D = 0x0DE1;
cxex GLenum GL_TEXTURE_BASE_LEVEL = 0x813C;
cxex GLenum GL_TEXTURE_COMPARE_FUNC = 0x884D;
cxex GLenum GL_TEXTURE_COMPARE_MODE = 0x884C;
cxex GLenum GL_TEXTURE_LOD_BIAS = 0x8501;
cxex GLenum GL_TEXTURE_MIN_FILTER = 0x2801;
cxex GLenum GL_TEXTURE_MAG_FILTER = 0x2800;
cxex GLenum GL_TEXTURE_MIN_LOD = 0x813A;
cxex GLenum GL_TEXTURE_MAX_LOD = 0x813B;
cxex GLenum GL_TEXTURE_MAX_LEVEL = 0x813D;
cxex GLenum GL_TEXTURE_SWIZZLE_R = 0x8E42;
cxex GLenum GL_TEXTURE_SWIZZLE_G = 0x8E43;
cxex GLenum GL_TEXTURE_SWIZZLE_B = 0x8E44;
cxex GLenum GL_TEXTURE_SWIZZLE_A = 0x8E45;
cxex GLenum GL_TEXTURE_WRAP_R = 0x8072;
cxex GLenum GL_TEXTURE_WRAP_S = 0x2802;
cxex GLenum GL_TEXTURE_WRAP_T = 0x2803;
cxex GLenum GL_VERTEX_SHADER = 0x8B31;
cxex GLenum GL_FRAGMENT_SHADER = 0x8B30;
cxex GLenum GL_DYNAMIC_DRAW = 0x88E8;
cxex GLenum GL_TRIANGLES = 0x0004;
cxex GLenum GL_UNSIGNED_INT = 0x1405;
cxex GLenum GL_BLEND = 0x0BE2;
cxex GLenum GL_FUNC_ADD = 0x8006;
cxex GLenum GL_SRC_ALPHA = 0x0302;
cxex GLenum GL_ONE_MINUS_SRC_ALPHA = 0x0303;
cxex GLenum GL_DEPTH_TEST = 0x0B71;
cxex GLenum GL_LESS = 0x0201;
cxex GLint GL_LINEAR = 0x2601;
cxex GLint GL_LINEAR_MIPMAP_LINEAR = 0x2703;
cxex GLint GL_REPEAT = 0x2901;
cxex GLint GL_CLAMP_TO_EDGE = 0x812F;
cxex GLbitfield GL_COLOR_BUFFER_BIT = 0x00004000;
cxex GLbitfield GL_DEPTH_BUFFER_BIT = 0x00000100;
cxex GLbitfield GL_STENCIL_BUFFER_BIT = 0x00000400;
cxex GLbitfield GL_MAP_WRITE_BIT = 0x0002;
cxex GLbitfield GL_MAP_PERSISTENT_BIT = 0x0040;
cxex GLbitfield GL_MAP_COHERENT_BIT = 0x0080;
cxex GLboolean GL_FALSE = 0;
cxex GLboolean GL_TRUE = 1;
cxex int WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
cxex int WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
cxex int WGL_CONTEXT_FLAGS_ARB = 0x2094;
cxex int WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 0x0002;
cxex int WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;
cxex int WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

#ifdef PROG_SYS_WIN

// GL function types
using wglGetProcAddress_T = GAFN(GLAPI*)(cx CS* fnName); // non-standard format
using wglCreateContextAttribsARB_T = HGLRC(GLAPI*)(HDC hDC, HGLRC hShareContext, const int* attribList);
using glClear_T = void(GLAPI*)(GLbitfield mask);
using glClearColor_T = void(GLAPI*)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
using glDeleteBuffers_T = void(GLAPI*)(GLsizei n, const GLuint* buffers);
using glGenBuffers_T = void(GLAPI*)(GLsizei n, GLuint* buffers);
using glNamedBufferData_T = void(GLAPI*)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
using glBindBuffer_T = void(GLAPI*)(GLenum target, GLuint buffer);
using glDeleteVertexArrays_T = void(GLAPI*)(GLsizei n, const GLuint* arrays);
using glGenVertexArrays_T = void(GLAPI*)(GLsizei n, GLuint* arrays);
using glBindVertexArray_T = void(GLAPI*)(GLuint array);
using glDisableVertexArrayAttrib_T = void(GLAPI*)(GLuint vaobj, GLuint index);
using glVertexArrayVertexBuffer_T = void(GLAPI*)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
using glEnableVertexArrayAttrib_T = void(GLAPI*)(GLuint vaobj, GLuint index);
using glDeleteTextures_T = void(GLAPI*)(GLsizei n, const GLuint* textures);
using glCreateTextures_T = void(GLAPI*)(GLenum target, GLsizei n, GLuint* textures);
using glBindTextureUnit_T = void(GLAPI*)(GLuint unit, GLuint texture);
using glTextureStorage2D_T = void(GLAPI*)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
using glTextureSubImage2D_T = void(GLAPI*)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
using glGenerateTextureMipmap_T = void(GLAPI*)(GLuint texture);
using glTextureParameteri_T = void(GLAPI*)(GLuint texture, GLenum pname, GLint param);
using glDeleteShader_T = void(GLAPI*)(GLuint shader);
using glCreateShader_T = GLuint(GLAPI*)(GLenum shaderType);
using glShaderSource_T = void(GLAPI*)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
using glCompileShader_T = void(GLAPI*)(GLuint shader);
using glGetShaderiv_T = void(GLAPI*)(GLuint shader, GLenum pname, GLint* params);
using glDeleteProgram_T = void(GLAPI*)(GLuint program);
using glCreateProgram_T = GLuint(GLAPI*)(void);
using glAttachShader_T = void(GLAPI*)(GLuint program, GLuint shader);
using glLinkProgram_T = void(GLAPI*)(GLuint program);
using glGetProgramiv_T = void(GLAPI*)(GLuint program, GLenum pname, GLint* params);
using glValidateProgram_T = void(GLAPI*)(GLuint program);
using glDetachShader_T = void(GLAPI*)(GLuint program, GLuint shader);
using glUseProgram_T = void(GLAPI*)(GLuint program);
using glGetUniformLocation_T = GLint(GLAPI*)(GLuint program, const GLchar* name);
using glUniform1i_T = void(GLAPI*)(GLint location, GLint v0);
using glDrawElements_T = void(GLAPI*)(GLenum mode, GLsizei count, GLenum type, const void* indices);
using glViewport_T = void(GLAPI*)(GLint x, GLint y, GLsizei width, GLsizei height);
using glBindBufferBase_T = void(GLAPI*)(GLenum target, GLuint index, GLuint buffer);
using glBindBufferRange_T = void(GLAPI*)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
using glDrawElementsInstanced_T = void(GLAPI*)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
using glNamedBufferStorage_T = void(GLAPI*)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
using glEnable_T = void(GLAPI*)(GLenum cap);
using glBlendEquation_T = void(GLAPI*)(GLenum mode);
using glBlendFunc_T = void(GLAPI*)(GLenum sfactor, GLenum dfactor);
using glVertexArrayAttribFormat_T = void(GLAPI*)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
using glVertexArrayAttribBinding_T = void(GLAPI*)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
using glUniformMatrix4fv_T = void(GLAPI*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
using glDepthFunc_T = void(GLAPI*)(GLenum func);
using glDepthMask_T = void(GLAPI*)(GLboolean flag);
using glClearDepth_T = void(GLAPI*)(GLdouble depth);
using glDisable_T = void(GLAPI*)(GLenum cap);

#endif

namespace gl
{

cxex SI PRE_LOAD_FN_CNT = 2;
cxex SI LOAD_FN_CNT = 52;
extern GAFN g_loadFn[LOAD_FN_CNT];
extern BO g_isLoaded;

inl cxex cx CS* g_loadFnName[LOAD_FN_CNT] = {
    "wglGetProcAddress",
    "wglCreateContextAttribsARB",
    "glClear",
    "glClearColor",
    "glDeleteBuffers",
    "glGenBuffers",
    "glNamedBufferData",
    "glBindBuffer",
    "glDeleteVertexArrays",
    "glGenVertexArrays",
    "glBindVertexArray",
    "glDisableVertexArrayAttrib",
    "glVertexArrayVertexBuffer",
    "glEnableVertexArrayAttrib",
    "glDeleteTextures",
    "glCreateTextures",
    "glBindTextureUnit",
    "glTextureStorage2D",
    "glTextureSubImage2D",
    "glGenerateTextureMipmap",
    "glTextureParameteri",
    "glDeleteShader",
    "glCreateShader",
    "glShaderSource",
    "glCompileShader",
    "glGetShaderiv",
    "glDeleteProgram",
    "glCreateProgram",
    "glAttachShader",
    "glLinkProgram",
    "glGetProgramiv",
    "glValidateProgram",
    "glDetachShader",
    "glUseProgram",
    "glGetUniformLocation",
    "glUniform1i",
    "glDrawElements",
    "glViewport",
    "glBindBufferBase",
    "glBindBufferRange",
    "glDrawElementsInstanced",
    "glNamedBufferStorage",
    "glEnable",
    "glBlendEquation",
    "glBlendFunc",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribBinding",
    "glUniformMatrix4fv",
    "glDepthFunc",
    "glDepthMask",
    "glClearDepth",
    "glDisable",
};

} // namespace gl

#define wglGetProcAddress reinterpret_cast<wglGetProcAddress_T>(gl::g_loadFn[0])
#define wglCreateContextAttribsARB reinterpret_cast<wglCreateContextAttribsARB_T>(gl::g_loadFn[1])
#define glClear reinterpret_cast<glClear_T>(gl::g_loadFn[2])
#define glClearColor reinterpret_cast<glClearColor_T>(gl::g_loadFn[3])
#define glDeleteBuffers reinterpret_cast<glDeleteBuffers_T>(gl::g_loadFn[4])
#define glGenBuffers reinterpret_cast<glGenBuffers_T>(gl::g_loadFn[5])
#define glNamedBufferData reinterpret_cast<glNamedBufferData_T>(gl::g_loadFn[6])
#define glBindBuffer reinterpret_cast<glBindBuffer_T>(gl::g_loadFn[7])
#define glDeleteVertexArrays reinterpret_cast<glDeleteVertexArrays_T>(gl::g_loadFn[8])
#define glGenVertexArrays reinterpret_cast<glGenVertexArrays_T>(gl::g_loadFn[9])
#define glBindVertexArray reinterpret_cast<glBindVertexArray_T>(gl::g_loadFn[10])
#define glDisableVertexArrayAttrib reinterpret_cast<glDisableVertexArrayAttrib_T>(gl::g_loadFn[11])
#define glVertexArrayVertexBuffer reinterpret_cast<glVertexArrayVertexBuffer_T>(gl::g_loadFn[12])
#define glEnableVertexArrayAttrib reinterpret_cast<glEnableVertexArrayAttrib_T>(gl::g_loadFn[13])
#define glDeleteTextures reinterpret_cast<glDeleteTextures_T>(gl::g_loadFn[14])
#define glCreateTextures reinterpret_cast<glCreateTextures_T>(gl::g_loadFn[15])
#define glBindTextureUnit reinterpret_cast<glBindTextureUnit_T>(gl::g_loadFn[16])
#define glTextureStorage2D reinterpret_cast<glTextureStorage2D_T>(gl::g_loadFn[17])
#define glTextureSubImage2D reinterpret_cast<glTextureSubImage2D_T>(gl::g_loadFn[18])
#define glGenerateTextureMipmap reinterpret_cast<glGenerateTextureMipmap_T>(gl::g_loadFn[19])
#define glTextureParameteri reinterpret_cast<glTextureParameteri_T>(gl::g_loadFn[20])
#define glDeleteShader reinterpret_cast<glDeleteShader_T>(gl::g_loadFn[21])
#define glCreateShader reinterpret_cast<glCreateShader_T>(gl::g_loadFn[22])
#define glShaderSource reinterpret_cast<glShaderSource_T>(gl::g_loadFn[23])
#define glCompileShader reinterpret_cast<glCompileShader_T>(gl::g_loadFn[24])
#define glGetShaderiv reinterpret_cast<glGetShaderiv_T>(gl::g_loadFn[25])
#define glDeleteProgram reinterpret_cast<glDeleteProgram_T>(gl::g_loadFn[26])
#define glCreateProgram reinterpret_cast<glCreateProgram_T>(gl::g_loadFn[27])
#define glAttachShader reinterpret_cast<glAttachShader_T>(gl::g_loadFn[28])
#define glLinkProgram reinterpret_cast<glLinkProgram_T>(gl::g_loadFn[29])
#define glGetProgramiv reinterpret_cast<glGetProgramiv_T>(gl::g_loadFn[30])
#define glValidateProgram reinterpret_cast<glValidateProgram_T>(gl::g_loadFn[31])
#define glDetachShader reinterpret_cast<glDetachShader_T>(gl::g_loadFn[32])
#define glUseProgram reinterpret_cast<glUseProgram_T>(gl::g_loadFn[33])
#define glGetUniformLocation reinterpret_cast<glGetUniformLocation_T>(gl::g_loadFn[34])
#define glUniform1i reinterpret_cast<glUniform1i_T>(gl::g_loadFn[35])
#define glDrawElements reinterpret_cast<glDrawElements_T>(gl::g_loadFn[36])
#define glViewport reinterpret_cast<glViewport_T>(gl::g_loadFn[37])
#define glBindBufferBase reinterpret_cast<glBindBufferBase_T>(gl::g_loadFn[38])
#define glBindBufferRange reinterpret_cast<glBindBufferRange_T>(gl::g_loadFn[39])
#define glDrawElementsInstanced reinterpret_cast<glDrawElementsInstanced_T>(gl::g_loadFn[40])
#define glNamedBufferStorage reinterpret_cast<glNamedBufferStorage_T>(gl::g_loadFn[41])
#define glEnable reinterpret_cast<glEnable_T>(gl::g_loadFn[42])
#define glBlendEquation reinterpret_cast<glBlendEquation_T>(gl::g_loadFn[43])
#define glBlendFunc reinterpret_cast<glBlendFunc_T>(gl::g_loadFn[44])
#define glVertexArrayAttribFormat reinterpret_cast<glVertexArrayAttribFormat_T>(gl::g_loadFn[45])
#define glVertexArrayAttribBinding reinterpret_cast<glVertexArrayAttribBinding_T>(gl::g_loadFn[46])
#define glUniformMatrix4fv reinterpret_cast<glUniformMatrix4fv_T>(gl::g_loadFn[47])
#define glDepthFunc reinterpret_cast<glDepthFunc_T>(gl::g_loadFn[48])
#define glDepthMask reinterpret_cast<glDepthMask_T>(gl::g_loadFn[49])
#define glClearDepth reinterpret_cast<glClearDepth_T>(gl::g_loadFn[50])
#define glDisable reinterpret_cast<glDisable_T>(gl::g_loadFn[51])

namespace gl
{

#ifdef PROG_SYS_WIN

dfa ER Load();

#endif

} // namespace gl

#include "glload.ipp"

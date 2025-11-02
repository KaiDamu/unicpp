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
using GLbitfield = U4;
using GLfloat = F4;

// GL constants
cxex GLbitfield GL_COLOR_BUFFER_BIT = 0x00004000;
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

#endif

namespace gl
{

cxex SI PRE_LOAD_FN_CNT = 2;
cxex SI LOAD_FN_CNT = 4;
extern GAFN g_loadFn[LOAD_FN_CNT];
extern BO g_isLoaded;

inl cxex cx CS* g_loadFnName[LOAD_FN_CNT] = {
    "wglGetProcAddress",
    "wglCreateContextAttribsARB",
    "glClear",
    "glClearColor",
};

} // namespace gl

#define wglGetProcAddress reinterpret_cast<wglGetProcAddress_T>(gl::g_loadFn[0])
#define wglCreateContextAttribsARB reinterpret_cast<wglCreateContextAttribsARB_T>(gl::g_loadFn[1])
#define glClear reinterpret_cast<glClear_T>(gl::g_loadFn[2])
#define glClearColor reinterpret_cast<glClearColor_T>(gl::g_loadFn[3])

namespace gl
{

#ifdef PROG_SYS_WIN

dfa ER Load();

#endif

} // namespace gl

#include "glload.ipp"

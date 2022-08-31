#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "GL/wgl.h"

#include <windows.h>
#include <windowsx.h>

static HMODULE s_module = NULL;

struct ContextWindowInternalGL
{
  HDC   m_hDCaffinity = NULL;
  HDC   m_hDC         = NULL;
  HGLRC m_hRC         = NULL;

  PFNWGLSWAPINTERVALEXTPROC        m_wglSwapIntervalEXT        = NULL;
  PFNWGLGETEXTENSIONSSTRINGARBPROC m_wglGetExtensionsStringARB = NULL;
  PFNWGLDELETEDCNVPROC             m_wglDeleteDCNV             = NULL;

  bool init(GLFWwindow* sourcewindow/*, ContextWindow* ctxwindow*/)
  {
    GLuint    PixelFormat;
    HWND      hWnd      = glfwGetWin32Window(sourcewindow);
    HINSTANCE hInstance = GetModuleHandle(NULL);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.cStencilBits = 8;

    
    m_hDC       = GetDC(hWnd);
    PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    SetPixelFormat(m_hDC, PixelFormat, &pfd);

    m_hRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hRC);

    HDC hdcContext = m_hDC;

    PFNWGLCREATECONTEXTATTRIBSARBPROC fn_wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if(fn_wglCreateContextAttribsARB)
    {
      HGLRC            hRC = NULL;
      std::vector<int> attribList;
#define ADDATTRIB(a, b)                                                                                                \
  {                                                                                                                    \
    attribList.push_back(a);                                                                                           \
    attribList.push_back(b);                                                                                           \
  }
      int maj = 4;
      int min = 1;
      ADDATTRIB(WGL_CONTEXT_MAJOR_VERSION_ARB, maj)
      ADDATTRIB(WGL_CONTEXT_MINOR_VERSION_ARB, min)

      ADDATTRIB(WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB)
      //ADDATTRIB(WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB)

      int ctxtflags = 0;
      ctxtflags |= WGL_CONTEXT_DEBUG_BIT_ARB;

      ADDATTRIB(WGL_CONTEXT_FLAGS_ARB, ctxtflags);
      ADDATTRIB(0, 0)
      int* p = &(attribList[0]);
      if(!(hRC = fn_wglCreateContextAttribsARB(hdcContext, 0, p)))
      {
        GLint MajorVersionContext = 0;
        GLint MinorVersionContext = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &MajorVersionContext);
        glGetIntegerv(GL_MINOR_VERSION, &MinorVersionContext);
        if((MajorVersionContext * 100 + MinorVersionContext * 10) < (maj * 100 + min * 10))
        {
          //LOGE("OpenGL version %d.%d not available. Only %d.%d found\n", maj, min, MajorVersionContext, MinorVersionContext);
        }
        //LOGE("wglCreateContextAttribsARB() failed for OpenGL context.\n");
        return false;
      }
      if(!wglMakeCurrent(m_hDC, hRC))
      {
        //LOGE("wglMakeCurrent() failed for OpenGL context.\n");
      }
      else
      {
        wglDeleteContext(m_hRC);
        m_hRC = hRC;
#ifdef _DEBUG

        //PFNGLDEBUGMESSAGECALLBACKARBPROC fn_glDebugMessageCallbackARB =
        //    (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
        //PFNGLDEBUGMESSAGECONTROLARBPROC fn_glDebugMessageControlARB =
        //    (PFNGLDEBUGMESSAGECONTROLARBPROC)wglGetProcAddress("glDebugMessageControlARB");

        // Because no ctxwindow
        //if(fn_glDebugMessageCallbackARB)
        //{
        //  glEnable(GL_DEBUG_OUTPUT);
        //  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        //  fn_glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        //  fn_glDebugMessageCallbackARB(myOpenGLCallback, ctxwindow);
        //}
#endif
      }
    }
    m_wglSwapIntervalEXT        = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    m_wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

    if(!s_module)
    {
      s_module = LoadLibraryA("opengl32.dll");
    }

    return true;
  }

  void deinit()
  {
    wglDeleteContext(m_hRC);
    if(m_hDCaffinity)
    {
      m_wglDeleteDCNV(m_hDCaffinity);
    }
  }
};


// Not called in SSAO sample
//void ContextWindow::makeContextCurrent()
//{
//  wglMakeCurrent(m_internal->m_hDC, m_internal->m_hRC);
//}
//
//void ContextWindow::makeContextNonCurrent()
//{
//  wglMakeCurrent(0, 0);
//}
//
// Directly called by main
//void ContextWindow::swapInterval(int i)
//{
//  m_internal->m_wglSwapIntervalEXT(i);
//}
//
// Directly called by main
//void ContextWindow::swapBuffers()
//{
//  SwapBuffers(m_internal->m_hDC);
//}

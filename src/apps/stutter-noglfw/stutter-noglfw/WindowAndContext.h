#define NOMINMAX
#include <windows.h>
#include <GL/GL.h>

#include "wglext.h"

#include <stdexcept>

#include <cassert>

#pragma comment (lib, "opengl32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HGLRC gOpenGLRenderingContext;
HDC gWindowHandleToDeviceContext;

// Loading OpenGL functions
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

HWND makeWindow()
{
    WNDCLASS wc      = {0}; 
    wc.lpfnWndProc   = WndProc;
    // Adapated to get the hinstance without winmain entrypoint
    HINSTANCE hInstance = GetModuleHandle(NULL);
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "oglversionchecksample";
    wc.style = CS_OWNDC; // Important
	if( !RegisterClass(&wc) )
	{
		throw std::logic_error{"Something something window registration failed."};
	}
	return CreateWindowA(
		wc.lpszClassName,
		"stutter-test native win32",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		0,0,640,480,
		0,
		0,
		hInstance,
		0);
}


bool serviceWindow()
{
    MSG msg          = {0};

    //while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
	//{
	//	DispatchMessage( &msg );
	//}

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        if (msg.message == WM_QUIT)
        {
			return false;
		}
	}
	return true;
}


void swapBuffers()
{
	SwapBuffers(gWindowHandleToDeviceContext);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		gWindowHandleToDeviceContext = GetDC(hWnd);

		int  letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(gWindowHandleToDeviceContext, &pfd); 
		SetPixelFormat(gWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

		gOpenGLRenderingContext = wglCreateContext(gWindowHandleToDeviceContext);
		wglMakeCurrent (gWindowHandleToDeviceContext, gOpenGLRenderingContext);

		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
		assert(wglSwapIntervalEXT);

		// This would be a blocking call showing the opengl version
		//MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

		//PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		//wglMakeCurrent(gWindowHandleToDeviceContext, NULL); Unnecessary; wglDeleteContext will make the context not current
		wglDeleteContext(gOpenGLRenderingContext);
		PostQuitMessage(0);
        return DefWindowProc(hWnd, message, wParam, lParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
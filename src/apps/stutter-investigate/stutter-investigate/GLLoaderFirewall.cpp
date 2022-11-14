#include "GLLoaderFirewall.h"

#include "nvgl/extensions_gl.hpp"


void* myGetProcAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    return p;
}


void myLoadGL()
{
    load_GL(&myGetProcAddress);
}

// Damn windows.h
#define NOMINMAX

// On Windows, also use the default GLFW
#define GLFW_WIN_CONTEXT
// Issue the same command sequence than in NVPRO sample to glClear() the backbuffer
//#define NVPRO_CLEAR
// Use nvpro approach to loading OpenGL symbols, instead of Glad.
// WARNING: This prevents the balls from rendering.
//#define NVPRO_GLLOADER
// Go fullscreen?
//#define FULLSCREEN

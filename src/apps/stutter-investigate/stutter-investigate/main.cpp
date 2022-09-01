////
//// Build controls
//// 
#include "BuildConfig.h"

#include "Bawls.h"
#include "Metrics.h"
#include "Timing.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <nvtx3/nvToolsExt.h>

#include <functional>

#include <cstdlib>


#ifdef _WIN32
#include "ContextWindows.h"
#endif

// Cannot include both glfw and some of the required headers
#include "GLLoaderFirewall.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


struct RangeNvtx
{
    RangeNvtx(const char * aName)
    {
        nvtxRangePush(aName);
    }

    RangeNvtx(const std::string & aName) :
        RangeNvtx(aName.c_str())
    {}

    ~RangeNvtx()
    {
        nvtxRangePop();
    }
};


/// Test bed to develop a gameloop which allows to decouple update rate from render rate.
/// Simulates balls bouncing around, with collisions on window borders and between balls.
struct Game
{
    void run()
    {
        glfwInit();

#if defined(GLFW_WIN_CONTEXT)
        // Added from nvpro
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // macOS requirement (as well as not going above to OpenGL 4.1)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        ContextWindowInternalGL ctx;
#endif

#if defined(FULLSCREEN)
        auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * videomode = glfwGetVideoMode(monitor);
        GLFWwindow * glfwWindow = glfwCreateWindow(videomode->width, videomode->height,
                                                   gAppName.c_str(), monitor, NULL);
#else
        GLFWwindow * glfwWindow = glfwCreateWindow(gWindowResolution.width(), gWindowResolution.height(),
                                                   gAppName.c_str(), NULL, NULL);
#endif

        glfwSetKeyCallback(glfwWindow, key_callback);

#if defined(GLFW_WIN_CONTEXT)
        glfwMakeContextCurrent(glfwWindow);
#else
        ctx.init(glfwWindow);
#endif

#if defined(NVPRO_GLLOADER)
        myLoadGL();
#else
        gladLoadGL();
#endif
        

        // VSync
#if defined(GLFW_WIN_CONTEXT)
        glfwSwapInterval(1);
#else
        ctx.m_wglSwapIntervalEXT(1);
#endif

        ad::Bawls scene{gWindowResolution};

        using ad::Metrics;

        ad::Timer timer;
        ad::TimePoint currentTime = timer.now();
        double frameTime = 0.;
        ad::Metrics metrics;

        double accumulator = 0.;

        while(! glfwWindowShouldClose(glfwWindow))
        {
            RangeNvtx range("Frame");
            accumulator += frameTime;

            //metrics.update(frameTime);

            // Simulation
            {
                RangeNvtx range("Simulation");
                scene.update(frameTime);
            }
            // Rendering
            {
                {
                    RangeNvtx range("Graphics");
                    auto p = metrics.probe(Metrics::Render);
                    {
                        RangeNvtx range("Clear");
#if defined(NVPRO_CLEAR)
                        // The way it is done in nvpro samples (does not change the glClear stutter problem though.
                        GLfloat bgColor[4]{0.2, 0.2, 0.2, 0.0};
                        {
                            RangeNvtx range("glClearBufferfv");
                            glClearBufferfv(GL_COLOR, 0, bgColor);
                        }
                        {
                            RangeNvtx range("glClearDepth");
                            glClearDepth(1.0);
                        }
                        {
                            RangeNvtx range("glClear");
                            glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                        }
                        {
                            RangeNvtx range("glEnable(GL_DEPTH_TEST)");
                            glEnable(GL_DEPTH_TEST);
                        }
#else
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#endif
                    }
                    {
                        RangeNvtx range("Render");
                        scene.render();
                    }
                }
                {
                    auto probe = metrics.probe(Metrics::Swap);
                    RangeNvtx range("Swap-call");
#if defined(GLFW_WIN_CONTEXT)
                    glfwSwapBuffers(glfwWindow);
#else
                    SwapBuffers(ctx.m_hDC);
#endif
                }
            }

            ad::TimePoint endFrameTime = timer.now();
            frameTime = endFrameTime - currentTime;
            currentTime = endFrameTime;

            metrics.record(Metrics::Frame, frameTime);
            metrics.record(Metrics::FPS, 1/frameTime);

            glfwPollEvents();
        }

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    inline static const std::string gAppName{"Gameloop decouple"};
    inline static const ad::math::Size<2, int> gWindowResolution{800, 600};
};

int main()
{
    try
    {
        Game game;
        game.run();
    }
    catch (const std::exception & aException)
    {
        std::cerr << "Exception reached the top level:\n"
            << aException.what()
            << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

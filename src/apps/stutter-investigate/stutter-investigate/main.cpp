#include "Bawls.h"
#include "Metrics.h"
#include "Timing.h"

#include <GLFW/glfw3.h>

#include <functional>

#include <cstdlib>

#ifdef _WIN32
#include "ContextWindows.h"
#endif


#define GLFW_WIN_CONTEXT


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


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

        GLFWwindow * glfwWindow = glfwCreateWindow(gWindowResolution.width(), gWindowResolution.height(),
                                                   gAppName.c_str(), NULL, NULL);

        glfwSetKeyCallback(glfwWindow, key_callback);

#if defined(GLFW_WIN_CONTEXT)
        glfwMakeContextCurrent(glfwWindow);
#else
        ctx.init(glfwWindow);
#endif
        gladLoadGL();

        // VSync
#if defined(GLFW_WIN_CONTEXT)
        glfwSwapInterval(1);
#else
        ctx.m_wglSwapIntervalEXT(1);
#endif


        ad::Bawls scene{gWindowResolution};

        using ad::Metrics;

        static constexpr double gSimulationDelta = 1./100.;

        ad::Timer timer;
        ad::TimePoint currentTime = timer.now();
        double frameTime = 0.;
        ad::Metrics metrics;

        double accumulator = 0.;

        while(! glfwWindowShouldClose(glfwWindow))
        {
            accumulator += frameTime;

            metrics.update(frameTime);

            // Simulation
            {
                auto probe = metrics.probe(Metrics::Simulation);
                int steps = 0;
                while (accumulator >= gSimulationDelta)
                {
                    auto probe = metrics.probe(Metrics::Update);
                    scene.update(gSimulationDelta);
                    accumulator -= gSimulationDelta;
                    ++steps;
                }
                metrics.record(Metrics::Steps, steps);
            }
            // Rendering
            {
                {
                    auto p = metrics.probe(Metrics::Render);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                    scene.render();
                }
                {
                    auto probe = metrics.probe(Metrics::Swap);
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

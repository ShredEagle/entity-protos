#include "Bawls.h"
#include "Metrics.h"
#include "Timing.h"

#include <GLFW/glfw3.h>

#include <functional>

#include <cstdlib>


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

        // Added from nvpro
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // macOS requirement (as well as not going above to OpenGL 4.1)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow * glfwWindow = glfwCreateWindow(gWindowResolution.width(), gWindowResolution.height(),
                                                   gAppName.c_str(), NULL, NULL);

        glfwSetKeyCallback(glfwWindow, key_callback);

        glfwMakeContextCurrent(glfwWindow);
        gladLoadGL();

        // VSync
        glfwSwapInterval(1);

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
                    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Imgui");
                    glPopDebugGroup();
                }
                {
                    auto probe = metrics.probe(Metrics::Swap);
                    glfwSwapBuffers(glfwWindow);
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

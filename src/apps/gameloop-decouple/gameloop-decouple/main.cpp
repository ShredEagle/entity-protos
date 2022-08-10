#include "Bawls.h"
#include "Metrics.h"
#include "Timing.h"

#include <graphics/ApplicationGlfw.h>

#include <imguiui/ImguiUi.h>

#include <functional>

#include <cstdlib>


/// Test bed to develop a gameloop which allows to decouple update rate from render rate.
/// Simulates balls bouncing around, with collisions on window borders and between balls.
struct Game
{
    Game()
    {
        using namespace std::placeholders;
        application.getAppInterface()
            ->registerKeyCallback(std::bind(&Game::handleKeyboard, this, _1, _2, _3, _4));
    }


    void prepareMenu(const ad::Metrics & aMetrics)
    {
        ImGui::Begin("Frame metrics");
        for (const auto & message : aMetrics.messages)
        {
            ImGui::Text(message.c_str());
        }
        ImGui::End();
    }

    void run()
    {
        using ad::Metrics;

        static constexpr double gSimulationDelta = 1./100.;

        ad::Timer timer;
        ad::TimePoint currentTime = timer.now();
        double frameTime = 0.;
        ad::Metrics metrics;

        double accumulator = 0.;

        while(application.handleEvents())
        {
            accumulator += frameTime;

            imgui.newFrame();
            //bool b = true;
            //ImGui::ShowDemoWindow(&b);

            metrics.update(frameTime);
            prepareMenu(metrics);

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
                    application.getAppInterface()->clear();
                    scene.render();
                }
                imgui.render();
                {
                    auto probe = metrics.probe(Metrics::Swap);
                    application.swapBuffers();
                }
            }

            ad::TimePoint endFrameTime = timer.now();
            frameTime = endFrameTime - currentTime;
            currentTime = endFrameTime;

            metrics.record(Metrics::Frame, frameTime);
            metrics.record(Metrics::FPS, 1/frameTime);

        }
    }

    void handleKeyboard(int key, int, int action, int)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            application.getAppInterface()->requestCloseApplication();
        }
    };

    inline static const std::string gAppName{"Gameloop decouple"};

    ad::graphics::ApplicationGlfw application{
        gAppName,
        800, 600,
        ad::graphics::ApplicationFlag::Fullscreen};
    ad::Bawls scene{*application.getAppInterface()};
    ad::imguiui::ImguiUi imgui{application};
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

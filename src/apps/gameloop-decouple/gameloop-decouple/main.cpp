#include "Bawls.h"
#include "Timing.h"

#include <graphics/ApplicationGlfw.h>

#include <imguiui/ImguiUi.h>

#include <functional>

#include <cstdlib>


/// Test bed to develop a gameloop which allows to decouple update rate from render rate.
/// Simulates balls bouncing around, with collisions on window borders and between balls.

namespace ad {

struct Metrics
{
    struct Entry
    {
        std::string present() const
        {
            return data.first + " time: " + std::to_string(data.second);
        }

        std::pair<std::string, double> data;
    };

    struct [[nodiscard]] Probe
    {
        Probe(Entry & aEntry) :
            entry{aEntry}
        {}

        ~Probe()
        {
            entry.data.second = Timer::now() - start;
        }

        Entry & entry;
        TimePoint start{Timer::now()};
    };

    void clear()
    {
        entries.clear();
    }

    void push(std::string aLabel, double aTime)
    {
        entries.push_back({.data = std::make_pair(std::move(aLabel), aTime)});
    }

    Probe probe(std::string aLabel)
    {
        push(std::move(aLabel), -1.);
        return Probe{entries.back()};
    }

    std::vector<Entry> entries;
};

} // namespace ad

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
        ImGui::Begin("Previous frame metrics");
        for (const auto & entry : aMetrics.entries)
        {
            ImGui::Text(entry.present().c_str());
        }
        ImGui::End();
    }

    void run()
    {
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

            prepareMenu(metrics);
            metrics.clear();

            // Simuation
            {
                auto probe = metrics.probe("Update");
                int steps = 0;
                while (accumulator >= gSimulationDelta)
                {
                    scene.update(gSimulationDelta);
                    accumulator -= gSimulationDelta;
                    ++steps;
                }
                probe.entry.data.first += std::to_string(steps);
            }
            // Rendering
            {
                {
                    auto p = metrics.probe("Render");
                    application.getAppInterface()->clear();
                    scene.render();
                }
                imgui.render();
                {
                    auto probe = metrics.probe("Swap");
                    application.swapBuffers();
                }
            }

            ad::TimePoint endFrameTime = timer.now();
            frameTime = endFrameTime - currentTime;
            currentTime = endFrameTime;

            metrics.push("Frame", frameTime);
            metrics.push("FPS", 1/frameTime);

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

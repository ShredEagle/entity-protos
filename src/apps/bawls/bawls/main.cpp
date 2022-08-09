#include "Bawls.h"

#include <graphics/ApplicationGlfw.h>

#include <utils/Timer.h>

#include <functional>

#include <cstdlib>


/// Simulates balls bouncing around, with collisions on window borders and between balls.
/// Controls:
/// * space: toggle pausing simulation.
/// While paused:
/// * left-arrow: rewind to previous saved state.
/// * right-arrow: advance to next saved state.
struct Game
{
    Game()
    {
        using namespace std::placeholders;
        application.getAppInterface()
            ->registerKeyCallback(std::bind(&Game::handleKeyboard, this, _1, _2, _3, _4));
    }

    void run()
    {
        timer = ad::Timer{};
        while(application.handleEvents())
        {
            if(running)
            {
                scene.update(timer.mark());
                application.getAppInterface()->clear();
                scene.render();
                application.swapBuffers();
            }
        }
    }

    void handleKeyboard(int key, int, int action, int)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            application.getAppInterface()->requestCloseApplication();
        }
        else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            running = !running; 
            if(running)
            {
                timer.unpause();
            }
            else
            {
                timer.pause();
            }
        }
        else if(
            (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) 
            && action == GLFW_PRESS && !running)
        {
            key == GLFW_KEY_LEFT ? scene.restorePrevious() : scene.restoreNext();
            application.getAppInterface()->clear();
            scene.redraw();
            application.swapBuffers();
        }
    };

    inline static const std::string gAppName{"Bawls"};

    ad::graphics::ApplicationGlfw application{
        gAppName,
        800, 600,
        ad::graphics::ApplicationFlag::Window_Keep_Ratio};
    ad::Bawls scene{*application.getAppInterface()};
    ad::Timer timer;
    bool running{true};
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

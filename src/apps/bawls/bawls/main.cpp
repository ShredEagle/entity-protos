#include "Bawls.h"
#include "Timer.h"

#include <graphics/ApplicationGlfw.h>

#include <cstdlib>

int main()
{
    try 
    {
        const std::string appName = "Bawls";

        ad::graphics::ApplicationGlfw application(
            appName,
            800, 600,
            ad::graphics::ApplicationFlag::Window_Keep_Ratio);

        ad::Bawls scene{*application.getAppInterface()};
        ad::Timer timer;

        while(application.handleEvents())
        {
            scene.update(timer.mark());
            application.getAppInterface()->clear();
            scene.render();
            application.swapBuffers();
        }
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

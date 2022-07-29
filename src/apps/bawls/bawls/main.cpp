#include "Bawls.h"

#include <graphics/ApplicationGlfw.h>

#include <cstdlib>

int main()
{
    const std::string appName = "Bawls";

    ad::graphics::ApplicationGlfw application(
        appName,
        800, 600);

    while(application.handleEvents())
    {
        application.swapBuffers();
    }

    return EXIT_SUCCESS;
}

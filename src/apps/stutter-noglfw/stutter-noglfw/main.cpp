#include "Timing.h"

#ifdef _WIN32
#include "WindowAndContext.h"
#else
static_assert(false)
#endif

#include <math/Vector.h>

#include <nvtx3/nvToolsExt.h>

#include <functional>

#include <cstdlib>


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


int main()
{
    try
    {
        ad::Timer timer;
        ad::TimePoint currentTime = timer.now();
        ad::TimePoint startTime = currentTime;
        double frameTime = 0.;
        const double gSpeed = 0.2;

        HWND window = makeWindow();
        wglSwapIntervalEXT(1);
        while(serviceWindow())
        {
            glClearColor(std::fmod((currentTime - startTime) * gSpeed, 1.), 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);
            swapBuffers();

            ad::TimePoint endFrameTime = timer.now();
            frameTime = endFrameTime - currentTime;
            currentTime = endFrameTime;
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

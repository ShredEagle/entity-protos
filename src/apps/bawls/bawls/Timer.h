#pragma once

#include <chrono>


namespace ad {


class Timer
{
    using Clock_t = std::chrono::high_resolution_clock;

public:
    Timer() :
        mTime{Clock_t::now()}
    {}

    double mark()
    {
        auto now = Clock_t::now();
        mDelta = (now - mTime).count() * gTickPeriod;
        mTime = now;
        return mDelta;
    }

    double delta() const
    {
        return mDelta;
    }


private:
    static constexpr double gTickPeriod{double(Clock_t::period::num) / Clock_t::period::den};
    std::chrono::time_point<Clock_t> mTime;
    double mDelta{0.};
};


} // namespace ad

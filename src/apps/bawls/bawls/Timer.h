#pragma once

#include <chrono>


namespace ad {


class Timer
{
    using Clock = std::chrono::high_resolution_clock;

public:
    using TimePoint = Clock::time_point;

    Timer() :
        mWallTime{Clock::now()},
        mSimulationTime{mWallTime}
    {}

    double mark()
    {
        mWallTime = now();
        auto newSimulationTime = mWallTime - mCumulativePause;
        mDelta = (newSimulationTime - mSimulationTime).count() * gTickPeriod;
        mSimulationTime = newSimulationTime;
        return mDelta;
    }

    void pause()
    {
        mPauseStart = now();
    }

    void unpause()
    {
        mCumulativePause += now() - mPauseStart;
    }

    double delta() const
    {
        return mDelta;
    }

    TimePoint simulationTime() const
    {
        return mSimulationTime;
    }

    TimePoint wallTime() const
    {
        return mWallTime;
    }

private:
    std::chrono::time_point<Clock> now()
    {
        return Clock::now();
    }

    static constexpr double gTickPeriod{double(Clock::period::num) / Clock::period::den};
    std::chrono::time_point<Clock> mWallTime;
    std::chrono::time_point<Clock> mSimulationTime;
    std::chrono::time_point<Clock> mPauseStart;
    Clock::duration mCumulativePause{0};
    double mDelta{0.};
};


} // namespace ad

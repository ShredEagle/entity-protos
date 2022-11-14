#pragma once


#include <chrono>


namespace ad {

class TimePoint;

class Timer
{
public:
    using Clock = std::chrono::high_resolution_clock;
    static constexpr double gTickPeriod{double(Clock::period::num) / Clock::period::den};

    static TimePoint now();
};


class TimePoint : public Timer::Clock::time_point
{
public :
    using Timer::Clock::time_point::time_point;

    double operator-(TimePoint aRhs) const
    { 
        return (parent() - aRhs.parent()).count() * Timer::gTickPeriod; 
    }

private:
    Timer::Clock::time_point & parent()
    {
        return *this;
    }

    Timer::Clock::time_point parent() const
    {
        return *this;
    }
};


//
// Implementations
//
inline TimePoint Timer::now()
{
    return Clock::now();
}


} // namespace ad

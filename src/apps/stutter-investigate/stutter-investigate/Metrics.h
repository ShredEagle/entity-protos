#pragma once


#include "Timing.h"

#include <array>
#include <string>

namespace ad {


struct Metrics
{
    enum Measure
    {
        Simulation,
        Update,
        Steps,
        Render,
        Swap,
        Frame,
        FPS,

        _end//
    };

    std::string to_string(Measure aMeasure)
    {
        switch(aMeasure)
        {
        case Simulation:
            return "Simulation";
        case Update:
            return "Update";
        case Steps:
            return "Steps";
        case Render:
            return "Render";
        case Swap:
            return "Swap";
        case Frame:
            return "Frame";
        case FPS:
            return "FPS";
        default:
            return "COMPLETE ME!!";
        }
    }
    
    struct Entry
    {
        std::string present() const
        {
            std::ostringstream oss;
            oss << accumulation / samples
                << " (" << min << "; " << max << ")";
            return oss.str();
        }

        void record(double aValue)
        {
            ++samples;
            accumulation += aValue;
            min = std::min(aValue, min);
            max = std::max(aValue, max);
        }

        void reset()
        {
            *this = Entry();
        }

        std::size_t samples{0};
        double accumulation{0.};
        double min{std::numeric_limits<double>::max()};
        double max{0.};
    };

    struct [[nodiscard]] Probe
    {
        Probe(Entry & aEntry) :
            entry{aEntry}
        {}

        ~Probe()
        {
            entry.record(Timer::now() - start);
        }

        Entry & entry;
        TimePoint start{Timer::now()};
    };

    void update(double aDelta)
    {
        duration += aDelta;
        while (duration > gPeriod)
        {
            for(std::size_t measure = 0; measure != Metrics::_end; ++measure)
            {
                messages[measure] = to_string((Measure)measure) + ": " + entries[measure].present();
                entries[measure].reset();
            }
            duration -= gPeriod;
        }
    };

    void record(Measure aMeasure, double aValue)
    {
        entries[aMeasure].record(aValue);
    }

    Probe probe(Measure aMeasure)
    {
        return Probe{entries[aMeasure]};
    }

    static constexpr double gPeriod = 2.;
    double duration{0.};
    std::array<Entry, Metrics::_end> entries;
    std::array<std::string, Metrics::_end> messages;
};


} // namespace ad

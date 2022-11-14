#pragma once

#include <math/Vector.h>


namespace ad {
namespace component {


struct Geometry
{
    math::Position<2, float> previousPosition{0.f, 0.f};
    math::Position<2, float> position;
    float radius{1.f};

    float mass() const
    { return std::pow(radius, 2); }
};


} // namespace component
} // namespace ad

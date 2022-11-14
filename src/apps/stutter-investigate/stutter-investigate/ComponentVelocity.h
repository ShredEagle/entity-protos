#pragma once

#include <math/Vector.h>


namespace ad {
namespace component {


struct Velocity : public math::Vec<2, float>
{
    using math::Vec<2, float>::Vec;
};


} // namespace component
} // namespace ad

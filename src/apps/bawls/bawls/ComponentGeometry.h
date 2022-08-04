#pragma once

#include <renderer/GL_Loader.h>

#include <math/Vector.h>


namespace ad {
namespace component {


struct Geometry
{
    math::Position<2, GLfloat> position;
    GLfloat radius{1.f};
};


} // namespace component
} // namespace ad

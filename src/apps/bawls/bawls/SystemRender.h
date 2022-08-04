#pragma once

#include "ComponentGeometry.h"

#include <entity/Query.h>

#include <graphics/2d/Shaping.h>


namespace ad {
namespace system {


class Render
{
public:
    Render(ent::EntityManager & aWorld) :
        mRenderables{aWorld}
    {}

    void update()
    {
        std::vector<graphics::r2d::Shaping::Circle> balls;
        mRenderables.each([&balls](component::Geometry & aGeometry)
            {
                balls.push_back({math::Position<3, GLfloat>{aGeometry.position, 0.f}});
            });
        mBalls.resetCircles(balls);
    }

    void render(const graphics::CameraProjection & aCameraProjection) const
    {
        mShaping.render(mBalls, aCameraProjection);
    }

private:
    graphics::r2d::ShapeSet mBalls;
    graphics::r2d::Shaping mShaping;

    ent::Query<component::Geometry> mRenderables;
};

} // namespace system
} // namespace ad

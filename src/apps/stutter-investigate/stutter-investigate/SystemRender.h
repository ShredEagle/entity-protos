#pragma once

#include "ComponentGeometry.h"

#include <entity/Query.h>

#if defined(NVPRO_GLLOADER)
#define GL_GLEXT_PROTOTYPES
#include "GL/glsubset.h"
#else
#include <graphics/2d/Shaping.h>
#endif

namespace ad {
namespace system {


class Render
{
public:
    Render(ent::EntityManager & aWorld) :
        mRenderables{aWorld}
    {}

#if defined(NVPRO_GLLOADER)
    void update()
    {
        mRenderables.each([](component::Geometry & aGeometry)
            {
            });
    }

    void render() const
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Frame");
        // No rendering atm with NVPRO_GLLOADER
        //mShaping.render(mBalls, aCameraProjection);
        glPopDebugGroup();
    }
#else
    void update()
    {
        std::vector<graphics::r2d::Shaping::Circle> balls;
        mRenderables.each([&balls](component::Geometry & aGeometry)
            {
                balls.push_back({math::Position<3, GLfloat>{aGeometry.position, 0.f}, aGeometry.radius});
            });
        mBalls.resetCircles(balls);
    }

    void render(const graphics::CameraProjection & aCameraProjection) const
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Frame");
        mShaping.render(mBalls, aCameraProjection);
        glPopDebugGroup();
    }
#endif

private:
#if !defined(NVPRO_GLLOADER)
    graphics::r2d::ShapeSet mBalls;
    graphics::r2d::Shaping mShaping;
#endif

    ent::Query<component::Geometry> mRenderables;
};

} // namespace system
} // namespace ad

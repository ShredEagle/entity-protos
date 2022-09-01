#pragma once

#include "ComponentGeometry.h"
#include "ComponentVelocity.h"

#include <entity/Query.h>

#include <math/Rectangle.h>


namespace ad {
namespace system {

class Move
{
public:
    Move(ent::EntityManager & aWorld, const math::Rectangle<float> & mBoundingBox) :
        mMovables{aWorld},
        mWorldBounds{mBoundingBox}
    {}

    void update(float aDelta)
    {
        mMovables.each([aDelta, this](component::Geometry & aGeometry, component::Velocity & aVelocity)
            {
                // Position integration
                aGeometry.previousPosition = aGeometry.position;
                aGeometry.position += aVelocity * aDelta;

                // Window border bounce
                math::Vec<2, float> radiusVec{aGeometry.radius, aGeometry.radius};

                // One each dimension:
                // the signed value of the overflow, or zero if there is none.
                math::Vec<2, float> overflow =
                    (max(mWorldBounds.topRight(), aGeometry.position + radiusVec)
                        - mWorldBounds.topRight())
                    + (min(mWorldBounds.bottomLeft(), aGeometry.position - radiusVec)
                        - mWorldBounds.bottomLeft())
                    ;
                if(overflow != math::Vec<2, float>::Zero())
                {
                    aGeometry.position -= overflow;
                    if (overflow.x()) aVelocity.x() *= -1.f;
                    if (overflow.y()) aVelocity.y() *= -1.f;
                }
            });
    }

private:
    math::Rectangle<float> mWorldBounds;
    ent::Query<component::Geometry, component::Velocity> mMovables;
};

} // namespace system
} // namespace ad

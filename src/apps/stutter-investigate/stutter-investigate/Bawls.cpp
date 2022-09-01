#include "Bawls.h"

#include "ComponentGeometry.h"
#include "ComponentVelocity.h"

#include <math/Transformations.h>


namespace ad {


namespace {


math::Size<2, float> getWindowSizeInWorld(math::Size<2, int> aWindowResolution,
                                            float aWindowHeight_world)
{
    float worldUnitPerPixel = aWindowHeight_world / aWindowResolution.height();
    return math::Size<2, float>{aWindowResolution * worldUnitPerPixel};
}


auto getProjection(math::Size<2, float> aWindowSize_world)
{
    constexpr float gFarPlane = 1.f;

    return math::trans3d::orthographicProjection<float>(
        math::Box<float>{
            math::Position<3, float>::Zero(),
            {aWindowSize_world, 2 * gFarPlane}
        }.centered());
}


} // anonymous namespace


Bawls::Bawls(math::Size<2, int> aWindowResolution) :
    mWindowSize_world{
        getWindowSizeInWorld(aWindowResolution, gWindowHeight_world)},
    mMoveSystem{mWorld, math::Rectangle<float>{{0.f, 0.f}, mWindowSize_world}.centered()},
    mCollideSystem{mWorld},
    mRenderSystem{mWorld}
{
    constexpr float gWindowHeight_world = 10;
#if !defined(NVPRO_GLLOADER)
    mCameraProjection.setCameraTransformation(getProjection(mWindowSize_world));
#endif

    ent::Phase prepopulate;
    mWorld.addEntity().get(prepopulate)
        ->add<component::Geometry>({.position = {0.f, 0.f}, .radius = 2.f})
        .add<component::Velocity>({1.f, 3.f})
        ;
    mWorld.addEntity().get(prepopulate)
        ->add<component::Geometry>({.position = {3.f, -1.f}, .radius = 0.5f})
        .add<component::Velocity>({1.2f, -3.5f})
        ;
    mWorld.addEntity().get(prepopulate)
        ->add<component::Geometry>({.position = {-3.f, 2.f}})
        .add<component::Velocity>({5.5f, 1.0f})
        ;
    mWorld.addEntity().get(prepopulate)
        ->add<component::Geometry>({.position = {3.f, 2.f}})
        .add<component::Velocity>({5.5f, 1.0f})
        ;
}


void Bawls::update(float aDelta)
{
    mMoveSystem.update(aDelta);
    mCollideSystem.update();
}


}  // namespace ad

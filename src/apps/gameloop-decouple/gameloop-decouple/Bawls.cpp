#include "Bawls.h"

#include "ComponentGeometry.h"
#include "ComponentVelocity.h"

#include <math/Transformations.h>


namespace ad {


namespace {


math::Size<2, GLfloat> getWindowSizeInWorld(math::Size<2, int> aWindowResolution,
                                            GLfloat aWindowHeight_world)
{
    GLfloat worldUnitPerPixel = aWindowHeight_world / aWindowResolution.height();
    return math::Size<2, GLfloat>{aWindowResolution * worldUnitPerPixel};
}


auto getProjection(math::Size<2, GLfloat> aWindowSize_world)
{
    constexpr GLfloat gFarPlane = 1.f;

    return math::trans3d::orthographicProjection<GLfloat>(
        math::Box<GLfloat>{
            math::Position<3, GLfloat>::Zero(),
            {aWindowSize_world, 2 * gFarPlane}
        }.centered());
}


} // anonymous namespace


Bawls::Bawls(const ad::graphics::AppInterface & aAppInterface) :
    mWindowSize_world{
        getWindowSizeInWorld(aAppInterface.getWindowSize(), gWindowHeight_world)},
    mMoveSystem{mWorld, math::Rectangle<GLfloat>{{0.f, 0.f}, mWindowSize_world}.centered()},
    mCollideSystem{mWorld},
    mRenderSystem{mWorld}
{
    constexpr GLfloat gWindowHeight_world = 10;
    mCameraProjection.setCameraTransformation(getProjection(mWindowSize_world));

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


void Bawls::render()
{
    mRenderSystem.update();
    mRenderSystem.render(mCameraProjection);
}


}  // namespace ad

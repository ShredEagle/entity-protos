#pragma once

#include "BuildConfig.h"

#include "SystemCollide.h"
#include "SystemMove.h"
#include "SystemRender.h"

#include <entity/EntityManager.h>


namespace ad {


class Bawls
{
    struct Backup
    {
        ent::State state;
    };

public:
    Bawls(math::Size<2, int> aWindowResolution);

    void update(float aDelta);

    void render()
    {
        mRenderSystem.update();
#if defined(NVPRO_GLLOADER)
        mRenderSystem.render();
#else
        mRenderSystem.render(mCameraProjection);
#endif
    }

private:
    static constexpr float gWindowHeight_world = 10;

    ent::EntityManager mWorld;
    math::Size<2, float> mWindowSize_world;
#if !defined(NVPRO_GLLOADER)
    graphics::CameraProjection mCameraProjection;
#endif

    system::Move mMoveSystem;
    system::Collide mCollideSystem;
    system::Render mRenderSystem;
};


} // namespace ad

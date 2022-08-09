#pragma once


#include "SystemCollide.h"
#include "SystemMove.h"
#include "SystemRender.h"

#include <entity/EntityManager.h>

#include <graphics/AppInterface.h>


namespace ad {

class Bawls
{
    struct Backup
    {
        ent::State state;
    };

public:
    Bawls(const ad::graphics::AppInterface & aAppInterface);

    void update(float aDelta);
    void render() const;

private:
    static constexpr GLfloat gWindowHeight_world = 10;

    ent::EntityManager mWorld;
    math::Size<2, GLfloat> mWindowSize_world;
    graphics::CameraProjection mCameraProjection;

    system::Move mMoveSystem;
    system::Collide mCollideSystem;
    system::Render mRenderSystem;
};


} // namespace ad

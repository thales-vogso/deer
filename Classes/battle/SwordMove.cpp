//
//  SwordMove.cpp
//  deer
//
//
//

#include "SwordMove.h"
USING_NS_CC;

bool SwordMove::init()
{
    if (!MotionStreak::initWithFade(0.3, 1, 20, Color3B::WHITE, "paticles/white.png"))
    {
        return false;
    }
    
    return true;
}

void SwordMove::update(float delta)
{
    MotionStreak::update(delta);
    
    if (_parVec.size() == 0)
    {
        _parVec.push_back(getPosition());
    }
    else
    {
        auto pos = getPosition();
        if (_parVec.back().distance(pos) > 30)
        {
            auto par = ParticleSystemQuad::create("swordMove.plist");
            par->setAutoRemoveOnFinish(true);
            par->setPosition(pos);
            addChild(par);
            _parVec.push_back(pos);
        }
    }
    
    if (_parVec.size() > 15)
    {
        _parVec.erase(_parVec.begin());
    }
}

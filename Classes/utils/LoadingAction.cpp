//
//  LoadingAction.cpp
//  deer
//
//  Created by allyiaa on 15-3-12.
//
//

#include "LoadingAction.h"
USING_NS_CC;
LoadingAction *LoadingAction::create(float time, float start, float end)
{
    auto ret = new LoadingAction();
    if (ret && ret->init(time, start, end))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool LoadingAction::init(float time, float start, float end)
{

    if (!ActionInterval::initWithDuration(time )) {
        return false;
    }
    _start = start;
    /**
     *  在end上添加一帧的距离，让最后一帧可以到底
     */
    _end = end;
    return true;
}

void LoadingAction::startWithTarget(cocos2d::Node *target)
{
    if (!dynamic_cast<cocos2d::ui::LoadingBar*>(target))
    {
        CCASSERT(false, "target must be LoadBar");
    }
    
    ActionInterval::startWithTarget(target);
}

void LoadingAction::update(float time)
{
    _percent = _start+ time * (_end- _start);
    auto target = (cocos2d::ui::LoadingBar*)_target;
    target->setPercent(_percent);
}

LoadingAction* LoadingAction::clone() const
{
    return LoadingAction::create(_duration);
}

LoadingAction* LoadingAction::reverse() const
{
    return LoadingAction::create(_duration);
}

ProgressTimerAction* ProgressTimerAction::create(float time,float start, float end)
{
    auto ret = new ProgressTimerAction();
    if (ret && ret->init(time, start, end))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool ProgressTimerAction::init(float time ,float start,float end)
{
    if (!ActionInterval::initWithDuration(time )) {
        return false;
    }
    _start = start;
    /**
     *  在end上添加一帧的距离，让最后一帧可以到底
     */
    _end = end;
    return true;
}

ProgressTimerAction* ProgressTimerAction::clone() const
{
    return ProgressTimerAction::create(_duration);
}
ProgressTimerAction* ProgressTimerAction::reverse() const
{
    return ProgressTimerAction::create(_duration);
}
void ProgressTimerAction::startWithTarget(cocos2d::Node* target)
{
    if (!dynamic_cast<cocos2d::ProgressTimer*>(target))
    {
        CCASSERT(false, "target must be ProgressTimer");
    }
    
    ActionInterval::startWithTarget(target);
}
void ProgressTimerAction::update(float time)
{
    float percent = _start + time * _end;
    if ((1-time) * _duration <= Director::getInstance()->getDeltaTime())
    {
        percent = 100;
    };
    auto target = (cocos2d::ProgressTimer*)_target;
    target->setPercentage(percent);
}

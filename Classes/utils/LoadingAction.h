//
//  LoadingAction.h
//  deer
//
//  Created by allyiaa on 15-3-12.
//
//

#ifndef __DEER_LOADINGACTION__
#define __DEER_LOADINGACTION__

#include <iostream>
#include "ui/CocosGUI.h"
#include "cocos2d.h"

class LoadingAction : public cocos2d::ActionInterval
{
public:
    static LoadingAction* create(float time, float start = 0, float end = 100);
CC_CONSTRUCTOR_ACCESS:
    
    bool init(float time, float start, float end);
    
    float _start;
    float _end;
    float _percent;
    
    virtual LoadingAction* clone() const override;
    virtual LoadingAction* reverse() const  override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
};
class ProgressTimerAction : public cocos2d::ActionInterval
{
public:
    static ProgressTimerAction* create(float time,float start = 0, float end = 100);
CC_CONSTRUCTOR_ACCESS:
    bool init(float time ,float start,float end);
    
    float _start;
    float _end;
    
    virtual ProgressTimerAction* clone() const override;
    virtual ProgressTimerAction* reverse() const override;
    virtual void startWithTarget(cocos2d::Node* target) override;
    
    virtual void update(float time) override;
};

#endif /* defined(__deer__LoadingAction__) */

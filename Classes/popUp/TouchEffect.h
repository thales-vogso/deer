/****************************************************************************
 *	@desc   触摸效果
 *	@date	2015-4-22
 *	@author	110101
     
 *	@file	popUp/TouchEffect.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_TOUCHEFFECT__
#define __DEER_TOUCHEFFECT__

#include <iostream>
#include "cocos2d.h"
class TouchEffect : public cocos2d::Layer
{
public:
    CREATE_FUNC(TouchEffect);
protected:
    bool init();
    
    cocos2d::EventListenerTouchOneByOne *_touchlis;
    
    bool onTouchBegan(cocos2d::Touch* t, cocos2d::Event* e);
    
    void onTouchMoved(cocos2d::Touch* t, cocos2d::Event* e);
    
    void onTouchEnded(cocos2d::Touch* t, cocos2d::Event* e);
    
    void onTouchCancelled(cocos2d::Touch* t, cocos2d::Event* e);
    
    std::vector<cocos2d::ParticleSystemQuad *> _pars;
//    cocos2d::ParticleSystemQuad *_par;
};

#endif /* defined(__deer__TouchEffect__) */

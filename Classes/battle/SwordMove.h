/****************************************************************************
 *	@desc   攻击动画，圣剑切割效果
 *	@date	2015-5-20
 *	@author	110101
     
 *	@file	battle/SwordMove.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER__SWORDMOVE__
#define __DEER__SWORDMOVE__

#include <iostream>
#include "cocos2d.h"
class SwordMove : public cocos2d::MotionStreak
{
public:
    CREATE_FUNC(SwordMove);
    
    bool init();
    
    virtual void update(float delta) override;
    
    std::vector<cocos2d::Vec2> _parVec;
};

#endif /* defined(__deer__SwordMove__) */

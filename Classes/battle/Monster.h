/********************************************************
        
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/MonsterModel.h
 *********************************************************/

#ifndef __DEER_MONSTERMODEL__
#define __DEER_MONSTERMODEL__

#include "cocos2d.h"
#include "battle/CombatBase.h"

class Monster : public CombatBase
{
public:
    static Monster* create(st_BaseData data);
//    CREATE_FUNC(Monster);
    
    bool init(st_BaseData data);
    
    /**
     *  攻击
     */
    virtual void attack() override;
    
    inline void setMissFun(std::function<void ()> missfun)
    {
        _missFun = missfun;
    }
    
protected:
    
    std::function<void ()> _missFun;
};
#endif /* defined(__deer__Monster__) */

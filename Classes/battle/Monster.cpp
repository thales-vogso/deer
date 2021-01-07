/******************************************************************
        
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/Monster.cpp
 **************************************************************/

#include "Monster.h"
#include "data/Crusade.h"

Monster *Monster::create(st_BaseData data)
{
    Monster *ret = new (std::nothrow)Monster();
    if (ret && ret->init(data))
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

bool Monster::init(st_BaseData data)
{
    if (!CombatBase::init())
    {
        return false;
    }
    /**
     *  初始化基本数据
     */
    _baseData = data;

    return true;
}

void Monster::attack()
{
    if (_enemy && _enemy->isLive())
    {
        /**
         *  怪物的攻击
         */
        float weaken(0);
        if (_status._nextAttackWeaken.first)
        {
            weaken = _status._nextAttackWeaken.second;
            _status._nextAttackWeaken.first = false;
            _status._nextAttackWeaken.second = 0;
        }
        
        /**
         *  dodge是闪避天赋占位符
         */
        float dodge(0);
        if (!data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kDodge).asString()].isNull())
        {
            dodge = data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kDodge).asString()].asValueMap()["value"].asFloat();
        }
        if (CCRANDOM_0_1() < _baseData._accuracy*(1 - dodge * 0.05))
        {
            float hurt = _baseData._power - weaken;
            //是否是在攻打钛矿中，若是则玩家不能死..
            if(data::Crusade::getInstance()->isInTAI())
            {
                //如果能造成死亡，则miss..
                if(_enemy->isDiedByThisHurt(hurt))
                {
                    if (_missFun)
                    {
                        _missFun();
                    }
                }
                else
                {
                    _enemy->hurt(hurt);
                }
            }
            else
            {
                _enemy->hurt(hurt);
            }
            
        }
        else
        {
            if (_missFun)
            {
                _missFun();
            }
        }
        
        if (_attackEnd)
        {
            _attackEnd(this);
        }
    }
}
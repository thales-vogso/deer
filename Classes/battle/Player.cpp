/***********************************************************************
        
 *	@desc	玩家战斗模型
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/Player.cpp
 ************************************************************************/

#include "Player.h"
#include "battle/Skills.h"
#include "data/Crusade.h"
#include "GlobalMacros.h"
#include "data/Guid.h"
#include "sound/SoundManager.h"

bool Player::init()
{
    if (!CombatBase::init())
    {
        return false;
    }
    
    /**
     *  基础数值初始化
     */
    
    auto playerList = data::Crusade::getInstance()->getCarriage().asValueVector();
    for (auto &i : playerList)
    {
        int rolecount = i.asValueMap()["current"].asInt();
        
        _baseData._MinHP = 0;
        _baseData._MaxHp += i.asValueMap()["life"].asFloat() * rolecount;
        _baseData._HP = _baseData._MaxHp;
        
        for (auto n(0); n < rolecount ; n++)
        {
            auto warrior = Warrior::create(i.asValueMap());
            warrior->setPlayer(this);
            addChild(warrior);
            _warriors.pushBack(warrior);
            warrior->setWid(_warriors.end() - _warriors.begin());
        }
    }
    
    /**
     *  生命强化
     */
    auto buffs =  data::Talent::getInstance()->getUserBuff();
    float enhance(0);
    if (!buffs.asValueMap()[cocos2d::Value(data::Talent::kLifeEnhance).asString()].isNull()) {
		_baseData._MaxHp += buffs.asValueMap()[cocos2d::Value(data::Talent::kLifeEnhance).asString()].asValueMap()["value"].asFloat();
        _baseData._HP = _baseData._MaxHp;
    }
    
    return true;
}

void Player::setEnemy(CombatBase *enemy)
{
    CombatBase::setEnemy(enemy);
    for (auto &i : _warriors)
    {
        i->setEnemy(enemy);
    }
}

void Player::attack()
{
    for (auto &i : _warriors)
    {
        i->attack();
    }
}

void Player::attack(int warriorIndex)
{
	if (warriorIndex <= _warriors.size())
    {
		_warriors.at(warriorIndex - 1)->attack();
    }
}
bool Player::isCanAttack(int index)
{
    if (index <= _warriors.size())
    {
        return _warriors.at(index - 1)->isCanAttack();
    }
    return false;
}

void Player::hurt(float delife)
{
   
    float defense(0);
    if (!data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kDefenseEnhance).asString()].isNull()) {
			defense = data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kDefenseEnhance).asString()].asValueMap()["value"].asFloat();
    }
    delife -= defense;
    if (delife < 1) delife = 1;
    CombatBase::hurt(delife);
}

void Player::setWarriorsCDBeganFunc(CombatFunc func)
{
    for (auto & i : _warriors)
    {
        i->setAttackCDBegan(func);
    }
}

void Player::attackCDAll()
{
    for (auto &i : _warriors)
    {
        i->attackCD();
    }
}

void Player::attackCD(int wid)
{
    _warriors.at(wid - 1)->attackCD();
}

void Player::setAttackEnd(CombatFunc func)
{
    for (auto &i : _warriors)
    {
        i->setAttackEnd(func);
    }
}

void Player::setAttackCDEnd(CombatFunc func)
{
    for (auto &i : _warriors)
    {
        i->setAttackCDEnd(func);
    }
}
void Player::setMissFunc(std::function<void ()> func)
{
    for (auto &i : _warriors)
    {
        i->setMissFunc(func);
    }
};

//-----------------战士---------------------

Warrior *Warrior::create(cocos2d::ValueMap data)
{
    Warrior *ret = new (std::nothrow)Warrior();
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

bool Warrior::init(cocos2d::ValueMap data)
{
    if (!CombatBase::init())
    {
        return false;
    }
    _effType = 0;
    
    auto iii = data;
    
    _baseData._accuracy = data["accuracy"].asFloat();
    _baseData._MaxHp = data["life"].asFloat();
    _baseData._HP = _baseData._HP;
    _baseData._speed = data["speed"].asFloat();
    _baseData._interval = 10.0 / _baseData._speed;
    _baseData._power = data["power"].asFloat();
    
    auto effdata = data["effect"].asValueMap();
    
    if (effdata.size() > 0)
    {
        _effType = effdata["type"].asInt();
        if (_effType == kStun)
        {
            auto skill = StunSkill::create(this, effdata);
            _skillManager->addNewSkill(skill);
        }
        else if(_effType == kWeaken)
        {
            auto skill = WeakenSkill::create(this, effdata);
            _skillManager->addNewSkill(skill);
        }
        else if (_effType == kHeal)
        {
            auto skill = HealSkill::create(this, effdata);
            _skillManager->addNewSkill(skill);
        }
        else if (_effType == kLastHurt)
        {
            auto skill = LastHurtSkill::create(this, effdata);
            _skillManager->addNewSkill(skill);
        }
    }
    
    
    return true;
}

void Warrior::attack()
{
    if (_enemy && _enemy->isLive() && !_status._isAttackCD)
    {
        
        /**
         *  精准计算
         */
        float additon(0);
		if (!data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kPrecise).asString()].isNull()) {
			additon = data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kPrecise).asString()].asValueMap()["value"].asFloat();
        }
        
        float realAccuracy = _baseData._accuracy * (1 +  additon * 0.05);
        

#if DEBUG_ACCURANCE == 1
        realAccuracy = 1;
#endif

        if (CCRANDOM_0_1() <= realAccuracy)
        {
            /**
             *  战士依靠技能攻击，因此至少有一个技能,这里使用所有技能
             */
            _skillManager->allSkillsUse();
            
            if (_baseData._power < 0)
            {
                _player->heal(-_baseData._power);
            }
            else
            {
                int hurt(0);
                if (_effType == kStun)
                {
                    hurt = _baseData._power;
                }
                else if (_effType == kWeaken)
                {
                    hurt = _baseData._power;
                }
                else if (_effType == kHeal)
                {
                    /**
                     *  1为占位符
                     */
                    hurt = _baseData._power - 1;
                }
                else if (_effType == kLastHurt)
                {
                    hurt = _baseData._power;
                }
                else
                {
                    hurt = _baseData._power;
                }
                
                /**
                 *  附加伤害
                 */
                float addtionhurt(0);
				if (!data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kAttackEnhance).asString()].isNull()) {
					addtionhurt = data::Talent::getInstance()->getUserBuff().asValueMap()[cocos2d::Value(data::Talent::kAttackEnhance).asString()].asValueMap()["value"].asFloat();
                }
                //新手引导临时处理强制miss
                if(data::Guid::getInstance()->getCurrentGuidId() != data::Guid::ATTACK1&&
                   data::Guid::getInstance()->getCurrentGuidId() != data::Guid::AUTO_ATTACK&&
                   (data::Guid::getInstance()->getCurrentGuidId() != data::Guid::FOOD_BACK||data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::GUID_TASK_ID::FOOD_BACK)))
                {
                    _enemy->hurt(hurt + addtionhurt);
                }
                else
                {
                    if (_missFunc)
                    {
                        _missFunc();
                    }
                }
            }
        }
        else
        {
            //目前玩家不会miss
            if (_missFunc)
            {
                _missFunc();
            }
        }
        
        if (_attackEnd)
        {
            _attackEnd(this);
        }
    }
}





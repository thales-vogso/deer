/***************************************************
        
 *	@desc	技能
 *	@date	2015-5-12
 *	@author	110101
     
 *	@file	battle/Skills.cpp
 *************************************************/

#include "Skills.h"
USING_NS_CC;

//-------------SkillManager----------------

SkillManager * SkillManager::create(CombatBase * owner)
{
    SkillManager *ret = new SkillManager();
    if (ret && ret->init(owner))
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

bool SkillManager::init(CombatBase *owner)
{
    if (!Node::init())
    {
        return false;
    }
    _owner = owner;
    
    return true;
}

void SkillManager::addNewSkill(Skill *skill)
{
    addChild(skill);
    _skills.pushBack(skill);
}

void SkillManager::removeSkill(int skillid)
{
    /**
     *  留作后期拓展 2015-5-12
     */
}

//void SkillManager::startCD()
//{
//    for (auto i : _skills)
//    {
//        i->startCD();
//    }
//}
//
//void SkillManager::startCD(int skillid)
//{
//    if (_skills.size() >= skillid)
//    {
//        if (_skills.at(skillid - 1)->isAlready())
//        {
//            _skills.at(skillid - 1)->startCD();
//        }
//    }
//}

void SkillManager::allSkillsUse()
{
    for (auto &i : _skills)
    {
        i->skillEffect();
    }
}

void SkillManager::setSkillEndCall(int skillid, std::function<void ()> fun)
{
    if (_skills.size() >= skillid)
    {
        _skills.at(skillid)->setEndCall(fun);
    }
}

//---------------skill----------------

bool Skill::init(CombatBase *owner)
{
    if (!Node::init())
    {
        return false;
    }
    
    _owner = owner;
    
//    _already = true;
    
    return true;
}

//void Skill::startCD()
//{
//    _already = false;
//    auto seq = Sequence::create(DelayTime::create(_cd), CallFunc::create(CC_CALLBACK_0(Skill::CDEnd, this)), NULL);
//    runAction(seq);
//    
//    //这里要并发调用显示..
//}

//void Skill::CDEnd()
//{
//    _already = true;
//    
//    //调用显示..
//    if (_cdEndCall)
//    {
//        _cdEndCall();
//    }
//}

//--------------stunSkill-----------------

bool StunSkill::init(CombatBase *owner, cocos2d::ValueMap data)
{
    if (!Skill::init(owner))
    {
        return false;
    }
    
    //解析技能数据..
    _val = data["value"].asFloat();
    
    return true;
}

void StunSkill::skillEffect()
{
    _owner->enemy()->stun(_val);
}

/********************** healskill **********************/
bool HealSkill::init(CombatBase *owner, cocos2d::ValueMap data)
{
    if (!Skill::init(owner))
    {
        return false;
    }
    
    //解析技能数据..
    _val = data["value"].asFloat();
    
    return true;
}


void HealSkill::skillEffect()
{
    /**
    *  自我治疗
    */
    _owner->heal(_val);
}

/****** 持续伤害  ***********/

bool LastHurtSkill::init(CombatBase *owner, cocos2d::ValueMap data)
{
    if (!Skill::init(owner))
    {
        return false;
    }
    _val = data["value"].asFloat();
    
    return true;
}

void LastHurtSkill::skillEffect()
{
    _owner->enemy()->LastHurt(_val);
}

/********** 减弱 *********/

bool WeakenSkill::init(CombatBase *owner, cocos2d::ValueMap data)
{
    if (!Skill::init(owner))
    {
        return false;
    }
    _val = data["value"].asFloat();
    return true;
}

void WeakenSkill::skillEffect()
{
    if (_owner->enemy())
    {
        /**
         *  调用敌人的weaker接口
         */
        _owner->enemy()->nextAttackWeaken(_val);
        CCLOG("weaken");
    }
}

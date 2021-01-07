/*********************************************
        
 *	@desc	技能
 *	@date	2015-5-12
 *	@author	110101
     
 *	@file	battle/Skills.h
 ********************************************/

#ifndef __DEER_SKILLS__
#define __DEER_SKILLS__

#include "cocos2d.h"
#include "battle/CombatBase.h"

enum EffctType
{
    kStun = 110101,
    kWeaken = 110201,
    kHeal = 110301,
    kLastHurt = 110401
};

class Skill;

class SkillManager : public cocos2d::Node
{
public:
    /**
     *
     *
     *  @param data  技能数据
     *  @param onwer 仅能拥有人
     *
     *  @return
     */
    static SkillManager *create(CombatBase * owner);
    
    /**
     *  初始化
     *
     *  @param data
     *  @param owner
     *
     *  @return
     */
    bool init(CombatBase *owner);
    
    /**
     *  添加技能
     *
     *  @return
     */
    virtual void addNewSkill(Skill * skill);
    
    /**
     *  减少技能
     *
     *  @return
     */
    void removeSkill(int skillid);
    
//    /**
//     *  cd全部开始
//     */
//    void startCD();
//    
//    /**
//     *  开始冷却某一个技能
//     *
//     *  @param skillid 技能id
//     */
//    void startCD(int skillid);
    
    /**
     *  使用所有技能
     */
    void allSkillsUse();
    
    /**
     *  添加技能冷却结束的回调
     *
     *  @param sillid 技能id
     *  @param fun    回调函数
     */
    void setSkillEndCall(int sillid, std::function<void ()> fun);
    
protected:
    /**
     *  被管理的技能
     */
    cocos2d::Vector<Skill* > _skills;
    
    /**
     *  战斗技能的拥有者
     */
    CombatBase * _owner;
};

/**
 *  技能类是一个虚基类，各种技能通过重写skillEffect()函数来实现；
 */
class Skill : public cocos2d::Node
{
public:
    
    bool init(CombatBase * owner);
    
//    virtual void startCD();
//    
//    inline bool isAlready()
//    {
//        return _already;
//    }
    
    inline void setEndCall(std::function<void ()> endcall)
    {
        _cdEndCall = endcall;
    }
    
    /**
     *  该函数为技能效果，派生类通过重写该函数来达到不同效果
     */
    virtual void skillEffect() = 0;
    
protected:
    
//    virtual void CDEnd();
    
//    bool _already;
    
//    float _cd;
    
//    int skillid;
    float _val;
    
    std::function<void ()> _cdEndCall;
    
    /**
     *  技能的拥有者
     */
    CombatBase *_owner;
};

#define SKILL_CREATE_FUNC(__TYPE__)\
static __TYPE__ * create(CombatBase *owner, cocos2d::ValueMap data)\
{\
    auto ret = new(std::nothrow) __TYPE__();\
    if (ret && ret->init(owner, data)) {\
        ret->autorelease();\
        return ret;\
    }\
    else\
    {\
        delete ret;\
        ret = nullptr;\
        return nullptr;\
    }\
}\

class StunSkill : public Skill
{
public:
    
    SKILL_CREATE_FUNC(StunSkill)
    
    bool init(CombatBase *owner, cocos2d::ValueMap data);
    
    virtual void skillEffect() override;
};

class WeakenSkill : public Skill
{
public:
    SKILL_CREATE_FUNC(WeakenSkill)
    
    bool init(CombatBase * owner, cocos2d::ValueMap data);
    
    virtual void skillEffect() override;
};

class HealSkill : public Skill
{
public:
    SKILL_CREATE_FUNC(HealSkill)
    
    bool init(CombatBase *owner, cocos2d::ValueMap data);
    
    virtual void skillEffect() override;
};

class LastHurtSkill : public Skill
{
public:
    SKILL_CREATE_FUNC(LastHurtSkill)
    
    bool init(CombatBase* owner, cocos2d::ValueMap data);
    
    virtual void skillEffect() override;
};


#endif /* defined(__deer__Skills__) */

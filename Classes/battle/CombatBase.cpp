/****************************************************************
        
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/CombatBase.cpp
 ****************************************************************/

#include "CombatBase.h"
#include "battle/Skills.h"
USING_NS_CC;

CombatBase::CombatBase()
:_status()
,_baseData(0, 0, 0, 0, 0, 0, 0)
{

}

CombatBase::~CombatBase()
{
    
}

bool CombatBase::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    /**
     *  初始化技能管理器
     */
    _skillManager = SkillManager::create(this);
    addChild(_skillManager);
    
    _attackManager = Node::create();
    addChild(_attackManager);
    
    _debuffManager = Node::create();
    addChild(_debuffManager);
    
    _buffManager = Node::create();
    addChild(_buffManager);
    
    //攻击中断..
    _attackPause = false;
    
    scheduleUpdate();
    
    /**
     *  派生类需要初始化的内容：_baseData（战斗单位的基本数据）;
     */
    return true;
}

void CombatBase::onEnter()
{
    Node::onEnter();
//    /**
//     *  技能cd
//     */
//    _skillManager->startCD();
//    
//    /**
//     *  普攻cd
//     */
//    attackCD();
}

void CombatBase::attack()
{
    CCLOG("combatBase::attack");
    /**
     *  这里根据战斗实体的不同来实现不同的战斗操作
     */
}

void CombatBase::attackCD()
{
    _status._isAttackCD = true;
    auto seq = Sequence::create(DelayTime::create(_baseData._interval), CallFunc::create(CC_CALLBACK_0(CombatBase::attackCDEnd, this)), NULL);
    _attackManager->runAction(seq);
    
    if (_attackCDBegan)
    {
        _attackCDBegan(this);
    }
}

void CombatBase::attackCDEnd()
{
    _status._isAttackCD = false;
    
    /**
     *  下一帧执行冷却结束的回调
     */
    _frameFlag.cdEnd = true;
}

void CombatBase::stun(float second)
{
    /**
     *  晕眩的时候停止技能管理器的工作
     */
    if (_stunFunc)
    {
        _stunFunc(this, second);
    }
    
    _status._isStun = true;
    _skillManager->pause();
    _attackManager->pause();
    
    auto seq = Sequence::create(DelayTime::create(second),
                                CallFunc::create(CC_CALLBACK_0(CombatBase::stunEnd, this)), NULL);
    this->runAction(seq);
}

void CombatBase::stunEnd()
{
    _skillManager->resume();
    _attackManager->resume();
    _status._isStun = false;
}

void CombatBase::hurt(float delife)
{
    _baseData._HP -= delife;
    
    if (_baseData._HP < _baseData._MinHP)
    {
        _baseData._HP = _baseData._MinHP;
    }
    
    if (_hurtFun)
    {
        _hurtFun(this, delife);
    }
    
    if (_baseData._HP == _baseData._MinHP)
    {
        died();
    }
}

void CombatBase::nextAttackWeaken(float var)
{
    _status._nextAttackWeaken.first = true;
    _status._nextAttackWeaken.second = var;
}

void CombatBase::LastHurt(float delife, float time)
{
    float delta = delife/ time;
    
    auto seq = Sequence::create(CallFunc::create([=]{this->hurt(delta);}),DelayTime::create(1), NULL);
    int num = (int)time;
    auto rep = Repeat::create(seq,num);
    _debuffManager->runAction(rep);
}

void CombatBase::heal(float hp)
{
    _baseData._HP += hp;
    if (_baseData._HP > _baseData._MaxHp)
    {
        _baseData._HP = _baseData._MaxHp;
    }
    
    if (_healFun)
    {
        _healFun(this, hp);
    }
}

void CombatBase::died()
{
    //停止攻击..
    _attackManager->stopAllActions();
    
    //停止仅能冷却..
    _skillManager->stopAllActions();
    
    //debuff 效果停止..
    _debuffManager->stopAllActions();
    
    //buff效果停止..
    _buffManager->stopAllActions();
    
    if (_diedFunc)
    {
        _diedFunc(this);
    }
}

void CombatBase::update(float delta)
{
    Node::update(delta);
    
    if (_frameFlag.cdEnd && _attackCDEnd && !_attackPause)
    {
        _attackCDEnd(this);
        _frameFlag.cdEnd = false;
    }
}
bool CombatBase::isCanAttack()
{
    if (_enemy && _enemy->isLive() && !_status._isAttackCD)
    {
        return true;
    }
    return false;
}


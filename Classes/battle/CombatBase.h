/*********************************************************
        
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/CombatBase.h
 ********************************************************/

#ifndef __DEER_COMBATBASE__
#define __DEER_COMBATBASE__

#include <iostream>
#include "cocos2d.h"

class CombatBase;

class SkillManager;

class Skill;

typedef std::function<void (CombatBase*)> CombatFunc;

typedef std::function<void (CombatBase*, float)> CombatFunc_f;

/**
 *  基础数值
 */
struct st_BaseData
{
    st_BaseData(){};
    st_BaseData(float power, float minhp, float maxhp, float hp, float interval, float speed, float accuracy)
    :_power(power)
    ,_MinHP(minhp)
    ,_MaxHp(maxhp)
    ,_HP(hp)
    ,_interval(interval)
    ,_speed(speed)
    ,_accuracy(accuracy)
    {
        
    };
    
    /**
     *  力量
     */
    float _power;
    
    /**
     *  最小生命
     */
    float _MinHP;
    
    /**
     *  最大生命
     */
    float _MaxHp;
    
    /**
     *  当前生命
     */
    float _HP;
    
    /**
     *  攻击间隔
     */
    float _interval;
    
    /**
     *  速度
     */
    float _speed;
    
    /**
     *  准确度
     */
    float _accuracy;
};

struct st_CombatStatus
{
    st_CombatStatus()
    :_isStun(false)
    ,_isBuff(false)
    ,_isDebuff(false)
    ,_isAttackCD(false)
    ,_nextAttackWeaken(std::make_pair(false, 0))
    {
        
    };
    
    /**
     *  晕眩中
     */
    bool _isStun;
    
    /**
     *  有减免效果
     */
    bool _isDebuff;
    
    /**
     *  有增益效果
     */
    bool _isBuff;
    
    /**
     *  普攻cd中
     */
    bool _isAttackCD;
    
    /**
     *
     */
    std::pair<bool, float> _nextAttackWeaken;
};

/**
 *  战斗单位的帧标记数据
 */
struct st_CombatFrameFlag
{
    st_CombatFrameFlag():cdEnd(false){};
    bool cdEnd;
};

class CombatBase : public cocos2d::Node
{
public:
    
    CombatBase();
    
    virtual ~CombatBase();
    
    CREATE_FUNC(CombatBase);
    
    virtual bool init();
    
public:
    /**
     *  设置敌人
     *
     *  @param enemy 敌人战斗单位
     */
    inline void setEnemy(CombatBase * enemy){_enemy = enemy;};
    
    /**
     *  获取该战斗实体的当前敌人
     *
     *  @return 另一个战斗实体（敌人战斗单位）
     */
    inline CombatBase* enemy(){return _enemy;};
    
    /**
     *  是否活着
     *
     *  @return
     */
    bool isLive(){return _baseData._HP > _baseData._MinHP;};
    
    /**
     *  这次攻击是否会造成死亡
     *
     *  @return bool
     */
    bool isDiedByThisHurt(float hurt){return hurt > _baseData._HP;};
    
    /**
     *  是否处于晕眩状态
     *
     *  @return
     */
    bool isStun(){return _status._isStun;};
    
    /**
     *  是否处于减疫状态
     *
     *  @return
     */
    inline bool isDebuff(){return _status._isDebuff;};
    
    /**
     *  是否处于增益状态
     *
     *  @return
     */
    inline bool isBuff(){return _status._isBuff;};
    
    /**
     *  普通攻击是否在cd中
     *
     *  @return
     */
    inline bool isAttackCD(){return _status._isAttackCD;};
    
    /**
     *  下一次攻击减弱
     *
     *  @param var
     */
    void nextAttackWeaken(float var);
    
    /**
     *  获取基础数据
     *
     *  @return 基础数据
     */
    virtual st_BaseData getBaseData(){return _baseData;};
    
    /**
     *  添加冷去结束回调
     *
     *  @param attackEnd 回调
     */
    inline virtual void setAttackCDEnd(CombatFunc attackCDEnd){_attackCDEnd = attackCDEnd;};
    
    /**
     *  普攻冷却开始回调
     *
     *  @param attackCDBegan 回调
     */
    inline virtual void setAttackCDBegan(CombatFunc attackCDBegan){_attackCDBegan = attackCDBegan;};
    
    /**
     *  添加攻击结束
     *
     *  @param attackEnd 攻击结束回调
     */
    inline virtual void setAttackEnd(CombatFunc attackEnd){_attackEnd = attackEnd;};
    
    /**
     *  添加受伤回调
     *
     *  @param hurtFun 受伤回调
     */
    inline virtual void setHurtFun(CombatFunc_f hurtFun){_hurtFun = hurtFun;};
    
    /**
     *  回血回调
     *
     *  @param healFun
     */
    inline virtual void setHealFun(CombatFunc_f healFun){_healFun = healFun;};
    
    /**
     *  晕眩回调
     *
     *  @param stunFunc 晕眩回调
     */
    inline virtual void setStunFunc(CombatFunc_f stunFunc){_stunFunc = stunFunc;};
    
    /**
     *  死亡回调
     *
     *  @param diedFunc 回调
     */
    inline virtual void setDiedFunc(CombatFunc diedFunc){_diedFunc = diedFunc;};
    
    /**
     *  攻击
     */
    virtual void attack();
    
    /**
     *  晕眩
     */
    virtual void stun(float second);
    
    /**
     *  死亡
     */
    virtual void died();
    
    /**
     *  收到伤害
     *
     *  @param 减少血量
     */
    virtual void hurt(float delife);
    
    /**
     *  持续掉血
     *
     *  @param delife 总血量
     *  @param time   总时间
     */
    virtual void LastHurt(float delife, float time = 3);
    
    /**
     *  治疗
     *
     *  @param hp 治疗血量
     */
    void heal(float hp);
    
    /**
     *  普攻冷却
     */
    void attackCD();
    
    /**
     *  刷新重载
     */
    void update(float delta) override;
    
    /**
     *  设置攻击暂停
     */
    inline virtual void setAttackPause(bool bRet){_attackPause = bRet;};
    
    /**
     *  获取攻击是否暂停
     */
    inline virtual bool getAttackPause(){return _attackPause;};
    
    /**
     *  是否能攻击
     */
    virtual bool isCanAttack();
    
    
protected:
    
    void onEnter() override;
    
    /**
     *  普攻冷却完毕
     */
    void attackCDEnd();
    
    /**
     *  晕眩结束
     */
    virtual void stunEnd();

    /**
     *  基础数据
     */
    st_BaseData _baseData;
    
    /**
     *  状态
     */
    st_CombatStatus _status;
    
    /**
     *  帧标记，这里将标记下一针要执行的操作
     */
    st_CombatFrameFlag _frameFlag;
    
    /**
     *  技能管理器
     */
    SkillManager * _skillManager;
    
    /**
     *  普攻管理器
     */
    cocos2d::Node * _attackManager;
    
    /**
     *  减损效果管理器
     */
    cocos2d::Node * _debuffManager;
    
    /**
     *  buff效果管理器
     */
    cocos2d::Node * _buffManager;
    
    /**
     *  敌人，一个战斗实体会把另一个战斗实体看成敌人，这个变量保存谁是敌人
        敌人是我方伤害技能和攻击的承受主体
     */
    CombatBase * _enemy;
    
    /**
     *  攻击冷却完毕回调
     */
    CombatFunc _attackCDEnd;
    
    /**
     *  普攻冷却开始回调
     */
    CombatFunc _attackCDBegan;
    
    /**
     *  攻击结束
     */
    CombatFunc _attackEnd;
    
    /**
     *  受伤回调
     */
    CombatFunc_f _hurtFun;
    
    /**
     *  回血
     */
    CombatFunc_f _healFun;
    
    /**
     *  晕眩回调
     */
    CombatFunc_f _stunFunc;
    
    /**
     *  死亡回调
     */
    CombatFunc _diedFunc;
    
    /**
     *  战斗中止
     */
    bool _attackPause;
    
    
};


#endif /* defined(__deer__CombatBase__) */

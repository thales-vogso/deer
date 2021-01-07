/***********************************************************************
        
 *	@desc	玩家战斗模型
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/Player.h
 *************************************************************************/


#ifndef __DEER_PLAYER__
#define __DEER_PLAYER__
#include "cocos2d.h"
#include "battle/CombatBase.h"

typedef std::vector<std::function<void ()>> FuncArr;


class Warrior;
/**
 *  player 作为整体拥有战斗单位属性
    内部又实例化每个战士
 */
class Player : public CombatBase
{
public:
    CREATE_FUNC(Player)
    
    bool init() override;
    
    /**
     *  添加敌人
     *
     *  @param CombatBase* 敌人
     */
    void setEnemy(CombatBase* enemy);
    
    /**
     *  重载屏蔽基类函数
     */
    void attack() override;
    
    /**
     *  攻击函数
     *
     *  @param warriorId 发动攻击的战士id；
     */
    void attack(int warriorIndex);
    
    /**
     *  全部技能开始cd
     */
    void attackCDAll();
    
    /**
     *   攻击cd
     *
     *  @param wid
     */
    void attackCD(int wid);
    
    /**
     *  受伤
     */
    void hurt(float delife) override;
    
    /**
     *  添加战士技能冷却回调
     */
    void setWarriorsCDBeganFunc(CombatFunc func);
    
    
    void setAttackEnd(CombatFunc func) override;
    
    void setAttackCDEnd(CombatFunc func) override;
    
    void setMissFunc(std::function<void()> func);
    
    bool isCanAttack(int index);
    
protected:
    /**
     *  战士
     */
    cocos2d::Vector<Warrior*> _warriors;
};

/**
 *  这是战士类
 */
class Warrior : public CombatBase
{
public:
    static Warrior* create(cocos2d::ValueMap data);
    
    bool init(cocos2d::ValueMap data);
    
    inline void setPlayer(Player * p)
    {
        _player = p;
    };
    
public:
    
    inline void setWid(int wid){_wid = wid;};
    
    inline int getWid(){return _wid;};
    
    void attack() override;
    
    inline void setMissFunc(std::function<void()> func)
    {
        _missFunc = func;
    };

protected:
    
    int _wid;
    
    int _effType;
    
    std::function<void()> _missFunc;
    
    Player *_player;
};

#endif /* defined(__deer__Player__) */

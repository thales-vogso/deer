/****************************************************************************
 *	@desc	战斗控制器
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleController.h
 ******************************************************************************/

#ifndef __DEER_BATTLECONTROLLER__
#define __DEER_BATTLECONTROLLER__

#include "cocos2d.h"
#include "battle/Monster.h"
#include "battle/Player.h"
#include "battle/BattleViewController.h"

#include "data/User.h"
#include "data/Crusade.h"

#include "manor/Manor.h"
#include "crusade/CrusadeMap.h"

#include "BattleControllerDelegate.h"
#include "manor/LayerBase.h"

/**
 *  战斗逻辑控制器
 */
class BattleController : public cocos2d::Layer , public BattleControllerDelegate,public LayerBase
{
public:
    
    static BattleController * create(cocos2d::ValueMap data, BattleViewController * viewCon);
    
//    CREATE_FUNC(BattleController);
    
    /**
     *  设置显示控制器
     *
     *  @param viewcon 显示控制
     */
    inline void setViewController(BattleViewController * viewcon){_viewCon = viewcon;}
    
    /**
     *  玩家战士攻击
     *
     *  @param wid 战士编号
     */
    void playerWarriorAttack(int wid);
    
    /**
     *  获取monster
     */
    Monster* getMonster();
    /**
     *  获取player
     */
    Player* getPlayer();
    
    void removeSelf() override;
    void updateProperty() override;
    
protected:
    
    virtual ~BattleController();
    
    bool init(cocos2d::ValueMap data, BattleViewController * viewCon);
    
    void onEnter();
    
    /**
     *  战斗开始
     */
    void battleStart();
    
protected:
    /**
     *  怪物cd开始
     */
    void onMonsterCDBegan(CombatBase*);
    
    /**
     *  怪物cd结束回调
     */
    void onMonsterCDEnd(CombatBase*);
    
    /**
     *  怪物攻击结束
     */
    void onMonsterAttackEnd(CombatBase*);
    
    /**
     *  怪物晕眩
     *
     *  @param CombatBase*
     *  @param second      晕眩时间
     */
    void onMonsterStun(CombatBase* , float second);
    
    /**
     *  怪物受伤
     *
     *  @param monster
     *  @param hp      
     */
    void onMonsterHurt(CombatBase* monster, float hp);
    
    /**
     *  怪物死亡
     *
     *  @param monster 怪物
     */
    void onMonsterDied(CombatBase *monster);
    
    /**
     *  怪物miss
     */
    void onMonsterMiss();
    
    /**
     *  战士cd
     *
     *  @param wid      战士id
     *  @param interval cd时间
     */
    void onWarriosSkillCDBegan(CombatBase *);
    
    /**
     *  玩家受伤
     */
    void onPlayerHurt(CombatBase*, float hp);
    
    /**
     *  玩家生命回复
     *
     *  @param CombatBase* 
     *  @param warrior
     */
    void onPlayerHeal(CombatBase*, float hp);
    
    /**
     *  玩家冷却结束
     *
     *  @param warrior 战士
     */
    void onPlayerCDEnd(CombatBase *warrior);
    
    /**
     *  玩家攻击结束
     *
     *  @param warrior 战士
     */
    void onPlayerAttackEnd(CombatBase *warrior);
    
    /**
     *  玩家死亡
     *
     *  @param player 玩家死亡
     */
    void onPlayerDied(CombatBase *player);
    
    /**
     *  玩家miss
     */
    void onPlayerMiss();
    
    /**
     *  技能按钮回调
     *
     *  @param sender
     */
    void onSkillBtnFunc(int btnid);
    
    /**
     *  建筑清空
     */
    void onSureInBuildClearup() override;
    
    /**
     *  从掉落界面退出
     */
    void onExitFromSettlement() override;
    
    /**
     *  进入选择确认
     */
    void onSureInEnterChoice() override;
    
    /**
     *  进入选择放弃
     */
    void onQuitInEnterChoice() override;
    
    /**
     *  玩家按下面包按钮
     */
    void onTouchBreadBtn() override;
    
    /**
     *  玩家按下自动战斗按钮
     */
    void onPressedAutoBattleBtn() override;
    
    /**
     *  死了
     *
     *  @param e 
     */
    void onAce(cocos2d::EventCustom* e);
    
    /**
     *  
     */
    void fightGoOn();

    /**
     *  数据
     */
    cocos2d::ValueMap _data;
    
    /**
     *  数据解析
     */
    void parseData(st_BaseData& data);
    
    /**
     *  显示控制器
     */
    BattleViewController *_viewCon;
    
    /**
     *  怪物模型
     */
    Monster * _monster;
    
    /**
     *  玩家模型
     */
    Player *_player;
    
    
    cocos2d::EventListenerCustom *_aceListener;
};

#endif /* defined(__deer__BattleController__) */

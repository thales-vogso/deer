/******************************************************************
        
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/BattleViewController.h
 *********************************************************************/

#ifndef __DEER_BATTLEVIEWCONTROLLER__
#define __DEER_BATTLEVIEWCONTROLLER__

#include "cocos2d.h"
#include "battle/BattleLayer.h"
#include "GlobalMacros.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "crusade/SettlementLayer.h"
#include "battle/BattleControllerDelegate.h"
#include "battle/BattleTextManager.h"


class BattleViewController : public cocos2d::Node
{
public:
    static BattleViewController *create(BattleLayer *target);
//    CREATE_FUNC(BattleViewController);
    
    inline void setTartget(BattleLayer *target)
    {
        _target = target;
    };
    
    inline void setDelegate(BattleControllerDelegate *delegate)
    {
        _delegate = delegate;
    };
    
    /**
     *  怪物效果部分
     */
    void setMonsterHP(float hp , float max);
    
    void playMonsterAttack();
    
    void playMonsterCD(float interval);
    
    void playMonsterHurt(float hp);
    
    void playMonsterMiss();
    
    void playMonsterLost();
    
    void playMonsterWin();
    
    void playMonsterStun(float interval);
    
    void playMonsterLastDropHP(float hp ,float interval);
    
    
    /**
     *  玩家效果部分
     */
    void setPlayerHP(float hp, float max);
    
    void playPlayerAttack(int wid);
    void playPlayerAttack();
    
    void playSkillCD(int skillid , float interval);
    
    void playPlayerHurt(float hp);
    
    void playPlayerHeal(float hp);
    
    void playPlayerMiss();
    
    void playPlayerLost();
    
    void playPlayerWin();
    
    /**
     *  弹层控制 部分
     */
    /**
     *  目前战斗场景，的界面包含
     *   
     *  1、掉落界面
     *  2、建筑胜利界面
     *  3、进入选择界面
     *
     */
    void showSettlement(cocos2d::ValueVector drops);
    
    void showBuildClearup();
    
    void showEnterChoice();
    
    /**
     *  触控反馈
     */
    void setSkillButtnFunc(SkillBtnFunc func);
    
    void setSkillBtnTouch(bool res);
    
    void setParticles(cocos2d::ParticleSystemQuad*  monsterParticle,
                      cocos2d::ParticleSystemQuad*  playerParticle);
    
protected:
    
    bool init(BattleLayer * target);
    
    /**
     *  触控反馈
     */
    void onTouchBreadBtn();
    
    void onPressedAutoBtn();
    
    /**
     *  其他效果
     */
    void stunEnd();
    
    void monsterShake();
    
    void playerShake();
    
    void stopMonsterShake();
    
    void stopPlayerShake();
    
    void stopPlayerAttkAtcion();
    
    void stopAllShake();
    
    BattleLayer * _target;
    
    cocos2d::ui::Button* _monsterAtkBar;
    
    cocos2d::Node* _monsterNode;
    
    BattleControllerDelegate *_delegate;
    
    BattleTextManager *_txtManager;
    
    cocos2d::ProgressTimer* skillLoad;
    
    //震动结点的起始位置..
    std::vector<cocos2d::Vec2> _monsterPos;
    std::vector<cocos2d::Vec2> _playerPos;
    
};

#endif /* defined(__deer__BattleViewController__) */

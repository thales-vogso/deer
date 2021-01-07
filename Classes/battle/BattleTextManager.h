/****************************************************************************
 *	@desc	文字效果管理
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleTextmanager.h
 ******************************************************************************/

#ifndef __DEER_BATTLETEXTMANAGER__
#define __DEER_BATTLETEXTMANAGER__

#include <iostream>
#include "cocos2d.h"
#include "GlobalMacros.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class BattleTextManager : public cocos2d::Node
{
    enum ModelKey
    {
        kHurtModel = 1,
        kMissModel = 2,
        kHealModel = 3,
        kMonsterHurtModel = 4,
        kMonsterMissModel = 5,
        kMonsterHealModel = 6
    };
public:
    CREATE_FUNC(BattleTextManager)
    
    bool init();
    
    void setTargetAndModel(cocos2d::ui::TextBMFont* hurt, cocos2d::ui::TextBMFont* miss, cocos2d::ui::TextBMFont* heal,
                           cocos2d::ui::TextBMFont* monsterhurt, cocos2d::ui::TextBMFont* monstermiss, cocos2d::ui::TextBMFont* monsterheal
                           );
    //cocos2d::ParticleSystemQuad*  monsterParticle,
    //cocos2d::ParticleSystemQuad*  playerParticle
    void setParticles(cocos2d::ParticleSystemQuad*  monsterParticle,
                      cocos2d::ParticleSystemQuad*  playerParticle);
    void addMonsterHurt(float num);
    
    void addMonsterMiss();
    
    void addMonsterHeal(float num);
    
    
    void addPlayerHurt(float num);
    
    void addPlayerMiss();
    
    void addPlayerHeal(float num);
    
protected:
    
    void onEnter() override;
    
    void update(float delta) override;
    
    cocos2d::Vector<cocos2d::ui::Widget*> _monsterTxt;
    
    cocos2d::Vector<cocos2d::ui::Widget*> _monsterWait;
    
    cocos2d::Vector<cocos2d::ui::Widget*> _playerTxt;
    
    cocos2d::Vector<cocos2d::ui::Widget*> _playerWait;
    
    float _normalMStep;
    
    float _currentMStep;
    
    float _normalPStep;
    
    float _currentPStep;
    
    cocos2d::Map<int, cocos2d::ui::TextBMFont*> _models;
    
    cocos2d::ParticleSystemQuad*  _monsterParticle;
    
    cocos2d::ParticleSystemQuad*  _playerParticle;
    
protected:
    
    float _topY;
    
    float _bottomY;
    
    cocos2d::Vec2  _monsterPos = cocos2d::Vec2(0,0);
    cocos2d::Vec2  _playerPos  = cocos2d::Vec2(0,0);
};

#endif /* defined(__deer__BattleTextManager__) */

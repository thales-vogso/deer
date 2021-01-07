/****************************************************************************
              
 *	@desc	战斗实体的基类
 *	@date	2015-5-11
 *	@author	110101
     
 *	@file	battle/BattleViewController.cpp
 ******************************************************************************/
#include "BattleViewController.h"
#include "BattleController.h"
#include "utils/DeerUtils.h"
#include "utils/AndroidCall.h"
#include "utils/LoadingAction.h"

#include "crusade/EnterChoicePop.h"
#include "data/Crusade.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"
#include "data/Lang.h"

USING_NS_CC;
USING_NS_DU;

#define TAG_VIBRATE 301
#define TAG_MONSTER_CD 302

#define STREAK_Z 100
#define STREAK_STEP 50

#define POPUP_LEVEL_1 60    //1级pop
#define POPUP_LEVEL_2 70    //2级pop
#define POPUP_LEVEL_3 80    //....

BattleViewController* BattleViewController::create(BattleLayer *target)
{
    BattleViewController *ret = new (std::nothrow)BattleViewController();
    if (ret && ret->init(target))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return ret;
    }
}

bool BattleViewController::init(BattleLayer * target)
{
    if (!Node::init())
    {
        return false;
    }
    
    _target = target;
    
    _monsterAtkBar = seekChildByNameWithRetType<ui::Button*>(_target, "mskillBtn");
    _monsterNode   = seekChildByName(_target, "monster");
    if (!_monsterAtkBar)
    {
        assert(0);
    }
    skillLoad = ProgressTimer::create(Sprite::create("difangcd.png"));
    skillLoad->setAnchorPoint(Vec2(0.5,0.5));
    skillLoad->setPosition(Vec2(56.5,57));
    _monsterAtkBar->addChild(skillLoad);
    skillLoad->setPercentage(0);
    /**
     *  初始化基本反馈逻辑
     */
    _target->recovery = CC_CALLBACK_0(BattleViewController::onTouchBreadBtn, this);
    _target->onPressedAutoBattle = CC_CALLBACK_0(BattleViewController::onPressedAutoBtn, this);
    
    _txtManager = BattleTextManager::create();
    _txtManager->setTargetAndModel(seekChildByNameWithRetType<ui::TextBMFont*>(_target, "lifeDecreaseFlag"),
                          seekChildByNameWithRetType<ui::TextBMFont*>(_target, "playermiss"),
                          seekChildByNameWithRetType<ui::TextBMFont*>(_target, "lifeRecoverFlag"),
                          seekChildByNameWithRetType<ui::TextBMFont*>(_target, "monsterDecrease"),
                          seekChildByNameWithRetType<ui::TextBMFont*>(_target, "monstermiss"),
                          seekChildByNameWithRetType<ui::TextBMFont*>(_target, "monsterRecover"));
    this->addChild(_txtManager, 100);
    
    return true;
}
void BattleViewController::setParticles(cocos2d::ParticleSystemQuad*  monsterParticle,
                                        cocos2d::ParticleSystemQuad*  playerParticle)
{
    _txtManager->setParticles(monsterParticle, playerParticle);
}

/********************怪物效果*******************/
void BattleViewController::setMonsterHP(float hp , float max)
{
    seekChildByNameWithRetType<ui::Text*>(_target, "monsterlife")->setString(autoKeepToString(hp) + "/" + autoKeepToString(max));
    seekChildByNameWithRetType<ui::LoadingBar*>(_target, "monsterLifeBar")->setPercent((hp / max) * 100);
}

void BattleViewController::playMonsterAttack()
{
    if(!((BattleController*)_delegate)->getMonster()->getAttackPause())
    {
        //shake(_target);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kEnemyAtk);
    
        //添加怪物攻击粒子..
        auto mskillBtn = seekChildByNameWithRetType<ui::Button*>(_target, "mskillBtn");
        auto view = seekChildByNameWithRetType<ui::Layout*>(_target, "Panel_1");
    
        Vec2 startPos = Vec2(mskillBtn->getPositionX()+view->getPositionX(),mskillBtn->getPositionY()+view->getPositionY());
        auto player   = seekChildByNameWithRetType<ui::Text*>(_target, "ourLife");
        Vec2 endPos   = Vec2(player->getPositionX()-player->getContentSize().width,player->getPositionY());
    
        auto par = ParticleSystemQuad::create("difanggongji.plist");
        //par->setStartSize(par->getStartSize()*SCALEY);
        par->resetSystem();
        par->setAngle(180-DeerUtils::getAngleTwoPoint(startPos, endPos));
        par->setPosition(startPos);
        _target->addChild(par);
        auto act  = CallFunc::create([=](){
            par->removeFromParent();
        });
   
        auto move = MoveTo::create(0.5, endPos);
        if(_delegate)//攻击循环继续则释放技能..
        {
            if(!((BattleController*)_delegate)->getMonster()->getAttackPause())
            {
                par->runAction(Sequence::create(move, act,NULL));
                
                auto act1 = CallFunc::create([=](){
                    auto par1 = ParticleSystemQuad::create("direnzhakai.plist");
                    par1->resetSystem();
                    par1->setPosition(endPos);
                    par1->setAutoRemoveOnFinish(true);
                    _target->addChild(par1);
                });
                auto act2 = CallFunc::create([=](){
                    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kHit);
                    ((BattleController*)_delegate)->getMonster()->attack();
                });
                this->runAction(Sequence::create(DelayTime::create(0.5),act1,act2, NULL));
                
            }
        }
    }

}

void BattleViewController::playMonsterCD(float interval)
{
    
    skillLoad->stopAllActionsByTag(TAG_MONSTER_CD);
    auto load = ProgressTimerAction::create(interval);
    load->setTag(TAG_MONSTER_CD);
    skillLoad->runAction(load);
}

void BattleViewController::playMonsterHurt(float hp)
{
    this->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create([=](){
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kEnemyHurt);
    }),NULL));
    _txtManager->addMonsterHurt(-hp);
    //添加怪物被击中震动..
    monsterShake();
}

void BattleViewController::playMonsterMiss()
{
    _txtManager->addPlayerMiss();
}

void BattleViewController::playMonsterLost()
{
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kEnemyDie);
    _monsterAtkBar->stopAllActions();
    stopPlayerAttkAtcion();
    stopAllShake();
}

void BattleViewController::playMonsterWin()
{
    auto rflag = seekChildByNameWithRetType<ui::Text*>(_target, "resultFlag");
    rflag->setString(data::Lang::getInstance()->getWord(130320));
    rflag->setVisible(true);
}

void BattleViewController::playMonsterStun(float interval)
{
    //暂停怪物动画..
    _monsterNode->pause();
    
    //暂停攻击进度条..
    _monsterAtkBar->pause();
    CallFunc * fun = CallFunc::create(CC_CALLBACK_0(BattleViewController::stunEnd, this));
    auto seq = Sequence::create(DelayTime::create(interval), fun, nullptr);
    runAction(seq);
}

void BattleViewController::stunEnd()
{
    _monsterAtkBar->resume();
    _monsterNode->resume();
}

void BattleViewController::playMonsterLastDropHP(float hp, float interval)
{
    int TPS = 10;//每秒掉血次数;;
    int totalT(0);
    float dev = hp / int(interval * TPS);
    auto rep = Repeat::create(CallFunc::create([=](){
//        playMonsterHurt(dev);
        _txtManager->addMonsterHurt(-dev);
    }), totalT);
    runAction(rep);
}


/***********玩家效果***********/

void BattleViewController::setPlayerHP(float hp, float max)
{
    auto ourLife = seekChildByNameWithRetType<ui::Text*>(_target, "ourLife");
    ourLife->setString(autoKeepToString(hp) + "/" + autoKeepToString(max));
    
    auto bar = seekChildByNameWithRetType<ui::LoadingBar*>(_target, "ourLifeBar");
    bar->stopAllActions();
    if(hp > bar->getPercent()/100*max)
    {
        LoadingAction* action = LoadingAction::create(0.5,bar->getPercent(),hp/max*100);
        bar->runAction(action);
    }
    else
    {
        bar->setPercent(hp/max*100);
    }
    
    
}

void BattleViewController::playPlayerAttack(int wid)
{
    if(!((BattleController*)_delegate)->getPlayer()->getAttackPause())
    {
        CCLOG("attack ");
       
        
        //添加战士攻击粒子..
        if(wid >= 1 && wid < _target->_skillButtonVec.size()+1)
        {
            if(((BattleController*)_delegate)->getPlayer()->isCanAttack(wid))
            {
                SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPlayerAtk);
                Vec2 startPos = _target->_skillButtonVec.at(wid-1)->getPosition();
                auto monster  = seekChildByNameWithRetType<ui::ImageView*>(_target, "monster");
                auto view = seekChildByNameWithRetType<ui::Layout*>(_target, "Panel_1");
                Vec2 endPos   = Vec2(monster->getPositionX()+view->getPositionX(),monster->getPositionY()+view->getPositionY());
                
                auto par = ParticleSystemQuad::create("wofanggongji.plist");
                par->resetSystem();
                par->setAngle(360-DeerUtils::getAngleTwoPoint(startPos, endPos));
                //par->setStartSize(par->getStartSize()*2*SCALEY);
                par->setPosition(startPos);
                _target->addChild(par);
                auto move = MoveTo::create(0.5, endPos);
                auto act  = CallFunc::create([=](){
                    //((BattleController*)_delegate)->getPlayer()->attack(wid);
                    par->removeFromParent();
                });
                par->runAction(Sequence::create(move, act,NULL));
                
                auto act1  = CallFunc::create([=](){
                    auto par1 = ParticleSystemQuad::create("wofangzhakai.plist");
                    par1->resetSystem();
                    par1->setPosition(endPos);
                    par1->setAutoRemoveOnFinish(true);
                    _target->addChild(par1);
                }  );
                auto act2 = CallFunc::create([=](){
                    ((BattleController*)_delegate)->getPlayer()->attack(wid);
                }  );
                this->runAction(Sequence::create(DelayTime::create(0.5),act1,act2, NULL) );
            }
            
        }

    }
}
void BattleViewController::playPlayerAttack()
{
    if(!((BattleController*)_delegate)->getPlayer()->getAttackPause())
    {
        for(int i = 0; i < _target->_skillButtonVec.size();i++)
        {
            if(((BattleController*)_delegate)->getPlayer()->isCanAttack(i+1))
            {
                SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPlayerAtk);
                Vec2 startPos = _target->_skillButtonVec.at(i)->getPosition();
                auto monster  = seekChildByNameWithRetType<ui::ImageView*>(_target, "monster");
                auto view = seekChildByNameWithRetType<ui::Layout*>(_target, "Panel_1");
                Vec2 endPos   = Vec2(monster->getPositionX()+view->getPositionX(),monster->getPositionY()+view->getPositionY());
                
                auto par = ParticleSystemQuad::create("wofanggongji.plist");
                par->resetSystem();
                par->setAngle(360-DeerUtils::getAngleTwoPoint(startPos, endPos));
                //par->setStartSize(par->getStartSize()*SCALEY);
                par->setPosition(startPos);
                _target->addChild(par);
                auto move = MoveTo::create(0.5, endPos);
                auto act  = CallFunc::create([=](){
                    ((BattleController*)_delegate)->getPlayer()->attack(i+1);
                    par->removeFromParent();
                }  );
                par->runAction(Sequence::create(move, act,NULL));
                
                auto act1  = CallFunc::create([=](){
                    auto par1 = ParticleSystemQuad::create("wofangzhakai.plist");
                    par1->resetSystem();
                    par1->setPosition(endPos);
                    par1->setAutoRemoveOnFinish(true);
                    _target->addChild(par1);
                }  );
                auto act2 = CallFunc::create([=](){
                    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kHit);
                    ((BattleController*)_delegate)->getPlayer()->attack(i+1);
                }  );
                this->runAction(Sequence::create(DelayTime::create(0.5),act1,act2, NULL) );
            }
        }
    }
}

void BattleViewController::playSkillCD(int skillid, float interval)
{
    if(_target->_skillButtonVec.size() >= skillid)
    {
        _target->_skillButtonVec.at(skillid - 1)->runCDAction(interval);
    }
}

void BattleViewController::playPlayerHurt(float hp)
{
    this->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create([=](){
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPlayerHurt);
    }),NULL));
    
    _txtManager->addPlayerHurt(-hp);
    //添加我方被攻击震动..
    playerShake();
    
}

void BattleViewController::playPlayerHeal(float hp)
{
    _txtManager->addPlayerHeal(hp);
}

void BattleViewController::playPlayerMiss()
{
    _txtManager->addMonsterMiss();
}

void BattleViewController::playPlayerLost()
{
//    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_PlayerDie);
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPlayerDie);
    stopPlayerAttkAtcion();
}

void BattleViewController::playPlayerWin()
{
    auto rflag = seekChildByNameWithRetType<ui::Text*>(_target, "resultFlag");
    rflag->setVisible(true);
}

/********** 界面产生逻辑和反馈逻辑 ***********/

void BattleViewController::showSettlement(ValueVector drops)
{
    auto settlementLayer = SettlementLayer::create(drops);
    settlementLayer->setDelegate(_target);
    settlementLayer->onShut = CC_CALLBACK_0(BattleControllerDelegate::onExitFromSettlement, _delegate);
    _target->addChild(settlementLayer, 10);
}

void BattleViewController::showBuildClearup()
{
    auto pop = BuildClearPop::create();
    pop->onSure = CC_CALLBACK_0(BattleControllerDelegate::onSureInBuildClearup, _delegate);
    _target->addChild(pop, POPUP_LEVEL_1);
}

void BattleViewController::showEnterChoice()
{
    auto pop = BattleChoice::create();
    pop->setQuitFunc([=](){
        auto cinfo = data::Crusade::getInstance()->getUserCrusade().asValueMap();
        CrusadeMap::goToCrusadeMapScene(cinfo["sid"].asInt());
    });
    _target->addChild(pop, POPUP_LEVEL_1);
}

void BattleViewController::setSkillButtnFunc(SkillBtnFunc func)
{
    for (auto &i : _target->_skillButtonVec)
    {
        i->_btnFunc = func;
    }
}
void BattleViewController::setSkillBtnTouch(bool res)
{
    for (auto &i: _target->_skillButtonVec)
    {
        i->setTouchResponse(res);
    }
}

void BattleViewController::onTouchBreadBtn()
{

    _delegate->onTouchBreadBtn();
    
}

void BattleViewController::onPressedAutoBtn()
{
    
    _delegate->onPressedAutoBattleBtn();
    
}

/************效果工具**********/

void BattleViewController::stopPlayerAttkAtcion()
{
    //auto &streak = _target->_streak1;
    //if (streak)
    //{
        //streak->stopAllActions();
        //streak->removeFromParent();
        //streak = nullptr;
    //}
    
}

void BattleViewController::monsterShake()
{
    int shakeStep(10);
    auto vibrate = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate->setTag(TAG_VIBRATE);
    auto vibrate2 = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate2->setTag(TAG_VIBRATE+1);
    auto vibrate3 = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate3->setTag(TAG_VIBRATE+2);
    auto vibrate4 = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate4->setTag(TAG_VIBRATE+3);
    
    auto node1 = seekChildByNameWithRetType<ui::Text*>(_target, "monstername");
    auto node2 = seekChildByNameWithRetType<ui::Text*>(_target, "monsterlife");
    auto node3 = seekChildByNameWithRetType<ui::LoadingBar*>(_target, "monsterLifeBar");
    auto node4 = seekChildByNameWithRetType<ui::ImageView*>(_target, "monster");
    auto panel = seekChildByNameWithRetType<ui::Layout*>(_target, "Panel_1");
    if(_monsterPos.size()< 4)
    {
        _monsterPos.push_back(node1->getPosition());
        _monsterPos.push_back(node2->getPosition());
        _monsterPos.push_back(node3->getPosition());
        _monsterPos.push_back(node4->getPosition());
    }
    
    node1->runAction(vibrate);
    node2->runAction(vibrate2);
    node3->runAction(vibrate3);
    node4->runAction(vibrate4);
    
    phoneVibrate();
    
    /**
     * 怪物流血效果
     */
    auto monblo = ParticleSystemQuad::create("blood1.plist");
    monblo->setSpeed(300);
    monblo->setSpeedVar(100);
    //monblo->setNormalizedPosition(Vec2(0.3, 0.7));
    monblo->setPosition(Vec2(node4->getPositionX()+panel->getPositionX(),node4->getPositionY()+panel->getPositionY()));
    monblo->setAutoRemoveOnFinish(true);
    _target->addChild(monblo);
}
void BattleViewController::playerShake()
{
    int shakeStep(10);
    auto vibrate = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate->setTag(TAG_VIBRATE+4);
    auto vibrate2 = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate2->setTag(TAG_VIBRATE+5);
    auto vibrate3 = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
    vibrate3->setTag(TAG_VIBRATE+6);
    
    
    auto node1 = seekChildByNameWithRetType<ui::Text*>(_target, "Text_6");
    auto node2 = seekChildByNameWithRetType<ui::Text*>(_target, "ourLife");
    auto node3 = seekChildByNameWithRetType<ui::LoadingBar*>(_target, "ourLifeBar");
    if(_playerPos.size() < 3)
    {
        _playerPos.push_back(node1->getPosition());
        _playerPos.push_back(node2->getPosition());
        _playerPos.push_back(node3->getPosition());
    }
    node1->runAction(vibrate);
    node2->runAction(vibrate2);
    node3->runAction(vibrate3);
    
    phoneVibrate();
    
    /**
     * 怪物流血效果
     */
    auto monblo = ParticleSystemQuad::create("blood1.plist");
    monblo->setSpeed(300);
    monblo->setSpeedVar(100);
    monblo->setPosition(node2->getPosition());
    monblo->setAutoRemoveOnFinish(true);
    _target->addChild(monblo);
    
}
void BattleViewController::stopMonsterShake()
{
    auto node1 = seekChildByNameWithRetType<ui::Text*>(_target, "monstername");
    auto node2 = seekChildByNameWithRetType<ui::Text*>(_target, "monsterlife");
    auto node3 = seekChildByNameWithRetType<ui::LoadingBar*>(_target, "monsterLifeBar");
    auto node4 = seekChildByNameWithRetType<ui::ImageView*>(_target, "monster");
    node1->stopAllActionsByTag(TAG_VIBRATE);
    node2->stopAllActionsByTag(TAG_VIBRATE+1);
    node3->stopAllActionsByTag(TAG_VIBRATE+2);
    node4->stopAllActionsByTag(TAG_VIBRATE+3);
    if(_monsterPos.size() >= 4 )
    {
        node1->setPosition(_monsterPos.at(0));
        node2->setPosition(_monsterPos.at(1));
        node3->setPosition(_monsterPos.at(2));
        node4->setPosition(_monsterPos.at(3));
    }
    

}
void BattleViewController::stopPlayerShake()
{
    auto node1 = seekChildByNameWithRetType<ui::Text*>(_target, "Text_6");
    auto node2 = seekChildByNameWithRetType<ui::Text*>(_target, "ourLife");
    auto node3 = seekChildByNameWithRetType<ui::LoadingBar*>(_target, "ourLifeBar");
    node1->stopAllActionsByTag(TAG_VIBRATE+4);
    node2->stopAllActionsByTag(TAG_VIBRATE+5);
    node3->stopAllActionsByTag(TAG_VIBRATE+6);
    if(_playerPos.size() >= 3)
    {
        node1->setPosition(_playerPos.at(0));
        node2->setPosition(_playerPos.at(1));
        node3->setPosition(_playerPos.at(2));
    }
    
}

void BattleViewController::stopAllShake()
{
    stopPlayerShake();
    stopMonsterShake();
}

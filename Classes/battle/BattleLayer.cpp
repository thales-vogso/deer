/****************************************************************************
 *	@desc	战斗界面
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleLayer.cpp
 ******************************************************************************/

#include "BattleLayer.h"
#include "utils/DeerUtils.h"
#include "crusade/SettlementLayer.h"
#include "manor/Manor.h"
#include "crusade/CrusadeMap.h"
#include "popUp/ChoicePopup.h"
#include "utils/LoadingAction.h"
#include "crusade/EnterChoicePop.h"
#include "utils/AndroidCall.h"
#include "utils/DoubleClickExit.h"
#include "popUp/CommenPopup.h"
#include "popUp/TouchEffect.h"
#include "GlobalMacros.h"
#include "battle/SwordMove.h"

#include "battle/BattleTextManager.h"
#include "battle/BattleController.h"
#include "sound/SoundManager.h"
#include "guide/Guide.h"

USING_NS_CC;
USING_NS_DU;

#define STREAK_Z 100
#define STREAK_STEP 50
#define QUIT_LAY_Z 52
#define VIBRATE_ACT 10

std::string const BattleLayer::PLAYER_DIE("PlayerDie");
std::string const BattleLayer::MONSTER_DIE("MonsterDie");
std::string const BattleLayer::SkillButton::PLAYER_ATTACK("PlayerAttack");

BattleLayer::~BattleLayer()
{
    //data::Crusade::getInstance()->removeListener(_encountListener);
    //data::Crusade::getInstance()->removeListener(_clearListener);
}
void BattleLayer::gotoBattleScene(ValueMap battleData, bool isInBuilding)
{
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BackGroundMusic_Battle, true);
    SoundManager::getInstance()->stopMusic();
    SoundManager::getInstance()->playMusic(SoundManager::MusicType::kBattle);
    
    Scene * battleScene = Scene::create();
    auto layer = BattleLayer::create(battleData);
    battleScene->addChild(layer);
    Director::getInstance()->replaceScene(battleScene);
}

BattleLayer *BattleLayer::create(cocos2d::ValueMap battleData, bool isInBuilding)
{
    BattleLayer *ret = new BattleLayer();
    if (ret && ret->init(battleData, isInBuilding))
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

bool BattleLayer::init(cocos2d::ValueMap battleData, bool isInBuilding)
{
    if (!Layer::init())
    {
        return false;
    }

    _battleData = battleData;
//    auto iiii = battleData;
//    if (battleData["field"].isNull())
//    {
//        _inBuilding = false;
//    }
//    else
//    {
//        _inBuilding = true;
//    }
    
    auto vv = data::Crusade::getInstance()->fieldView();
    
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("encounter.csb");
    view->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //初始化拖动渐隐对象..
    //_streak1 = SwordMove::create();
    //addChild(_streak1, STREAK_Z);

    //初始化敌人的血量数据..
    _lifeMap["monsterTotal"] = _battleData["blood"];
    _lifeMap["monsterCurrent"] = _battleData["blood"];
    
    //初始化敌人的显示...
    std::string monstername = battleData["img"].asString();
    
    auto monster = seekChildByNameWithRetType<ui::ImageView*>(this, "monster");
    monster->loadTexture(monstername);
    monster->setPositionY(monster->getPositionY()- (winSize.height - SCREEN_HEIGHT)/2);
    auto monstermask = seekChildByNameWithRetType<ui::ImageView*>(view, "mask");
    monstermask->setPositionY(monstermask->getPositionY() - (winSize.height - SCREEN_HEIGHT)/2);
    seekChildByNameWithRetType<ui::Text*>(this, "monstername")->setString(battleData["name"].asString());
    seekChildByNameWithRetType<ui::Text*>(this, "monsterDes")->setString(battleData["summary"].asString());
    
    auto playerList = data::Crusade::getInstance()->getCarriage().asValueVector();

    /**
     *  我方显示初始化
     */
    //初始化cd按钮的Model
    auto skillmodel = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn");
    
    _cdBtnModel.pushBack(skillmodel);
    skillmodel->removeFromParent();
    auto originpos = skillmodel->getPosition();
    
    auto skillBtn1 = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn1");
    auto skillBtn2 = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn2");
    auto skillBtn3 = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn3");
    auto skillBtn4 = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn4");
    auto skillBtn5 = seekChildByNameWithRetType<ui::Button*>(view, "skillBtn5");
    auto newY = originpos.y;
    std::vector<Vec2> pos({Vec2(skillBtn2->getPositionX(), newY),Vec2(skillBtn5->getPositionX(), newY-152),Vec2(skillBtn1->getPositionX(), newY),Vec2(skillBtn4->getPositionX(), newY - 152),Vec2(skillmodel->getPositionX(), newY),Vec2(skillBtn3->getPositionX(), newY - 152)});
    skillBtn1->removeFromParent();
    skillBtn2->removeFromParent();
    skillBtn3->removeFromParent();
    skillBtn4->removeFromParent();
    skillBtn5->removeFromParent();
    
    int posN = 0;
    for (auto i(0) ; i < playerList.size(); i++)
    {
        int num = playerList.at(i).asValueMap()["current"].asInt();
        for (int j(0) ;j < num; j++)
        {
            
            auto button = SkillButton::create(playerList.at(i).asValueMap(), this);
            button->setBtnId(posN + 1);
            button->setPosition(pos.at(posN++));
            button->addListener(SkillButton::PLAYER_ATTACK, CC_CALLBACK_1(BattleLayer::playerAttack, this));
            this->addChild(button);
            _skillButtonVec.pushBack(button);
        }
    }
    
    //根据玩家设置数据初始化是否自动战斗..
    autoBattle = seekChildByNameWithRetType<ui::CheckBox*>(this, "autoBtn");
    autoBattle->setSelected(data::User::getInstance()->isAutoBattle());
    autoBattle->addEventListener(CC_CALLBACK_2(BattleLayer::autoBattleButtonCallback, this));
    
    //面包按钮..
    breadBtn = seekChildByNameWithRetType<ui::Button*>(view, "breadBtn");
    ProgressTimer* breadLoad = ProgressTimer::create(Sprite::create("zidongzhandou.png"));
    breadLoad->setAnchorPoint(Vec2(0.5,0.5));
    breadLoad->setPosition(Vec2(56.5, 57));
    breadLoad->setName("breadLoad");
    breadBtn->addChild(breadLoad);
    
    
    breadBtn->addTouchEventListener(CC_CALLBACK_2(BattleLayer::recoveryButtonCallback, this));
    seekChildByNameWithRetType<ui::Text*>(this, "breadNum")->setString(Value(data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD)).asString());
    
    seekChildByNameWithRetType<ui::TextBMFont*>(view, "lifeDecreaseFlag")->setVisible(false);
    
    auto autoBtn = seekChildByNameWithRetType<ui::CheckBox*>(view, "autoBtn");
    auto selfTxt = seekChildByNameWithRetType<ui::Text*>(view, "Text_6");
    auto ourLife = seekChildByNameWithRetType<ui::Text*>(view, "ourLife");
    auto ourLb = seekChildByNameWithRetType<ui::LoadingBar*>(view, "ourLifeBar");
    //    auto breadBtn = seekChildByNameWithRetType<ui::Button*>(view, "breadBtn");
//    auto breadTip = seekChildByNameWithRetType<ui::ImageView*>(view, "Image_4");
    auto lifeDec = seekChildByNameWithRetType<ui::TextBMFont*>(view, "lifeDecreaseFlag");
    auto lifeRec = seekChildByNameWithRetType<ui::TextBMFont*>(view, "lifeRecoverFlag");
    auto playermiss = seekChildByNameWithRetType<ui::TextBMFont*>(view, "playermiss");
    lifeRec->setVisible(false);
    playermiss->setVisible(false);
    
    autoBtn->retain();autoBtn->removeFromParent();this->addChild(autoBtn);autoBtn->release();
    selfTxt->retain();selfTxt->removeFromParent();this->addChild(selfTxt);selfTxt->release();
    ourLife->retain();ourLife->removeFromParent();this->addChild(ourLife);ourLife->release();
    ourLb->retain();ourLb->removeFromParent();this->addChild(ourLb);ourLb->release();
    breadBtn->retain();breadBtn->removeFromParent();this->addChild(breadBtn);breadBtn->release();
//    breadTip->retain();breadTip->removeFromParent();this->addChild(breadTip);breadTip->release();
    lifeDec->retain();lifeDec->removeFromParent();addChild(lifeDec);lifeDec->release();
    lifeRec->retain();lifeRec->removeFromParent();addChild(lifeRec);lifeRec->release();
    playermiss->retain();playermiss->removeFromParent();addChild(playermiss);playermiss->release();
    
    ParticleSystemQuad* monsterBlood = ParticleSystemQuad::create("bloodadd.plist");
    ParticleSystemQuad* playerBlood  = ParticleSystemQuad::create("bloodadd.plist");
    
    Vec2 mbloodPos =  seekChildByNameWithRetType<ui::LoadingBar*>(this, "monsterLifeBar")->getPosition();
    Vec2 pbloodPos =  seekChildByNameWithRetType<ui::LoadingBar*>(this, "ourLifeBar")->getPosition();
    monsterBlood->setPosition(Vec2(mbloodPos.x+seekChildByNameWithRetType<ui::LoadingBar*>(this, "monsterLifeBar")->getContentSize().width,mbloodPos.y));
    playerBlood->setPosition(Vec2(pbloodPos.x-seekChildByNameWithRetType<ui::LoadingBar*>(this, "ourLifeBar")->getContentSize().width,pbloodPos.y));
    view->getChildByName("Panel_1")->addChild(monsterBlood);
    addChild(playerBlood);
    //事件的侦听..
    _encountListener = data::Crusade::getInstance()->addListener(data::Crusade::ENCOUNTER, CC_CALLBACK_1(BattleLayer::encountCallback, this));
    addEventPool(_encountListener,data::Crusade::getInstance());
    _clearListener = data::Crusade::getInstance()->addListener(data::Crusade::FIELD_CLEAR, CC_CALLBACK_1(BattleLayer::fieldClearCallback, this));
    addEventPool(_clearListener,data::Crusade::getInstance());
    
    this->addChild(TouchEffect::create(), 155);
    
    //安卓双击返回退出..
    this->addChild(DoubleClickExit::create());
    
    /**
     *  添加显示控制器
     */
    _viewCon = BattleViewController::create(this);
    addChild(_viewCon);
    ((BattleViewController*)_viewCon)->setParticles(monsterBlood,playerBlood);
    
    monsterBlood->setPosition(Vec2(-SCREEN_HEIGHT,-SCREEN_HEIGHT));
    playerBlood->setPosition(Vec2(-SCREEN_HEIGHT,-SCREEN_HEIGHT));
    
    /**
     *  添加逻辑控制器
     */
    batcon = BattleController::create(_battleData, ((BattleViewController*)_viewCon));
    addChild(batcon);
    
    ((BattleViewController*)_viewCon)->setDelegate((BattleController*)batcon);
    
    /**
     *  新手引导
     */
    Guide* guide = Guide::create(data::Guid::BATTLE_LAYER);
    guide->setSkillBtnVector(_skillButtonVec);
    guide->setHero(((BattleController*)batcon)->getPlayer());
    guide->setMonster(((BattleController*)batcon)->getMonster());
    guide->setAutoBattle(autoBattle);
    guide->setBreadBtn(breadBtn);
    this->addChild(guide,10000);
    //如果正在引导中设置所有按钮不可用..
    if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK1))
    {
        guide->setBattleBtnEnable(false);
    }
    
    return true;
}
void BattleLayer::removeSelf()
{

}
void BattleLayer::updateProperty()
{

}
void BattleLayer::playerAttack(cocos2d::EventCustom* event)
{

}

void BattleLayer::playerAttackAnimation(float dt)
{
    /*if (_streak1->getPosition().x < SCREEN_WIDTH)
    {
        _streak1->setPosition(_streak1->getPosition() + Vec2(STREAK_STEP, -STREAK_STEP));
        auto par1 = ParticleSystemQuad::create("swordMove.plist");
        par1->setPosition(_streak1->getPosition());
        par1->setAutoRemoveOnFinish(true);
        addChild(par1);
    }
    else
    {
        unschedule("playerAttack");
    }*/
}

void BattleLayer::autoBattleButtonCallback(cocos2d::Ref *sender, cocos2d::ui::CheckBox::EventType eventType)
{
    if (eventType == ui::CheckBox::EventType::SELECTED)
    {
        setAutoBattle(true);
        if (onPressedAutoBattle)
        {
            onPressedAutoBattle();
        }
        //更新新手引导..
        ((BattleController*)batcon)->getMonster()->setAttackPause(false);
        ((BattleController*)batcon)->getPlayer()->setAttackPause(false);
        ((BattleViewController*)_viewCon)->playPlayerAttack();
        
        data::Guid::getInstance()->updateGuidInfo(data::Guid::BATTLE_AUTO_BTN);
    }
    else if (eventType == ui::CheckBox::EventType::UNSELECTED)
    {
        setAutoBattle(false);
    }
}

void BattleLayer::exitFightingLayer(cocos2d::EventCustom *event)
{
//    if (_inBuilding)
//    {
//        auto nonext = data::Crusade::getInstance()->fieldSiege(_battleData["block"].asValueMap()["x"].asInt(), _battleData["block"].asValueMap()["y"].asInt());
//        if (nonext)
//        {
//            /**无怪胜利**/
//            addChild(BuildClearPop::create(), QUIT_LAY_Z);
//            auto field = data::Crusade::getInstance()->fieldView().asValueMap();
////            data::Crusade::getInstance()->fieldClear(field["x"].asInt(), field["y"].asInt());
//        }
//        else
//        {
//            /**有下一层就询问是否进入**/
//            _battleData["key"] = true;
//            auto pop = BattleChoice::create();
//            /**
//             *   外部挂载一个退出回到地图的函数
//             */
//            pop->_quitCall = [=](){
//                auto sid = data::Crusade::getInstance()->getUserCrusade().asValueMap()["sid"].asInt();
//                CrusadeMap::goToCrusadeMapSceneNoFade(sid);
//            };
//            
//            this->addChild(pop, QUIT_LAY_Z);
//        }
//    }
//    else
//    {
//        /**
//         * 野外遇到怪物，回到地图界面
//         */
//        auto sid = data::Crusade::getInstance()->getUserCrusade().asValueMap()["sid"].asInt();
//        CrusadeMap::goToCrusadeMapSceneNoFade(sid);
//    }
}

void BattleLayer::recoveryButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        //更新新手引导..
        ((BattleController*)batcon)->getMonster()->setAttackPause(false);
        ((BattleController*)batcon)->getPlayer()->setAttackPause(false);
        if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::FOOD_BACK))
        {
            data::User::getInstance()->setAutoBattle(true);
            ((BattleController*)batcon)->getPlayer()->attack(1);
        }
        data::Guid::getInstance()->updateGuidInfo(data::Guid::BATTLE_FOOD_BTN);
        
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        auto breadBtn = (ui::Button*)sender;
        breadBtn->setTouchEnabled(false);
        breadBtn->getChildByName("breadLoad")->setVisible(true);
        
        ProgressTimerAction * act1 = ProgressTimerAction::create(2);
        CallFunc *act2 = CallFunc::create([=](){
            breadBtn->getChildByName("breadLoad")->setVisible(false);
            seekChildByNameWithRetType<ui::Button*>(this, "breadBtn")->setTouchEnabled(true);
            breadBtn->getChildByName("breadText")->setOpacity(255);
        });
        Sequence *actSeq = Sequence::create(act1, act2, NULL);
        breadBtn->getChildByName("breadLoad")->runAction(actSeq);
        
        if (recovery)
        {
            recovery();
        }

        /**
         *  刷新面包数目..
         */
        int breadcount = data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD);
        std::string breadNum = Value(breadcount).asString();
        seekChildByNameWithRetType<ui::Text*>(this, "breadNum")->setString(breadNum);
        
        //面包计数..
        data::Achievement::getInstance()->count(data::Achievement::CountID::EAT_FOOD);
        data::Task::getInstance()->eatCheck();
        if (breadcount == 0)
        {
            seekChildByNameWithRetType<ui::Button*>(this, "breadBtn")->setEnabled(false);
        }

    }
}

void BattleLayer::encountCallback(cocos2d::EventCustom *event)
{
    auto data =  (*(Value*)(event->getUserData()));
    DelayTime *delay = DelayTime::create(0.3);
    CallFunc *fun = CallFunc::create([=](){
        BattleLayer::gotoBattleScene(data.asValueMap());});
    this->runAction(Sequence::create(delay, fun, NULL));
}

void BattleLayer::fieldClearCallback(cocos2d::EventCustom *event)
{
//    CrusadeMap::goToCrusadeMapScene(sid);
}

//void BattleLayer::shakeScreen(cocos2d::Node * target)
//{
//    int shakeStep(10);
//    auto vibrate = Sequence::create(MoveBy::create(0.1, Vec2(-shakeStep, 0)), MoveBy::create(0.1, Vec2(0, shakeStep)), MoveBy::create(0.1,Vec2(shakeStep, 0)), MoveBy::create(0.1, Vec2(0, -shakeStep)), NULL);
//    vibrate->setTag(VIBRATE_ACT);
//    target->runAction(vibrate);
//    phoneVibrate();
//}

void BattleLayer::setAutoBattle(bool autbat)
{
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
//    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
    data::User::getInstance()->setAutoBattle(autbat);
//    for (auto i : _skillButtonVec)
//    {
//        i->setAutoBattle(autbat);
//    }
    
}
void BattleLayer::stopAutoBattleAction()
{
//    for (auto i : _skillButtonVec)
//    {
//        i->setAutoBattle(false);
//    }
}

/******************技能按钮*************************/

BattleLayer::SkillButton *BattleLayer::SkillButton::create(cocos2d::ValueMap roleData, BattleLayer *delegate)
{
    SkillButton *ret = new SkillButton();
    ret->_delegate = delegate;
    if (ret && ret->init(roleData))
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

bool BattleLayer::SkillButton::init(cocos2d::ValueMap roleData)
{
    if (!Node::init())
    {
        return false;
    }
    _roleData = roleData;
    _isAutoBattle = false;
    //初始化显示..
    auto view = _delegate->_cdBtnModel.at(0)->clone();
    _view = view;
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
    auto button = (ui::Button*)view;;
    button->setTouchEnabled(false);
    skillLoad = ProgressTimer::create(Sprite::create("zidongzhandou.png"));
    skillLoad->setAnchorPoint(Vec2(0.5,0.5));
    skillLoad->setPosition(Vec2(56.5,57));
    view->addChild(skillLoad);
    skillLoad->setPercentage(0);
    
    seekChildByNameWithRetType<ui::Layout*>(this, "skillBtnArea")->addTouchEventListener(CC_CALLBACK_2(BattleLayer::SkillButton::skillButtonCallback, this));
    auto skillText = seekChildByNameWithRetType<ui::Text*>(this, "SkillText");
    skillText->setString(DeerUtils::insertEnterInChineseStr(_roleData["skill"].asString(), 6) );
    seekChildByNameWithRetType<ui::Layout*>(this, "skillBtnArea")->setOpacity(0);
    return true;
}

void BattleLayer::SkillButton::setTouchResponse(bool res)
{
    seekChildByNameWithRetType<ui::Layout*>(this, "skillBtnArea")->setTouchEnabled(res);
}

void BattleLayer::SkillButton::skillButtonCallback(Ref* sender, ui::Button::TouchEventType eventType)
{
    if (eventType == ui::Layout::TouchEventType::ENDED)
    {
        if(!data::User::getInstance()->isAutoBattle()&&isAttackEnd)
        {
            SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
            if (_btnFunc)
            {
                isAttackEnd = false;
                _btnFunc(_btnid);
            }
        }
        
    }
}

void BattleLayer::SkillButton::runCDAction(float interval)
{
    skillLoad->setVisible(true);
    auto seq = Sequence::create(ProgressTimerAction::create(interval), CallFunc::create(CC_CALLBACK_0(SkillButton::CDEnd, this)), NULL);
    skillLoad->runAction(seq);
}

void BattleLayer::SkillButton::CDEnd()
{
    isAttackEnd = true;
    this->getChildByName("skillBtn")->getChildByName("SkillText")->setOpacity(255);
    skillLoad->setVisible(false);
    seekChildByNameWithRetType<ui::Layout*>(this, "skillBtnArea")->setTouchEnabled(true);
}


bool BattleLayer::SkillButton::isAutoBattle()
{
    return _isAutoBattle;
}

void BattleLayer::SkillButton::setAutoBattle(bool autoBattle)
{
    _isAutoBattle = autoBattle;
    if (_isAutoBattle)
    {
        auto checkFunc = CallFuncN::create([=](Node* node)
        {
            auto button = seekChildByNameWithRetType<ui::Button*>(this, "skillBtn");
            if(button->isTouchEnabled())
            {
                this->skillButtonCallback(button, ui::Button::TouchEventType::ENDED);
                node->stopActionByTag(50);
            }});
        DelayTime *delay = DelayTime::create(1);
        Sequence *seq = Sequence::create(checkFunc, delay, NULL);
        auto rep = RepeatForever::create(seq);
        rep->setTag(50);
        this->runAction(rep);
    }
}

void BattleLayer::SkillButton::endBattleCallback(cocos2d::EventCustom *event)
{
    
}
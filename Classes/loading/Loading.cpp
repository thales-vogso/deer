/********************************************************
  
 *	@desc	入口
 *	@date	2014-12-12
 *	@author	110101
     
 *	@file	loading/Loading.cpp
 *********************************************************/

#include "Loading.h"
#include "utils/DeerUtils.h"
#include "utils/LoadingAction.h"
#include "popUp/BlackMask.h"
#include "popUp/TouchEffect.h"
#include "GlobalMacros.h"
#include "data/User.h"
#include "battle/Monster.h"
#include "sound/SoundManager.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MobClickCpp.h"

#endif

USING_NS_CC;
USING_NS_DU;
const int Loading::LOADINGTIME = 10;

bool Loading::init()
{
    if (!Layout::init())
    {
        return false;
    }
    _progress = 0;

  //  if (time(NULL) > DeerUtils::_appTime)//2015/12/30日到期 1435136400 1436716800
//    if (time(NULL) > 1457280000)
//    {
//        Director::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        exit(0);
//#endif
//        return true;
//    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string value = umeng::MobClickCpp::getConfigParam("Valid");
    std::string time = LibDate::date("", true);
    if (time > value)
    {
        umeng::MobClickCpp::end();
        Director::getInstance()->end();
        exit(0);
    }
#endif
	this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("start.csb");
    view->setContentSize(win);
    
    addChild(view);
    ui::Helper::doLayout(this);
    
    auto act = CSLoader::getInstance()->createTimeline("loading.csb");
    this->runAction(act);
    act->gotoFrameAndPlay(0, true);
    
    _bar = seekChildByNameWithRetType<ui::LoadingBar*>(this, "LoadingBar_1");
    
    this->addChild(TouchEffect::create(), 155);
    
    return true;
}

void Loading::onEnter()
{
    Layout::onEnter();
    
    _delegate->gameInit();
}

void Loading::guideCheck()
{
    if (data::User::getInstance()->getFirstPlay())
    {
        seekChildByNameWithRetType<Node*>(this, "ProjectNode_1")->setVisible(false);
        auto guide = seekChildByNameWithRetType<Node*>(this, "ProjectNode_2");
        _guideact = CSLoader::getInstance()->createTimeline("guid.csb");
        guide->runAction(_guideact);
        _guideact->gotoFrameAndPlay(0, false);
        _guideact->setLastFrameCallFunc(CC_CALLBACK_0(Loading::loadingEnd, this));
        
        SoundManager::getInstance()->playMusic(SoundManager::MusicType::kMap);
        data::User::getInstance()->setFirstPlay(false);
    }
    else
    {
        loadingEnd();
    }
}

void Loading::storyOver()
{
    runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(Loading::loadingEnd, this)), NULL));
}

void Loading::loadingEnd()
{
    _delegate->gotoManor();
}

void Loading::setprogressAdd(int add)
{
    _progress += add;
    _bar->setPercent(_progress);
}

void Loading::runRest()
{
    if (_progress >= 90)
    {
        _progress = 90;
    }
    
    float resttime = float(100.0 - _progress)/100;
    
    _bar->runAction(Sequence::create(LoadingAction::create(LOADINGTIME * resttime, _progress, 95), CallFunc::create(CC_CALLBACK_0(Loading::guideCheck, this)), NULL));
}

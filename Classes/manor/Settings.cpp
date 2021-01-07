/***************************************************
    
 *	@desc	建造界面
 *	@date	2015-3-26
 *	@author	110101
     
 *	@file	manor/Settings.cpp
 *  @modify	null
 **********************************************/

#include "Settings.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "gateWay/Gateway.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;

bool Settings::init()
{
    if (!Layout::init())
    {
        return false;
    }
    addChild(BlackMask::create());
    Size winsize = Director::getInstance()->getWinSize();
    auto view = seekChildByNameWithRetType<ui::Layout*>(CSLoader::getInstance()->createNodeWithFlatBuffersFile("settings.csb"), "panel");
    view->retain();
    view->removeFromParent();
    setContentSize(winsize);
    addChild(view);
    view->release();
    ui::Helper::doLayout(this);
    
    auto panel = seekChildByNameWithRetType<ui::Layout*>(view, "panel");
    panel->setTouchEnabled(true);
    panel->addTouchEventListener(CC_CALLBACK_2(Settings::onTouch, this));
    
	//版本..
	panel->getChildByName<ui::Text*>("version")->setString("1.1.0");
	//按钮..
    seekChildByNameWithRetType<ui::Layout*>(view, "backBtnArea")->addTouchEventListener(CC_CALLBACK_2(Settings::backBtnCall, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "recoverBtnArea")->addTouchEventListener(CC_CALLBACK_2(Settings::recoverBtnCall,this));
    
    auto musicSlider = seekChildByNameWithRetType<ui::Slider*>(view, "musicSlider");
    /**
     *  这里添加两个函数，一个函数完成滑动中的音效改变
     *  另一个函数在玩家松开的时候保存数据
     */
    musicSlider->addEventListener(CC_CALLBACK_2(Settings::musicBtnCall, this));
    musicSlider->addTouchEventListener(CC_CALLBACK_2(Settings::musicTouchCall, this));
    
    int bgm = data::User::getInstance()->getBGM();
    musicSlider->setPercent(bgm);
    _musicTxt = seekChildByNameWithRetType<Node*>(musicSlider, "musicTxt");
    _musicTxt->setPositionX(float(bgm)/100 * musicSlider->getContentSize().width);

    std::stringstream str;
    str<<Value(bgm).asString()<<"%";
    seekChildByNameWithRetType<ui::Text*>(this, "musicPercent")->setString(str.str());
    
    if (bgm == 0)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "musicImage")->loadTexture("yinyueguanbi.png");
    }
    else
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "musicImage")->loadTexture("shenyinyou.png");
    }

    auto effectSlider = seekChildByNameWithRetType<ui::Slider*>(view, "effectSlider");
    
    effectSlider->addEventListener(CC_CALLBACK_2(Settings::soundBtnCall, this));
    effectSlider->addTouchEventListener(CC_CALLBACK_2(Settings::soundTouchCall, this));
    
    int se = data::User::getInstance()->getSE();
    effectSlider->setPercent(se);
    _effectTxt = seekChildByNameWithRetType<Node*>(effectSlider, "effectTxt");
    _effectTxt->setPositionX(float(se)/100 * effectSlider->getContentSize().width);
    
    std::stringstream str1;
    str1<<Value(se).asString()<<"%";
    seekChildByNameWithRetType<ui::Text*>(this, "effectpercent")->setString(str1.str());
    
    if (se == 0)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "effectimage")->loadTexture("yinyueguanbi.png");
    }
    else
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "effectimage")->loadTexture("shenyinyou.png");
    }
    //设置天空大小，天空海阔你与我~~~
    auto starSky = panel->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winsize.width/2,winsize.height*75/128));

    return true;
}

void Settings::onEnter()
{
    ui::Layout::onEnter();
    Manor::getInstance()->setTouchChangeEnable(false);
}

void Settings::onExit()
{
    Manor::getInstance()->setTouchChangeEnable(true);
    ui::Layout::onExit();
}

void Settings::onTouch(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        auto widget = dynamic_cast<ui::Widget*>(sender);
        auto movel = widget->getTouchEndPosition().x - widget->getTouchBeganPosition().x;
        if (movel > DeerUtils::deerResouseSize.width /3)
        {
            Manor::getInstance()->showLayer(Manor::kMainLayer);
        }
    }
}

void Settings::backBtnCall(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t)
{
    auto btn = dynamic_cast<Node*>(sender);
    if (t == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() - BACKBTN_ACT_DISTANCE);
    }
    else if (t == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
    }
    else if (t == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
//    BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Settings::backReal, this));
}

void Settings::backReal()
{
    Manor::getInstance()->showLayer(Manor::LayoutName::kMainLayer);
}

void Settings::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL));
}
void Settings::updateProperty()
{

}
void Settings::recoverBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        recoverReal();
    }
}

void Settings::recoverReal()
{
    addChild(RecoverAsk::create(), 20);
}

void Settings::musicTouchCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t)
{
    if (ui::Button::TouchEventType::ENDED == t)
    {
        SoundManager::getInstance()->saveVolume();
    }
}

void Settings::musicBtnCall(cocos2d::Ref *sender, cocos2d::ui::Slider::EventType t)
{
    auto slider = dynamic_cast<ui::Slider*>(sender);
    if (t == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        int percent = slider->getPercent();
        _musicTxt->setPositionX(
        float(percent)/100 * slider->getContentSize().width
        );
        SoundManager::getInstance()->setBackgroundMusicVolume(percent);
//        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volumFactorHelp(percent) * BACKGROUN_MUSIC_FACTOR);
        if (percent == 0)
        {
            seekChildByNameWithRetType<ui::ImageView*>(this, "musicImage")->loadTexture("yinyueguanbi.png");
        }
        else
        {
            seekChildByNameWithRetType<ui::ImageView*>(this, "musicImage")->loadTexture("shenyinyou.png");
        }
        std::stringstream str;
        str<<Value(percent).asString()<<"%";
        seekChildByNameWithRetType<ui::Text*>(this, "musicPercent")->setString(str.str());
    }
}

void Settings::soundTouchCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (ui::Button::TouchEventType::ENDED == t)
    {
        SoundManager::getInstance()->saveVolume();
    }
}

void Settings::soundBtnCall(cocos2d::Ref *sender, cocos2d::ui::Slider::EventType t)
{
    auto slider = dynamic_cast<ui::Slider*>(sender);
    if (t == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        int percent = slider->getPercent();
        _effectTxt->setPositionX(float(percent)/100 * slider->getContentSize().width);
        
        SoundManager::getInstance()->setEffectsVolume(percent);
        if (percent == 0)
        {
            seekChildByNameWithRetType<ui::ImageView*>(this, "effectimage")->loadTexture("yinyueguanbi.png");
        }
        else
        {
            seekChildByNameWithRetType<ui::ImageView*>(this, "effectimage")->loadTexture("shenyinyou.png");
        }
        std::stringstream str;
        str<<Value(percent).asString()<<"%";
        seekChildByNameWithRetType<ui::Text*>(this, "effectpercent")->setString(str.str());
    }
}

float Settings::volumFactorHelp(int percent)
{
    float vFactor = 0;
    if (percent < 0)
    {
        return vFactor;
    }
    else if (percent > 100)
    {
        vFactor = 100;
        return vFactor;
    }
    
    if (percent <= 50)
    {
        vFactor = 0.004 * percent;
    }
    else if (percent <= 70)
    {
        vFactor = 0.01 * percent - 0.3;
    }
    else if (percent <= 100)
    {
        vFactor = 0.02 * percent - 1.0;
    }
    
    return vFactor;
}

/*********************************************恢复数据界面**************************************/
bool Settings::RecoverAsk::init()
{
    if (!Layout::init())
    {
        return false;
    }
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("recoverAsk.csb");
    view->setContentSize(Director::getInstance()->getWinSize());
    addChild(view);
    ui::Helper::doLayout(this);
    
    seekChildByNameWithRetType<ui::Button*>(view, "sureBtn")->addTouchEventListener(CC_CALLBACK_2(RecoverAsk::sureBtnCall, this));
    seekChildByNameWithRetType<ui::Button*>(view, "quitBtn")->addTouchEventListener(CC_CALLBACK_2(RecoverAsk::quitBtnCall, this));
    
    return true;
}

void Settings::RecoverAsk::sureBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureReal();
    }
    
    //BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Settings::RecoverAsk::sureReal, this))
}

void Settings::RecoverAsk::sureReal()
{
    recoveryRun();
}

void Settings::RecoverAsk::quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitReal();
    }
    //BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Settings::RecoverAsk::quitReal, this))
}

void Settings::RecoverAsk::quitReal()
{
    removeFromParent();
}
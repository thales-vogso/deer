//
//  Register.cpp
//  deer
//
//  Created by xinhua on 15/6/10.
//
//

#include "RankRegister.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "popUp/BlackMask.h"
#include "GlobalMacros.h"
#include "core/web/Client.h"
#include "manor/Manor.h"
#include "core/custom/LibString.h"
#include "notice/Notice.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "Ios_GlobleMethod.h"
#endif

USING_NS_CC;

#define BOARD_RZ 50

bool RankRegister::init()
{
    if(!Layer::init())
    {
        return false;
    }
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("paihangbangtanchuang.csb");
    view->setContentSize(win);
    auto panel = view->getChildByName("Panel_1");
    panel->setPositionY(win.height/2);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    _inputText = view->getChildByName("Panel_1")->getChildByName<ui::TextField*>("TextField_1");
    _inputText->addEventListener(cocos2d::ui::TextField::ccTextFieldCallback(CC_CALLBACK_2(RankRegister::inputTextCallback,this)));
    
    auto btnSure = view->getChildByName("Panel_1")->getChildByName<ui::Button*>("Button_1");
    auto btnCancle = view->getChildByName("Panel_1")->getChildByName<ui::Button*>("Button_1_0");
    btnSure->addTouchEventListener(CC_CALLBACK_2(RankRegister::btnSureCallback, this));
    btnCancle->addTouchEventListener(CC_CALLBACK_2(RankRegister::btnCancleCallback, this));
    
    //TIPS
    this->addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER);
    
    return true;
}

void RankRegister::setRegisterCallback(const std::function<void (std::string)>& callback)
{
    _registerCallback = callback;
}
void RankRegister::inputTextCallback(cocos2d::Ref* ref, cocos2d::ui::TextField::EventType event)
{
    if(event == cocos2d::ui::TextField::EventType::INSERT_TEXT)
    {
        _inputText->setPlaceHolder("");
        const std::string str = _inputText->getString();
        _userName   =  const_cast<char*>(str.c_str());
    }
    else if(event == cocos2d::ui::TextField::EventType::DETACH_WITH_IME)
    {
        const std::string str = _inputText->getString();
        _userName   =  const_cast<char*>(str.c_str());
    }
}

void RankRegister::btnSureCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        const std::string str = _inputText->getString();
        
        //字段处理
        std::string str_1 = str;
        webdevlib::trim(str_1);
        _userName   =  const_cast<char*>(str_1.c_str());
        std::string  nullstr = "";
        
        CCLOG("name :%siii",_userName);
        if(str_1 != nullstr)
        {
            sendRegisterMessage();
            this->removeFromParent();
        }
        else
        {
            Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130301));
        }
    }
}
void RankRegister::btnCancleCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        Manor::getInstance()->showLayer(Manor::kVenture);
        this->removeFromParent();
    }
}

void RankRegister::sendRegisterMessage()
{
    if(_registerCallback)
    {
        cocos2d::ValueMap param;
        param["name"]   = _userName;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        param["device"] = getDeviceID_IOS();
#else
        param["device"] = "123456";
#endif
        
        web::Client::sendAndReceive("http://www.deer.vogso.com/user.php?ac=createUserRole", param, _registerCallback);
    }
}




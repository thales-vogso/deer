/**************************************************
        
 *	@desc	设置界面
 *	@date	2015-3-26
 *	@author	110101
     
 *	@file	manor/Settings.h
 *  @modify	null
 *************************************************/

#ifndef __DEER_SETTINGS__
#define __DEER_SETTINGS__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "popUp/BlackMask.h"
#include "data/DestroyData.h"
#include "manor/LayerBase.h"
#include "manor/LayerNoTouchTrans.h"

class Settings : public cocos2d::ui::Layout, public LayerBase
{
public:
    
    
    CREATE_FUNC(Settings);
    
    virtual void removeSelf();
    
    virtual void updateProperty();
    
protected:
    bool init();
    
    /**
     *  这个设置页面重写这两个函数的主要目的是控制manor的滑动控制器
     */
    void onEnter() override;
    void onExit() override;
    
    void backBtnCall(cocos2d::Ref*, cocos2d::ui::Button::TouchEventType t);
    void backReal();
    
    void musicBtnCall(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType t);
    void musicTouchCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    
    void recoverBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void recoverReal();
    
    void soundBtnCall(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType t);
    void soundTouchCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    
    /**
     *  这个函数的作用是支持滑动退出
     *
     *  @param sender 事件发送者
     *  @param t      触摸事件类型
     */
    void onTouch(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    
    float volumFactorHelp(int percent);
    
    cocos2d::Node* _musicTxt = nullptr;
    cocos2d::Node* _effectTxt = nullptr;
    
    class RecoverAsk : public cocos2d::ui::Layout
    {
    public:
        CREATE_FUNC(RecoverAsk);
    protected:
        
        bool init();
        
        void sureBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
        void sureReal();
        
        void quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
        void quitReal();
    };
};

#endif /* defined(__deer__Settings__) */

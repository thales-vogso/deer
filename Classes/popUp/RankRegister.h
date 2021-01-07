//
//  Register.h
//  deer
//  注册界面
//
//  Created by xinhua on 15/6/10.
//
//

#ifndef deer_Register_h
#define deer_Register_h

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"

class RankRegister : public cocos2d::Layer
{
public:
    CREATE_FUNC(RankRegister);
    
    /**
     *  设置注册回调事件
     *
     *  @param callback
     */
    void setRegisterCallback(const std::function<void (std::string)>& callback);
    
private:
    bool init();
    
    /**
     *  发送注册请求
     *
     */
    void   sendRegisterMessage();
    
    /**
     *  输入回调函数
     *
     */
    void   inputTextCallback(cocos2d::Ref* ref, cocos2d::ui::TextField::EventType event);
    
    /**
     *  注册请求的回掉函数
     *
     */
    std::function<void (std::string)> _registerCallback;
    
    void btnSureCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    void btnCancleCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    
    
    
    /**
     *  用户名
     */
    char*  _userName = NULL;
    
    
    cocos2d::ui::TextField* _inputText;
    
    
};
#endif

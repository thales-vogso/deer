//
//  ButtonAction.h
//  deer
//
//  Created by allyiaa on 15-4-22.
//
//

#ifndef __deer__ButtonAction__
#define __deer__ButtonAction__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#define __BTN__ACT__TAG__ 99
#define BUTTONACTION_TIME 0.3

/**
 *  按钮效果宏
 *
 *  @param _touch_type_  触摸类型
 *  @param _button_      按钮的透明度改变的部分
 *  @param _button_area_ 按钮的触摸层
 *  @param __func__      按钮在松开后0.3秒后的回调函数
 *
 *  @return null
 */
#define BTN_CALL_ACT(_touch_type_, _button_, _button_area_ ,__func__) \
if(_touch_type_ == cocos2d::ui::Widget::TouchEventType::BEGAN)\
{\
    stopActionByTag(__BTN__ACT__TAG__);\
    auto __button__ = dynamic_cast<cocos2d::Node*>(_button_);\
    __button__->setOpacity(255);\
}\
else if (_touch_type_ == cocos2d::ui::Widget::TouchEventType::MOVED)\
{\
    auto __button__area__ = dynamic_cast<cocos2d::ui::Widget*>(_button_area_);\
    auto __button__ = dynamic_cast<cocos2d::Node*>(_button_);\
    float de = __button__area__->getTouchBeganPosition().distance(__button__area__->getTouchMovePosition());\
    if (de > 20)\
    {\
        __button__->setOpacity(140);\
    }\
}\
else if (_touch_type_ == cocos2d::ui::Widget::TouchEventType::ENDED)\
{\
    auto __button__ = dynamic_cast<cocos2d::Node*>(_button_);\
    auto seq = Sequence::create(FadeTo::create(BUTTONACTION_TIME, 140), CallFunc::create(__func__), nullptr);\
    seq->setTag(__BTN__ACT__TAG__);\
    __button__->runAction(seq);\
}\
else if (_touch_type_ == cocos2d::ui::Widget::TouchEventType::CANCELED)\
{\
    auto __button__area__ = dynamic_cast<cocos2d::ui::Widget*>(_button_area_);\
    auto __button__ = dynamic_cast<cocos2d::Node*>(_button_);\
    float de = __button__area__->getTouchBeganPosition().distance(__button__area__->getTouchMovePosition());\
    if (de <= 20)\
    {\
        auto seq = Sequence::create(FadeTo::create(BUTTONACTION_TIME, 140), CallFunc::create(__func__), nullptr);\
        seq->setTag(__BTN__ACT__TAG__);\
        __button__->runAction(seq);\
    }\
}\


#endif /* defined(__deer__ButtonAction__) */

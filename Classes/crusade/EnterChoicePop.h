/****************************************************************************
 *	@desc   选择确认界面
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/EnterChoicePop.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_ENTERCHOICEPOP__
#define __DEER_ENTERCHOICEPOP__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "crusade/CrusadeMap.h"

class EnterChoicePop : public cocos2d::Layer
{
public:
    
    static EnterChoicePop *create(cocos2d::ValueMap data);
    
    inline void setDelegate(CrusadeMapDelegate *delegate)
    {
        _delegate = delegate;
    };
    
CC_CONSTRUCTOR_ACCESS:

    bool init(cocos2d::ValueMap data);
    
    std::function<void (cocos2d::ValueMap &)> _sureCall;
    
    std::function<void ()> _quitaddition;
    
private:
    /**
     *  确定按钮的回调
     *
     *  @param sender 按钮
     *  @param type   触摸类型
     */
    void sureCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    void surecall();
    
    /**
     *  确定按钮的回调
     *
     *  @param sender 按钮
     *  @param type   触摸类型
     */
    void quitCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    void quitcall();
    
    /**
     *  地图类的代理
     */
    CrusadeMapDelegate *_delegate;
    
    
    /**
     *  建筑数据
     */
    cocos2d::ValueMap _data;
};

class BattleChoice : public cocos2d::ui::Layout
{
public:
    
    static BattleChoice *create();
    
    bool init();
    
    void sureCallback(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t);
    void quitCallback(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t);

    inline void setQuitFunc(std::function<void()> func)
    {
        _quitCall = func;
    }
    
protected:
    cocos2d::ValueMap _fieldData;
    
    std::function<void()> _quitCall;
    
    /**
     *  代理
     */
    CrusadeMapDelegate * _delegate;
};


class BuildClearPop : public cocos2d::ui::Layout
{
public:
    
    static BuildClearPop *create();
    
    std::function<void ()> onSure;
protected:
    
    bool init();
    
    void quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t);
    
    void quit();
    
    cocos2d::ValueMap _data;
};

#endif /* defined(__deer__EnterChoicePop__) */

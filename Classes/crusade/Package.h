/****************************************************************************
 *	@desc   背包
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/Package.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_PACKAGE__
#define __DEER_PACKAGE__
#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Package : public cocos2d::Layer
{
public:
    CREATE_FUNC(Package);
CC_CONSTRUCTOR_ACCESS:
    bool init();
private:
    /**
     *  返回按钮的回调
     *
     *  @param sender 按钮
     *  @param type   触摸类型
     */
    void backBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
    
    
    /**
     *  显示信息
     *
     *  @param text 信息
     */
    void showInfo(std::string text, cocos2d::Node* node);
    
    /**
     *  item模板..
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    class ListItem : public cocos2d::ui::Layout
    {
    public:
        static ListItem* create(cocos2d::ValueMap data, Package *delegate);
    private:
        
        bool init(cocos2d::ValueMap data);
        
        /**
         *  信息按钮的回调
         *
         *  @param sender 按钮
         *  @param type   触摸类型
         */
        void infoBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
        
        /**
         *  数据
         */
        cocos2d::ValueMap _data;
        
        /**
         *  代理
         */
        Package* _delegate;
    };
};

#endif /* defined(__deer__Package__) */

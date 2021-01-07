/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-2-27
 *	@author	110101
     
 *	@file	crusade/WildTavern.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER__WILDTAVERN__
#define  __DEER__WILDTAVERN__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "manor/LayerBase.h"

class WildTavernDelegate
{
public:
    /**
     *  展示信息
     *
     *  @param text 信息内容
     */
    virtual void showInfo(std::string text,cocos2d::Node* node) = 0;
};

class WildTavern : public cocos2d::ui::Widget , public WildTavernDelegate,public LayerBase
{
public:
    static WildTavern *create(int tavernid = 0);
    
    /**
     *  析构
     */
    virtual ~WildTavern();
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
CC_CONSTRUCTOR_ACCESS:
    
    /**
     *  英雄酒馆初始化
     *
     *  @param tavernid 酒馆id
     *
     *  @return 初始化是否成功 true 成功， false 失败
     */
    bool init(int tavernid);
    
    void showPurchase(cocos2d::Value data);
    /**
     *  显示baord
     *
     */
    void showBoard();
    
private:
    
    class TavernListItem : public cocos2d::ui::Widget
    {
        /**
         *  代理
         */
        WildTavern * _delegate;
        
        /**
         *  角色数据
         */
        cocos2d::Value _rdata;
    public:
        /**
         *  实例创建
         *
         *  @return 实例
         */
        static TavernListItem * create(cocos2d::Value rdata, WildTavern * delegate);
        
    CC_CONSTRUCTOR_ACCESS:
        
        /**
         *  初始化函数
         *
         *  @param rdata 角色数据
         *
         *  @return 初始化是否成功
         */
        bool init(cocos2d::Value rdata);
        
    private:
        
        /**
         *  信息按钮的回调，玩家用来阅览信息
         *
         *  @param sender 按钮
         *  @param tevent 触摸类型
         */
        void infoBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType tevent);
        
        /**
         *  购买按钮的回调，玩家的购买
         *
         *  @param sender 按钮
         *  @param tevent 触摸类型
         */
        void purchaseBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType tevent);
        
    };
    
    class PurchasePop : public cocos2d::ui::Layout
    {
    public:
        static PurchasePop* create(cocos2d::Value data);
        
        bool init(cocos2d::Value data);
        
        void sureBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType tevent);
        void sureReal();
        
        void quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType tevent);
        void quitReal();
    private:
        cocos2d::Value _data;
    };
    
    /**
     *  展示信息
     *
     *  @param text 信息
     */
    virtual void showInfo(std::string text,cocos2d::Node* node) override;
    
    /**
     *  返回按钮的回调
     *
     *  @param sender 按钮
     *  @param touch  触摸类型
     */
    void backBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touch);
    void backReal();
    
private:
    /**
     *  
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    /**
     *	列表改变尺寸
     */
    void listResize();
    /**
     *  board尺寸改变
     *  @param e
     */
    void onBoardResize(cocos2d::EventCustom* e);
};

#endif /* defined(__deer__WildTavern__) */

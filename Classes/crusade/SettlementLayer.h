/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	crusade/BattleLayer.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_SETTLEMENTLAYER__
#define __DEER_SETTLEMENTLAYER__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"

class BattleLayer;

class SettlementLayer : public cocos2d::ui::Layout, public custom::Event
{
public:
    
    /**
     *  create函数（需要数据）
     *
     *  @return 战利品界面
     */
    static SettlementLayer *create(int mid);
    
    static SettlementLayer* create(cocos2d::ValueVector drops);
    
    SettlementLayer();
    virtual ~SettlementLayer();
    
    /**
     *  刷新item；
     */
    void upDateItems();
    
    inline void setDelegate(BattleLayer *delegate)
    {
        _delegate = delegate;
    }
    
    std::function<void ()> onShut;
    
public:
    static const std::string EXIT_FROM_SETTLEMENT;
    
protected:
    
    bool init(int mid);
    
    bool init(cocos2d::ValueVector drops);
    
    /**
     *  关闭回调
     *
     *  @param sender        按钮
     *  @param eventType     事件
     */
    void shutDownCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType eventType);
    void shutDown();
    
    /**
     *  获得全部回调
     *
     *  @param sender 按钮
     *  @param type   事件
     */
    void getAllCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type);
    void getAll();
    
    /**
     *  刷新背包容量
     */
    void updatePackCapacity();
    
    
private:
    
    /**
     *  一个内部类，来实现列表项
     */
    class ListItem : public cocos2d::ui::Layout
    {
    public:
        enum ItemPlace
        {
            kPackage = 1,
            kDropheap = 2
        } _place;
        
        /**
         *  获取iid
         *
         *  @return iid
         */
        int getiid();
        
        inline cocos2d::ValueMap& getData()
        {
            return _itemData;
        };
        
        /**
         *  create函数
         *
         *  @param itemData 数据
         *
         *  @return 列表项
         */
        static ListItem* create(cocos2d::ValueMap itemData, SettlementLayer* delegate);
        
        /**
         *  选择回调
         *
         *  @param sender 按钮
         *  @param type   触摸类型
         */
        void choiceButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
        void choiceIt();
        
        /**
         *  设置数量
         *
         *  @param amount 数量
         */
        void setAmount(int amount);
        
        int getAmount();
        
        void decrease(int amount = 0);
        
        void increase(int amount = 0);
        
    CC_CONSTRUCTOR_ACCESS:
        /**
         *  init函数
         *
         *  @param itemData item的数据
         *
         *  @return 初始化是否成功
         */
        bool init(cocos2d::ValueMap itemData);
        
    private:
        
        /**
         *  道具数据
         */
        cocos2d::ValueMap _itemData;
        
        /**
         *  控制代理
         */
        SettlementLayer * _delegate;
    };
    
    /**
     *  转移一个
     */
    void transfer(SettlementLayer::ListItem * item, int amount = 1);
    
    /**
     *  模板字典..
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    /**
     *  怪物id
     */
    int _mid;
    
    /**
     *  这个字典时个临时数据字典，目的是记录玩家的操作..
     */
    cocos2d::ValueMap _spiolsData;
    
    /**
     *  battleLayer
     */
    BattleLayer *_delegate;
    
    };


#endif /* defined(__deer__SettlementLayer__) */

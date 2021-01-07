/*****************************************************
 *	@desc   探险界面
 *	@date	2015-1-14
 *	@author	110101
     
 *	@file	manor/VentureLayer.cpp
 *   @modify	120101 2015-2-11
 ***************************************************/

#ifndef __DEER_VENTURELAYER__
#define __DEER_VENTURELAYER__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "manor/LayerBase.h"
#include "guide/Guide.h"

class VentureLayer : public cocos2d::ui::Layout, public custom::Event, public LayerBase
{
public:
    
    enum BtnsID
    {
        kCrusadeBtn = 130100,
        kRankBtn = 130300,
        kAchievemtnBtn = 130400,
        kTalentBtn = 130500
    };
    
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
    CREATE_FUNC(VentureLayer);
    /**
     *  析构
     */
    virtual ~VentureLayer();
    
    /**
     *  更新ListItem
     */
    void upDateItems();
    
    /**
     *  显示board和tips
     */
    void showBoard();
    
    /**
     *  这个函数有monar控制器在运行这个界面的时候调用，将在0.2秒以后调用runButtonShowAct()
     */
    void runButtonShowCheck();
    
    /**
     *  播放按钮出现动画
     */
    void runButtonShowAct();
    
    /**
     * 新手引导手指
     */
    Guide* _guide = NULL;
    
    static const std::string VENTURE;
    static const std::string BACK;
    static const std::string RANK;
    static const std::string ACHIEVEMENT;
    static const std::string TALENT;
    static const std::string TAIKONGFOOD;
    static const std::string SOLGIER;
    
    /**
     *  新手引导结果
     */
    cocos2d::ValueMap  result = cocos2d::ValueMapNull;
    
    
public:
    /**
     *  切换到远征酒馆页面
     */
//    static const std::string TURN_TO_MANORMAIN;
    
protected:
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init();
    
    void onEnter() override;
    
   
private:
    
    /**
     *  返回按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 事件类型
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void backReal();
    
    /**
     *  排行榜
     *
     *  @param sender
     *  @param touchType 
     */
    void rankingBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void rankingBtnCall();
    
    /**
     *  成就按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 事件类型
     */
    void achievementButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void achievementBtnCall();
    
    /**
     *  天赋按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 事件类型
     */
    void tallentButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void tallentBtnCall();
    
    /**
     *  探险按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 时间类型
     */
    void goVentureButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void goVenture();
    
    /**
     *  当角色变化时触发的事件
     *
     *  @param event 事件
     */
    void onRoleChange(cocos2d::EventCustom *event);
    
    /**
     *  当背包中的道具变化
     *
     *  @param event 事件
     */
    void onPackageChange(cocos2d::EventCustom *event);
    
    /**
     *  训练回调
     *
     *  @param event 事件
     */
    void onRoleTrain(cocos2d::EventCustom *event);
    
    void onTaskCompleted(cocos2d::EventCustom *e);
    
    /**
     *  刷新按钮
     */
    void updateBtns();
    
    /**
     *  item模板
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    
    /**
     *  侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
    
    /**
     *  解锁
     */
    cocos2d::Vector<cocos2d::Node*> _btnToRunShowAct;
    
    bool _isUpdateList = false;
    
    
    class CrusadeListItem : public cocos2d::ui::Layout, public custom::Event, public LayerBase
    {
    public:
        /**
         *  创建函数
         *
         *  @param itemData 数据
         *
         *  @return 一项
         */
        static CrusadeListItem *create(cocos2d::ValueMap itemData, VentureLayer* delegate);
        
        /**
         *
         */
        virtual ~CrusadeListItem();
        
        void removeSelf() override;
        
        void updateProperty() override;
        
        inline int getItemType()
        {
            return _itemType;
        }
        
        inline int getItemId()
        {
            if (_itemType == ItemType::kRole)
            {
                return  _itemData["rid"].asInt();
            }
            else
            {
                return _itemData["iid"].asInt();
            }
        }
        int  idAdd;
        int  idDecrease;
        int  _fastOpreateIndex = 0;
        
    protected:
        /**
         *  初始化函数
         *
         *  @param itemData 数据
         *
         *  @return 是否初始化成功
         */
        bool init(cocos2d::ValueMap itemData);
        
    private:
        
        /**
         *  数据
         */
        cocos2d::ValueMap _itemData;
        
        /**
         *  增加按钮的回调
         *
         *  @param sender    按钮
         *  @param eventType 触摸类型
         */
        void increaseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType eventType);
        void increase();
        void increaseFast();
        bool _longPressedIncrease;
        
        /**
         *  减少按钮的回调
         *
         *  @param sender    按钮
         *  @param envetType 触摸类型
         */
        void decreaseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType eventType);
        void decrease();
        void decreaseFast();
        bool _longPressedDecrease;
        
        void playFastAddMusic();
        /**
         *  数字改变
         *
         *  @param event 事件
         */
        void onNumberChange(cocos2d::EventCustom * event);
    private:
        enum ItemType
        {
            kRole = 1,
            kItem = 2
        } _itemType;
        
        /**
         *  代理
         */
        VentureLayer* _delegate;
        
        cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _lisMap;
    };
};

#endif /* defined(__deer__VentureLayer__) */

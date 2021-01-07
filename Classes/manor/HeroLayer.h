/****************************************************************************
     
 *	@desc	英雄界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/HeroLayer.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_HEROLAYER__
#define __DEER_HEROLAYER__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "LayerBase.h"
#include "guide/Guide.h"

class HeroLayer : public cocos2d::Layer , public LayerBase
{
public:
    CREATE_FUNC(HeroLayer);
    
    /**
     *  析构
     */
    virtual ~HeroLayer();
    
    void removeSelf() override;
    
    void updateProperty() override;
    
    void updateList();
    
private:
    bool init();
    
    void onEnter() override;
    
    void onExit() override;
    
    /**
     *  添加tips和board
     */
    void showBoard();
    
    /**
     *  listResize
     */
    void listResize();
    
    /**
     *  unFold
     */
    void unFold(cocos2d::EventCustom *e);
    /**
     *  返回按钮的回调
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void backReal();
    
    /**
     *  添加弹窗
     *
     *  @param event 事件事件数据
     */
    void addTrainHeroPopup(int rid);
    
    /**
     *  训练添加新产生的角色
     *
     *  @param event 事件数据
     */
    void onTrainNewRole(cocos2d::EventCustom *event);
    
    /**
     *  招募按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void recruitBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void recruit();
    
    void showInfo(std::string &info, cocos2d::Node* node);
    
    
    /**
     * 新手引导手指
     */
    Guide* _guide = NULL;
    
    bool _isUpdateList = false;
    
    /**
     *  按钮
     */
    cocos2d::ui::Layout* _recruitBtnArea = nullptr;
    cocos2d::ui::Button* _recruitBtn = nullptr;
    
    cocos2d::Value preRoleListData = cocos2d::Value();
    
private:
    class HeroListItem : public cocos2d::ui::Layout, public custom::Event
    {
    public:
        /**
         *  创建对象
         *
         *  @return 对象指针
         */
        static HeroListItem* create(cocos2d::ValueMap itemData, HeroLayer* delegate);
        
        /**
         *  析构
         */
        virtual ~HeroListItem();
        
        /**
         *  刷新显示
         */
        void updateView(cocos2d::ValueMap data);
        
        /**
         *  角色id
         */
        inline int getRid();
        
        inline bool isUpdated()
        {
            return _updated;
        };
        
        inline void setUpdated(bool upd)
        {
            _updated = upd;
        };
        
        //按钮..
        cocos2d::ui::Layout* _nameBtn;
        
    public://共有变量..
        
        /**
         *  点击训练英雄弹出选择弹窗的事件
         */
        static std::string const TRAIN_HERO_CHOICE;
        
        /**
         *  英雄的数据
         */
        cocos2d::ValueMap _itemData;
        
    protected:
        /**
         *  初始化函数
         *
         *  @return 初始化是否成功
         */
        bool init(cocos2d::ValueMap itemData);
    private:
        
        /**
         *  训练回调
         *
         *  @param sender    触发按钮
         *  @param touchType 事件类型
         */
        void trainHeroCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
        void trainHero();
        
        /**
         *  summary按钮的回调
         *
         *  @param sender
         *  @param touchType 
         */
        void infoBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
        
        /**
         *
         */
        void showInfo();
        
    private:
        
        /**
         *  角色id 
         */
        int _rid;
        
        
        /**
         *  训练的侦听
         */
        cocos2d::EventListenerCustom *_trainListener;
        
        /**
         *  代理
         */
        HeroLayer* _delegate;
        
        /**
         *  更新标识位
         */
        bool _updated;
    };
    
    bool _showRecuitNew;
    
    /**
     *  item的模板..
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    /**
     *  事件侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
    
};
#endif /* defined(__deer__HeroLayer__) */

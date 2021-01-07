/****************************************************************************
 *	@desc   天赋界面
 *	@date	2015-2-6
 *	@author	110101
     
 *	@file	manor/TalentLayer.cpp
 *  @modify	null
 ******************************************************************************/

#ifndef __DEEER_TALENTLAYER_
#define __DEEER_TALENTLAYER_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "manor/LayerBase.h"

class TalentLayer : public cocos2d::Layer , public LayerBase
{
public:
    CREATE_FUNC(TalentLayer);
    virtual ~TalentLayer();
    
    /**
     *  延时移除自身
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
CC_CONSTRUCTOR_ACCESS:
    
    /**
     *  初始化
     *
     *  @return 初始化成功返回true 初始化失败返回false
     */
    bool init();
    
    void onEnter();
    
    void showBoard();
    
    /**
     *  刷新item
     */
    void updateItems();
    
private:
    
    class TalentListItem : public cocos2d::ui::Layout
    {
    public:
        /**
         *  创建一个列表项
         *
         *  @param  tid  天赋ID
         *
         *  @return TalentListItem的实例
         */
        static TalentListItem *create(cocos2d::ValueMap itemData, TalentLayer *delegate);
        
    CC_CONSTRUCTOR_ACCESS:
        
        /**
         *  构造
         */
        TalentListItem();
        
        /**
         *  初始化
         *
         *  @param tid 天赋id
         *
         *  @return 初始化成功返回true 初始化失败返回false
         */
        bool init(cocos2d::ValueMap itemData);
        
    protected:
        
        /**
         *  惊叹号的按钮的回调
         *
         *  @param sender 按钮
         *  @param type   触摸类型
         */
        void tipsCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
        
        /**
         *  父级可控制器
         */
        TalentLayer * _delegate;
        
        /**
         *  item 数据
         */
        cocos2d::ValueMap _itemData;
    };
    
    
    /**
     *  滑动框变长
     *
     *  @param e
     */
    void setListLong(cocos2d::EventCustom* e);
    
    /**
     *  滑动框变短
     *
     *  @param e
     */
    void setListShort(cocos2d::EventCustom* e);
    
    /**
     *   展示信息
     *
     *  @param info
     */
    void showInfo(std::string& info , cocos2d::Node* node);
    
    /**
     *  返回按钮的回调
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
    void backReal();
    
private:
    
    /**
     *  item的模板..
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;

    /**
     *   侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _liMap;

};

#endif /* defined(__deer__TalentLayer__) */

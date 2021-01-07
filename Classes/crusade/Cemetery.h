/****************************************************************************
 *	@desc   墓园
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/Cemetery.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_CEMETERY__
#define __DEER_CEMETERY__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class Cemetery : public cocos2d::Layer
{
public:
    enum BUYHERO_ID
    {
        BUYHERO_ID1 = 110802,
        BUYHERO_ID2 = 110803,
        BUYHERO_ID3 = 110804,
        BUYHERO_ID4 = 110805,
        BUYHERO_ID5 = 110806
    };
    
    CREATE_FUNC(Cemetery);
    
    void updateItems();
private:
    
    bool init();
    
    void onEnter() override;
    
    void onExit() override;
    
    void showInfo(std::string &info,cocos2d::Node* node);
    
    /**
     *  返回回调
     *
     *  @param sender
     *  @param type   
     */
    void backCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
    void backReal();
    
    /**
     *  复活
     *
     *  @param data 英雄数据
     */
    void revival(cocos2d::ValueMap data);
    
    class ListItem : public cocos2d::ui::Layout
    {
    public:
        static ListItem* create(cocos2d::ValueMap data, Cemetery* delegate);
        
    private:
        bool init(cocos2d::ValueMap data);
        
        void infoBtnCall(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType touchType);
        
        void nameBtnCall(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType touchType);
        
        void showInfo();
        
        /**
         *  数据
         */
        cocos2d::ValueMap _data;
        
        /**
         *  代理
         */
        Cemetery* _delegate;
        
    };
    
    /**
     *  item 模板
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    
};

/**************************************复活选择界面****************************************/
class RevivalChoice : public cocos2d::ui::Layout
{
public:
    static RevivalChoice *create(cocos2d::ValueMap data);
    bool init(cocos2d::ValueMap data);
    
    inline void setDelegate(Cemetery *delegate)
    {
        _delegate = delegate;
    };
protected:
    
    cocos2d::ValueMap _data;
    
    void revivalBtnCall(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t);
    void revivalReal();
    
    void buryBtnCall(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t);
    void buryReal();
    
    void quitBtnCall(cocos2d::Ref * sender, cocos2d::ui::Button::TouchEventType t);
    void quitReal();
    
    Cemetery *_delegate;
};

#endif /* defined(__deer__Cemetery__) */

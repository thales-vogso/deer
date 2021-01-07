/********************************************
 *	@desc   战斗界面
 *	@date	2015-5-18
 *	@author	110101
     
 *	@file	crusade/TransGate.h
 *  @modify	null
 ********************************************/

#ifndef __DEER_TRANSGATE__
#define __DEER_TRANSGATE__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "crusade/CrusadeMap.h"

class TransGate : public cocos2d::ui::Layout
{
    class ListItem;
    class QuitListItem;
public:
    CREATE_FUNC(TransGate);
    bool init();
    
    int _tempsid;
    
protected:
    
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
};

class TransGate::ListItem : public cocos2d::ui::Layout
{
public:
    /**
     *  create 函数
     *  @param 显示的模版
     */
    static ListItem* createWithModel(cocos2d::ui::Layout* model);
CC_CONSTRUCTOR_ACCESS:
    /**
     *  init函数
     *
     *  @param itemData item的数据
     *
     *  @return 初始化是否成功
     */
//    bool init(cocos2d::ValueMap itemData);
    bool init(cocos2d::ui::Layout *model);
    
    /**
     *  更新显示部分
     */
    virtual void upView(cocos2d::ValueMap data);
    
protected:
    /**
     *  选在对应的关卡的回调
     *
     *  @param sender 按钮
     *  @param type   触摸类型
     */
    void choiceButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
    virtual void choiceReal();
protected:
    
    /**
     *  地图名称
     */
    cocos2d::ui::Text *_name;
    /**
     *  地图的sid
     */
    int _sid;
};

class TransGate::QuitListItem : public TransGate::ListItem
{
public:
    static QuitListItem* createWithModel(cocos2d::ui::Layout * model);

    inline void setController(TransGate * con)
    {
        _con = con;
    };
protected:
    bool init(cocos2d::ui::Layout * model);
    
    virtual void choiceReal();
    
    TransGate *_con;
};

#endif /* defined(__deer__TransGate__) */

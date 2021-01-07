/****************************************************************************
    
 *	@desc	制造界面
 *	@date	2015-06-30
 *	@author	120103
     
 *	@file	manor/Produce.h
 *  @modify	minamoto
 ******************************************************************************/

#ifndef __deer__Produce__
#define __deer__Produce__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Caches.h"
#include "manor/LayerBase.h"

#define PURCHASE_BATCH_Z 55
class Produce : public cocos2d::Layer, public custom::Caches,  public LayerBase{
    class Item;
public:
    CREATE_FUNC(Produce);
    /**
     *  刷新列表
     */
    void updateList();
    
    void updateProperty() override;
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void openProduce();
    
    cocos2d::Node* getPattern(bool huge = false);
protected:
    
    virtual ~Produce();
    
    bool init();
    /**
     *  返回按钮的回调
     */
    void backBtnClick(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void backExe();
    /**
     *  展示建筑的信息
     */
    void showInfo(std::string text, cocos2d::Node* node);
    /**
     *	重构
     */
    void onEnter() override;
    void onExit() override;
    /**
     *	显示board栏
     */
    void showBoard();
    /**
     *	运动间隔
     */
    static const float ACT_DURATION;
    
    /**
     *  添加批量处理界面
     *
     *  @param data 数据
     */
    void addBatchPop(cocos2d::ValueMap data);
private:
    /**
     *	容器
     */
    cocos2d::ui::Layout* _panel;
    /**
     *	列表容器
     */
    cocos2d::ui::ListView* _list;
    /**
     *	列表组合
     */
    cocos2d::ui::Layout* _listItem;
    /**
     *	底部遮罩
     */
    cocos2d::ui::Layout* _bottomMask;
    cocos2d::ui::Layout* _topmask;
    /**
     *	旧坐标
     */
    cocos2d::ValueMapIntKey _oldCoord;
    /**
     *	间隔
     */
    const float ITEM_SPACE = 50.0f;
    
    /**
     *	模块模板(小)
     */
    cocos2d::ui::Layout* _itemPattern;
    /**
     *	模块模板(大)
     */
    cocos2d::ui::Layout* _itemHugePattern;
    /**
     *  建筑界面的侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
    /**
     *	列表顶部间距
     */
    const float LIST_MARGIN = 188.0f;
    
    /**
     *  列表最大高度
     */
    const float _listMaxHeight = 1095;
    const float _topMaskHeight = 1072;
    /**
     *  滑动框变长
     *  @param e
     */
    void setListLong(cocos2d::EventCustom* e);
    /**
     *  滑动框变短
     *  @param e
     */
    void setListShort(cocos2d::EventCustom* e);
    /**
     *  board尺寸改变
     *  @param e
     */
    void onBoardResize(cocos2d::EventCustom* e);
    /**
     *	列表改变尺寸
     */
    void listResize();
    /**
     *  任务完成侦听
     *  @param e
     */
    void onTaskCompleted(cocos2d::EventCustom* e);
    /**
     *	根据id获取元素
     */
    Produce::Item* getItemByID(int id);
    
    cocos2d::Value _preUserProduct = cocos2d::Value();
    
    bool _isOpened = false;
    
};
class Produce::Item : public cocos2d::ui::Layout,public LayerBase{
public:
    /**
     *	构造
     */
    Item();
    /**
     *	析构
     */
    virtual ~Item();
    
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
    /**
     *	初始化
     */
    bool init();
    /**
     *	设置代理
     */
    static void setDelegate(Produce* param){
        _delegate = param;
    }
    /**
     *	设置数据
     */
    void setData(cocos2d::Value data);
    /**
     *	获取数据
     */
    cocos2d::Value getData(){
        return _data;
    }
    /**
     *	获取id
     */
    int getID(){
        return _data.asValueMap()["iid"].asInt();
    }
    
    /**
     *  刷新材料的状态
     *
     *  @param e
     */
    void updateMaterial(cocos2d::EventCustom* e);
private:
    /**
     *	代理父级
     */
    static Produce* _delegate;
    /**
     *	数据
     */
    cocos2d::Value _data;
    /**
     *  克隆的显示层
     */
    cocos2d::Node* _view;
    /**
     *	信息标签
     */
    const int INFO_BTN_ACT = 12;
    const int BTNACT_TAG = 11;
    const float BTN_FADETIME = 0.3;
    /**
     *	材料文字间隔
     */
    const float MATERIAL_SPACE = 20.0f;
    const float MATERIAL_SPACEH = 30.0F;
    
    /**
     *  长按标志位
     */
    bool _longTouching;
    const int LongPressActionTag;
    
    bool _updateList = false;
    
    /**
     *  消耗
     */
    cocos2d::Vector<cocos2d::ui::Text*> _consumes;
    /**
     *  触及回调
     *  @param s	名字按钮
     *  @param e	触摸类型
     */
    void nameClick(cocos2d::Ref * s, cocos2d::ui::Button::TouchEventType e);
    /**
     *  建造
     */
    void build();
    /**
     *  信息按钮的回调
     *  @param s	按钮
     *  @param e	触摸类型
     */
    void infoClick(cocos2d::Ref * s, cocos2d::ui::Button::TouchEventType e);
    /**
     *   展示信息
     */
    void showInfo();
};

#endif /* defined(__deer__Produce__) */

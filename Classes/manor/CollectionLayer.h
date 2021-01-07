/****************************************************************************
     
 *	@desc	主界面
 *	@date	2015-1-12
 *	@author	110101
     
 *	@file	manor/Collection.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_COLLECTIONLayer__
#define __DEER_COLLECTIONLayer__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "popUp/Finger.h"
#include "manor/LayerBase.h"
#include "guide/Guide.h"

class CollectionLayer : public cocos2d::ui::Layout, public custom::Event, public LayerBase
{
public:
    /**
     *  create函数
     *
     *  @return 一个Collection的显示层
     */
    static CollectionLayer* create();
    
    /**
     *  给这个界面添加tips和board
     */
    void showBoard();
    
    void runButtonShowCheck();
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
    /**
     *  进入该界面时调用
     */
    void show();
    
protected:
    
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init();
    
    /**
     *  构造函数
     */
    CollectionLayer();
    
    /**
     *  析构
     */
    virtual ~CollectionLayer();
    
    /**
     *  进入stage
     */
    void onEnter() override;
    
    void update(float delta) override;
    
    /**
     *  刷新显示项目
     */
    void updateItems(cocos2d::EventCustom * event);
    
    /**
     *  返回按钮的回调
     *
     *  @param sender    按钮
     *  @param eventType 事件类型
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType eventType);
    void backReal();
    
    /**
     *  工人数量来改变
     *
     *  @param event 事件数据
     */
    void onWorkerChange(cocos2d::EventCustom *event);
    
    /**
     *  收获按钮的回调
     *
     *  @param sender 按钮
     *  @param type   触摸类型
     */
    void gatherButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
    
    /**
     *  检查计时器是否可用
     *
     *  @param event 事件
     */
    void checkTimerAble(cocos2d::EventCustom *event);
    
    /**
     *  开始计时
     */
    void runTimeAction();
    
    /**
     *  采集回调
     */
    void gainCall();
    
    /**
     *  展示信息
     *
     *  @param txt 搜需要展示的信息
     */
    void showInfo(std::string txt, cocos2d::Node* node);
    
    /**对于一个按钮我们会分为四种方式处理
       1、按钮尚未解锁，对于未解锁的按钮，我们将其隐藏；
       2、按钮刚刚解锁，这样的按钮需要播放刚解锁的动画（在移动到这个界面的时候，我们这个按钮fadeout出来，并添加粒子效果和新标记）
       3、这个按钮玩家解锁但是没有点击，此时不需要播放动画，只需要将按钮置于“新状态”
       4、按钮的普通状态
     **/
    
    /**
     *  等待播放动画的按钮
     */
    cocos2d::Vector<cocos2d::Node*> _btnToRunShowAct;
    
    /**
     *  在完成task时执行
     *
     *  @param e 事件参数
     */
    void onTaskCompleted(cocos2d::EventCustom* e);
    
    /**
     *  运行按钮显示动画
     */
    void runButtonShowAct();
    
    /**
     *  显示全部内容
     */
    void showAll();
    
    /**
     * 新手引导手指
     */
    Guide* _guide;
    
    bool _isUpdateList = false;
    
    float _fontWidth = 21; //采集工人数量字体占像素宽度..
    
    int   _tipsFreeWorkerNum = 5;
    
    /**
     *  按钮
     */
    cocos2d::ui::ImageView* _backBtn = nullptr;
    cocos2d::ui::Button* _gatherBtn = nullptr;
    cocos2d::ui::Layout* _gatherBtnArea = nullptr;
    
    cocos2d::ui::LoadingBar* _gatherLoadBar = nullptr;
    
    cocos2d::Value _preGainListData = cocos2d::Value();
    
private:
    
    class CollectionItem : public cocos2d::ui::Widget
    {
    public:
        //这个函数需要数据--
        static CollectionItem* create(cocos2d::ValueMap itemData, CollectionLayer* delegate);
        
        /**
         *  析构
         */
        virtual ~CollectionItem();
        
        inline int getWid()
        {
            return _itemData["wid"].asInt();
        };
        
        void updateView(cocos2d::ValueMap data);
        
        int robotId;
        int robotDecreseId;
        int robotNameId;
        
        /**
         *  是否可以重复点击
         */
        bool _canDoubleClick = true;
        
        int  _fastOpreateIndex = 0;
        
    private:
        
        /**
         *  初始化函数
         *
         *  @return
         */
        bool init(cocos2d::ValueMap itemData);
        
        /**
         *  展示信息
         *
         *  @param sender
         *  @param t      
         */
        void showWorkerInfo(cocos2d::Ref * sender, cocos2d::ui::Text::TouchEventType t);
        
        /**
         *  增加回调
         *
         *  @param sender    按钮
         *  @param touchType 触摸类型
         */
        void increaseCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
        void increase();
        void increaseFast();
        bool _longPressedIncrease;
        
        /**
         *  减少回调
         *
         *  @param sender    按钮
         *  @param touchType 触摸类型
         */
        void reduceCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
        void reduce();
        void reduceFast();
        bool _longPressedReduce;
        
        void playFastAddEffect();
        
    private://私有变量--
        
        /**
         *  数据
         */
        cocos2d::ValueMap _itemData;
        
        /**
         *  事件侦听
         */
        cocos2d::EventListenerCustom * _buildWorkerChange;
        
        /**
         *  代理;
         */
        CollectionLayer* _delegate;
    };
    
    //数字动画..
    class TimerAction : public cocos2d::ActionInterval
    {
    public:
        static TimerAction * create(float time);
        bool init(float time);
    private:
        virtual void update(float t) override;
        //计时事件..
        float _time;
    };
    
    /**
     *  item的模板
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    //计时状态..
    bool _inTimer;
    
    //事件侦听器字典..
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*>  _listenerMap;
    
    /**
     *  返回按钮的启示X坐标
     */
    float _backBtnX;
    
    /**
     *  下一帧运行计时器
     */
    bool _runTimeActNextFram;
};

#endif /* defined(__deer_CollectionLayer__) */

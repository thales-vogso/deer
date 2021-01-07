/****************************************************************************
     
 *	@desc   进入游戏的界面
 *	@date	2014-12-12
 *	@author	110101
     
 *	@file	manor/ManorMainLayer.h
 *  @modify	NULL
 ******************************************************************************/

#ifndef __DEER_MANORMAINLAYER__
#define __DEER_MANORMAINLAYER__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "manor/Manor.h"
#include "manor/LayerBase.h"
#include "guide/Guide.h"

class ManorMainLayer : public cocos2d::ui::Layout , public custom::Event, public LayerBase
{
public:
    
    static ManorMainLayer * create();
    
    /**
     *  播放开始动画
     */
    void playStartAnimation();
    
    /**
     *  新出现的按钮效果,当进入这个页面后或者第一次启动游戏动画播放完毕时调用
     */
    void runButtonShowAct();
    
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    
    void updateProperty() override;
    
    /**
     *  获取仓库按钮的位置
     *
     *  @return －1左边， 0 中间， 1 右边
     */
    int getWareHouseBtnPosType();
    
    /**
     *  为该页面添加board板
     */
    void showBoard();
public://共有成员变量..
    /**
     *  来自cocostudio的主界面名字
     */
    static const std::string MAIN_VIEW_NAME_CS;
    
    /**
     *  去收集界面的事件名
     */
    static const std::string TURN_COLLECTION;
    
    /**
     *  去探险界面的事件名称
     */
    static const std::string TURN_VENTURE;
    
protected:
    
    bool init();
    ManorMainLayer();
    virtual ~ManorMainLayer();
    
    /**
     *  为按钮添加回调
     */
    void addCallbackForButton();
    
    /**
     *  设置全部按钮的可用状态
     */
    void setButtonEnable(bool bRet);
    
    /**
     *  设置只有某个按钮可用
     */
    void setButtonEnableByBtnId(int BtnId);
    
    /**
     *  重写刷新函数
     *
     *  @param delta 时间间隔
     */
    void update(float delta) override;
    
    /**
     *  道具刷新
     */
    void itemChange(cocos2d::EventCustom* e);
    
private:
    
    /**
     *  商店按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void storeButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void storeBtnCall();
    
    cocos2d::ui::Layout* _storeBtnArea = nullptr;
    cocos2d::Node*       _storeBtn = nullptr;
    /**
     *  能量按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void alchemyButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void alchemyBtnCall();
    
    cocos2d::ui::Layout* _alchemyBtnArea = nullptr;
    cocos2d::Node*       _alchemyBtn = nullptr;
    /**
     *  建造按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void buildButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void buildBtnCall();
    
    cocos2d::ui::Layout* _buildBtnArea = nullptr;
    cocos2d::Node*       _buildBtn = nullptr;
    
    /**
     *  制作按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void practiceButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void practiceBtnCall();
    
    cocos2d::ui::Layout* _practiceBtnArea = nullptr;
    cocos2d::Node*       _practiceBtn     = nullptr;
    /**
     *  英雄按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void heroButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void heroBtnCall();
    
    cocos2d::ui::Layout* _heroBtnArea = nullptr;
    cocos2d::Node*       _heroBtn     = nullptr;
    /**
     *  钻石按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void warehouseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    void warehouseBtnCall();
    
    cocos2d::ui::Layout* _warehouseBtnArea = nullptr;
    cocos2d::Node*       _warehouseBtn     = nullptr;
    /**
     *  探险按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void ventureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    
    cocos2d::ui::Layout*   _ventureBtnArea = nullptr;
    
    /**
     *  收集按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void collectionButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    
    cocos2d::ui::Layout*    _collectionBtnArea = nullptr;
    /* 设置按钮  */
    void settingsButtonCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void settingsBtnCall();
    
    cocos2d::ui::Layout*    _settingBtnArea = nullptr;
    cocos2d::Node*          _settingBtn     = nullptr;
    /*  商城按钮   */
    void crystalBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void crystalCall();
    
    cocos2d::ui::Layout*    _crystalBtnArea = nullptr;
    cocos2d::Node*          _crystalBtn     = nullptr;
    /**
     *  完成任务
     *
     *  @param e 事件数据
     */
    void onTaskCompleted(cocos2d::EventCustom* e);
    
    /**
     *  当物品改变
     *
     *  @param e 物品数据
     */
    void onItemChange(cocos2d::EventCustom *e);
    /**
     *  刷新按钮
     */
    void updateBtns();
    
    /**
     *  board打开的动画
     *
     *  @param e
     */
    void onBoardFold(cocos2d::EventCustom* e);
    
    /**
     *  board关闭的动画
     *
     *  @param e
     */
    void onBoardUnfold(cocos2d::EventCustom* e);
    
    void onEnter() override;
    
    /**
     *  离线生产检测
     */
    void outLineGainCheck();
    
    /**
     *  给按钮们排序
     */
    void sortBtns();
    
    /**
     *  将按钮展示为从未点击过的状态
     *
     *  @param btnid 按钮的id
     */
    void showUnTouchState(int btnid);
    
    /**
     *  初始化遍历所有按钮查看按钮是否从未点击，如果是将按钮置于未点击状态
     */
    void initShowUnTouchState();
    
    /**
     *  停止出现动画，函数的功能是如果玩家在出现动画播放完成之前就点击则中断下面的动画的播放
     *
     *  @param btnid 按钮的ID
     */
    void stopBtnShowAct(int btnid);
    
    void frameCall(cocostudio::timeline::Frame* f);

	/**
	*	钻石商城
	*/
	void rmbBuyBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    
private:/**私有变量**/
    
    /**
     *  开始动画
     */
    cocostudio::timeline::ActionTimeline *_startAction;
    
    /**
     *  侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenMap;
    
    /**
     *  属于主页的按钮IDa
     */
    const std::vector<int> _btnsInMain;
    
    /**
     *  主页上的所有按钮
     */
    cocos2d::Map<int, Node*> _btnsMapInMain;
    
    /**
     *  这个字典保存了需要排列位置的按钮，key : 按钮ID value: 按钮指针
     */
    cocos2d::Map<int, Node*> _sortButttons;
    
    /**
     *  需要执行新出现动画的按钮
     */
    std::vector<int> _btnsToRunShowAct;
    
    /**
     *  按钮位置数据
     */
    std::map<int ,std::vector<cocos2d::Vec2>> _buttonPosData;
    
    /**
     *  按钮效果初始化标志位
     */
    bool _btnInited;
    
    /**
     *  新手引导
     */
    Guide* _guide;
    
    cocos2d::ui::Text* _goldTxt;
    cocos2d::ui::Text* _foodTxt;
    cocos2d::ui::Text* _rmbTxt;
    cocos2d::ui::Button* _rmbBtn;
    cocos2d::ui::Layout* _rmbBuyBtn;
    
    /**
     *  上一次任务完成的数据
     */
    cocos2d::Value _preCompleteValue = cocos2d::Value();
};

#endif /* defined(__deer__ManorMainLayer__) */

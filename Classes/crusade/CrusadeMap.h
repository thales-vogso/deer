/******************************************************
        
 *	@desc   战斗界面
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	crusade/CrusadeMap.h
 *   @modify	null
 *******************************************************/

#ifndef __DEER_CRUSADEMAP__
#define __DEER_CRUSADEMAP__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "crusade/CrusadeMapPopupManager.h"
#include "guide/Guide.h"


/**
 *  这个部分要分为三部分
 1、本身作为控制器 需要控制地图显示和移动部分功能
 2、事件管理类 负责处理事件的响应逻辑
 3、弹层管理类 这个类承载上层弹层，管理弹层逻辑
 *
 */

class CrusadeMapEventManager;

class CrusadeMapDelegate
{
public:
    
    virtual CrusadeMapPopupManager *getPopupManager() = 0;
};

class CrusadeMap : public cocos2d::Layer, public CrusadeMapDelegate
{
public:
    /**
     *  create一个地图界面层
     *
     *  @return CrusadeMap对象的指针
     */
    static CrusadeMap *create(int sid,bool newScene = false);
    
    /**
     *  以scene的方式进入地图
     */
    static void goToCrusadeMapScene(int sid = 0,bool newScene = false);
    
    
    static void goToCrusadeMapSceneNoFade(int sid);
	/**
	 *	切换战争迷雾
	 */
	static void toggleFog(bool fog);
    /**
     *  刷新庇护所
     */
    void updataShelter();
    
    /**
     *  地图
     */
    CrusadeMap();
    
    /**
     *  析构函数
     */
    virtual ~CrusadeMap();
    
CC_CONSTRUCTOR_ACCESS:
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init(int sid,bool newScene = false);
    
    void setGuidScene(cocos2d::Scene* scene);
    
    //是否要进行战斗场景恢复..
    void CrusadeRecover();
    
protected:
    enum class TouchMode
    {
        kNone = 0,
        kPreMoveOrHit = 1,
        kZoom = 2,
        kMove = 3,
        kHit =4
    } _mode;
    
    void onEnter() override;
    
    /**
     *  触摸回调
     *
     *  @param touch 触摸
     *  @param e     事件
     */
    void touchesEndedCall(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *e);
    
    /**
     *  触摸移动回调
     *
     *  @param touches
     *  @param e
     */
    void touchesMovedCall(const std::vector<cocos2d::Touch *> &touches , cocos2d::Event *e);
    
    /**
     *  触摸开始
     *
     *  @param touches
     *  @param e       
     */
    void touchesBeganCall(const std::vector<cocos2d::Touch *> &touches , cocos2d::Event *e);
    
    /**
     *  触摸取消
     *
     *  @param touches
     *  @param e       
     */
    void touchesCancelledCall(const std::vector<cocos2d::Touch *> &touches , cocos2d::Event *e);
    
    /**
     *  将 point 移动到中心
     *
     *  @param cocos2d::Vec2 
     */
    void movePointToCenter(cocos2d::Vec2 point);
    
    /**
     *  获取弹层管理器
     *
     *  @return 
     */
    CrusadeMapPopupManager *getPopupManager();
    
    void onEnterShelter(cocos2d::EventCustom *event);
    void enterShelter();
    
    /**
     *  进入的是庇护所
     *
     *  @param e 
     */
    void onClearField(cocos2d::EventCustom *e);
    
    /**
     * 从战利品界面退出
     *
     *  @param e 
     */
    void exitFromSettlement(cocos2d::EventCustom *e);
    
    /**
     *  延时触摸开关
     *
     *  @param able 
     */
    void setTouchEnabelInDelay(bool able);
    
    /**
     *  背包按钮的回调
     */
    void packageBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
    void packageReal();
    
    /**
     *  进入庇护所
     *
     *  @param data 
     */
    void enterShelter(const cocos2d::ValueMap &data);
    
    /**
     *  进入野外商店
     *
     *  @param data
     */
    void enterWildShop(const cocos2d::ValueMap &data);
    
    /**
     *  进入墓园
     *
     *  @param data 数据
     */
    void enterCemetery(const cocos2d::ValueMap &data);
    
    void onPackageChange(cocos2d::EventCustom* e);
    
    void onItemChange(cocos2d::EventCustom* e);
    
    /**
     *  回程卷轴按钮
     */
    void backManor(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void backManorReal();
    
    /**
     *  团灭回城
     *
     *  @param e
     */
    void playerAce(cocos2d::EventCustom* e);
    
    void moveNothingHappen(cocos2d::EventCustom* e);
    
    /****************显示方法***************/
    
    void playWow();
    
    /**
     *  新手引导手指
     */
    Guide* _guide = NULL;
    
    /**
     *  来一二三左脚右脚迈起来
     */
    int _footStep = 1;
    
    /**
     *  昨天我拥有的面包
     */
    int _preBreadNum = 0;
    /**
     *  昨天我包里拥有的东西
     */
    int _prePackageNum = 0;
    
    
private:
    /**
     *  地图
     */
    cocos2d::TMXTiledMap *_crusadeMap;
    
	/**
	 *	迷雾层，主要配合debug开关迷雾
	 */
	static cocos2d::TMXLayer* _fogLayer;
    /**
     *  英雄
     */
    cocos2d::Sprite* _player;
    
    /**
     *  地图瓦片的尺寸
     */
    cocos2d::Size _tileSize;
    
    /**
     *  地图尺寸
     */
    cocos2d::Size _mapSize;
    
    /**
     *  地图的sid
     */
    int _sid;
    
    /**
     *  触摸监听用来监听触摸移动
     */
    cocos2d::EventListenerTouchAllAtOnce * _touchListener;
    
    cocos2d::ui::Layout* _packageBtn = nullptr;
    cocos2d::ui::Layout* _homeBtn = nullptr;
    
    //初始化的默认显示数据字典..
    std::map<int, std::string> _mapbackFileName;
    std::map<int, float> _defaultScale;
    std::map<int, std::pair<int, int>> _mapBackTile;
    
    /**
     *  移动开关
     */
    bool _ismove;
    
    /**
     *  放大地图过程中使用的变量..
     */
    float _startScale;
    
    /**
     *  
     */
    cocos2d::MoveTo *_playerMoveAct;
    
    /**
     *  预退出状态
     */
    bool _prequit;
    
    /**
     *  缓存即将进入的场地数据；
     */
    cocos2d::ValueMap _enterData;
    
    cocos2d::Value _enconterData;
    
    CrusadeMapPopupManager * _popUpManager;
    
    friend class CrusadeMapEventManager;
    
    friend class CrusadeMapPopupManager;
    
    /**
     *  是否从城堡而来
     */
    bool _newScene = false;
};

///**
// *  事件管理器
// */
//class CrusadeMapEventManager : public cocos2d::Node
//{
//public:
//    
//    virtual ~CrusadeMapEventManager();
//    static CrusadeMapEventManager *createWithTarget(CrusadeMap *target);
//    
//CC_CONSTRUCTOR_ACCESS:
//    
//    bool initWithTarget(CrusadeMap *target);
//    
//    /**
//     *  遇到敌人
//     *
//     *  @param event 事件数据
//     */
//    void onEncounter(cocos2d::EventCustom * event);
//    void enconterMonster();
//    
//    /**
//     *  进入场景
//     *
//     *  @param event 事件数据
//     */
//    void onEnterField(cocos2d::EventCustom *event);
//    void enterFieldAct();
//    
//    /**
//     *  刷新战争迷雾
//     */
//	void onFogClear(cocos2d::EventCustom* e);
//    
//    void onEnterShelter(cocos2d::EventCustom * event);
//    
//    /**
//     *  管理对象
//     */
//    CrusadeMap *_target;
//    
//    /**
//     *  遇到的怪物数据
//     */
//    cocos2d::Value _enconterData;
//    
//    /**
//     *  缓存即将进入的场地数据；
//     */
//    cocos2d::ValueMap _enterData;
//    
//    /**
//     *  事件侦听器字典
//     */
//    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
//};


#endif /* defined(__deer__CrusadeMap__) */

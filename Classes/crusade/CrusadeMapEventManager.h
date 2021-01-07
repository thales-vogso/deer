/******************************************************
 *	@desc   地图事件管理
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	crusade/CrusadeMapEventManager.h
 *   @modify	null
 *******************************************************/



#ifndef __DEER_CRUSADEMAPEVENTMANAGER__
#define __DEER_CRUSADEMAPEVENTMANAGER__

#include "cocos2d.h"
#include "manor/LayerBase.h"
/**
 *  这个类的主要作用是实现对数据部分事件的响应逻辑
 */
class CrusadeMap;

class CrusadeMapEventManager : public cocos2d::Node,public LayerBase
{
public:
    static CrusadeMapEventManager *createWithTarget(CrusadeMap *target);
    
CC_CONSTRUCTOR_ACCESS:
    
    virtual ~CrusadeMapEventManager();
    
    bool initWithTarget(CrusadeMap *target);
    
    /**
     *  遇到敌人
     *
     *  @param event 事件数据
     */
    void onEncounter(cocos2d::EventCustom * event);
    void enconterMonster();
    
    void removeSelf() override;
    void updateProperty() override;
    
    /**
     *  进入场景
     *
     *  @param event 事件数据
     */
    void onEnterField(cocos2d::EventCustom *event);
    void enterFieldAct();
    
    /**
     *  刷新战争迷雾
     */
	void onFogClear(cocos2d::EventCustom* e);
    
    void onEnterShelter(cocos2d::EventCustom * event);
    
    /**
     *  管理对象
     */
    CrusadeMap *_target;
    
    /**
     *  遇到的怪物数据
     */
    cocos2d::Value _enconterData;
    
    /**
     *  缓存即将进入的场地数据；
     */
    cocos2d::ValueMap _enterData;
    
    /**
     *  事件侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
};


#endif /* defined(__deer__CrusadeMapEventManager__) */

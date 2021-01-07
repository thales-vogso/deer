/****************************************************************************
    
 *	@desc	界面base
 *	@date	2015-5-4
 *	@author	110101
     
 *	@file	manor/LayerBase.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_LAYERBASE__
#define __DEER_LAYERBASE__

#include "core/custom/Event.h"
#include "cocos2d.h"

class LayerBase
{
public:
	virtual ~LayerBase();
	/**
     *  通过重写该方法 实现移除自身的不同效果
     */
    virtual void removeSelf() = 0;
    /**
     *  通过重写该方法 实现设置某些属性
     */
    virtual void updateProperty() = 0;
    
	/**
	 *	事件池
	 */
	//std::vector<cocos2d::EventListener*> _eventPool;
	std::map<cocos2d::EventListener*, custom::Event*> _eventPool;
	/**
	 *	添加事件池
	 */
	void addEventPool(cocos2d::EventListener* listener);
	void addEventPool(cocos2d::EventListener* listener, custom::Event* e);
	/**
	 *	清空事件池
	 */
	void clearEventPool();
};

#endif /* defined(__deer__LayerBase__) */

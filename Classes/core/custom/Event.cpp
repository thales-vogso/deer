/****************************************************************************
 
*	@desc	自定义事件
*	@date	2014-11-2
*	@author	110101
   
*	@file	custom/Event.cpp
*	@modify	null
******************************************************************************/
#include "Event.h"
USING_NS_CC;

namespace custom{
	Event::Event(){
		_evt = new EventDispatcher();
		_evt->setEnabled(true);
	}
	Event::~Event(){
        if(_evt)
        {
            _evt->release();
        }
        
	}
	cocos2d::EventListenerCustom* Event::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback){
		return _evt->addCustomEventListener(eventName, callback);
	}
	void Event::addListener(cocos2d::EventListener *listener, int prority){
		_evt->addEventListenerWithFixedPriority(listener, prority);
	}
	void Event::addListener(cocos2d::EventListener *listener, Node* node){
		_evt->addEventListenerWithSceneGraphPriority(listener, node);
	}
	void Event::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback, Node* node){
		EventListenerCustom* listener = addListener(eventName, callback);
		addListener(listener, node);
	}
	void Event::removeListener(const std::string &eventName){
		_evt->removeCustomEventListeners(eventName);
	}
	void Event::removeListener(EventListener* listener){
		if(_evt && listener)
        {
            _evt->removeEventListener(listener);
        }
	}
	void Event::dispatch(const std::string &eventName, void* param){
		if(_evt) _evt->dispatchCustomEvent(eventName, param);
	}
}

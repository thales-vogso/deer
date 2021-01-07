/****************************************************************************
 
*	@desc	自定义事件
*	@date	2014-11-2
*	@author	110101
   
*	@file	custom/Event.h
*	@modify	null
******************************************************************************/
#ifndef  __CUSTOM_EVENT_H__
#define  __CUSTOM_EVENT_H__

#include "cocos2d.h"
namespace custom{
	class Event
	{
	public:
		Event();
		virtual ~Event();
		/**
		*	添加事件
		*/
		cocos2d::EventListenerCustom* addListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback);
		void addListener(cocos2d::EventListener *listener, int prority = 1);
		void addListener(cocos2d::EventListener *listener, cocos2d::Node* node);
		void addListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback, cocos2d::Node* node);
		/*
		*	移除监听
		*/
		void removeListener(const std::string &eventName);
		void removeListener(cocos2d::EventListener *listener);
	protected:
		/**
		 *	发送事件
		 */
		void dispatch(const std::string &eventName, void* param);
	private:
		/**
		*	事件分发器
		*/
		cocos2d::EventDispatcher* _evt;
	};
}

#endif // __CUSTOM_EVENT_H__
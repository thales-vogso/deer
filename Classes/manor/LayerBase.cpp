/****************************************************************************
    
 *	@desc	界面base
 *	@date	2015-5-4
 *	@author	110101
     
 *	@file	manor/LayerBase.cpp
 *  @modify	null
 ******************************************************************************/

#include "LayerBase.h"

USING_NS_CC;

LayerBase::~LayerBase(){
	clearEventPool();
}
void LayerBase::addEventPool(EventListener* listener, custom::Event* e){
	//_eventPool.push_back(listener);
	_eventPool[listener] = e;
}
void LayerBase::addEventPool(EventListener* listener){
	//
}
void LayerBase::clearEventPool(){
	for (auto pair : _eventPool){
        if(pair.second&&pair.first)
        {
            pair.second->removeListener(pair.first);
        }
	}
}
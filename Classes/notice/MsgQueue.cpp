/************************************************************
   
*	@desc	消息列队
*	@date	2015-8-3
*	@author	110101
   
*	@file	notice/MsgQueue.cpp
*	@modify	null
*****************************************************/

#include "MsgQueue.h"

USING_NS_CC;

MsgQueue::MsgQueue(){

}
MsgQueue::~MsgQueue()
{
	clear();
}


void MsgQueue::add(Value msg, int index){
	ValueVector::iterator pos = _msgs.begin() + index;
	if (pos > _msgs.end()) pos = _msgs.begin();
	_msgs.insert(pos, msg);
}
Value MsgQueue::del(int index){
	ValueVector::iterator pos = _msgs.begin() + index;
	if (pos > _msgs.end()) pos = _msgs.begin();
	Value msg = *pos;
	_msgs.erase(pos);
	return msg;
}
int MsgQueue::size(){
	return _msgs.size();
}
void MsgQueue::clear(){
	_msgs.clear();
}
void MsgQueue::lock(){
	_isLock = true;
}
void MsgQueue::unlock(){
	_isLock = false;
}
bool MsgQueue::isLock(){
	return _isLock;
}
/****************************************************************************
    
 *	@desc	消息列队
 *	@date	2015-8-3
 *	@author	110101
    
 *	@file	notice/MsgQueue.h
 *	@modify	null
 ******************************************************************************/

#ifndef __MSG_QUEUE_TIPS__
#define __MSG_QUEUE_TIPS__


#include "cocos2d.h"

class MsgQueue
{
    
public:
	MsgQueue();
	virtual ~MsgQueue();
	/**
	 *	添加消息
	 *	@param	msg	消息
	 *	@param	index	消息序列
	 */
	void add(cocos2d::Value msg, int index);
	void add(cocos2d::Value msg){
		add(msg, 0);
	}
	/**
	 *	删除消息
	 *	@param	index	消息序列
	 *	@return	删除消息
	 */
	cocos2d::Value del(int index);
	cocos2d::Value del(){
		return del(0);
	}
	/**
	 *	数据个数
	 */
	int size();
	/**
	 *	清空
	 */
	void clear();
	/**
	 *	是否锁定
	 *	@return	锁定状态
	 */
	bool isLock();
	/**
	 *	锁定
	 */
	void lock();
	/**
	 *	解锁
	 */
	void unlock();
private:
	/**
	 *	消息列队
	 */
	cocos2d::ValueVector _msgs = cocos2d::ValueVectorNull;
	/**
	 *	上次执行时间
	 */
	clock_t _lastStartTime = 0;
	/**
	 *	显示间隔
	 */
	const clock_t SHOW_DURATION = 500;
	/**
	 *	是否锁定
	 */
	bool _isLock = false;
	
};


#endif // __MSG_QUEUE_TIPS__ 

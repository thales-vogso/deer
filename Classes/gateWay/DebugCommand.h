/************************************************************
   
*	@desc	命令行测试
*	@date	2015-7-9
*	@author	110101
   
*	@file	gateWay/DebugCommand.h
*	@modify	null
*****************************************************/

#ifndef  __DEBUG_COMMAND__
#define  __DEBUG_COMMAND__

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  DebugCommand
{
public:
	DebugCommand();
	virtual ~DebugCommand();
	/**
	 *	启动
	 */
	static void launch();
private:
	/**
	 *	道具初始化
	 */
	void itemInit();
	/**
	 *	状态初始化
	 */
    void statusInit();
	/**
	 *	出征初始化
	 */
	void crusadeInit();
	/**
	 *	天赋初始化
	 */
	void talentInit();
	/**
	 *	事件初始化
	 */
	void taskInit();
    /**
     *  角色初始化
     */
    void roleInit();
    /**
     *  apk失效时间
     */
    //void appTimeInit();
    
private:
	/**
	 *	自身
	 */
	static DebugCommand* _instance;
	/**
	 *	端口
	 */
	const int TELNET_PORT = 5678;
};

#endif // __DEBUG_COMMAND__


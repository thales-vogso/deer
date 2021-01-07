/****************************************************************************
    
 *	@desc	提示信息
 *	@date	2015-6-12
 *	@author	110101
    
 *	@file	popup/Tips.h
 *	@modify	null
 ******************************************************************************/

#ifndef __NOTICE_TIPS__
#define __NOTICE_TIPS__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "MsgQueue.h"


class Tips : public cocos2d::ui::Layout
{
private:
	/**
	*	颜色
	*/
	const std::string COLOR_NORMAL = "#ffffff";
	const std::string COLOR_WARNING = "#ed5151";
	const std::string COLOR_MINT = "#ffe13e";
public:
	CREATE_FUNC(Tips);
	virtual ~Tips();
	/**
	 *	显示
	 */
	void show(std::string msg, std::string color);
	void show(std::string msg){
		show(msg, COLOR_NORMAL);
	}
	/**
	 *	炼金
	 *	@param	msg
	 */
	void mint(std::string msg){
		show(msg, COLOR_MINT);
	}
	/**
	 *	警告，立即显示，不进队列
	 *	@param	msg
	 */
	void warning(std::string msg){
		show(msg, COLOR_WARNING);
	}
	/**
	 *	清空
	 */
	void clear();
protected:
	/**
	 *	初始化
	 */
	bool init();
private:
	/**
	 *	消息列队
	 */
	MsgQueue* _msgQueue;
	/**
	 *	上次执行时间
	 */
	clock_t _lastStartTime = 0;
	/**
	 *	显示间隔
	 */
	const float SHOW_DURATION = 0.5;
	/**
	 *	字号
	 */
	const float FONT_SIZE = 35;

	/**
	 *	显示执行
	 */
	void showExe();
	/**
	 *	文字运动
	 *	@param	msg	信息
	 *	@param	color	颜色，#加十六进制
	 */
	void textMotion(std::string msg, std::string color);
	void textMotion(std::string msg){
		textMotion(msg, COLOR_NORMAL);
	}
};


#endif // __NOTICE_TIPS__ 

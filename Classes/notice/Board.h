/****************************************************************************
 *	@desc   board信息板
 *	@date	2015-7-6
 *	@author	110101
    
 *	@file	notice/Board.h
 *  @modify	null
 ******************************************************************************/

#ifndef __NOTICE_BOARD_H__
#define __NOTICE_BOARD_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "MsgQueue.h"


class Board : public cocos2d::ui::Widget,public custom::Event
{
public:
	CREATE_FUNC(Board);
	virtual ~Board();
	/**
	 *	显示
	 */
	void show(std::string msg);
	/**
	 *	显示历史
	 */
	void showHistory(cocos2d::Value all);
	/**
	 *	主页模式
	 */
	void mainMode();
	/**
	 *	子页面模式
	 */
	void subMode();
	/**
	 *	获取按钮，用于添加折叠展开功能
	 */
	cocos2d::ui::Layout* getFoldBtn();
	/**
	 *	获取当前高度，展开和折叠时候的高度
	 */
	float getHeight();
	/**
	 *	折叠
	 */
	void fold();
	/**
	 *	展开
	 */
	void unFold();

	static const std::string BOARD_FOLD;
protected:
	/**
	 *	初始化
	 */
	bool init();
private:
	/**
	 *	view层
	 */
	cocos2d::Node* _view;
	/**
	 *	列表容器
	 */
	cocos2d::ui::ListView* _list;
	/**
	 *	模块模板
	 */
	cocos2d::ui::Layout* _pattern;
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
	const clock_t SHOW_DURATION = 500;
    /**
     *  board的最大宽度
     */
    const float MAX_BOARD_WIDTH = 900;
	/**
	 *	移动时间间隔
	 */
	const float BoardMoveDuration = 0.3f;
	/**
	 *	旋转时间间隔
	 */
	const float BoardBtnDuration = 0.1f;
	/**
	 *	展开高度
	 */
	const float HEIGHT = 414;
	/**
	 *	是否折叠
	 */
	bool _isFold = false;
	/**
	 *	分页折叠
	 */
	bool _isSubFold = false;
    /**
     *  字体宽度
     */
    float _letterWidth = 39;
    
	/**
	*	显示执行
	*/
	void showExe();
	/**
	*  开关按钮的回调动作
	*
	*  @param sender 按钮
	*  @param type   触摸类型
	*/
	void onSwitchBtnClick(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);
};
#endif //__NOTICE_BOARD_H__

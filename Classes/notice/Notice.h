/****************************************************************************
 *	@desc   信息管理器
 *	@date	2015-7-7
 *	@author	110101
    
 *	@file	manor/Notice.h
 *  @modify	null
 ******************************************************************************/

#ifndef __NOTICE_H__
#define __NOTICE_H__

#include "Board.h"
#include "Tips.h"

class Notice : public cocos2d::Ref , public custom::Event
{
public:
	enum TipsType
	{
		kNormal = 0,
		kWarning = 1,
		kMint = 2
	};

	static Notice* getInstance();
    
    static void destoryInstance();
    /**
     *  广播信息框
	 *	@param	isMain	是否主页
     */
    Board* getBoard(bool isMain = false); 
    /**
     *  获取提示框
     */
    Tips* getTips();
    
    /**
     *  发送广播
     *  @param msg 消息
     */
    void postBoard(std::string msg);
    /**
     *  发送提示
     *  @param msg 消息
     */
    void postTips(std::string msg, int type);
	void postTips(std::string msg){
		postTips(msg, kNormal);
	}
    /**
     *  获取广播版的高度
     */
    float getBoardHeight();
	/**
	 * 广播面板高度
	 */
	static const int BOARD_Z_ORDER;
	/**
	 * 提示面板高度
	 */
	static const int TIPS_Z_ORDER;
	/**
	 *	事件
	 */
	static const std::string FOLD;
	static const std::string UNFOLD;

protected:

	Notice();
    virtual ~Notice();
	
    
    bool init();
    /**
     *  展开
     */
    void foldNoticeBoard();
    
    /**
     *  收起
     */
    void unfoldNoticeBoard();
    
    /**
     *  是否展开board栏
     *
     *  @return true 展开，false 关闭
     */
	inline bool isShow();
private:
	/**
	 *	实体
	 */
	static Notice *_instance;
	/**
	 *	广播栏
	 */
	Board* _board;
	/**
	 *	侦听池,因为当前类没有实体，所以没有继承layerbase
	 */
	std::map<cocos2d::EventListener*, custom::Event*> _eventPool;
	/**
	 *	提示栏
	 */
	Tips* _tips;
	/**
	*  开关按钮的回调动作
	*  @param e
	*/
	void onBoardFold(cocos2d::EventCustom* e);
	/**
	*  事件完成回调
	*  @param e
	*/
	void onTaskComplete(cocos2d::EventCustom* e);
	/**
	*  随机事件完成回调
	*  @param e
	*/
	void onSuddenConfirm(cocos2d::EventCustom* e);
	/**
	*  道具改变回调
	*  @param e
	*/
	void onItemChange(cocos2d::EventCustom* e);
};


#endif // __NOTICE_H__

/****************************************************************************
 
*	@desc	游戏数据
*	@date	2014-12-16
*	@author	110101
   
*	@file	data/Game.h
******************************************************************************/
#ifndef __DATA_GAME_H__
#define __DATA_GAME_H__

#include "cocos2d.h"
#include "data/User.h"

namespace data {
/**
     打开副将位置的枚举
     */
enum assisOpenType {
    noOpen = 0,
    openLeft = 1,
    openRight = 2,
    openAll = 3
};
class Game : public custom::Event {
public:
    /**
		 *	初始化
		 */
    Game();
    /**
		 *	析构函数，要清除有指针的内容
		 */
    virtual ~Game();
    /**
		 *	获取实体
		 */
    static Game* getInstance();
    static void destroyInstance();
    /**
		*	初始化
		*/
    void init();
    /**
		 *	从表中读取数据
		 */
    cocos2d::Value getDetail();
    /**
		 *	设置金币
		 *	@param	num	金币数量
		 */
    void setGold(int num);
    /**
		 *	获取金币
		 *	@return	金币数量
		 */
    int getGold();
    /**
		 *	金币增加
		 *	@param	num	 增加的数量，可为负数
		 */
    bool goldPlus(int num);
    /**
		 *	金币减少
		 *	@param	num	减少的数量，不能为负数
		 *	@return	结果
		 */
    bool goldReduce(unsigned num);
    /**
		*	设置体力
		*	@param	num	体力数量
		*/
    void setAct(int num);
    /**
		*	获取体力
		*	@return	金币数量
		*/
    int getAct();
    /**
		*	体力增加
		*	@param	num	 增加的数量，可为负数
		*/
    bool actPlus(int num);
    /**
		*	体力减少
		*	@param	num	减少的数量，不能为负数
		*	@return	结果
		*/
    bool actReduce(unsigned num);
    /**
		 *	获取用户id
		 */
    int getUID();
    /**
		 *	设置日期
		 *	@param	date	日期，可以用时间戳，可以用datetime，没参数为当前日期
		 */
    void setDate(time_t timestamp);
    void setDate(std::string dateStr);
    void setDate();
    /**
     *  获取副将解锁的位置
     *
     *  @return 0,1,2,3 分别代表 没有解锁、左侧解锁、右侧解锁、全部解锁
     */
    int getAssisOpen();
    /**
     *  解锁副将位置
     *
     *  @param p 1,2  分别代表  左、右
     */
    void openAssisPosition(int p);
    /**
		 *	获取日期
		 *	@return	日期
		 */
    std::string getDate();
    /**
		 *	金币改变事件
		 */
    static const std::string GOLD_CHANGE;
    /**
		*	体力改变事件
		*/
    static const std::string ACT_CHANGE;

private:
    /**
		 *	自身
		 */
    static Game* _instance;
    /**
		 *	数据库
		 */
    DBSqlite* _db;
    /**
		 *	金币数量
		 */
    int _gold;
    /**
		 *	体力
 		 */
    int _act;
    /**
		 *	用户uid
		 */
    int _uid;
    /**
		 *	姓名
		 */
    std::string _name;
    /**
		*	用户表
		*/
    std::string TABLE_USER = "user";
    /**
		 *	用户游戏表
		 */
    std::string TABLE_USER_GAME = "user_game";
};
}
#endif // __DATA_GAME_H__
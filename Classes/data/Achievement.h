/****************************************************************************
      
*	@desc	成就
*	@date	2015-2-5
*	@author	110101
   
*	@file	data/Achievement.h
******************************************************************************/
#ifndef __DATA_ACHIEVEMENT_H__
#define __DATA_ACHIEVEMENT_H__

#include "cocos2d.h"
#include "Item.h"

namespace data {

class Achievement : public custom::Event,custom::Memories {
public:
	enum CountID
	{
		/*
		ALCHEMY = 110100,
		SCAN = 110200,
        GAMBLE = 110300,
        TRAIN = 210100,
        TALENT = 210200,
        BREAKTHROUGH = 310100,
        CONSUME = 310200,
        DEFEAT =310300,
        KILL = 310400,
        COLLECTION = 410100,
        BURY = 410200,
        */
        /*****割********/
		MINT = 110101,
		EXPLORE = 110202,//已完成..
        GAMEBLING = 110303,//赌博..
        BABIE_TOWER = 110104,//巴别塔完成..
        EAT_FOOD = 110505,//太空能量完成..
        DIG = 110606,    //采矿完成..
        KILL_ENEMY = 110707,//杀敌完成..
        
        OTHER =510100
	};
	enum Status
	{
		UNEXCHANGE = 0,
		EXCHANGED = 1
	};
	/**
	*	初始化
	*/
	Achievement();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Achievement();
    /**
	*	获取实体
	*/
	static Achievement* getInstance();
	/**
	 *	实体析构
	 */
    static void destroyInstance();
	/**
	 *	初始化
	 */
	void init();
	/**
	 *	通过aid获取成就
	 *	@param	aid
	 *	@return	成就
	 */
	cocos2d::Value getAchievementByAID(int aid);
	/**
	 *	通过aid获取用户成就
	 *	@param	aid
	 *	@return	用户成就
	 */
	cocos2d::Value getUserAchievementByAID(int aid);
	/**
	 *	添加成就
	 *	@param	aid
	 */
	bool addAchievement(int aid);
	/**
	 *	兑换成就
	 */
	bool exchange(int aid);
	/**
	 *	获取用户成就
	 */
	cocos2d::Value getUserAchievements();
	/**
	 *	计数
	 *	@param	cid	计数器id
	 *	@param	amount	数量
	 */
	void count(int cid, int amount);
	void count(int cid){
		count(cid, 1);
	}
	/**
	 *	通过cid获取计数器实际数字
	 */
	int getCounterAmountByCid(int cid);
	/**
	 *	成就完成
	 */
	static const std::string ACHIEVEMENT_COMPLETE;
private:
    /**
	 *	自身
	 */
	static Achievement* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
    
	/**
	 *	 成就表
	 */
	std::string TABLE_ACHIEVEMENT = "achievement";
    /**
     *  成就中一些需要记录的数据
     */
    std::string TABLE_USER_ACHIEVEMENT = "user_achievement";
	/**
	 *	 计数器
	 */
	std::string TABLE_COUNTER = "counter";
	/**
	 *  用户计数器
	 */
	std::string TABLE_USER_COUNTER = "user_counter";
};

}
#endif // __DATA_ACHIEVEMENT_H__
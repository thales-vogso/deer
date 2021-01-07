/****************************************************************************
 
*	@desc	建筑物数据
*	@date	2015-1-7
*	@author	110101
   
*	@file	data/Item.h
******************************************************************************/
#ifndef __DATA_BUILDING_H__
#define __DATA_BUILDING_H__

#include "cocos2d.h"
#include "data/Item.h"

namespace data {

class Building : public custom::Event,custom::Memories {
public:
    enum WORKER_ID
    {
        PLANT_WORKERID = 110101,
        FOOD_WORKERID = 110102
    };
	/**
	*	初始化
	*/
	Building();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Building();
    /**
	*	获取实体
	*/
	static Building* getInstance();
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	根据建筑物bid获取建筑物
	 *	@return	建筑物信息
	 */
	cocos2d::Value getBuildingByBID(int bid);
	/**
	 *	根据建筑物bid获取建造
	 *	@return	建造信息
	 */
	cocos2d::Value getBuildByBID(int bid);
	/**
	 *	获取用户建造列表
	 *	@return	用户建造列表
	 */
	cocos2d::Value getUserBuildList();
	/**
	 *	添加用户制造
	 *	@param	bid	 建造的建筑物
	 */
	void addUserBuild(int bid);
	/**
	 *	解锁工人
	 *	@param	wid	工人id
	 */
	void addUserWorker(int wid);
	/**
	 *	建造
	 *	@param	bid	 建造的建筑物
	 */
	cocos2d::Value build(int bid);
	/**
	 *	通过bid获取用户建筑物
	 *	@param	bid	 建造的建筑物
	 *	@return	用户建筑物
	 */
	cocos2d::Value getUserBuildByBID(int bid);
	/**
	 *	通过bid获取用户建筑物数量
	 *	@param	bid	建筑物id
	 *	@return	建筑物数量
	 */
	int getUserBuildAmountByBID(int bid);
	/**
	 *	通过wid获取用户工人数量
	 *	@param	wid	工人id
	 *	@return	工人数量
	 */
	int getUserWorkerAmountByWID(int wid);
	/**
	 *	通过bid获取工人
	 */
	cocos2d::Value getWorkersByBID(int bid);
	/**
	*	获取工人总数
	*/
	int getWorkTotal();
	/**
	 *	获取空闲工人数
	 */
	int getWorkFree();
	/**
	*	增加工人
	*	@param	wid	工人id
	*	@amount	amount	数量
	*	@return	结果，比如没有足够的工人
	*/
	bool workerPlus(int wid, int amount = 1);
	/**
	*	减少工人
	*	@param	wid	工人id
	*	@amount	amount	数量
	*	@return	结果，比如工人为0
	*/
	bool workerReduce(int wid, int amount = 1);
	/**
	 *	获取收获列表
	 */
	cocos2d::Value getUserGainList();
	/**
	 *	根据wid获取工人生产
	 */
	cocos2d::Value getWorkerGainByWID(int wid);
	/**
	 *	获取用户收益
	 */
	cocos2d::Value getUserGain();
	/**
	 *	采集
	 */
	cocos2d::Value gather();
	/**
	 *	收获
	 */
	cocos2d::Value gain();
	/**
	 *	获取收益间隔
	 */
	int getGainInterval();
	/**
	 *	收获回调
	 *	回调的字符串
	 */
	void gainCallback(std::string str);
	/**
	*	工人人数建筑物
	*/
	enum BuildingID
	{
		SUPPORT_MIN = 110101,
		SUPPORT_MAX = 110138,
		ASSEMBLY = 111501
	};
	const int SUPPORT_TOTAL = 38;
	/**
	*	建造
	*/
    static const std::string BUILD;
	/**
	 *	工人改变
	 */
	static const std::string WORKER_CHANGE;
    
    /**
     *  收集事件
     */
    static const std::string EVENT_GATHER;
    
    /**
     *  自动收获
     */
    static const std::string GAIN;
    
private:
    /**
	 *	自身
	 */
	static Building* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	定时器
	 */
	cocos2d::Scheduler _scheduler;
	/**
	 *	用户建筑物
	 *	@param	bid	 建造的建筑物
	 */
	void userBuildPlus(int bid);
	
	/**
	 *	一个工人建筑物可以承载的工人数
	 */
	int WORKER_BUILDING_UNIT = 5;
	/**
	 *	 建筑物
	 */
	std::string TABLE_BUILDING = "building";
    /**
	 *	 建造
	 */
    std::string TABLE_BUILD = "build";
	/**
	*	 工人
	*/
	std::string TABLE_WORKER = "worker";
	/**
	*	 工人收益
	*/
	std::string TABLE_WORKER_GAIN = "worker_gain";
	/**
	 *	 用户建造
	 */
	std::string TABLE_USER_BUILD = "user_build";
	/**
	*	 用户工人
	*/
	std::string TABLE_USER_WORKER = "user_worker";
	/**
	 *	 用户收益
	 */
	std::string TABLE_USER_GAIN = "user_gain";
};

}
#endif // __DATA_BUILDING_H__
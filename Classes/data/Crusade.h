/****************************************************************************
      
*	@desc	关卡
*	@date	2015-1-22
*	@author	110101
   
*	@file	data/Crusade.h
******************************************************************************/
#ifndef __DATA_CRUSADE_H__
#define __DATA_CRUSADE_H__

#include "cocos2d.h"
#include "User.h"
#include "Stage.h"
#include "Monster.h"
#include "Role.h"
#include "Building.h"


namespace data {

class Crusade : public custom::Event, custom::Memories {
public:
//	/**
//	 *	场地id
//	 */
//	enum FIELD_ID
//	{
//		PORTAL = 110101,
//		GRAVE = 110201,
//        SHELTER = 110301,
//		RECALL = 210101,
//		CLEARANCE = 210201,
//        TAVERN = 310101,
//        SHOP = 310201,
//		ARENA_MIN = 310301,
//		ARENA_MAX = 610101
//	};
	enum ACE_CAUSE
	{
		STARVATION = 110101,
		KILLED = 110201
	};
    
    enum InOutRetCode
    {
        INOUT_ERROR_UNKNOWN = -1,
        INOUT_OK = 0,
        IN_ERROR_SHOTAGE = 1,
        IN_ERROR_OVER = 2,
        OUT_ERROR_SHOTAGE = 3
    };
	/**
	*	初始化
	*/
	Crusade();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Crusade();
    /**
	*	获取实体
	*/
	static Crusade* getInstance();
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	启程
	 *	@param	sid	关卡id
	 *	@return	结果
	 */
	bool depart(int sid);
	bool depart();
	/**
	 *	回城
	 */
	void townPortal();
	/**
	 *	获取用户启程信息
	 */
	cocos2d::Value getUserCrusade();
    
    /**
     *  获取用户启程信息，不初始化用户数据
     */
    cocos2d::Value getUserCrusadeWithoutInit();
	
	/**
	 *	获取用户背包
	 */
	cocos2d::Value getPackage();
	cocos2d::Value getPackage(bool strict);
	/**
	 *	获取用户马车
	 */
	cocos2d::Value getCarriage();
	cocos2d::Value getCarriage(bool strict);
	/**
	 *	获取背包当前装载
	 *	@param	iid	指定道具iid
	 */
	int getPackageCurrent();
	int getPackageCurrent(int iid);
	/**
	 *	获取背包占用空间
	 */
	int getPackageSpace();
	int getPackageSpace(int iid);
	/**
	 *	获取背包总大小
	 */
	int getPackageTotal();
	/**
	 *	获取马车当前装载
	 *	@param	rid	指定角色rid
	 */
	int getCarriageCurrent();
	int getCarriageCurrent(int rid);
	/**
	 *	获取马车总大小
	 */
	int getCarriageTotal();
	/**
	 *	背包装入
	 */
	int packageIn(int iid, int amount = 1);
    /**
     *  在地图界面拾取奖励时使用
     */
    int packageInByPickup(int iid, int amount = 1);
	/**
	 *	背包取出
	 */
	int packageOut(int iid, int amount = 1);
	/**
	*	背包出库
	*/
	void packageDelivery();
	/**
	 *	背包获取
	 */
	void packageTake();
	/**
	 *	背包丢失
	 */
	cocos2d::Value packageLose();
	/**
	 *	马车进入
	 */
	int carriageIn(int rid, int amount = 1);
	/**
	*	马车走出
	*/
	bool carriageOut(int rid, int amount = 1);
    
	/**
	 *	团灭
	 */
	void ace(int cause);
	void ace();
	/**
	 *	掉宝
	 *	@param	iid	道具id
	 *	@param	amount	道具数量
	 */
	void drop(int iid, int amount);
	void drop(int iid){
		drop(iid, 1);
	}
	/**
	 *	掉的所有宝
	 */
	cocos2d::Value getDrops();
	/**
	 *	掉落清空
	 */
	void dropsClear();
	/**
	 *	获取关卡地图
	 *	@return	关卡地图
	 */
	std::vector<std::vector<cocos2d::Value>> getStageMap();
	/**
	 *	获取场景
	 */
	cocos2d::ValueMap getStage();
	/**
	 *	副本观测
	 */
	cocos2d::Value fieldView();
    
    /**
     *  是否是在钛矿里
     *
     */
    bool isInTAI();
    
    /**
     *  是否需要战斗恢复
     *
     */
    bool isNeedRecoverBattle();
    
	/**
	 *	场地进入
	 *	@param	关卡地图场地id
	 *	@param	x	坐标x
	 *	@param	y	坐标y
	 *	@param	block	地图区块
	 */
	bool fieldEnter(int stageMapID);
	bool fieldEnter(int x, int y);
	bool fieldEnter(cocos2d::Value block);
	/**
	 *	场地攻克
	 *	@param	关卡地图场地id
	 *	@param	x	坐标x
	 *	@param	y	坐标y
	 *	@param	block	地图区块
	 *	@return	 是否通关
	 */
	bool fieldSiege(int stageMapID);
	bool fieldSiege(int x, int y);
	bool fieldSiege(cocos2d::Value block);
	/**
	 *	场地失败
	 *	@param	关卡地图场地id
	 *	@param	x	坐标x
	 *	@param	y	坐标y
	 *	@param	block	地图区块
	 */
	void fieldFailed(int stageMapID);
	void fieldFailed(int x, int y);
	void fieldFailed(cocos2d::Value block);
	/**
	 *	场地过关
	 *	@param	关卡地图场地id
	 *	@param	x	坐标x
	 *	@param	y	坐标y
	 *	@param	block	地图区块
	 */
	void fieldClear(int stageMapID);
	void fieldClear(int x, int y);
	void fieldClear(cocos2d::Value block);

	/**
	 *	副本进入
	 */
	bool fieldEnter();
	/**
	 *	移动
	 *	@param	x
	 *	@param	y
	 *	@param	direct	直接到
	 *	@return	最终位置
	 */
	cocos2d::Vec2 move(int x, int y, bool direct = false);
	/**
	 *	移动向北
	 */
	cocos2d::Vec2 moveNorth();
	/**
	 *	移动向南
	 */
	cocos2d::Vec2 moveSouth();
	/**
	*	移动向东
	*/
	cocos2d::Vec2 moveEast();
	/**
	*	移动向西
	*/
	cocos2d::Vec2 moveWest();
    /**
     * 再原点移动
     */
    cocos2d::Vec2 moveOrigin();
	/**
	 *	探索
	 *	@param	探索中心
	 */
	void scan(int x, int y);
	/**
	 *	消耗面包
	 */
	void consume();
    
    /**
     *  战斗中声明恢复消耗
     */
    void consumeByEnhanceLife();
	/**
	 *	杀怪
	 */
	void killMonster(cocos2d::Value monster);
	void killMonster(int mid);
	/**
	 *	推塔
	 */
	void pushTower();
	/**
	*	马车改变
	*/
	static const std::string CARRIAGE_CHANGE;
	/**
	*   背包改变
	*/
	static const std::string PACKAGE_CHANGE;
    /**
	 *	遇敌
	 */
	static const std::string ENCOUNTER;
    /**
     *  什么都没发生
     */
    static const std::string NOTHING_HAPPEN;
	/**
	 *	庇护所捡道具
	 */
	static const std::string SHELTER_DROP;
	/**
	 * 团灭
	 */
	static const std::string ACE;
	/**
	 *	进入场地
	 */
	static const std::string FIELD_ENTER;
	/**
	 *	场地过关
	 */
	static const std::string FIELD_CLEAR;
	/**
	 *	清雾
	 */
	static const std::string FOG_CLEAR;
    /**
     *  队伍中总数改变
     */
    static const std::string TEAMTOTAL_CHANGE;
	//---------------------------- debug ----------------------------
	/**
	 *	设置遇敌
	 */
	void setEncounter(bool flag){
		_encounter = flag;
	}
	/**
	*	设置移动消耗
	*/
	void setConsumeForMove(bool flag){
		_consumeForMove = flag;
	}
	/**
	*	设置战斗消耗
	*/
	void setConsumeForBattle(bool flag){
		_consumeForBattle = flag;
	}
private:
    /**
	 *	自身
	 */
	static Crusade* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	关卡id
	 */
	int _sid;
	/**
	 *	关卡数据
	 */
	cocos2d::ValueMap _stage;
	/**
	 *	移动步数
	 */
	float _step = 0.0f;
	/**
	 *	坐标系
	 */
	std::vector<std::vector<cocos2d::Value>> _coordinate;
	/**
	 *	获取坐标内容
	 */
	cocos2d::Value getCoord(int x, int y);
	/**
	 *	坐标过期
	 */
	void coordExpire(int x, int y);
	/**
	 *	坐标
	 */
	int _x;
	int _y;
	/**
	 *	食品
	 */
	float _food = 0.0f;
	/**
	 *	食品单位消耗
	 */
	float _foodUnit = 1.0f;
	/**
	 *	遇到的怪兽
	 */
	cocos2d::Value _monster;
	/**
	 *	背包默认道具
	 */
	std::vector<int> _package;
	/**
	 *	类型-道具
	 */
	int TYPE_ITEM = 1;
	/**
	 *	类型-角色
	 */
	int TYPE_ROLE = 10;
    /**
	 *	用户出征表
	 */
    std::string TABLE_USER_CRUSADE = "user_crusade";
	/**
	 *	 掉宝
	 */
	std::string TABLE_USER_CRUSADE_DROP = "user_crusade_drop";
	/**
	 *	 用户出征背包
	 */
	std::string TABLE_USER_CRUSADE_PACKAGE = "user_crusade_package";
	/**
	 *	 用户出征马车
	 */
	std::string TABLE_USER_CRUSADE_CARRIAGE = "user_crusade_carriage";
	//---------------------------- debug ----------------------------
	/**
	 *	是否遇敌
	 */
	bool _encounter = true;
	/**
	 *	行军消耗面包
	 */
	bool _consumeForMove = true;
	/**
	 *	战斗消耗面包
	 */
	bool _consumeForBattle = true;
};
}
#endif // __DATA_CRUSADE_H__
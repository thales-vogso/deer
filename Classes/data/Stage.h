/****************************************************************************
      
*	@desc	关卡
*	@date	2015-1-22
*	@author	110101
   
*	@file	data/Stage.h
******************************************************************************/
#ifndef __DATA_STAGE_H__
#define __DATA_STAGE_H__

#include "cocos2d.h"
#include "Item.h"
#include "Monster.h"

namespace data {

class Stage : public custom::Event, custom::Memories {
public:
	enum FieldVisit
	{
		ALREADY_VISIT = 1, //已经访问
		NORMAL_VISIT = 0, //正常访问
		NEVER_VISIT = -1 //从未访问
	};
	enum FieldType
	{
		PORTAL = 100101,//主城
		GRAVE = 100201,//墓地
		RECALL = 100301,//传送门
		TOWER = 100401,//巴别塔
		SHOP = 100501,//商店tavern
		TAVERN = 100601,//酒馆
		DUNGEON = 110101,//不可重复挑战副本
		SHELTER = 110102,//庇护所
		ARENA = 110201,//竞技场
		GATE_BAR = 110301,//过关传送门有怪
		GATE_FREE = 110302//过关传送门无怪
	};
	enum StageFog{
		COORD_CLEAR = 0,
		COORD_FUZZY = 1,
		INIT_CLEAR = 5
	};
	/**
	*	初始化
	*/
	Stage();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Stage();
    /**
	*	获取实体
	*/
	static Stage* getInstance();
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	获取用户场景
	 *	@return	场景列表
	 */
	cocos2d::Value getUserStages();
    
    /**
     *  获取用户场景是否第一次进入
     *  @param sid 场景id
     *
     *  @return bool 是否第一次进入
     */
    bool isUserStageFirstEnter(int sid);
    
	/**
	*	获取关卡
	*	@param	sid	关卡id
	*	@return	关卡详细数据
	*/
	cocos2d::Value getStageBySID(int sid);
	/**
	 *	获取场地
	 *	@param	fid	场地id
	 *	@return	场地数据
	 */
	cocos2d::Value getFieldByFID(int fid);
    /**
     *  获取场地进入道具要求
     *  @param fid 场地id
     *  @return 场地道具需求数据
     */
    cocos2d::Value getFieldKeyByFID(int fid);
	/**
	*	获取关卡主城
	*	@param	sid	关卡id
	*	@return	主城
	*/
	cocos2d::Value getPortal(int sid);
	/**
	 *	场地重制
	 */
	void fieldRest(int sid);
	/**
	 *	场地失败
	 */
	void fieldFailed(int sid, int x, int y);
	/**
	 *	场地进入
	 */
	void fieldEnter(int sid, int x, int y);
	void fieldEnter(int id);
	/**
	 *	攻城
	 *	@return	是否攻克
	 */
	bool fieldSiege(int sid, int x, int y);
	bool fieldSiege(int id);
	/**
	 *	副本通关
	 */
	void fieldClear(int sid, int x, int y);
	void fieldClear(int id);
	/**
	 *	获取副本怪物
	 *	@param	fid	副本id
	 *	@param	round	回合
	 */
	cocos2d::Value getFieldMonster(int fid, int round);
	/**
	 *	获取副本奖励
	 *	@param	fid	副本id
	 */
	cocos2d::Value getFieldReward(int fid);
	/**
	 *	解锁用户场景
	 *	@param	sid	场景列表
	 */
	void addUserStage(int sid);
	/**
	 *	获取关卡地图
	 *	@param	sid	关卡id
	 *	@return	地图数据
	 */
	std::vector<std::vector<cocos2d::Value>> getStageMap(int sid);
	/**
	 *	扫描地图
	 */
	bool scanMap(int sid, int x, int y);
	/**
	 *	通过id获取关卡地图
	 */
	cocos2d::Value getStageMapByID(int id);
    
	/**
	 *	通过坐标获取关卡地图 （通过坐标和关卡ID获取地图上某一小块的信息）
	 */
	cocos2d::Value getStageMapByCoord(int sid, int x, int y);
	/**
	 *	 过关
	 */
	static const std::string STAGE_CLEAR;
	/**
	 *	副本改变
	 */
	static const std::string FIELD_CHANGE;
private:
    /**
	 *	自身
	 */
	static Stage* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	坐标系
	 */
	std::vector<std::vector<cocos2d::Value>> _coordinate;
	/**
	 *	获取关卡场地
	 *	@param	sid	关卡id
	 *	@return	关卡详细数据
	 */
	cocos2d::Value getStageFields(int sid);
	/**
	 *	添加用户关卡地图
	 *	@param	sid	关卡id
	 */
	void addUserStageMap(int sid);
	/**
	 *	添加地图坐标
	 */
	void addMapCoord(int sid, int fid, unsigned x, unsigned y);
	/***************************************************副本**************************************************/
	/**
	 *	通过fid获取副本中的怪物
	 */
	cocos2d::Value getFieldMonsterByFID(int fid);
	/**
	 *	坐标类型-固定
	 */
	int COORD_TYPE_FIX = 0;
	/**
	 *	坐标类型-范围
	 */
	int COORD_TYPE_SCOPE = 1;
	/**
	 *	没有场地
	 */
	int FIELD_NONE = 0;
	/**
	*	主城id
	*/
	int FIELD_PORTAL_ID = 100101;
	/**
	 *	 关卡
	 */
	std::string TABLE_STAGE = "stage";
	/**
	 *	用户关卡
	 */
	std::string TABLE_USER_STAGE = "user_stage";
    /**
	 *	场地
	 */
    std::string TABLE_FIELD = "field";
    /**
     *  进入场地需要的道具
     */
    std::string TABLE_FIELD_KEY = "field_key";
	/**
	*	场地-怪物
	*/
	std::string TABLE_FIELD_MONSTER = "field_monster";
	/**
	*	场地-奖品
	*/
	std::string TABLE_FIELD_REWARD = "field_reward";
	/**
	 *	关卡场地数据
	 */
	std::string TABLE_STAGE_FIELD = "stage_field";
	/**
	 *	用户关卡地图
	 */
	std::string TABLE_USER_STAGE_MAP = "user_stage_map";
};

}
#endif // __DATA_STAGE_H__
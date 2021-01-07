/****************************************************************************
      
*	@desc	事件
*	@date	2015-3-18
*	@author	110101
   
*	@file	data/Task.h
******************************************************************************/
#ifndef __DATA_TASK_H__
#define __DATA_TASK_H__

#include "cocos2d.h"
#include "Item.h"
#include "Building.h"
#include "Crusade.h"
#include "Talent.h"
#include "Achievement.h"
#include "base/CCAsyncTaskPool.h"

namespace data {

class Task : public custom::Event,custom::Memories {
public:
	enum TaskType
	{
		/**
		*	什么也不做
		*/
		NONE = 110101,
		/**
		 *	自定义
		 */
		CUSTOM = 110103,
		/**
		*	事件
		*/
		TASK = 210101,
		/**
		 *	随机事件
		 */
		SUDDEN = 210201,
        /**
         *  赌博事件
         */
        GAMEBLING = 210202,
		/**
		*	按钮类型
		*/
		BUTTON = 310101,
		/**
		 *	建筑物
		 */
		BUILDING = 410201,
		BUILD = 410202,
		WORKER = 410203,
		/**
		 *	角色
		 */
		ROLE = 410301,
		/**
		*	场地类型
		*/
		FIELD_ENTER = 810101,
		FIELD_CLEAR = 810102,
		FIELD_QUIT = 810103,
		FIELD_FAILED = 810104,
        /**
        *   探索度
        */
        SCAN = 430203,
		/**
		*	道具
		*/
		ITEM = 410101,
		ITEM_PRODUCE = 410102,
		ITEM_DROP = 410103,
		ITEM_PRODUCE_CANNEL = 410104,
		/**
		 *	商店
		 */
		SHOP_SHOW = 410402,
		/**
		 *	场景
		 */
		STAGE_ENTER = 430201,//地图到达..
		STAGE_UNLOCK = 430202,//地图解锁..
		STAGE_CLEAR = 320203,//地图通关..
        /**
         *  成就
         */
        ACHIEVEMENT = 510201,
		/**
		 *	统计
		 */
		COUNTER = 510202,
        /**
         *  天赋
         */
        TALENT = 510101,
        
        /**
         * 太空巴别塔
         */
        TOWER = 810201,
        /**
         * 吃面包
         */
        EAT_BREAD = 810202,
        /**
         *  杀怪
         */
        KILL_ENEMY = 810401,
        /**
         *  埋葬
         */
        BURIED  = 410304
	};
	enum CustomTaskID{
		wormhole = 160101
	};
	enum TaskSuddenType
	{
		SUDDEN_TYPE_MIN = 1,
		SUDDEN_TYPE_MAX = 4
	};
    
    enum BtnID
    {
        kAlchemyBtn = 110100,
        kBuildBtn = 110200,
        kWarehouseBtn = 110300,
        kStoreBtn = 110400,
        kProduceBtn = 110500,
        kHeroBtn = 110600,
        kCrystalBtn = 110700,
        kSettingsBtn = 110800,
        kCollectionBtn = 110900,
        kVentureBtn = 111000,
        
        kRecruit = 110601,
        kCollect = 120100,
        kCrusade = 130100,
        
    };
	enum SuddenID
	{
		kSuddenMap = 160301,
		kSuddenSell0 = 160302,
		kSuddenSell1 = 160303,
		kSuddenLend0 = 160304,
		kSuddenLend1 = 160305,
		kSuddenJeu = 160306
	};
	/**
	 *	随机事件发生间隔
	 */
	static const int SUDDEN_INTERVAL = 100;

	/**
	*	初始化
	*/
	Task();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Task();
    /**
	*	获取实体
	*/
	static Task* getInstance();
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	//------------------常规事件------------------
	/**
	 *	检验事件
	 */
	bool baseCheck();
	//------------------自定义事件------------------
	/**
	 *	自定义事件
	 */
	bool customCheck();
	bool wormholeCheck();
	//------------------按钮事件------------------
	/**
	 *	按钮检测
	 */
	bool buttonCheck(int btn);
	//------------------建造事件------------------
	/**
	*	建造检测
	*/
	bool buildCheck(int building);
	//------------------地图事件------------------
	/**
	 *	地图到达
	 *	@param	sid	场景id
	 */
	void stageEnterCheck(int sid);
	/**
	 *	地图完成
	 *	@param	sid	场景id
	 */
	void stageClearCheck(int sid);
	//------------------场地事件------------------
	/**
	 *	场地检测
	 *	@fid	场地id
	 *	@type	类型
	 */
	bool fieldCheck(int fid, int type);
	/**
	 *	场地检测 进入，过关，退出，失败
	 */
	bool fieldEnterCheck(int fid);
	bool fieldClearCheck(int fid);
	bool fieldQuitCheck(int fid);
	bool fieldFailedCheck(int fid);
    
    /**
     *  探索度检测
     *
     */
    void scanCheck(int sid);
    
    //------------------成就事件-------------------
    bool acheivementCheck(int aid = 0);
    
    //------------------天赋事件-------------------
    bool talentCheck(int tid = 0);
	//------------------道具事件------------------
	/**
	*	道具检测
	*	@iid	道具id
	*	@type	类型
	*/
	void itemCheck(int iid);
	//------------------角色事件------------------
	/**
	 *	角色检测
	 */
	void roleCheck(int rid);
    //------------------巴别塔事件----------------
    /**
     *  巴别塔检测
     */
    void towerCheck(int round);
    bool checkTowerCondition(int tid,int towerRound);
    //------------------吃面包事件----------------
    /**
     *  吃面包检测
     */
    void eatCheck();
    //------------------杀怪事件------------------
    /**
     *  杀怪检测
     */
    void killEnemyCheck(int mid);
    //------------------赌博事件------------------
    /**
     *  赌博检测
     */
    void gamblingCheck();
    //------------------埋葬事件------------------
    /**
     *  埋葬检测
     */
    void buriedCheck(int rid);
    
	//------------------随机事件------------------
	/**
	 *	随机事件检测
	 *	@return	事件tid
	 */
	int suddenCheck();
	/**
	 *	随机事件确认
	 */
	bool suddenConfirm(int tid);
	/**
	 *	随机事件人民币
	 */
	void suddenRMB(int tid);
	/**
	 *	随机事件取消
	 */
	void suddenCancel(int tid);
    /**
	 *	获取按钮状态
	 */
	std::vector<int> getButtons();

	/**
	 *	通过tid获取事件
	 */
	cocos2d::Value getTaskByTID(int tid);
	/**
	 *	运行结果
	 *	@param	tid	事件id
	 */
	bool runResult(int tid);
    
    bool BtnIsTouched(int btid);
    
    void setBtnShowNew(int btid);
    /**
	 *	是否用户事件存在
	 */
	bool isUserTaskExist(int tid);
	/**
	 *	完成
	 */
	bool achieve(int tid);
	/**
	 *	 事件完成
	 */
	static const std::string TASK_COMPLETE;
	/**
	 *	随机事件发生
	 */
	static const std::string SUDDEN_OCCUR;
	/**
	 *	随机事件确认
	 */
	static const std::string SUDDEN_CONFIRM;
	/**
	 *	随机事件取消
	 */
	static const std::string SUDDEN_CANCEL;
private:
    /**
	 *	自身
	 */
	static Task* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	已经解锁的按钮，缓存解决数据优化
	 */
	std::vector<int> _btns;

	/**
	 *	随机事件完成
	 */
	bool suddenAchieve(int tid);
	/**
	 *	获取事件触发条件
	 *	@param	tid	事件id
	 */
	cocos2d::Value getCause(int tid);
	/**
	 *	获取事件先决条件
	 *	@param	tid	事件id
	 */
	cocos2d::Value getCondition(int tid);
	/**
	 *	获取事件结果
	 *	@param	tid	事件id
	 */
	cocos2d::Value getResult(int tid);
	/**
	 *	随机事件
	 *	@param	tid	事件id
	 */
	cocos2d::Value getSudden(int tid);
	/**
	 *	检验条件
	 */
	bool checkCondition(int tid);
	/**
	 *	事件线程完结
	 */
	void taskThreadComplete(void* param);
	/**
	 *	 事件
	 */
	std::string TABLE_TASK = "task";
	/**
	 *	事件类型
	 */
	std::string TABLE_TASK_TYPE = "task_type";
    /**
	 *	事件按钮
	 */
	std::string TABLE_TASK_BUTTON = "task_button";
	/**
	 *	事件处罚
	 */
	std::string TABLE_TASK_CAUSE = "task_cause";
	/**
	*	事件条件
	*/
	std::string TABLE_TASK_CONDITION = "task_condition";
	/**
	 *	事件结果
	 */
	std::string TABLE_TASK_RESULT = "task_result";
	/**
	 *	随机事件
	 */
	std::string TABLE_TASK_SUDDEN = "task_sudden";
	/**
	 *	事件结果
	 */
	std::string TABLE_USER_TASK = "user_task";
    
    /**
     *  按钮表
     */
    std::string TABLE_USER_BUTTON = "user_button";
    
};

}
#endif // __DATA_TASK_H__
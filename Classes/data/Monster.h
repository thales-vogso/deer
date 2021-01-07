/****************************************************************************
      
*	@desc	怪物
*	@date	2015-1-27
*	@author	110101
   
*	@file	data/Monster.h
******************************************************************************/
#ifndef __DATA_MONSTER_H__
#define __DATA_MONSTER_H__

#include "cocos2d.h"
#include "Item.h"

namespace data {

class Monster : public custom::Event, custom::Memories {
public:
    /**
	*	初始化
	*/
	Monster();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Monster();
    /**
	*	获取实体
	*/
	static Monster* getInstance();
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	获取太空巴别塔怪物
	 *	@param	round	层数
	 */
	cocos2d::Value getTowerMonster(int round);
    int _preTowerRound = -1;
    cocos2d::ValueMap _towerMonsterMap = cocos2d::ValueMapNull;
	/**
	 *	通过id获取怪物
	 */
	cocos2d::Value getMonsterByMID(int mid);
	/**
	 *	通过关卡获取怪物
	 *	@param	sid	关卡id
	 *	@return	怪物数据
	 */
	cocos2d::Value getMonsterBySID(int sid);
	/**
	 *	获取怪物掉落道具
	 */
	cocos2d::Value getMonsterDropItems(int mid);
	/**
	 *	 怪物死亡
	 */
	static const std::string MONSTER_DEAD;
    

private:
    /**
	 *	自身
	 */
	static Monster* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
    
    /**
	 *	 怪物
	 */
	std::string TABLE_MONSTER = "monster";
	/**
	 *	怪物掉落
	 */
	std::string TABLE_MONSTER_DROP = "monster_drop";
    /**
	 *	关卡出现怪物
	 */
	std::string TABLE_STAGE_MONSTER = "stage_monster";
	
};

}
#endif // __DATA_MONSTER_H__
/****************************************************************************
      
*	@desc	天赋
*	@date	2015-2-5
*	@author	110101
   
*	@file	data/Talent.h
******************************************************************************/
#ifndef __DATA_TALENT_H__
#define __DATA_TALENT_H__

#include "cocos2d.h"
#include "Item.h"

namespace data {

class Talent : public custom::Event, custom::Memories {
public:
    
    enum BuffId
    {
        kDodge = 110101,
        kPrecise = 110201,
        kDetecation = 110301,
        kResistant = 110401,
        kAttackFirst = 120101,
        kLifeEnhance = 210101,
        kAttackEnhance = 210201,
        kDefenseEnhance = 210301
    };
    
    /**
	*	初始化
	*/
	Talent();
    /**
	*	析构函数
	*/
	virtual ~Talent();
    /**
	*	获取单例
	*/
	static Talent* getInstance();
	/**
	 *	实体析构
	 */
    static void destroyInstace();
    /**
	 *	初始化
	 */
    void init();
	/**
	 *	根据id获取天赋
	 */
	cocos2d::Value getTalentByID(int id);
	/**
	 *	获取用户天赋
	 */
	cocos2d::Value getUserTalentList();
	/**
	 *	根据用户天赋获取所有增益
	 */
	cocos2d::Value getUserBuff();
	/**
	 *	获得天赋
	 *	@param	id
	 */
	void addTalent(int id);
	/**
	 *	 天赋获取
	 */
	static const std::string TALENT_RECEIVE;
private:
    /**
	 *	自身
	 */
	static Talent* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	 天赋
	 */
	std::string TABLE_TALENT= "talent";
	/**
	*	 增益
	*/
	std::string TABLE_BUFF = "buff";
	/**
	*	 天赋增益
	*/
	std::string TABLE_TALENT_BUFF = "talent_buff";
	/**
	*	 用户天赋
	*/
	std::string TABLE_USER_TALENT = "user_talent";
};

}
#endif // __DATA_TALENT_H__
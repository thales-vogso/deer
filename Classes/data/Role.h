/****************************************************************************
      
*	@desc	角色数据
*	@date	2015-1-16
*	@author	110101
   
*	@file	data/Role.h
******************************************************************************/
#ifndef __DATA_ROLE_H__
#define __DATA_ROLE_H__

#include "cocos2d.h"
#include "data/Item.h"

namespace data {

class Role : public custom::Event, custom::Memories{
public:
	enum RoleID
	{
		/**
		*	格瓦拉
		*/
		BASE_ROLE_RID = 110001
	};
	/**
	*	初始化
	*/
	Role();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Role();
    /**
	*	获取实体
	*/
	static Role* getInstance();
    static void destoyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	根据角色id角色
	 *	@return	角色信息
	 */
	cocos2d::Value getRoleByRID(int rid);
	/**
	 *	通过eid获取技能特效
	 *	@return	技能特效
	 */
	cocos2d::Value getSkillEffectByEID(int eid);
	/**
	 *	获取用户角色列表
	 *	@return	用户角色列表
	 */
	cocos2d::Value getUserRoles();
	/**
	 *	通过rid获取用户角色
	 *	@return	角色
	 */
	cocos2d::Value getUserRoleByRID(int rid);
	/**
	 *	通过rid获取用户角色数量
	 *	@return	角色数量
	 */
	int getUserRoleAmountByRID(int rid);
	/**
	 *	通过rid获取用户死亡角色数量
	 *	@return	 死亡角色数量
	 */
	int getUserRoleDeadByRID(int rid);
	/**
	 *	通过rid获取训练需要的道具和原始角色
	 *	@param	rid	最终角色
	 *	@return	需要的道具和角色
	 */
	cocos2d::Value getTrainByRID(int rid);
	/**
	 *	获取当前角色可以升级成的角色
	 *	@param	rid	当前角色id
	 *	@return	可升级的角色列表
	 */
	cocos2d::Value getRoleByOriginRID(int rid);
    /**
     *  获取酒馆
     *
     *  @param tid 酒馆id
     *
     *  @return 酒馆角色的数据
     */
    cocos2d::Value getTavern(int tid);
	/**
	 *	训练角色
	 *	@param	rid	角色id(该rid是目标角色的rid)
	 */
	cocos2d::Value train(int rid);
    /**
     *  添加角色
     *  @param rid 角色id(该rid是目标角色的rid)
     */
    void addRole(int rid);
	/**
	 *	获取死亡的角色
	 */
	cocos2d::Value getDeadRoles();
	/**
	 *	死亡
	 */
	void dead(int rid);
	/**
	 *	 复活
	 */
	bool revive(int rid);
	/**
	 *	掩埋
	 */
	bool bury(int rid);
    /**
     *  角色训练
     */
    static std::string const ROLE_TRAIN;
    /**
     *  事件名称埋葬
     */
    static std::string const BURY;
    
private:
    /**
	 *	自身
	 */
	static Role* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	获取复活需要道具
	 *	@param	lv
	 */
	cocos2d::Value getReviveItemByLevel(int lv);
	/**
	 *	类型-道具
	 */
	int TRAIN_TYPE_ITEM = 1;
	/**
	 *	类型-角色
	 */
	int TRAIN_TYPE_ROLE = 10;
	/**
	 *	 角色
	 */
	std::string TABLE_ROLE = "role";
	/**
	 *	特效
	 */
	std::string TABLE_EFFECT = "effect";
	/**
	 *	特效类型
	 */
	std::string TABLE_EFFECT_TYPE = "effect_type";
    /**
	 *	训练
	 */
    std::string TABLE_TRAIN = "train";
	/**
	 *	 用户角色
	 */
	std::string TABLE_USER_ROLE = "user_role";
    
    /**
     *  酒馆
     */
    std::string TABLE_TAVERN = "tavern";
};

}
#endif // __DATA_ROLE_H__
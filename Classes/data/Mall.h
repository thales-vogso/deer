/****************************************************************************
      
*	@desc	商城
*	@date	2015-5-18
*	@author	110101
   
*	@file	data/Mall.h
******************************************************************************/
#ifndef __DATA_MALL_H__
#define __DATA_MALL_H__

#include "cocos2d.h"
#include "Item.h"

namespace data {

class Mall : public custom::Event, custom::Memories {
public:
    /**
	*	初始化
	*/
	Mall();
    /**
	*	析构函数，要清除有指针的内容
	*/
	virtual ~Mall();
    /**
	*	获取实体
	*/
	static Mall* getInstance();
	/**
	 *	销毁实体
	 */
    static void destroyInstance();
    /**
	*	初始化
	*/
    void init();
	/**
	 *	获取商城列表
	 */
	cocos2d::Value getMallList();
	/**
	 *	通过id获取商品
	 *	@param	id	mall_id
	 */
	cocos2d::Value getMallByID(int id);
	/**
	 *	购买
	 *	@param	id	商品id
	 */
	bool buy(int id);
    
    /**
     *  mall改变
     */
    static const std::string USER_MALL_CHANGE;
private:
    /**
	 *	自身
	 */
	static Mall* _instance;
    /**
	 *	数据库
	 */
    DBSqlite* _db;
    /**
	 *	用户uid
	 */
    int _uid;
	/**
	 *	 商城
	 */
	std::string TABLE_MALL = "mall";
	/**
	 *	 用户商城商城
	 */
	std::string TABLE_USER_MALL = "user_mall";
};

}
#endif // __DATA_MALL_H__
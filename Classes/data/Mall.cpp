/****************************************************************************
      
*	@desc	商城
*	@date	2015-5-18
*	@author	110101
   
*	@file	data/Mall.cpp
******************************************************************************/

#include "data/Mall.h"

USING_NS_CC;

namespace data
{
    const std::string Mall::USER_MALL_CHANGE = "userMallChange";
    
	Mall* Mall::_instance = nullptr;

	Mall::Mall() :Event()
	{
		this->init();
	}
	Mall::~Mall()
	{
        
	}
	Mall* Mall::getInstance()
	{
		if (!_instance) {
			_instance = new Mall();
		}
		return _instance;
	}
    
	void Mall::destroyInstance(){
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Mall::init()
	{
		_db = DBConnector::getInstance()->open();
		_uid = User::getInstance()->getUID();
		std::stringstream sql;
		//更新表--
		_db->upgrade(TABLE_MALL, Table::getInstance()->getTableByName(TABLE_MALL), Table::getInstance()->getCSVByName(TABLE_MALL));//商城..
		_db->upgrade(TABLE_USER_MALL, Table::getInstance()->getTableByName(TABLE_USER_MALL));//用户购买...
	}

	Value Mall::getMallList()
	{
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_MALL << "` WHERE `parent_mall_id` = 0 ORDER BY `mall_id` ASC" << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
        {
			row = getMallByID(row.asValueMap()["mall_id"].asInt());
		}
		return all;
	}
	cocos2d::Value Mall::getMallByID(int id){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_MALL << "` WHERE `mall_id` = '" << id << "' LIMIT 1 " << comment;
		Value val = _db->getRow(sql.str());
		val.asValueMap()["complete"] = 0;
		sql.str(""); sql.clear();
		sql << "SELECT * FROM `" << TABLE_MALL << "` WHERE `parent_mall_id` = '" << id << "' ORDER BY `mall_id` ASC  " << comment;
		Value children = _db->getAll(sql.str());
		if (children.asValueVector().size() == 0){
			if (val.asValueMap()["multiple"].asInt() == 0){
				sql.str(""); sql.clear();
				sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_MALL << "` WHERE `mall_id` = '" << id << "' LIMIT 1 " << comment;
				if (_db->getOne(sql.str()).asBool()){
					val.asValueMap()["complete"] = 1;
				}
			}
		}else{
			bool complete = false;
			for (Value row : children.asValueVector()){
				sql.str(""); sql.clear();
				sql << "SELECT COUNT(*) FROM `" << TABLE_USER_MALL << "` WHERE `mall_id` = '" << row.asValueMap()["mall_id"].asInt() << "' LIMIT 1 " << comment;
				bool exist = _db->getOne(sql.str()).asBool();
				if (exist){
					complete = true;
				}else{
					sql.str(""); sql.clear();
					sql << "SELECT * FROM `" << TABLE_MALL << "` WHERE `mall_id` = '" << row.asValueMap()["mall_id"].asInt() << "' LIMIT 1 " << comment;
					val = _db->getRow(sql.str());
					complete = false;
					break;
				}
			}
			if (complete){
				val.asValueMap()["complete"] = 1;
			}
		}
		return val;
	}
	bool Mall::buy(int id)
    {
		Value mall = getMallByID(id);
		if (mall.asValueMap()["complete"].asBool())
        {
			return false;
		}
		//调用sdk
		Item::getInstance()->userItemPlus(mall.asValueMap()["iid"].asInt());
        
        
        
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		custom::LibDate d;
		sql << "INSERT INTO `" << TABLE_USER_MALL << "` (`uid`, `mall_id`, `cdate`) VALUES ( '" << _uid << "', '" << id << "', '" << d.datetime() << "') " << comment;
		bool res = _db->query(sql.str());
		delMem(id);
        if(res)
        {
            dispatch(USER_MALL_CHANGE, &mall);
        }
		
        return res;
	}
}
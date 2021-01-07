/****************************************************************************
 
*	@desc	游戏数据
*	@date	2015-1-7
*	@author	110101
   
*	@file	data/Item.cpp
******************************************************************************/

#include "Role.h"
#include "data/Achievement.h"

USING_NS_CC;

namespace data {

    std::string const Role::ROLE_TRAIN("roleTrain");
    
    std::string const Role::BURY("roleBury");

	Role* Role::_instance = nullptr;

	Role::Role() :Event()
	{
		this->init();
	}
	Role::~Role()
	{
	}
	Role* Role::getInstance()
	{
		if (!_instance) {
			_instance = new Role();
		}
		return _instance;
	}
    
    void Role::destoyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Role::init()
	{
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
		//更新表--
        _db->upgrade(TABLE_TAVERN, Table::getInstance()->getTableByName(TABLE_TAVERN), Table::getInstance()->getCSVByName(TABLE_TAVERN));//英雄酒馆..
		_db->upgrade(TABLE_ROLE, Table::getInstance()->getTableByName(TABLE_ROLE), Table::getInstance()->getCSVByName(TABLE_ROLE));//角色表..
		_db->upgrade(TABLE_TRAIN, Table::getInstance()->getTableByName(TABLE_TRAIN), Table::getInstance()->getCSVByName(TABLE_TRAIN));//训练表..
		_db->upgrade(TABLE_EFFECT, Table::getInstance()->getTableByName(TABLE_EFFECT), Table::getInstance()->getCSVByName(TABLE_EFFECT));//技能表..
		_db->upgrade(TABLE_EFFECT_TYPE, Table::getInstance()->getTableByName(TABLE_EFFECT_TYPE), Table::getInstance()->getCSVByName(TABLE_EFFECT_TYPE));//技能类型表..
		_db->upgrade(TABLE_USER_ROLE, Table::getInstance()->getTableByName(TABLE_USER_ROLE));//用户角色表..
	}

	Value Role::getRoleByRID(int rid)
	{
		Value val = getMem(rid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__); 
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_ROLE << "` WHERE rid = '" << rid << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		val.asValueMap()["revive"] = getReviveItemByLevel(val.asValueMap()["level"].asInt());
		val.asValueMap()["train"] = getTrainByRID(rid);
		val.asValueMap()["effect"] = getSkillEffectByEID(val.asValueMap()["effect"].asInt());
		setMem(rid, val);
		return val;
	}
	
	Value Role::getSkillEffectByEID(int eid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_EFFECT << "` `e`, `" << TABLE_EFFECT_TYPE << "` `t` WHERE `e`.`type` = `t`.`etid` AND `e`.`eid` = '" << eid << "' LIMIT 1 " << comment;
		Value row = _db->getRow(sql.str());
		return row;
	}
	Value Role::getUserRoles(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ROLE << "` `u`, `" << TABLE_ROLE << "` `r` WHERE `u`.`rid` = `r`.`rid` AND `u`.`amount` != 0 AND `u`.`uid` = '" << _uid << "' ORDER BY `r`.`level` DESC, `r`.`rid` DESC " << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector()){
			Value role = getRoleByRID(row.asValueMap()["rid"].asInt());
            for (auto &i : role.asValueMap())
            {
                row.asValueMap()[i.first] = i.second;
            }
		}
		return all;
	}
	Value Role::getUserRoleByRID(int rid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ROLE << "` WHERE `rid` = '" << rid << "' LIMIT 1";
        auto row = _db->getRow(sql.str());
		return row;
	}
	int Role::getUserRoleAmountByRID(int rid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_ROLE << "` WHERE `rid` = '" << rid << "' LIMIT 1 " << comment;
		return _db->getOne(sql.str()).asInt();
	}
	int Role::getUserRoleDeadByRID(int rid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `dead` FROM `" << TABLE_USER_ROLE << "` WHERE `rid` = '" << rid << "' LIMIT 1 " << comment;
		return _db->getOne(sql.str()).asInt();
	}
	cocos2d::Value Role::getTrainByRID(int rid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TRAIN << "` WHERE `rid` = '" << rid << "' ";
		Value all = _db->getAll(sql.str());
		ValueMap train;
		train["items"] = ValueVectorNull;
		for (Value row : all.asValueVector()){
			ValueMap map = row.asValueMap();
			if (map["type"].asInt() == TRAIN_TYPE_ROLE) train["role"] = getRoleByRID(map["origin_id"].asInt());
			if (map["type"].asInt() == TRAIN_TYPE_ITEM){
				Value item = data::Item::getInstance()->getItemByIID(map["origin_id"].asInt());
				item.asValueMap()["amount"] = map["amount"].asInt();
				train["items"].asValueVector().push_back(item);
			}
		}
		return Value(train);
	}
	cocos2d::Value Role::getRoleByOriginRID(int rid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `rid` FROM `" << TABLE_TRAIN << "` WHERE `type`='" << TRAIN_TYPE_ROLE << "' AND `origin_id` = '" << rid << "' ORDER BY `rid` ASC " << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector()){
			Value role = getRoleByRID(row.asValueMap()["rid"].asInt());
			row = role;
		}
		return all;
	}
    
    cocos2d::Value Role::getTavern(int fid)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
		sql << "SELECT rid FROM " << TABLE_TAVERN << " WHERE fid = " << fid << " ORDER BY `rid` ASC " << comment;
        Value all = _db->getAll(sql.str());
		for (auto &i : all.asValueVector())
        {
			Value role = getRoleByRID(i.asValueMap()["rid"].asInt());
			i = role;
        }
		return all;
    }
    
	Value Role::train(int rid){
		ValueMap ret;
		ret["code"] = 0;
		Value role = getRoleByRID(rid);
		
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		int originRoleID = 0;
		if (!role.asValueMap()["train"].asValueMap()["role"].isNull()){
			originRoleID = role.asValueMap()["train"].asValueMap()["role"].asValueMap()["rid"].asInt();
			int amount = getUserRoleAmountByRID(originRoleID);
			if (amount == 0){
				ret["code"] = 1;
				role.asValueMap()["train"].asValueMap()["role"].asValueMap()["lack"] = 1;
				ret["data"] = role;
				return Value(ret);
			}
		}
		for (Value &row : role.asValueMap()["train"].asValueMap()["items"].asValueVector())
		{
			int amount = Item::getInstance()->getUserItemAmountByIID(row.asValueMap()["iid"].asInt()) - row.asValueMap()["amount"].asInt();
			if (amount < 0)
			{
				ret["code"] = 1;
				row.asValueMap()["lack"] = amount;
			}
			else
			{
				amount = row.asValueMap()["amount"].asInt();
			}
			//ret["data"].asValueMap()[row.asValueMap()["iid"].asString()] = amount;
		}
		ret["data"] = role;
		if (ret["code"].asInt() == 0){
			for (Value &row : role.asValueMap()["train"].asValueMap()["items"].asValueVector())
			{
				data::Item::getInstance()->userItemReduce(row.asValueMap()["iid"].asInt(), row.asValueMap()["amount"].asInt());
			}
			if (originRoleID != 0){
				sql.clear();
				sql.str("");
				sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `amount`=`amount`-1 WHERE `uid`='" << _uid << "' AND `rid` = '" << originRoleID << "'";
				_db->query(sql.str());
			}
			sql.clear();
			sql.str("");
			sql << "SELECT COUNT(*) FROM `" << TABLE_USER_ROLE << "` WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "' LIMIT 1";
			bool exist = _db->getOne(sql.str()).asBool();
			sql.clear();
			sql.str("");
			custom::LibDate d;
			if (exist){
				sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `amount`=`amount`+ 1 , `mdate`='" << d.datetime() << "' WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "'";
			}
			else{
				sql << "REPLACE INTO `" << TABLE_USER_ROLE << "` VALUES ('" << _uid << "', '" << rid << "', '" << 1 << "', '0', '" << d.datetime() << "', '" << d.datetime() << "')";
			}
			bool res = _db->query(sql.str());
			if (res)
			{
				ValueMap eventData(ret);
				eventData["rid"] = Value(rid);
				eventData = role.asValueMap();
				eventData["isnew"] = Value(!exist);
				Crusade::getInstance()->carriageOut(originRoleID);
				this->dispatch(ROLE_TRAIN, &eventData);
				Task::getInstance()->roleCheck(rid);
			}
		}
		return Value(ret);
	}
    void Role::addRole(int rid)
    {
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        
        sql.clear();
        sql.str("");
        sql << "SELECT COUNT(*) FROM `" << TABLE_USER_ROLE << "` WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "' LIMIT 1";
        bool exist = _db->getOne(sql.str()).asBool();
        sql.clear();
        sql.str("");
        custom::LibDate d;
        if (exist){
            sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `amount`=`amount`+ 1 , `mdate`='" << d.datetime() << "' WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "'";
        }
        else{
            sql << "REPLACE INTO `" << TABLE_USER_ROLE << "` VALUES ('" << _uid << "', '" << rid << "', '" << 1 << "', '0', '" << d.datetime() << "', '" << d.datetime() << "')";
        }
        _db->query(sql.str());
    }
	Value Role::getDeadRoles(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ROLE << "` WHERE `dead` != 0 AND `uid` = '" << _uid << "' ORDER BY `rid` ASC " << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector()){
			Value role = getRoleByRID(row.asValueMap()["rid"].asInt());
			row.asValueMap()["role"] = role;
		}
		return all;
	}
	void Role::dead(int rid){
		std::stringstream sql;
		int amount = getUserRoleAmountByRID(rid);
		if (amount > 0){
			char comment[200];
			sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
			sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `amount` = `amount` - 1,`dead` = `dead` + 1 WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "' " << comment;
			bool res = _db->query(sql.str());
		}
	}
	bool Role::revive(int rid){
		std::stringstream sql;
		int amount = getUserRoleDeadByRID(rid);
		if (amount > 0){
			Value role = getRoleByRID(rid);
			int reviveItemAmount = role.asValueMap()["revive"].asValueMap()["amount"].asInt();
			bool res = Item::getInstance()->userItemReduce(Item::REVIVE, reviveItemAmount);
			if (!res) return false;
			char comment[200];
			sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
			sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `amount` = `amount` + 1,`dead` = `dead` - 1 WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "'" << comment;
			res = _db->query(sql.str());
			if (!res) return false;
			return true;
		}
		return false;
	}
	bool Role::bury(int rid){
		std::stringstream sql;
		int amount = getUserRoleDeadByRID(rid);
		if (amount > 0){
			char comment[200];
			sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
			sql << "UPDATE `" << TABLE_USER_ROLE << "` SET `dead` = `dead` - 1 WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "'" << comment;
			bool res = _db->query(sql.str());
			if (!res) return false;
            Item::getInstance()->userItemPlus(Item::BURIED);
			Value role = getRoleByRID(rid);
			this->dispatch(Role::BURY, &role);
            Task::getInstance()->buriedCheck(rid);
			return true;
		}
		return false;
	}
	Value Role::getReviveItemByLevel(int lv){
		Value item = Item::getInstance()->getItemByIID(Item::REVIVE);
		int amount = 0;
		switch (lv)
		{
		case 1:
			amount = 5;
			break;
		case 2:
			amount = 20;
			break;
		case 3:
			amount = 50;
			break;
		case 4:
			amount = 100;
			break;
		case 5:
			amount = 150;
			break;
		case 6:
			amount = 200;
			break;
		default:
			break;
		}
		item.asValueMap()["amount"] = amount;
		return item;
	}
	

}
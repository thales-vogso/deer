/****************************************************************************
 
*	@desc	道具数据
*	@date	2015-1-7
*	@author	110101
   
*	@file	data/Item.cpp
******************************************************************************/

#include "Item.h"
#include "utils/DeerUtils.h"
#include "data/Task.h"

USING_NS_CC;

namespace data {
	const std::string Item::USER_ITEM_CHANGE = "userItemChange";
    const std::string Item::PRODUCE = "produceEvent";
    
	Item* Item::_instance = nullptr;


	Item::Item() :Event()
	{
		this->init();
	}
	Item::~Item()
	{
	}
	Item* Item::getInstance()
	{
		if (!_instance) {
			_instance = new Item();
		}
		return _instance;
	}
    void Item::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Item::init()
	{
		bool res = false;
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		_db = DBConnector::getInstance()->open();
        
		//更新表--
		_db->upgrade(TABLE_ITEM, Table::getInstance()->getTableByName(TABLE_ITEM), Table::getInstance()->getCSVByName(TABLE_ITEM));//道具及其数据..
		_db->upgrade(TABLE_USER_ITEM, Table::getInstance()->getTableByName(TABLE_USER_ITEM));//用户道具..
		_db->upgrade(TABLE_PRODUCE, Table::getInstance()->getTableByName(TABLE_PRODUCE), Table::getInstance()->getCSVByName(TABLE_PRODUCE));//生产及其数据..
		_db->upgrade(TABLE_USER_PRODUCT, Table::getInstance()->getTableByName(TABLE_USER_PRODUCT));//用户生产..
		_db->upgrade(TABLE_SHOP, Table::getInstance()->getTableByName(TABLE_SHOP), Table::getInstance()->getCSVByName(TABLE_SHOP));//商店及其数据..
		_db->upgrade(TABLE_USER_SHOP, Table::getInstance()->getTableByName(TABLE_USER_SHOP));//用户商店..
		//配置..
		_db->upgrade(TABLE_GATHER, Table::getInstance()->getTableByName(TABLE_GATHER), Table::getInstance()->getCSVByName(TABLE_GATHER));//采集表..
		_db->upgrade(TABLE_MINT, Table::getInstance()->getTableByName(TABLE_MINT), Table::getInstance()->getCSVByName(TABLE_MINT));//炼金表..
		_db->upgrade(TABLE_PACKAGE, Table::getInstance()->getTableByName(TABLE_PACKAGE), Table::getInstance()->getCSVByName(TABLE_PACKAGE));//背包表..
		_db->upgrade(TABLE_CARRIAGE, Table::getInstance()->getTableByName(TABLE_CARRIAGE), Table::getInstance()->getCSVByName(TABLE_CARRIAGE));//马车表..
		_db->upgrade(TABLE_FACTORY, Table::getInstance()->getTableByName(TABLE_FACTORY), Table::getInstance()->getCSVByName(TABLE_FACTORY));//自动工厂表..
		_db->upgrade(TABLE_DRUG, Table::getInstance()->getTableByName(TABLE_DRUG), Table::getInstance()->getCSVByName(TABLE_DRUG));//药剂表..
        
        /**
         *  添加商店中的起始可见商品,这里希望有一个初始化表，这样就可以从后台导入，起始商品
         */
        if (data::User::getInstance()->getFirstPlay()) /* 判断是否是第一次启动 */
        {
            stringstream sql;
            sql << "SELECT * FROM `" << TABLE_SHOP << "` WHERE `fid` != " << MANOR_SHOP_ID;
            auto all = _db->getAll(sql.str());
            for (auto &i : all.asValueVector())
            {
                shopItemShow(i.asValueMap()["fid"].asInt(), i.asValueMap()["iid"].asInt());
            }
        }
	}

	Value Item::getItemByIID(int iid)
	{
		Value val = getMem(iid);
		if (!val.isNull()) return val;
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_ITEM << "` WHERE iid = '" << iid << "' LIMIT 1";
		val = _db->getRow(sql.str());
		Value produce = getProduceByIID(iid);
		if (produce.asValueVector().size() > 0) val.asValueMap()["produce"] = produce;
        
		setMem(iid, val);
		return val;
	}
	Value Item::getUserItems()
	{
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_ITEM << "` `i` WHERE `u`.`iid` = `i`.`iid` AND `u`.`uid` = '" << _uid << "' AND `u`.`amount` > 0 " << comment;
		ValueVector vec(_db->getAll(sql.str()).asValueVector());
		return Value(vec);
	}
	Value Item::getResouces(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_ITEM << "` `i` WHERE `u`.`iid` = `i`.`iid` AND `i`.`space` >= 0 AND `u`.`uid` = '" << _uid << "' AND `u`.`amount` > 0 ORDER BY `iid` ASC " << comment;
		Value all = _db->getAll(sql.str());
		return all;
	}
	void Item::userItemPlus(int iid, int amount){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		int num = getUserItemAmountByIID(iid);
		if (num == 0)
        {
			sql << "REPLACE INTO `" << TABLE_USER_ITEM << "` (`uid`,`iid`,`amount`) VALUES ('" << _uid << "', '" << iid << "', '" << amount << "')" << comment;
		}
        else
        {
			sql << "UPDATE `" << TABLE_USER_ITEM << "` SET `amount` = `amount` + '" << amount << "' WHERE `iid`='" << iid << "' AND `uid`='" << _uid << "'" << comment;
		}
		int res = _db->query(sql.str());
		Task::getInstance()->itemCheck(iid);
		Value item = getItemByIID(iid);
        item.asValueMap()["total"] = getUserItemAmountByIID(iid);
		item.asValueMap()["amount"] = amount;
        if(res)
        {
            dispatch(Item::USER_ITEM_CHANGE, &item);
        }
	}
    
	bool Item::userItemReduce(int iid, int amount){
		std::stringstream sql;
		amount = abs(amount);
		int num = getUserItemAmountByIID(iid);
		if (amount > num) return false;
		sql << "UPDATE `" << TABLE_USER_ITEM << "` SET `amount` = `amount` - '" << amount << "' WHERE `iid`='" << iid << "' AND `uid`='" << _uid << "'";
		int res = _db->query(sql.str());
		Value item = getItemByIID(iid);
		item.asValueMap()["amount"] = -amount;
        item.asValueMap()["total"] = getUserItemAmountByIID(iid);
        if(res)
        {
            dispatch(Item::USER_ITEM_CHANGE, &item);
        }
		return true;
	}
	Value Item::getUserItemByIID(int iid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ITEM << "` WHERE uid = '" << _uid << "' AND iid = '" << iid << "' LIMIT 1";
		return _db->getRow(sql.str());
	}
	int Item::getUserItemAmountByIID(int iid){
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_ITEM << "` WHERE uid = '" << _uid << "' AND iid = '" << iid << "' LIMIT 1";
		return _db->getOne(sql.str()).asInt();
	}
	Value Item::getUserProducts()
    {
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_PRODUCT << "` WHERE uid = '" << _uid << "' ";
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value item = getItemByIID(row.asValueMap()["iid"].asInt());
            if (row.asValueMap()["mdate"].asString() == TIME_NULL) row.asValueMap()["new"] = true;
			for (Value& i : item.asValueMap()["produce"].asValueVector()){
                auto material = getItemByIID(i.asValueMap()["material_iid"].asInt());
                for (auto &j : material.asValueMap())
                {
                    i.asValueMap()[j.first] = j.second;
                }
				i.asValueMap()["shortage"] = i.asValueMap()["amount"].asInt() > Item::getInstance()->getUserItemAmountByIID(i.asValueMap()["iid"].asInt());
			}
            for (auto &i : item.asValueMap())
            {
                row.asValueMap()[i.first] = i.second;
            }
		}
		return Value(all);
	}
	void Item::addUserProduct(int iid){
        
        Task::getInstance()->setBtnShowNew(Task::BtnID::kProduceBtn);
        
		std::stringstream sql;
		sql << "REPLACE INTO `" << TABLE_USER_PRODUCT << "` (uid, iid, cdate) VALUES ('" << _uid << "', '" << iid << "', '" << data::User::getInstance()->getTimeDate() << "')";
		_db->query(sql.str());
	}
	void Item::delUserProduct(int iid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "DELETE FROM `" << TABLE_USER_PRODUCT << "` WHERE `uid` = '" << _uid << "' AND `iid` = '" << iid << "' " << comment;
        _db->query(sql.str());
	}
	void Item::userProductInitial(int iid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE " << TABLE_USER_PRODUCT << " SET mdate = '0000-00-00 00:00:00' WHERE iid = " << iid << comment;
		_db->query(sql.str());
	}
	void Item::userProductVisited(int iid){
		Value item = getItemByIID(iid);
		if (item.asValueMap()["userProductMtime"].asString().size() > 0 && item.asValueMap()["userProductMtime"].asString() != "0000-00-00 00:00:00") return;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		custom::LibDate d(0);
		sql << "UPDATE " << TABLE_USER_PRODUCT << " SET mdate = '" << d.datetime() << "' WHERE iid = " << iid << comment;
		_db->query(sql.str());
		item.asValueMap()["userProductMtime"] = d.datetime();
	}
	cocos2d::Value Item::getProduceByIID(int iid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_PRODUCE << "` WHERE `produce_iid` = '" << iid << "' ";
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector()){
			Value item = getItemByIID(row.asValueMap()["material_iid"].asInt());
            auto temp = item.asValueMap();
		}
		return all;
	}
	Value Item::produce(int iid, int num)
    {
		userProductVisited(iid);
        
		Value item = getItemByIID(iid);
		int code = 0;
		ValueMap data;
		for (Value material : item.asValueMap()["produce"].asValueVector())
		{
			int total = getUserItemAmountByIID(material.asValueMap()["material_iid"].asInt());
			int current = material.asValueMap()["amount"].asInt();
			bool flag = true;
			if (total >= current){
				int n = total / current;
				if (n < num) num = n;
				flag = false;
			}
			if (flag)
            {
				code = 1;
                auto temp = getItemByIID(material.asValueMap()["material_iid"].asInt());
                for (auto &i : temp.asValueMap())
                {
                    material.asValueMap()[i.first] = i.second;
                }
                data[material.asValueMap()["material_iid"].asString()] = material;
            }
		}
		if (code == 0)
        {
			userItemPlus(iid, num);
            
			for (Value material : item.asValueMap()["produce"].asValueVector())
			{
				userItemReduce(material.asValueMap()["material_iid"].asInt(), material.asValueMap()["amount"].asInt() * num);
			}
			produceCheck(iid);
		}
		ValueMap ret;
		ret["code"] = code;
		ret["data"] = data;
        ret["num"]  = num;
        ValueMap eventData = ret;
        eventData["name"] = item.asValueMap()["name"];
        dispatch(data::Item::PRODUCE, &eventData);
        
		return Value(ret);
	}
	Value Item::produce(int iid){
		return produce(iid, 1);
	}
	bool Item::isProduceDisposable(int iid){
		Value item = getItemByIID(iid);
		bool res = item.asValueMap()["space"].asInt() == -1;
        if(res)
        {
            delUserProduct(iid);
        }
		return res;
	}
	void Item::produceCheck(int iid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_GATHER << "` WHERE `tool_iid` = '" << iid << "' LIMIT 1 " << comment;
		bool exist = _db->getOne(sql.str()).asBool();
		if (exist){
			delUserProduct(iid);
			sql.str(""); sql.clear();
			int iid2 = iid + 1;
			sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_GATHER << "` WHERE `tool_iid` = '" << iid2 << "' LIMIT 1 " << comment;
			exist = _db->getOne(sql.str()).asBool();
			if (exist){
				addUserProduct(iid2);
			}
			return;
		}
		std::vector<std::string> vec;
		vec.push_back(TABLE_MINT);
		vec.push_back(TABLE_CARRIAGE);
		vec.push_back(TABLE_PACKAGE);
		for (std::string table : vec)
		{
			sql.str(""); sql.clear();
			sql << "SELECT COUNT(*) AS `num` FROM `" << table << "` WHERE `iid` = '" << iid << "' LIMIT 1 " << comment;
			bool exist = _db->getOne(sql.str()).asBool();
			if (exist){
				delUserProduct(iid);
				sql.str(""); sql.clear();
				int iid2 = iid + 1;
				sql << "SELECT COUNT(*) AS `num` FROM `" << table << "` WHERE `iid` = '" << iid2 << "' LIMIT 1 " << comment;
				exist = _db->getOne(sql.str()).asBool();
				if (exist){
					addUserProduct(iid2);
				}
				return;
			}
		}
	}
	bool Item::mint(){
		int amount = getMint();
		userItemPlus(ItemID::COIN, amount);
		Achievement::getInstance()->count(Achievement::MINT);
		return true;
	}
	Value Item::getUserGather(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `g`.`tool_iid` AS `tool_iid`, `g`.`gather_iid` AS `iid`, `g`.`amount` AS `amount` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_GATHER << "` `g` WHERE `u`.`iid` = `g`.`tool_iid` ORDER BY `g`.`tool_iid` DESC " << comment;
		Value all = _db->getAll(sql.str());
		if (all.asValueVector().size() == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `tool_iid`, `gather_iid` AS `iid`, `amount` FROM `" << TABLE_GATHER << "` WHERE `tool_iid` = 0 " << comment;
			all = _db->getAll(sql.str());
		}
		int toolIID = -1;
		ValueVector vec = ValueVectorNull;
		for (Value row : all.asValueVector())
		{
			if (toolIID != -1 && row.asValueMap()["tool_iid"].asInt() != toolIID){
				break;
			}
			toolIID = row.asValueMap()["tool_iid"].asInt();
			Value item = getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["amount"] = row.asValueMap()["amount"].asInt();
			vec.push_back(item);
		}
		return Value(vec);
	}
	int Item::getValidOutGainTime(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `f`.`time` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_FACTORY << "` `f` WHERE `u`.`iid` = `f`.`iid` ORDER BY `f`.`iid` DESC LIMIT 1";
		int time = _db->getOne(sql.str()).asInt();
		if (time == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `time` FROM `" << TABLE_FACTORY << "` WHERE `iid` = 0 LIMIT 1";
			time = _db->getOne(sql.str()).asInt();
		}
		return time;
	}
	int Item::getAlchemy(){
		std::stringstream sql;
		sql << "SELECT `p`.`amount` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_MINT << "` `p` WHERE `u`.`iid` = `p`.`iid` ORDER BY `p`.`iid` DESC LIMIT 1";
		int amount = _db->getOne(sql.str()).asInt();
		if (amount == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `amount` FROM `" << TABLE_MINT << "` WHERE `iid` = 0 LIMIT 1";
			amount = _db->getOne(sql.str()).asInt();
		}
		return amount;
	}
	int Item::getMint(){
		return getAlchemy();
	}
	int Item::getPackage(){
		std::stringstream sql;
		sql << "SELECT `p`.`amount` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_PACKAGE << "` `p` WHERE `u`.`iid` = `p`.`iid` ORDER BY p.`iid` DESC LIMIT 1";
		int amount = _db->getOne(sql.str()).asInt();
		if (amount == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `amount` FROM `" << TABLE_PACKAGE << "` WHERE `iid` = 0 LIMIT 1";
			amount = _db->getOne(sql.str()).asInt();
		}
		return amount;
	}
	int Item::getCarriage(){
		std::stringstream sql;
		sql << "SELECT `c`.`amount` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_CARRIAGE << "` `c` WHERE `u`.`iid` = `c`.`iid` ORDER BY c.`iid` DESC LIMIT 1";
		int amount = _db->getOne(sql.str()).asInt();
		if (amount == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `amount` FROM `" << TABLE_CARRIAGE << "` WHERE `iid` = 0 LIMIT 1";
			amount = _db->getOne(sql.str()).asInt();
		}
		return amount;
	}
	int Item::getDrug(){
		std::stringstream sql;
		sql << "SELECT `d`.`amount` FROM `" << TABLE_USER_ITEM << "` `u`, `" << TABLE_DRUG << "` `d` WHERE `u`.`iid` = `d`.`iid` ORDER BY `d`.`iid` DESC LIMIT 1";
		int amount = _db->getOne(sql.str()).asInt();
		if (amount == 0){
			sql.clear();
			sql.str("");
			sql << "SELECT `amount` FROM `" << TABLE_DRUG << "` WHERE `iid` = 0 LIMIT 1";
			amount = _db->getOne(sql.str()).asInt();
		}
		return amount;
	}
	int Item::getGold(){
		return getUserItemAmountByIID(ItemID::COIN);
	}
	int Item::getPoint(){
		return getUserItemAmountByIID(ItemID::POINT);
	}
	Value Item::getShopByFID(int sid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
        sql << "SELECT * FROM `" << TABLE_SHOP << "` `s`,`" << TABLE_USER_SHOP << "` `u` WHERE `s`.`fid` = '" << sid << "' AND `s`.`iid`=`u`.`iid` "<< " AND `u`.`fid`='"<<sid<<"'"<< comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value price = row.asValueMap()["price"];
			Value item = getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["price"] = price;
            item.asValueMap()["rmb"]   = row.asValueMap()["rmb"];
            
            //判断价格是否足够..
            item.asValueMap()["shortage"] = price.asInt() > getUserItemAmountByIID(ItemID::COIN);
            
            //判断用户是否拥有..
            if (row.asValueMap()["mdate"].asString() == TIME_NULL)
            {
                item.asValueMap()["new"] = true;
            }
            else
            {
                item.asValueMap()["new"] = false;
            }
            
			row = item;
		}
		return all;
	}

	
	Value Item::buy(int fid, int iid, int amount)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_SHOP << "` WHERE `fid` = '" << fid << "' AND `iid` = '" << iid << "' ";
		Value row = _db->getRow(sql.str());
		if (row.isNull()) return Value();
        
        /**
         *  这一段添加mdate的记录
         */
        sql.str("");
		sql << "UPDATE " << TABLE_USER_SHOP << " SET `mdate` = '" << data::User::getInstance()->getTimeDate() << "' WHERE `iid` = " << iid << comment;
        _db->query(sql.str());
        
		int gold = getGold();
		int unitPrice = row.asValueMap()["price"].asInt();
		int num = gold / unitPrice;
		if (num == 0) return Value();
		if (num < amount){
			amount = num;
		}
		bool res = userItemReduce(ItemID::COIN, amount * unitPrice);
		if (!res) return Value();
        if (row.asValueMap()["disposable"].asInt() == 1){
            shopItemHide(fid, iid);
        }
		userItemPlus(iid, amount);
		
		return Value(amount);
	}
	Value Item::cash(int fid, int iid, int amount)
	{
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_SHOP << "` WHERE `fid` = '" << fid << "' AND `iid` = '" << iid << "' ";
		Value row = _db->getRow(sql.str());
		if (row.asValueMap()["fid"].isNull()) return Value();
		/**
		*  这一段添加mdate的记录
		*/
		sql.str("");
		sql << "UPDATE " << TABLE_USER_SHOP << " SET `mdate` = '" << data::User::getInstance()->getTimeDate() << "' WHERE `iid` = " << iid << comment;
		_db->query(sql.str());
		//支付----
		float unitPrice = row.asValueMap()["rmb"].asFloat();
		userItemPlus(iid, amount);
		if (row.asValueMap()["disposable"].asInt() == 1){
			shopItemHide(fid, iid);
		}
		return Value(amount);
	}
	int Item::sell(int iid, int amount){
		Value row = getUserItemByIID(iid);
		if (row.asValueMap()["amount"].asInt() < amount) amount = row.asValueMap()["amount"].asInt();
		bool res = userItemReduce(iid, amount);
        if(res)
        {
            Value item = getItemByIID(iid);
            userItemPlus(ItemID::COIN, amount * item.asValueMap()["price"].asInt());
        }
		return amount;
	}
	void Item::shopItemShow(int fid, int iid){
		std::stringstream sql;
		custom::LibDate d;
		sql << "REPLACE INTO `" << TABLE_USER_SHOP << "` (`uid`, `fid`, `iid`, `cdate`) VALUES ('" << _uid << "', '" << fid << "', '" << iid << "', '" << d.datetime() << "')";
		_db->query(sql.str());
	}
	void Item::shopItemShow(int iid){
        Task::getInstance()->setBtnShowNew(Task::BtnID::kStoreBtn);
		shopItemShow(MANOR_SHOP_ID, iid);
	}
	void Item::shopItemHide(int fid, int iid){
		std::stringstream sql;
		sql << "DELETE FROM `" << TABLE_USER_SHOP << "` WHERE `fid`='" << fid << "' AND `iid`='" << iid << "' AND  `uid`='" << _uid << "'";
		_db->query(sql.str());
	}
}
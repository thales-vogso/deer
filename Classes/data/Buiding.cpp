/****************************************************************************
 
*	@desc	游戏数据
*	@date	2015-1-7
*	@author	110101
   
*	@file	data/Item.cpp
******************************************************************************/

#include "Building.h"
#include "GlobalMacros.h"
#include "data/Task.h"
#include "utils/DeerUtils.h"
#include "data/Guid.h"

USING_NS_CC;

namespace data {
	const std::string Building::BUILD = "buildingBuild";
	const std::string Building::WORKER_CHANGE = "workerChange";
    const std::string Building::EVENT_GATHER = "eventGather";
    const std::string Building::GAIN = "userBuidingGain";

	Building* Building::_instance = nullptr;

	Building::Building() :Event()
	{
		this->init();
	}
	Building::~Building()
	{
	}
	Building* Building::getInstance()
	{
		if (!_instance) {
			_instance = new Building();
		}
		return _instance;
	}
    void Building::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Building::init()
	{
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
		//更新表--
		_db->upgrade(TABLE_BUILDING, Table::getInstance()->getTableByName(TABLE_BUILDING), Table::getInstance()->getCSVByName(TABLE_BUILDING));//建筑物表..
		_db->upgrade(TABLE_BUILD, Table::getInstance()->getTableByName(TABLE_BUILD), Table::getInstance()->getCSVByName(TABLE_BUILD));//建造表..
		_db->upgrade(TABLE_WORKER, Table::getInstance()->getTableByName(TABLE_WORKER), Table::getInstance()->getCSVByName(TABLE_WORKER));//工人表..
		_db->upgrade(TABLE_WORKER_GAIN, Table::getInstance()->getTableByName(TABLE_WORKER_GAIN), Table::getInstance()->getCSVByName(TABLE_WORKER_GAIN));//工人收益表..
		_db->upgrade(TABLE_USER_BUILD, Table::getInstance()->getTableByName(TABLE_USER_BUILD));//用户建造表..
		_db->upgrade(TABLE_USER_WORKER, Table::getInstance()->getTableByName(TABLE_USER_WORKER));//用户工人表..
		_db->upgrade(TABLE_USER_GAIN, Table::getInstance()->getTableByName(TABLE_USER_GAIN));//用户收益表..
	}

	Value Building::getBuildingByBID(int bid)
	{
		Value val = getMem(bid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_BUILDING << "` WHERE bid = '" << bid << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		val.asValueMap()["build"] = getBuildByBID(bid);
		val.asValueMap()["isSupport"] = (bid >= SUPPORT_MIN && bid <= SUPPORT_MAX);
		val.asValueMap()["workers"] = getWorkersByBID(bid);
		setMem(bid, val);
		return val;
	}
	Value Building::getBuildByBID(int bid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_BUILD << "` WHERE `bid` = '" << bid << "' " << comment;
		Value all = _db->getAll(sql.str());
		for (Value& row : all.asValueVector())
		{
			Value item = data::Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["amount"] = row.asValueMap()["amount"];
			row = item;
		}
		return all;
	}
	Value Building::getUserBuildList(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_BUILD << "` WHERE uid = '" << _uid << "' ORDER BY `bid` NOT BETWEEN '"<<BuildingID::SUPPORT_MIN<<"' AND '"<<BuildingID::SUPPORT_MAX<<"', `amount` = 1, `cdate` DESC " << comment;
		ValueVector all = _db->getAll(sql.str()).asValueVector();
		for (Value &row : all)
		{
			Value building = getBuildingByBID(row.asValueMap()["bid"].asInt());
			if (row.asValueMap()["mdate"].asString() == TIME_NULL) row.asValueMap()["new"] = true;
			if (!building.asValueMap()["isSupport"].asBool() && row.asValueMap()["amount"].asInt() >= 1)row.asValueMap()["complete"] = true;
			if (row.asValueMap()["amount"].asInt() == SUPPORT_TOTAL) row.asValueMap()["complete"] = true;
			for (Value& item : building.asValueMap()["build"].asValueVector())
            {
				item.asValueMap()["shortage"] = item.asValueMap()["amount"].asInt() > Item::getInstance()->getUserItemAmountByIID(item.asValueMap()["iid"].asInt());
			}
			for (auto &i : building.asValueMap())
			{
				row.asValueMap()[i.first] = i.second;
			}
		}
		return Value(all);
	}
    Value Building::getUserBuildByBID(int bid)
    {
        std::stringstream sql;
        if (BuildingID::SUPPORT_MIN <= bid && bid <= BuildingID::SUPPORT_MAX)
        {
            sql << "SELECT * FROM `" << TABLE_USER_BUILD << "` WHERE uid = '" << _uid << "' AND bid > "<<BuildingID::SUPPORT_MIN <<" AND bid < "<<BuildingID::SUPPORT_MAX;
        }
        else
        {
            sql << "SELECT * FROM `" << TABLE_USER_BUILD << "` WHERE uid = '" << _uid << "' AND bid ="<<bid;
        }
		
		Value row = _db->getRow(sql.str());
        
        Value building = getBuildingByBID(row.asValueMap()["bid"].asInt());
        if (!building.asValueMap()["isSupport"].asBool() && row.asValueMap()["amount"].asInt() >= 1)row.asValueMap()["complete"] = true;
        
        return row;
    }
	void Building::addUserBuild(int bid){
        
        Task::getInstance()->setBtnShowNew(Task::BtnID::kBuildBtn);
        
		std::stringstream sql;
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_BUILD << "` WHERE `uid` = '" << _uid << "' AND `bid` = '" << bid << "' LIMIT 1";
		int num = _db->getOne(sql.str()).asInt();
		if (num != 0) return;
		sql.clear();
		sql.str("");
		sql << "REPLACE INTO `" << TABLE_USER_BUILD << "` (uid, bid, amount, cdate) VALUES ('" << _uid << "', '" << bid << "', 0, '" << data::User::getInstance()->getTimeDate() << "')";
		_db->query(sql.str());
	}
	void Building::userBuildPlus(int bid){
		std::stringstream sql;
		Value building = getBuildingByBID(bid);
		if (building.asValueMap()["isSupport"].asBool())
        {
            //support跟别的建筑物有一定区别，因为他是可以建造多个的..
			std::string bidChange;
            if (bid >= SUPPORT_MIN && bid < SUPPORT_MAX)
            {
                bidChange = ",`bid`=`bid`+1";
            }
            else if (bid == SUPPORT_MAX)
            {
                int i(0);
            }
            else
            {
                CCLOG("bid > support_Max");
            }
            sql << "UPDATE `" << TABLE_USER_BUILD << "` SET `amount` = `amount` + 1 "<< bidChange<<" , `mdate` = '"<<data::User::getInstance()->getTimeDate() << "' WHERE `uid`='" << _uid << "' AND `bid` >= '" << BuildingID::SUPPORT_MIN << "' AND `bid` <= '" << BuildingID::SUPPORT_MAX << "'";
		}else
        {
			sql << "UPDATE `" << TABLE_USER_BUILD << "` SET `amount` = `amount` + 1, `mdate` = '" <<data::User::getInstance()->getTimeDate() << "' WHERE `bid`='" << bid << "' AND `uid`='" << _uid << "'";
		}
		int res = _db->query(sql.str());
        //建造成功, 检测事件...
        if (res)
        {
			if (building.asValueMap()["isSupport"].asBool())bid = SUPPORT_MIN;
			data::Task::getInstance()->buildCheck(bid);
            
            //如果建造的是民宅，则通知工人变化..
            if (building.asValueMap()["isSupport"].asBool())
            {
                this->dispatch(WORKER_CHANGE, nullptr);
            }
        }
	}
	Value Building::build(int bid)
    {
        /** 这部分是记录玩家什么时候点击过制造该道具的按钮的时间 **/
        if (getUserBuildByBID(bid).asValueMap()["mdate"].asString() == TIME_NULL)
        {
            std::stringstream sql;
            sql<<"UPDATE "<<TABLE_USER_BUILD<< " SET mdate = '"<<data::User::getInstance()->getTimeDate()<<"' WHERE bid = "<< bid;
            _db->query(sql.str());
        }
        
		ValueMap ret;
		ret["code"] = 0;
		ret["data"] = ValueMapNull;
		std::stringstream sql;
		Value building = getBuildingByBID(bid);
		if (building.isNull())
        {
			ret["code"] = 3;
			return Value(ret);
		}
		if (building.asValueMap()["amount"].asInt() != 0 && !building.asValueMap()["isSupport"].asBool())
        {
			ret["code"] = 2;
			return Value(ret);
		}
		ret["name"] = building.asValueMap()["name"];
		for (Value item : building.asValueMap()["build"].asValueVector())
		{
			int amount = data::Item::getInstance()->getUserItemAmountByIID(item.asValueMap()["iid"].asInt()) - item.asValueMap()["amount"].asInt();
			if (amount < 0)
				ret["code"] = 1;
			else
				amount = item.asValueMap()["amount"].asInt();
			ret["data"].asValueMap()[item.asValueMap()["iid"].asString()] = amount;
		}
		if (ret["code"].asInt() == 0){
			for (ValueMap::value_type pair : ret["data"].asValueMap())
			{
                data::Item::getInstance()->userItemReduce(atoi(pair.first.c_str()), pair.second.asInt());
			}
            ret["builddata"] = building;
            
            //成功更新新手引导状态..
            if(bid == 111201||bid == 110102||bid == 110103||bid ==111301||bid == 111701)
            {
                if(bid == 111201)//食品工厂建造完成..
                {
                    data::Guid::getInstance()->showGuid(data::Guid::WORKERS_USE1);
                }
                else if(bid == 110102)//第二个庇护所完成..
                {
                    data::Guid::getInstance()->showGuid(data::Guid::WORKERS_USE2);
                }
                else if(bid == 110103)//第三个庇护所完成..
                {
                    data::Guid::getInstance()->showGuid(data::Guid::WORKERS_USE3);
                }
                else if(bid == 111301)//星级军港建造完成..
                {
                    data::Guid::getInstance()->showGuid(data::Guid::SOLDIERS_BUILD);
                }
                else if(bid == 111701)//兵营建造完成..
                {
                    data::Guid::getInstance()->showGuid(data::Guid::TRAINING);
                }
                
            }
            else
            {
                data::Guid::getInstance()->updateGuidInfo(data::Guid::BUID_BUILD);
            }
            userBuildPlus(bid);
            //如果建造成功，就发送建筑物数量变换事件..
            this->dispatch(BUILD, &ret);
		}
        else
        {
            //失败更新新手引导状态
            data::Guid::getInstance()->updateGuidInfo();
        }
		return Value(ret);
	}
	int Building::getUserBuildAmountByBID(int bid){
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_BUILD << "` WHERE uid = '" << _uid << "' AND bid = '" << bid << "' LIMIT 1";
		return _db->getOne(sql.str()).asInt();
	}
	int Building::getUserWorkerAmountByWID(int wid){
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_WORKER << "` WHERE uid = '" << _uid << "' AND wid = '" << wid << "' LIMIT 1 /*building->getUserWorkerAmountByWID*/";
		return _db->getOne(sql.str()).asInt();
	}
	Value Building::getWorkersByBID(int bid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_WORKER << "` WHERE bid = '" << bid << "' /*building->getWorkerByBID*/";
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector()){
			row.asValueMap()["gain"] = getWorkerGainByWID(row.asValueMap()["wid"].asInt());
		}
		return all;
	}
	void Building::addUserWorker(int wid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_WORKER << "` WHERE `uid` = '" << _uid << "' AND `wid` = '" << wid << "' LIMIT 1" << comment;
		int num = _db->getOne(sql.str()).asInt();
		if (num != 0) return;
		sql.clear();
		sql.str("");
		sql << "REPLACE INTO `" << TABLE_USER_WORKER << "` (uid, wid, amount, cdate) VALUES ('" << _uid << "', '" << wid << "', 0, '" << data::User::getInstance()->getTimeDate() << "')" << comment;
		_db->query(sql.str());
	}
	int Building::getWorkTotal(){
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_BUILD << "` WHERE uid = '" << _uid << "' AND `bid` >= '" << BuildingID::SUPPORT_MIN << "' AND `bid` <= '" << BuildingID::SUPPORT_MAX << "' LIMIT 1";
		int num = WORKER_BUILDING_UNIT * _db->getOne(sql.str()).asInt();
		return num;
	}
	int Building::getWorkFree(){
		std::stringstream sql;
		sql << "SELECT SUM(`amount`) FROM `" << TABLE_USER_WORKER << "` WHERE uid = '" << _uid << "' LIMIT 1 /*building->getWorkFree*/";
		int num = getWorkTotal() - _db->getOne(sql.str()).asInt();
		return num;
	}
	bool Building::workerPlus(int wid, int amount){
		std::stringstream sql;
		int num = getWorkFree();
		if (num < amount){
			return false;
		}else{
			sql << "SELECT COUNT(*) AS `size` FROM `" << TABLE_USER_WORKER << "` WHERE `wid`='" << wid << "' AND `uid`='" << _uid << "'  LIMIT 1";
			bool exist = _db->getOne(sql.str()).asBool();
			sql.clear();
			sql.str("");
			std::string date = User::getInstance()->getTimeDate();
			if (exist)
				sql << "UPDATE `" << TABLE_USER_WORKER << "` SET `amount` = `amount` + '" << amount << "', `mdate` = '" << date << "' WHERE `wid`='" << wid << "' AND `uid`='" << _uid << "' /*building->workerPlus*/";
			else
				sql << "REPLACE INTO `" << TABLE_USER_WORKER << "` (`uid`, `wid`, `amount`, `mdate`, `cdate`) VALUES ('" << _uid << "','" << wid << "','" << amount << "', '" << date << "','" << date << "')  /*building->workerPlus*/";
			_db->query(sql.str());
		}
        this->dispatch(WORKER_CHANGE, nullptr);
		return true;
	}
	bool Building::workerReduce(int wid, int amount){
		std::stringstream sql;
		int num = getUserWorkerAmountByWID(wid);
		if (amount > num) return false;
		std::string date = User::getInstance()->getTimeDate();
		sql << "UPDATE `" << TABLE_USER_WORKER << "` SET `amount` = `amount` - '" << amount << "', `mdate` = '" << date << "' WHERE `wid`='" << wid << "' AND `uid`='" << _uid << "' /*building->workerReduce*/";
		_db->query(sql.str());
        this->dispatch(WORKER_CHANGE, nullptr);
		return true;
	}
	Value Building::getUserGainList(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT GROUP_CONCAT(`bid`) FROM `" << TABLE_USER_BUILD << "` WHERE `uid` = '" << _uid << "' " << comment;
		std::string builds = _db->getOne(sql.str()).asString();
		sql.str("");
		sql << "SELECT *  FROM `" << TABLE_USER_WORKER << "` `u`, `" << TABLE_WORKER << "` `w` WHERE `uid` = '" << _uid << "' AND `u`.`wid` = `w`.`wid` " << comment;
		Value all = _db->getAll(sql.str());
		ValueMap map;
		ValueVector workers = ValueVectorNull;
		for (Value &row : all.asValueVector())
		{
			Value gain = getWorkerGainByWID(row.asValueMap()["wid"].asInt());
			row.asValueMap()["gain"] = gain;
			for (Value item : gain.asValueVector()){
				std::string key = item.asValueMap()["iid"].asString();
				int amount = item.asValueMap()["amount"].asInt() * row.asValueMap()["amount"].asInt();
				if (item.asValueMap()["amount"].asInt() > 0) row.asValueMap()["key"] = key;
				if (map[key].isNull()){
					map[key] = amount;
				}else{
					map[key] = map[key].asInt() + amount;
				}
			}
			if(builds.find(row.asValueMap()["bid"].asString()) != std::string::npos) workers.push_back(row);
		}
		for (Value &worker : workers)
		{
			for (auto pair : map){
				if (worker.asValueMap()["key"].asString() == pair.first){
					Value gather = data::Item::getInstance()->getItemByIID(Value(pair.first).asInt());
					gather.asValueMap()["amount"] = pair.second.asInt();
					worker.asValueMap()["gather"] = gather;
					break;
				}
			}
		}
		return Value(workers);
	}
	Value Building::getWorkerGainByWID(int wid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_WORKER_GAIN << "` WHERE `wid` = '" << wid << "' ORDER BY `amount` ASC";
		Value all = _db->getAll(sql.str());
		for (Value& row : all.asValueVector())
		{
			Value item = data::Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["amount"] = row.asValueMap()["amount"].asInt();
			row = item;
		}
		return all;
	}
	Value Building::getUserGain(){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_GAIN << "` WHERE `uid` = '" << _uid << "' LIMIT 1";
		Value row = _db->getRow(sql.str());
		if (row.asValueMap()["uid"].isNull()){
			sql.str(""); sql.clear();
			sql << "REPLACE INTO `" << TABLE_USER_GAIN << "` (`uid`) VALUES ('" << _uid << "')";
			_db->query(sql.str());
			sql.str(""); sql.clear();
			sql << "SELECT * FROM `" << TABLE_USER_GAIN << "` WHERE `uid` = '" << _uid << "' LIMIT 1";
			Value row = _db->getRow(sql.str());
		}
		return row;
	}
	Value Building::gather(){
        ValueMap userGain = getUserGain().asValueMap();
        int t = (int)time(NULL);
        userGain["gather_expire"].isNull() ? 0 : userGain["gather_expire"].asInt();
        ValueVector gains;
        //if (t - gatherExpire > kGATHER / 1000){
        if (true){
            std::stringstream sql;
            sql << "UPDATE `" << TABLE_USER_GAIN << "` SET `gather_expire`='" << t << "' WHERE `uid` = '" << _uid << "'";
            bool res = _db->query(sql.str());
            gains = Item::getInstance()->getUserGather().asValueVector();
            for (Value &item : gains)
            {
                int iid = item.asValueMap()["iid"].asInt();
                int amount = item.asValueMap()["amount"].asInt();
                float seed = random(0.9, 1.1);
                amount = ceil((float)amount * seed);
                item.asValueMap()["amount"] = amount;
                data::Item::getInstance()->userItemPlus(iid, amount);
                item.asValueMap()["total"] = Item::getInstance()->getUserItemAmountByIID(iid);
            }
            if(res)
            {
                this->dispatch(EVENT_GATHER, &gains);
            }
            
        }
        return Value(gains);
	}
	int Building::getGainInterval(){
		int num = getUserBuildAmountByBID(BuildingID::ASSEMBLY);
		if (num > 0) return kGAIN / 2000;
		return kGAIN / 1000;
	}
	Value Building::gain()
    {
        custom::LibDate::getRunTime();
		ValueMap userGain = getUserGain().asValueMap();
		int t = (int)time(NULL);
		ValueMap gainMap;
		int diff = t - userGain["gain_expire"].asInt();
		int validGainTime = Item::getInstance()->getValidOutGainTime();
		if (diff > validGainTime) diff = validGainTime;
		int num = diff / getGainInterval();
		if (num > 0)
        {
			std::stringstream sql;
			sql << "UPDATE `" << TABLE_USER_GAIN << "` SET `gain_expire`='" << t << "' WHERE `uid` = '" << _uid << "'";
			bool res = _db->query(sql.str());
			sql.clear();
			sql.str("");
			sql << "SELECT * FROM `" << TABLE_USER_WORKER << "` WHERE `uid` = '" << _uid << "'";
			Value all = _db->getAll(sql.str());
			for (Value row : all.asValueVector()){
				Value gain = getWorkerGainByWID(row.asValueMap()["wid"].asInt());
				int quantity = row.asValueMap()["amount"].asInt() * num;
				for (Value item : gain.asValueVector()){
					int iid = item.asValueMap()["iid"].asInt();
					int amount = item.asValueMap()["amount"].asInt() * quantity;
					if (amount < 0){
						int total = Item::getInstance()->getUserItemAmountByIID(iid);
						if (total < amount){
							quantity = total / item.asValueMap()["amount"].asInt();
						}
						bool res = data::Item::getInstance()->userItemReduce(iid, quantity * item.asValueMap()["amount"].asInt());
						if (!res){
							break;
						}
					}
					else
					{
						data::Item::getInstance()->userItemPlus(iid, quantity * item.asValueMap()["amount"].asInt());
					}
					std::string key = item.asValueMap()["iid"].asString();
					if (gainMap[key].isNull()){
						item.asValueMap()["amount"] = quantity * item.asValueMap()["amount"].asInt();
						gainMap[key] = item;
					}
					else{
						int lastAmount = gainMap[key].asValueMap()["amount"].asInt();
						gainMap[key].asValueMap()["amount"] = lastAmount + quantity * item.asValueMap()["amount"].asInt();
					}
					gainMap[key].asValueMap()["total"] = Item::getInstance()->getUserItemAmountByIID(gainMap[key].asValueMap()["iid"].asInt());
				}
			}
            if(res)
            {
                this->dispatch(GAIN, &gainMap);
            }
		}
		custom::LibDate::getRunTime();
		return Value(gainMap);
	}

}
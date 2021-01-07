/*******************************************************
      
*	@desc	关卡
*	@date	2015-1-22
*	@author	110101
   
*	@file	data/Crusade.cpp
****************************************************/

#include "Crusade.h"
#include "Task.h"
#include "notice/Notice.h"
#include "Guid.h"

USING_NS_CC;


namespace data {
	const std::string Crusade::ENCOUNTER = "userEncounter";
    const std::string Crusade::NOTHING_HAPPEN   = "nothing_happen";
	const std::string Crusade::FIELD_ENTER = "fieldEnter";
	const std::string Crusade::FIELD_CLEAR = "fieldClear";
	const std::string Crusade::FOG_CLEAR = "fogClear";
	const std::string Crusade::CARRIAGE_CHANGE = "crusadeCarriageChange";
	const std::string Crusade::PACKAGE_CHANGE = "crusadePackageChange";
	const std::string Crusade::ACE = "crusadeTeamAce";
	const std::string Crusade::SHELTER_DROP = "shelterPickUp";

	Crusade* Crusade::_instance = nullptr;

	Crusade::Crusade() :Event()
	{
		this->init();
	}
	Crusade::~Crusade()
	{
	}
	Crusade* Crusade::getInstance()
	{
		if (!_instance) {
			_instance = new Crusade();
		}
		return _instance;
	}
    void Crusade::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Crusade::init()
	{
		_package.push_back(Item::FOOD);
		_package.push_back(Item::CRACKER_1);
		_package.push_back(Item::CRACKER_2);
		_package.push_back(Item::CRACKER_3);
		_package.push_back(Item::CRACKER_4);
		
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
        
		//更新表--
		_db->upgrade(TABLE_USER_CRUSADE, Table::getInstance()->getTableByName(TABLE_USER_CRUSADE));//用户出征..
		_db->upgrade(TABLE_USER_CRUSADE_DROP, Table::getInstance()->getTableByName(TABLE_USER_CRUSADE_DROP));//掉宝..
		_db->upgrade(TABLE_USER_CRUSADE_PACKAGE, Table::getInstance()->getTableByName(TABLE_USER_CRUSADE_PACKAGE));//用户出征背包..
		_db->upgrade(TABLE_USER_CRUSADE_CARRIAGE, Table::getInstance()->getTableByName(TABLE_USER_CRUSADE_CARRIAGE));//用户出征马车..
	}
	bool Crusade::depart(int sid)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		std::string date = User::getInstance()->getTimeDate();
		sql << "UPDATE `" << TABLE_USER_CRUSADE << "` SET `sid`= '" << sid << "', `depart` = 1 WHERE `uid`='" << _uid << "' " << comment;
		bool res = _db->query(sql.str());
		if (res)
        {
			packageDelivery();
			Stage::getInstance()->fieldRest(sid);
			_coordinate.clear();
			getUserCrusade();
			Value portal = Stage::getInstance()->getPortal(sid);
			move(portal.asValueMap()["x"].asInt(), portal.asValueMap()["y"].asInt(), true);
			
			return true;
		}
		return false;
	}
	bool Crusade::depart(){
		int sid = getUserCrusade().asValueMap()["sid"].asInt();
		return depart(sid);
	}
	void Crusade::townPortal()
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_CRUSADE << "` SET `depart`= '0' WHERE `uid`='" << _uid << "' " << comment;
		bool res = _db->query(sql.str());
		if (res)
        {
			packageTake();
			_sid = 0;
		}
	}
	Value Crusade::getUserCrusade(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT COUNT(*) FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1 " << comment;
		bool exist = _db->getOne(sql.str()).asBool();
		if (!exist){
			//row.asValueMap()["portal"] = true;
			std::string date = User::getInstance()->getTimeDate();
			Value stages =  Stage::getInstance()->getUserStages();
			int sid = stages.asValueVector()[0].asValueMap()["sid"].asInt();
			sql.clear(); sql.str("");
			sql << "REPLACE INTO `" << TABLE_USER_CRUSADE << "` VALUES ('" << _uid << "','" << sid << "','" << 0 << "','" << 0 << "','" << 0 << "','" << 0 << "','" << date << "','" << date << "')" << comment;
			bool res = _db->query(sql.str());
			if (!res) return Value();
		}
		sql.clear(); sql.str("");
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1 " << comment;
		Value row = _db->getRow(sql.str());
		_sid = row.asValueMap()["sid"].asInt();
		_x = row.asValueMap()["x"].asInt();
		_y = row.asValueMap()["y"].asInt();
		_food = Value(getPackageCurrent(Item::FOOD)).asFloat();
		if (_coordinate.empty()){
			_coordinate = Stage::getInstance()->getStageMap(_sid);
			_stage = Stage::getInstance()->getStageBySID(_sid).asValueMap();
		}
		return row;
	}
    Value Crusade::getUserCrusadeWithoutInit()
    {
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        sql << "SELECT COUNT(*) FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1 " << comment;
        bool exist = _db->getOne(sql.str()).asBool();
        if (!exist){
            //row.asValueMap()["portal"] = true;
            std::string date = User::getInstance()->getTimeDate();
            Value stages =  Stage::getInstance()->getUserStages();
            int sid = stages.asValueVector()[0].asValueMap()["sid"].asInt();
            sql.clear(); sql.str("");
            sql << "REPLACE INTO `" << TABLE_USER_CRUSADE << "` VALUES ('" << _uid << "','" << sid << "','" << 0 << "','" << 0 << "','" << 0 << "','" << 0 << "','" << date << "','" << date << "')" << comment;
            bool res = _db->query(sql.str());
            if (!res) return Value();
        }
        sql.clear(); sql.str("");
        sql << "SELECT * FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1 " << comment;
        Value row = _db->getRow(sql.str());
        return row;
    }
	//----------package-----------
	Value Crusade::getPackage(){
		ValueVector vec = ValueVectorNull;
		for (int iid : _package){
			int total = Item::getInstance()->getUserItemAmountByIID(iid);
			if (total == 0) continue;
			Value item = Item::getInstance()->getItemByIID(iid);
			item.asValueMap()["current"] = getPackageCurrent(iid);
			item.asValueMap()["total"] = total;
			vec.push_back(item);
		}
		return Value(vec);
	}
	Value Crusade::getPackage(bool strict)
	{
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' ";
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["current"] = row.asValueMap()["amount"].asInt();
			row = item;
		}
		return all;
	}
	int Crusade::getPackageCurrent(){
		std::stringstream sql;
		sql << "SELECT SUM(`amount`) FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' LIMIT 1 ";
		int num = _db->getOne(sql.str()).asInt();
		return num;
	}
	int Crusade::getPackageCurrent(int iid){
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' AND `iid`='" << iid << "' LIMIT 1 ";
		int num = _db->getOne(sql.str()).asInt();
		return num;
	}
	int Crusade::getPackageSpace(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' " << comment;
		Value all = _db->getAll(sql.str());
		int num = 0;
		for (Value row : all.asValueVector()){
			Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			num += row.asValueMap()["amount"].asInt() * item.asValueMap()["space"].asInt();
		}
		return num;
	}
	int Crusade::getPackageSpace(int iid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' AND `iid`='" << iid << "' LIMIT 1 " << comment;
		Value row = _db->getRow(sql.str());
		Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
		int num = row.asValueMap()["amount"].asInt() * item.asValueMap()["space"].asInt();
		return num;
	}
	int Crusade::getPackageTotal(){
		return Item::getInstance()->getPackage();
	}
	int Crusade::packageIn(int iid, int amount){
		int total = Item::getInstance()->getUserItemAmountByIID(iid);
        int current = getPackageCurrent(iid);
		if (current == total) return IN_ERROR_SHOTAGE;
        if (total < amount + current)
        {
			amount = total - current;
		}
        return packageInByPickup(iid, amount);
	}
    int Crusade::packageInByPickup(int iid, int amount)
    {
		Value item = Item::getInstance()->getItemByIID(iid);
		int itemSpace = item.asValueMap()["space"].asInt() * amount;
		if (getPackageSpace() + itemSpace > getPackageTotal())
        {
            return IN_ERROR_OVER;
        }
		
		std::stringstream sql;
		sql << "SELECT COUNT(`iid`) AS `num` FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid` = '" << _uid << "' AND  `iid` = '" << iid << "' LIMIT 1";
		bool exist = _db->getOne(sql.str()).asBool();
        sql.str("");
		if (exist)
			sql << "UPDATE `" << TABLE_USER_CRUSADE_PACKAGE << "` SET `amount`=`amount`+'" << amount << "' WHERE `uid`='" << _uid << "' AND `iid` = '" << iid << "' ";
		else
			sql << "INSERT INTO `" << TABLE_USER_CRUSADE_PACKAGE << "` (uid, iid, amount) VALUES ('" << _uid << "', '" << iid << "'," << amount << ")";
		bool res = _db->query(sql.str());
		if (res){
			Value item = Item::getInstance()->getItemByIID(iid);
			item.asValueMap()["amount"] = Item::getInstance()->getUserItemByIID(iid).asValueMap()["amount"];
			item.asValueMap()["change"] = amount;
			this->dispatch(PACKAGE_CHANGE, &item);
			if (iid == Item::FOOD){
				_food = Value(getPackageCurrent(Item::FOOD)).asFloat();
			}
			return INOUT_OK;
		}
		return INOUT_ERROR_UNKNOWN;
    }
	int Crusade::packageOut(int iid, int amount)
    {
		amount = abs(amount);
		int current = getPackageCurrent(iid);;
		if (current - amount < 0) return OUT_ERROR_SHOTAGE;
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_CRUSADE_PACKAGE << "` SET `amount`=`amount`-'" << amount << "' WHERE `uid`='" << _uid << "' AND `iid` = '" << iid << "' ";
		bool res = _db->query(sql.str());
		if (res)
        {
			Value item = Item::getInstance()->getItemByIID(iid);
			item.asValueMap()["amount"] = Item::getInstance()->getUserItemByIID(iid).asValueMap()["amount"];
			item.asValueMap()["change"] = -amount;
			this->dispatch(PACKAGE_CHANGE, &item);
			return INOUT_OK;
		}
		return INOUT_ERROR_UNKNOWN;
	}
	void Crusade::packageDelivery(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::string conditon;
		conditon = " AND `iid` IN (";
		bool first = true;
		for (int iid : _package){
			if (first){
				first = false;
			}
			else{
				conditon.append(",");
			}
			conditon.append(Value(iid).asString());
		}
		conditon.append(")");
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "'" << conditon << comment;
		Value all = _db->getAll(sql.str());
		for (Value row : all.asValueVector())
		{
			Item::getInstance()->userItemReduce(row.asValueMap()["iid"].asInt(), row.asValueMap()["amount"].asInt());
		}
	}
	void Crusade::packageTake(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::string conditon;
		conditon = " AND `iid` NOT IN (";
		bool first = true;
		for (int iid : _package){
			if (first){
				first = false;
			}
			else
            {
				conditon.append(",");
			}
			conditon.append(Value(iid).asString());
		}
		conditon.append(")");
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "' " << comment;
		Value all = _db->getAll(sql.str());
        
        int noticeIndex = 0;
        std::string notice;
		if (!all.isNull()){
			for (Value row : all.asValueVector())
			{
				Item::getInstance()->userItemPlus(row.asValueMap()["iid"].asInt(), row.asValueMap()["amount"].asInt());
                if (row.asValueMap()["amount"].asInt())
                {
                    noticeIndex++;
                    auto itemdata = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
                    notice += itemdata.asValueMap()["name"].asString() + "X" + row.asValueMap()["amount"].asString()+" ";
                    if(noticeIndex%4 == 0)
                    {
                        notice+="\n";
                    }
                }
			}
		}
        if (notice.size() > 0)
        {
            notice = "远征带回: " + notice;
			Notice::getInstance()->postBoard(notice);
        }
        
		sql.clear();
		sql.str("");
		sql << "DELETE FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "'" << conditon << comment;
		_db->query(sql.str());
	}
	Value Crusade::packageLose(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "'" << comment;
		Value all = _db->getAll(sql.str());
		ValueVector lost = ValueVectorNull;
		for (Value row : all.asValueVector())
		{
			int iid = row.asValueMap()["iid"].asInt();
			Value item = Item::getInstance()->getItemByIID(iid);
			if (item.asValueMap()["space"].asInt() == 0){
				Item::getInstance()->userItemPlus(row.asValueMap()["iid"].asInt(), row.asValueMap()["amount"].asInt());
				continue;
			}
			item.asValueMap()["amount"] = row.asValueMap()["amount"];
			lost.push_back(item);
		}
		sql.clear();
		sql.str("");
		sql << "DELETE FROM `" << TABLE_USER_CRUSADE_PACKAGE << "` WHERE `uid`='" << _uid << "'" << comment;
		_db->query(sql.str());
		return Value(lost);
	}
	//----------carriage-----------
	cocos2d::Value Crusade::getCarriage()
    {
		Value all = Role::getInstance()->getUserRoles();
		ValueVector vec = ValueVectorNull;
		for (Value row : all.asValueVector())
		{
			Value role = Role::getInstance()->getRoleByRID(row.asValueMap()["rid"].asInt());
			Value userRole = Role::getInstance()->getUserRoleByRID(row.asValueMap()["rid"].asInt());
			int current = getCarriageCurrent(row.asValueMap()["rid"].asInt());
			role.asValueMap()["current"] = current;
			int diff = userRole.asValueMap()["amount"].asInt() - current;
			if (diff < 0){
				current = userRole.asValueMap()["amount"].asInt();
				carriageOut(role.asValueMap()["rid"].asInt(), diff);
			}
			role.asValueMap()["total"] = current;
			vec.push_back(role);
		}
		return Value(vec);
	}
	cocos2d::Value Crusade::getCarriage(bool strict)
    {
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_CARRIAGE << "` WHERE `uid`='" << _uid << "' ";
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value role = Role::getInstance()->getRoleByRID(row.asValueMap()["rid"].asInt());
			role.asValueMap()["current"] = getCarriageCurrent(row.asValueMap()["rid"].asInt());
			row = role;
		}
		return all;
	}
	int Crusade::getCarriageCurrent()
    {
		std::stringstream sql;
		sql << "SELECT SUM(`amount`) FROM `" << TABLE_USER_CRUSADE_CARRIAGE << "` WHERE `uid`='" << _uid << "' LIMIT 1 ";
		int num = _db->getOne(sql.str()).asInt();
		return num;
	}
	int Crusade::getCarriageCurrent(int rid)
    {
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_CRUSADE_CARRIAGE << "` WHERE `uid`='" << _uid << "' AND `rid`='" << rid << "' LIMIT 1 ";
		int num = _db->getOne(sql.str()).asInt();
		return num;
	}
	int Crusade::getCarriageTotal()
    {
		return Item::getInstance()->getCarriage();
	}
	int Crusade::carriageIn(int rid, int amount)
    {
        /* 某种角色总量 */
		int total = Role::getInstance()->getUserRoleByRID(rid).asValueMap()["amount"].asInt();
        //总量不足提示..
        
		if (getCarriageCurrent(rid) + amount > total) return InOutRetCode::IN_ERROR_SHOTAGE;
    
        int current = getCarriageCurrent();
//        int carrCap = getCarriageTotal();
        if (current + amount > getCarriageTotal())
        {
            //溢出提示..
            return InOutRetCode::IN_ERROR_OVER;
        }
        
        /* 如果添加数量大于剩余空间就只填满剩余空间 */
//		if ( amount > remain)
//        {
//			amount = remain;
//		}
		std::stringstream sql;
		sql << "SELECT COUNT(`rid`) AS `num` FROM `" << TABLE_USER_CRUSADE_CARRIAGE << "` WHERE `uid` = '" << _uid << "' AND  `rid` = '" << rid << "' LIMIT 1";
		bool exist = _db->getOne(sql.str()).asBool();
        sql.str("");
		if (exist)
			sql << "UPDATE `" << TABLE_USER_CRUSADE_CARRIAGE << "` SET `amount`=`amount`+'" << amount << "' WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "' ";
		else
			sql << "INSERT INTO `" << TABLE_USER_CRUSADE_CARRIAGE << "` (uid, rid, amount) VALUES ('" << _uid << "', '" << rid << "'," << amount << ")";
		bool res = _db->query(sql.str());
		if (res)
        {
			Value role = Role::getInstance()->getRoleByRID(rid);
			role.asValueMap()["amount"] = Role::getInstance()->getUserRoleByRID(rid).asValueMap()["amount"];
			role.asValueMap()["change"] = amount;
			this->dispatch(CARRIAGE_CHANGE, &role);
			return INOUT_OK;
		}
		return InOutRetCode::INOUT_ERROR_UNKNOWN;
	}
	bool Crusade::carriageOut(int rid, int amount){
		amount = abs(amount);
		int current = getCarriageCurrent(rid);;
		if (current - amount < 0) return OUT_ERROR_SHOTAGE;
		if (current < amount)
        {
			amount = current;
		}
		std::stringstream sql;
		sql << "SELECT COUNT(`rid`) AS `num` FROM `" << TABLE_USER_CRUSADE_CARRIAGE << "` WHERE `uid` = '" << _uid << "' AND  `rid` = '" << rid << "' LIMIT 1";
		bool exist = _db->getOne(sql.str()).asBool();
        sql.str("");
		if (exist)
			sql << "UPDATE `" << TABLE_USER_CRUSADE_CARRIAGE << "` SET `amount`=`amount`-'" << amount << "' WHERE `uid`='" << _uid << "' AND `rid` = '" << rid << "' ";
		else
			sql << "INSERT INTO `" << TABLE_USER_CRUSADE_CARRIAGE << "` (uid, rid, amount) VALUES ('" << _uid << "', '" << rid << "'," << amount << ")";
		bool res = _db->query(sql.str());
		if (res)
        {
			Value role = Role::getInstance()->getRoleByRID(rid);
			role.asValueMap()["amount"] = Role::getInstance()->getUserRoleByRID(rid).asValueMap()["amount"];
			role.asValueMap()["change"] = -amount;
			this->dispatch(CARRIAGE_CHANGE, &role);
			return INOUT_OK;
		}
		return INOUT_ERROR_UNKNOWN;
	}
	
	void Crusade::ace(int cause)
    {
//        NotificationManager::getInstance()->postDeathNotice(cause);
        
		Value all = getCarriage(true);
		ValueVector dead = ValueVectorNull;
		for (Value row : all.asValueVector()){
			int size = row.asValueMap()["current"].asInt();
			Value role = Role::getInstance()->getRoleByRID(row.asValueMap()["rid"].asInt());
			role.asValueMap()["amount"] = size;
			dead.push_back(role);
			for (int i = 0; i < size; i++){
				Role::getInstance()->dead(row.asValueMap()["rid"].asInt());
			}
		}
        //将所有的成员清空..
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        sql<<"DELETE FROM `"<< TABLE_USER_CRUSADE_CARRIAGE<<"`"<< comment;
        bool res = _db->query(sql.str());sql.clear();sql.str("");
        //清空背包..
		Value lost = packageLose();
		Stage::getInstance()->fieldFailed(_sid, _x, _y);
		ValueMap param = ValueMapNull;
		param["items"] = lost;
		param["roles"] = dead;
		param["cause"] = cause;
        if(res)
        {
            dispatch(ACE, &param);
        }
		townPortal();
	}
	void Crusade::ace(){
		ace(ACE_CAUSE::KILLED);
	}
	void Crusade::drop(int iid, int amount){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "REPLACE INTO `" << TABLE_USER_CRUSADE_DROP << "` (`uid`, `iid`, `amount`) VALUES ('" << _uid << "', '" << iid << "', '" << amount << "')" << comment;
		_db->query(sql.str());
	}
	cocos2d::Value Crusade::getDrops(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_CRUSADE_DROP << "` WHERE `uid` = '" << _uid << "' AND `amount` > 0 " << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
        {
			Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["amount"] = row.asValueMap()["amount"];
			row = item;
		}
		return all;
	}
	void Crusade::dropsClear(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "DELETE FROM `" << TABLE_USER_CRUSADE_DROP << "` " << comment;
		_db->query(sql.str());
	}
	std::vector<std::vector<cocos2d::Value>> Crusade::getStageMap(){
		return _coordinate;
	}
	ValueMap Crusade::getStage(){
		return _stage;
	}
	Value Crusade::fieldView(){
		Value row = getUserCrusadeWithoutInit();//getUserCrusadeWithoutInit
		Value map = Stage::getInstance()->getStageMapByCoord(_sid, row.asValueMap()["x"].asInt(), row.asValueMap()["y"].asInt());
		int fid = map.asValueMap()["fid"].asInt();
		if (fid == 0) return map;
		Value field = map.asValueMap()["field"];
        Value field_key = map.asValueMap()["field_key"];
		if (!field.asValueMap()["monsters"].isNull() || fid == Stage::FieldType::TOWER){
			int round = map.asValueMap()["round"].asInt();
			map.asValueMap()["monster"] = Stage::getInstance()->getFieldMonster(fid, round + 1);
		}
		map.asValueMap()["key"] = true;
        for(auto i : field_key.asValueVector())
        {
            int key_iid = i.asValueMap()["iid"].asInt();
            int key_iid_num = i.asValueMap()["num"].asInt();
            if(getPackageCurrent(key_iid)<key_iid_num)
            {
                field.asValueMap()["key"] = false;
                break;
            }
        }
		map.asValueMap()["field"] = field;
		return map;
	}
    bool Crusade::isInTAI()
    {
        Value row = getUserCrusade();
        Value map = Stage::getInstance()->getStageMapByCoord(_sid, row.asValueMap()["x"].asInt(), row.asValueMap()["y"].asInt());
        int fid = map.asValueMap()["fid"].asInt();
        if (fid == 110701)
        {
            return true;
        }
        else{
            return false;
        }
    }
	bool Crusade::fieldEnter(Value block)
    {
		int fid = block.asValueMap()["fid"].asInt();
		Value field = Stage::getInstance()->getFieldByFID(fid);
        Value field_key = Stage::getInstance()->getFieldKeyByFID(fid);
		if (block.asValueMap()["visit"].asInt() == Stage::ALREADY_VISIT){
			return false;
		}
        if (block.asValueMap()["visit"].asInt() == Stage::NEVER_VISIT){
            for(auto i : field_key.asValueVector())
            {
                int key_iid = i.asValueMap()["iid"].asInt();
                int key_num = i.asValueMap()["num"].asInt();
                Value item = Item::getInstance()->getItemByIID(key_iid);
                int ret = packageOut(key_iid,key_num);
                if(ret != INOUT_OK)
                {
                    if (item.asValueMap()["space"].asInt() == 0){
                        bool res = Item::getInstance()->userItemReduce(key_iid,key_num);
                        if (!res) return false;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
		}
		Stage::getInstance()->fieldEnter(block.asValueMap()["id"].asInt());
		if (field.asValueMap()["monster"].asInt() > 0)
        {
			int round = block.asValueMap()["round"].asInt() + 1;
			Value monster = Stage::getInstance()->getFieldMonster(fid, round);
			monster.asValueMap()["field"] = 1;
			monster.asValueMap()["block"] = block;
			dispatch(ENCOUNTER, &monster);
		}
        else
        {
			Value items = Stage::getInstance()->getFieldReward(block.asValueMap()["fid"].asInt());
			if (items.asValueVector().size() > 0)
            {
				for (Value item : items.asValueVector())
				{
					int amount = MAX(abs(block.asValueMap()["x"].asInt() - _stage["width"].asInt() / 2) / 2, abs(block.asValueMap()["y"].asInt() - _stage["height"].asInt() / 2) / 2) + 2;
					amount *= item.asValueMap()["amount"].asInt();
					drop(item.asValueMap()["iid"].asInt(), amount);
				}
				dispatch(SHELTER_DROP, nullptr);
			}
			fieldClear(block);
		}
		return true;
	}
	bool Crusade::fieldEnter(int stageMapID)
    {
		Value block = Stage::getInstance()->getStageMapByID(stageMapID);
		return fieldEnter(block);
	}
	bool Crusade::fieldEnter(int x, int y)
    {
		Value block = Stage::getInstance()->getStageMapByCoord(_sid, x, y);
		return fieldEnter(block);
	}
	
	bool Crusade::fieldSiege(cocos2d::Value block)
    {
		bool res = Stage::getInstance()->fieldSiege(block.asValueMap()["id"].asInt());
		if (res)
        {
			fieldClear(block);
			return true;
		}
		coordExpire(block.asValueMap()["x"].asInt(), block.asValueMap()["y"].asInt());
		return false;
	}
	bool Crusade::fieldSiege(int stageMapID){
		Value block = Stage::getInstance()->getStageMapByID(stageMapID);
		return fieldSiege(block);
	}
	bool Crusade::fieldSiege(int x, int y)
    {
		Value block = Stage::getInstance()->getStageMapByCoord(_sid, x, y);
		return fieldSiege(block);
	}
	void Crusade::fieldFailed(cocos2d::Value block)
	{
		Stage::getInstance()->fieldFailed(_sid, block.asValueMap()["x"].asInt(), block.asValueMap()["y"].asInt());
		coordExpire(block.asValueMap()["x"].asInt(), block.asValueMap()["y"].asInt());
	}
	void Crusade::fieldFailed(int stageMapID){
		Value block = Stage::getInstance()->getStageMapByID(stageMapID);
		fieldFailed(block);
	}
	void Crusade::fieldFailed(int x, int y)
	{
		Value block = Stage::getInstance()->getStageMapByCoord(_sid, x, y);
		fieldFailed(block);
	}
	void Crusade::fieldClear(Value block)
    {
		Value field = Stage::getInstance()->getFieldByFID(block.asValueMap()["fid"].asInt());
		Stage::getInstance()->fieldClear(block.asValueMap()["id"].asInt());
		coordExpire(block.asValueMap()["x"].asInt(), block.asValueMap()["y"].asInt());
        //场地清除时，检测任务..
		Task::getInstance()->fieldClearCheck(field.asValueMap()["fid"].asInt());
		dispatch(FIELD_CLEAR, nullptr);
	}
	void Crusade::fieldClear(int stageMapID)
    {
		Value block = Stage::getInstance()->getStageMapByID(stageMapID);
		fieldClear(block);
	}
	void Crusade::fieldClear(int x, int y)
    {
		Value block = Stage::getInstance()->getStageMapByCoord(_sid, x, y);
		fieldClear(block);
	}
	cocos2d::Vec2 Crusade::move(int x, int y, bool direct)
    {
		std::stringstream sql;
		std::string date = User::getInstance()->getTimeDate();
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1";
		bool exist = _db->getOne(sql.str()).asBool();
		sql.clear();sql.str("");
		if (exist)
        {
			sql << "UPDATE `" << TABLE_USER_CRUSADE << "` SET `x` = '" << x << "', `y` = '" << y << "', `mdate` = '" << date << "' WHERE `uid` = '" << _uid << "' ";
		}
		else{

			sql << "REPLACE INTO `" << TABLE_USER_CRUSADE << "` VALUES ('" << _uid << "','" << _sid << "','" << x << "','" << y << "','" << 0 << "','" << date << "','" << date << "')";
		}
		bool res = _db->query(sql.str());
		_x = x;
        _y = y;
		if (res && !direct)
        {
			Value stageMap = getCoord(x, y);
			int fid = stageMap.asValueMap()["fid"].asInt();
			int visit = stageMap.asValueMap()["visit"].asInt();
            if (fid > 0 && visit != Stage::ALREADY_VISIT)
            {
				Value field = stageMap.asValueMap()["field"];
                Value field_key = stageMap.asValueMap()["field_key"];
				field.asValueMap()["visit"] = visit;
				field.asValueMap()["x"] = x;
				field.asValueMap()["y"] = y;
				field.asValueMap()["key"] = true;
				if (field_key.asValueVector().size() > 0  && visit == Stage::NEVER_VISIT){
                    for(auto i :field_key.asValueVector())
                    {
                        int key_iid = i.asValueMap()["iid"].asInt();
                        int key_iid_num = i.asValueMap()["num"].asInt();
                        Value item = Item::getInstance()->getItemByIID(key_iid);
                        int amount = getPackageCurrent(key_iid);
                        if(amount < key_iid_num)
                        {
                            if(item.asValueMap()["space"].asInt() == 0)
                            {
                                amount = Item::getInstance()->getUserItemAmountByIID(key_iid);
                                if(amount < key_iid_num)
                                {
                                    field.asValueMap()["key"] = false;
                                    break;
                                }
                            }
                        }
                    }
				}
				if (field.asValueMap()["type"].asInt() == Stage::PORTAL)
                {
					townPortal();
				}
				dispatch(FIELD_ENTER, &field);
				coordExpire(x, y);
			}
            else
            {
				if (_encounter){
					float seed = random(0.0f, 1.0f);
					Value stage = Stage::getInstance()->getStageBySID(_sid);
					float encounter = stage.asValueMap()["encounter"].asFloat() + _step * stage.asValueMap()["encounter_increase"].asFloat();
                    //在引导过程中不能碰到怪物..
                    if(data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::GUID_TASK_ID::MOVE2))
                    {
                        if (seed < encounter)
                        {
                            Value monster = Monster::getInstance()->getMonsterBySID(_sid);
                            dispatch(ENCOUNTER, &monster);
                            _step = 0.0f;
                        }
                        else
                        {
                            dispatch(NOTHING_HAPPEN, NULL);
                        }
                    }
                    else
                    {
                        dispatch(NOTHING_HAPPEN, NULL);
                    }
					
				}
                else
                {
                    dispatch(NOTHING_HAPPEN, NULL);
                }
                
				consume();
			}
			_step += 1.0f;
		}
		scan(_x, _y);
		return Vec2(_x, _y);
	}
	cocos2d::Vec2 Crusade::moveEast(){
		int size = _stage["width"].asInt() - 1;
		if (_x >= size){
			_x = size;
			return Vec2(_x, _y);
		}else{
			_x++;
			return move(_x, _y);
		}
	}
	cocos2d::Vec2 Crusade::moveSouth(){
		if (_y == 0) return Vec2(_x, _y);
		_y--;
		return move(_x, _y);
	}
    cocos2d::Vec2 Crusade::moveOrigin(){
        
        std::stringstream sql;
        std::string date = User::getInstance()->getTimeDate();
        sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_CRUSADE << "` WHERE `uid` = '" << _uid << "' LIMIT 1";
        bool exist = _db->getOne(sql.str()).asBool();
        sql.clear();sql.str("");
        if (exist)
        {
            sql << "UPDATE `" << TABLE_USER_CRUSADE << "` SET `x` = '" << _x << "', `y` = '" << _y << "', `mdate` = '" << date << "' WHERE `uid` = '" << _uid << "' ";
        }
        else{
            
            sql << "REPLACE INTO `" << TABLE_USER_CRUSADE << "` VALUES ('" << _uid << "','" << _sid << "','" << _x << "','" << _y << "','" << 0 << "','" << date << "','" << date << "')";
        }
        bool res = _db->query(sql.str());
        if (res)
        {
            Value stageMap = getCoord(_x, _y);
            int fid = stageMap.asValueMap()["fid"].asInt();
            int visit = stageMap.asValueMap()["visit"].asInt();
            if (fid > 0 && visit != Stage::ALREADY_VISIT)
            {
                Value field = stageMap.asValueMap()["field"];
                Value field_key = stageMap.asValueMap()["field_key"];
                field.asValueMap()["visit"] = visit;
                field.asValueMap()["x"] = _x;
                field.asValueMap()["y"] = _y;
                field.asValueMap()["key"] = true;
                if (field_key.asValueVector().size() > 0 && visit == Stage::NEVER_VISIT){
                    
                    for(auto i :field_key.asValueVector())
                    {
                        int key_iid = i.asValueMap()["iid"].asInt();
                        int key_iid_num = i.asValueMap()["num"].asInt();
                        Value item = Item::getInstance()->getItemByIID(key_iid);
                        int amount = getPackageCurrent(key_iid);
                        if(amount < key_iid_num)
                        {
                            if(item.asValueMap()["space"].asInt() == 0)
                            {
                                amount = Item::getInstance()->getUserItemAmountByIID(key_iid);
                                if(amount < key_iid_num)
                                {
                                    field.asValueMap()["key"] = false;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (field.asValueMap()["type"].asInt() == Stage::PORTAL)
                {
                    townPortal();
                }
                dispatch(FIELD_ENTER, &field);
                coordExpire(_x, _y);
            }
            else
            {
                if (_encounter){
                    float seed = random(0.0f, 1.0f);
                    Value stage = Stage::getInstance()->getStageBySID(_sid);
                    float encounter = stage.asValueMap()["encounter"].asFloat() + _step * stage.asValueMap()["encounter_increase"].asFloat();
                    if (seed < encounter)
                    {
                        Value monster = Monster::getInstance()->getMonsterBySID(_sid);
                        dispatch(ENCOUNTER, &monster);
                        _step = 0.0f;
                    }
                    else
                    {
                        dispatch(NOTHING_HAPPEN, NULL);
                    }
                }
                else
                {
                    dispatch(NOTHING_HAPPEN, NULL);
                }
            }
            _step += 1.0f;
        }
        scan(_x, _y);
        return Vec2(_x, _y);
    }
	cocos2d::Vec2 Crusade::moveWest(){
		if (_x == 0) return Vec2(_x, _y);
		_x--;
		return move(_x, _y);
	}
	cocos2d::Vec2 Crusade::moveNorth(){
		int size = _stage["height"].asInt() - 1;
		if (_y >= size){
			_y = size;
			return Vec2(_x, _y);
		}
		else{
			_y++;
			return move(_x, _y);
		}
	}
	void Crusade::scan(int x, int y)
    {
        int addition(0);
		if (!data::Talent::getInstance()->getUserBuff().asValueMap()[Value(Talent::kDetecation).asString()].isNull())
        {
			addition = data::Talent::getInstance()->getUserBuff().asValueMap()[Value(Talent::kDetecation).asString()].asValueMap()["value"].asInt();
        }
		int sight =  1 + addition;
		int size = 1 + sight * 2;
//		int offsetY = 0, offsetX = 0;//从中心点开始
		ValueVector vec = ValueVectorNull;
		if (Stage::getInstance()->scanMap(_sid, x, y))
        {
			coordExpire(x, y);
			vec.push_back(getCoord(x, y));
		}
//        
//		offsetX++;
//		for (int i = 1; i < size * size; i++)
//		{
//			int factX = x + offsetX;
//			int factY = y + offsetY;
//			if (Stage::getInstance()->scanMap(_sid, factX, factY))
//            {
//				coordExpire(factX, factY);
//				vec.push_back(getCoord(factX, factY));
//			}
//			if (offsetX <= - offsetY && offsetY < 0)
//			{
//				offsetX++;
//			}
//			else if (offsetX > 0 && offsetX > offsetY)
//			{
//				offsetY++;
//			}
//			else if (offsetX > -offsetY && offsetY > 0)
//			{
//				offsetX--;
//			}
//			else if (offsetX < 0 && offsetX < offsetY)
//			{
//				offsetY--;
//			}
//		}
        
        int center = (size-1)/2;
        for (int i(0); i<size; ++i)
        {
            for (int j(0); j<size; ++j)
            {
                if (i==center&&j==center) continue;
                int factX = x + i - center;
                int factY = y + j - center;
                if (Stage::getInstance()->scanMap(_sid, factX, factY))
                {
                    coordExpire(factX, factY);
                    vec.push_back(getCoord(factX, factY));
                }
            }
        }
        
		if (vec.size() > 0) dispatch(FOG_CLEAR, &vec);
	}
	void Crusade::consume()
    {
		if (!_consumeForMove) return;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        
        float factor(1);
		if (!data::Talent::getInstance()->getUserBuff().asValueMap()[Value(Talent::kResistant).asString()].isNull())
        {
			factor = data::Talent::getInstance()->getUserBuff().asValueMap()[Value(Talent::kResistant).asString()].asValueMap()["value"].asFloat();
        }
        CCLOG("_food is %f", _food);
		_food = _food - _foodUnit * factor;
        CCLOG("_food is %f", _food);
		if (_food < 0)
        {
			ace(ACE_CAUSE::STARVATION);
			return;
		}
        
		if (getPackageCurrent(data::Item::FOOD) - _food >= 1)
        {
			Item::getInstance()->userItemReduce(Item::FOOD);
            packageOut(Item::FOOD);
            _food =  Value(getPackageCurrent(Item::FOOD)).asFloat();
		}
        
		if (abs(_stage["width"].asInt() / 2 - _x) + abs(_stage["height"].asInt() / 2 - _y) > (int)_food){
			Task::getInstance()->wormholeCheck();
		}
	}
    void Crusade::consumeByEnhanceLife()
    {
		if (!_consumeForBattle) return;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        
		if (_food < 1){
			return;
		}
        else
        {

            Item::getInstance()->userItemReduce(Item::FOOD);
            packageOut(Item::FOOD);
            _food =  Value(getPackageCurrent(Item::FOOD)).asFloat();
		}
    }
	void Crusade::killMonster(cocos2d::Value monster){
		Value items = Monster::getInstance()->getMonsterDropItems(monster.asValueMap()["mid"].asInt());
		for (Value item : items.asValueVector())
		{
			drop(item.asValueMap()["iid"].asInt(), item.asValueMap()["fit"].asInt());
		}
		Value block = getCoord(_x, _y);
		Value field = Stage::getInstance()->getFieldByFID(block.asValueMap()["fid"].asInt());
		if (block.asValueMap()["round"].asInt() + 1 == field.asValueMap()["monster"].asInt()){
			Value items = Stage::getInstance()->getFieldReward(block.asValueMap()["fid"].asInt());
			for (Value item : items.asValueVector())
			{
				drop(item.asValueMap()["iid"].asInt(), item.asValueMap()["amount"].asInt());
			}
		}
        //杀怪统计..
        data::Achievement::getInstance()->count(data::Achievement::CountID::KILL_ENEMY);
        data::Task::getInstance()->killEnemyCheck(monster.asValueMap()["mid"].asInt());
	}
	void Crusade::killMonster(int mid){
		if (mid == 0)
			pushTower();
		else
			killMonster(Monster::getInstance()->getMonsterByMID(mid));
	}
	void Crusade::pushTower(){
		Value block = getCoord(_x, _y);
		int round = block.asValueMap()["round"].asInt();
		int num = 20 + round;
		drop(Item::ItemID::COIN, num);
		drop(Item::ItemID::IDENTITY);
        //data::Achievement::getInstance()->count(data::Achievement::CountID::BABIE_TOWER);
        data::Task::getInstance()->towerCheck(round);
	}
	Value Crusade::getCoord(int x, int y){
		Value &coord = _coordinate[y][x];
		if (!coord.isNull()){
			if (!coord.asValueMap()["expire"].asBool()){
				return coord;
			}
		}
		coord = Stage::getInstance()->getStageMapByCoord(_sid, x, y);
		return coord;
	}
	void Crusade::coordExpire(int x, int y){
		Value &coord = _coordinate[y][x];
		coord.asValueMap()["expire"] = true;
	}
}
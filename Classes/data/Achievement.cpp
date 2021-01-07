/****************************************************************************
      
*	@desc	成就
*	@date	2015-2-5
*	@author	110101
   
*	@file	data/Achievement.cpp
******************************************************************************/

#include "Achievement.h"
#include "data/Role.h"
#include "base/CCAsyncTaskPool.h"

USING_NS_CC;

namespace data {
	const std::string Achievement::ACHIEVEMENT_COMPLETE = "achievementComplete";

	Achievement* Achievement::_instance = nullptr;

	Achievement::Achievement()
	{
		this->init();
	}
	Achievement::~Achievement()
	{
	}
	Achievement* Achievement::getInstance()
	{
		if (!_instance) {
			_instance = new Achievement();
		}
		return _instance;
	}
    void Achievement::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Achievement::init()
	{
		_db = DBConnector::getInstance()->open();
		_uid = User::getInstance()->getUID();
		//更新表--
		_db->upgrade(TABLE_ACHIEVEMENT, Table::getInstance()->getTableByName(TABLE_ACHIEVEMENT), Table::getInstance()->getCSVByName(TABLE_ACHIEVEMENT));//成就表..
        _db->upgrade(TABLE_USER_ACHIEVEMENT, Table::getInstance()->getTableByName(TABLE_USER_ACHIEVEMENT));
		_db->upgrade(TABLE_COUNTER, Table::getInstance()->getTableByName(TABLE_COUNTER), Table::getInstance()->getCSVByName(TABLE_COUNTER));//计数器表..
		_db->upgrade(TABLE_USER_COUNTER, Table::getInstance()->getTableByName(TABLE_USER_COUNTER));
	}
	Value Achievement::getAchievementByAID(int aid){
		Value val = getMem(aid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_ACHIEVEMENT << "` WHERE `aid` = '" << aid << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		setMem(aid, val);
		return val;
	}
	Value Achievement::getUserAchievementByAID(int aid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_ACHIEVEMENT << "` WHERE `uid` = '" << _uid << "' AND `aid` = '" << aid << "' LIMIT 1 " << comment;
		return _db->getRow(sql.str());
	}
	bool Achievement::addAchievement(int aid){
		if (getUserAchievementByAID(aid).asBool()) return false;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		custom::LibDate d(NULL);
		sql << "REPLACE INTO `" << TABLE_USER_ACHIEVEMENT << "`(`uid`, `aid`, `cdate`) VALUES ('" << _uid << "', '" << aid << "', '" << d.datetime() << "') " << comment;
		bool res = _db->query(sql.str());
        //成就结果里已经加了成就点数，不需要再去兑换成就点数了..
		//if (res) exchange(aid);
		return res;
	}
	bool Achievement::exchange(int aid){
		Value row = getUserAchievementByAID(aid);
		if (row.asValueMap()["aid"].isNull()) return false;
		Value achievemnt = getAchievementByAID(aid);
		Item::getInstance()->userItemPlus(Item::ItemID::POINT, achievemnt.asValueMap()["point"].asInt());
		return true;
	}
	Value Achievement::getUserAchievements(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `a`.*,`u`.`uid` FROM `" << TABLE_ACHIEVEMENT << "` AS `a` " << " LEFT JOIN `" << TABLE_USER_ACHIEVEMENT << "` AS `u` " << " ON `a`.`aid` = `u`.`aid` " << " ORDER BY `aid` ASC " << comment;
		Value all = _db->getAll(sql.str());
		ValueMapIntKey counter = ValueMapIntKeyNull;
		for (Value &row : all.asValueVector())
		{
			Value achievement = getAchievementByAID(row.asValueMap()["aid"].asInt());
			achievement.asValueMap()["complete"] = row.asValueMap()["uid"].asInt() > 0? true:false;
            if(achievement.asValueMap()["complete"].asBool())
            {
                CCLOG("就是这么牛逼..");
            }
			int cid = achievement.asValueMap()["cid"].asInt();
			if (cid > 0){
				if (counter[cid].isNull()){
					counter[cid] = getCounterAmountByCid(cid);
				}
				achievement.asValueMap()["isCounter"] = true;
				achievement.asValueMap()["counter"] = counter[cid];
				achievement.asValueMap()["total"] = achievement.asValueMap()["condition"];
			}
			row = achievement;
		}
		return all;
	}
	void Achievement::count(int cid, int amount){
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			[](void*){},
			nullptr,
			[=](){
				char comment[200];
				sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
				std::stringstream sql;
				sql << "SELECT COUNT(*) FROM `" << TABLE_USER_COUNTER << "` WHERE `uid` = '" << _uid << "' AND `cid` = '" << cid << "' LIMIT 1 " << comment;
				bool res = _db->getOne(sql.str()).asBool();
				custom::LibDate d;
				sql.str(""); sql.clear();
				if (res) {
					sql << "UPDATE `" << TABLE_USER_COUNTER << "` SET `amount` = `amount` + '" << amount << "', `mdate` = '" << d.datetime() << "' WHERE `uid` = '" << _uid << "' AND `cid` = '" << cid << "' " << comment;
				}
				else{
					sql << "REPLACE INTO `" << TABLE_USER_COUNTER << "` (`uid`, `cid`, `amount`, `mdate`, `cdate`) VALUES ('" << _uid << "', '" << cid << "', '" << amount << "', '0000-00-00 00:00:00', '" << d.datetime() << "') " << comment;
				}
				res = _db->query(sql.str());
			}
		);

	}
	int Achievement::getCounterAmountByCid(int cid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `amount` FROM `" << TABLE_USER_COUNTER << "` WHERE `uid` = '" << _uid << "' AND `cid` = '" << cid << "' LIMIT 1 " << comment;
		return _db->getOne(sql.str()).asInt();
	}

}
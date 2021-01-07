/****************************************************************************
      
*	@desc	天赋
*	@date	2015-2-5
*	@author	110101
   
*	@file	data/Talent.cpp
******************************************************************************/

#include "Talent.h"

USING_NS_CC;

namespace data {
	const std::string Talent::TALENT_RECEIVE = "talentReceive";

	Talent* Talent::_instance = nullptr;

	Talent::Talent() :Event()
	{
		this->init();
	}
	Talent::~Talent()
	{
	}
	Talent* Talent::getInstance()
	{
		if (!_instance) {
			_instance = new Talent();
		}
		return _instance;
	}
    void Talent::destroyInstace()
    {
        if (_instance) {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Talent::init()
	{
		_db = DBConnector::getInstance()->open();
		_uid = User::getInstance()->getUID();
		std::stringstream sql;
		//更新表--
		_db->upgrade(TABLE_TALENT, Table::getInstance()->getTableByName(TABLE_TALENT), Table::getInstance()->getCSVByName(TABLE_TALENT));//天赋表..
		_db->upgrade(TABLE_BUFF, Table::getInstance()->getTableByName(TABLE_BUFF), Table::getInstance()->getCSVByName(TABLE_BUFF));//buff表..
		_db->upgrade(TABLE_TALENT_BUFF, Table::getInstance()->getTableByName(TABLE_TALENT_BUFF), Table::getInstance()->getCSVByName(TABLE_TALENT_BUFF));//天赋buff表..
		_db->upgrade(TABLE_USER_TALENT, Table::getInstance()->getTableByName(TABLE_USER_TALENT));//用户天赋表..
	}

	cocos2d::Value Talent::getTalentByID(int id){
		Value val = getMem(id);
		if (!val.isNull()) return val;
		std::stringstream sql;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		sql << "SELECT * FROM `" << TABLE_TALENT << "` WHERE `talent_id` = '" << id << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		sql.clear(); sql.str("");
		sql << "SELECT * FROM `" << TABLE_TALENT_BUFF <<  "`WHERE `talent_id` = '" << id << "'"<< comment;
		val.asValueMap()["buff"] = _db->getAll(sql.str());
		setMem(id, val);
		return val;
	}
    
	cocos2d::Value Talent::getUserBuff(){
		std::stringstream sql;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		sql << "SELECT * FROM `" << TABLE_USER_TALENT << "` WHERE `uid` = '" << _uid << "' " << comment;
		Value all = _db->getAll(sql.str());
		ValueMap buff = ValueMapNull;
		for (Value row : all.asValueVector())
		{
			Value talent = getTalentByID(row.asValueMap()["talent_id"].asInt());
            if(!talent.isNull())
            {
                for (Value buffSingle : talent.asValueMap()["buff"].asValueVector())
                {
                    std::string buffID = buffSingle.asValueMap()["buff_id"].asString();
                    if (buff[buffID].isNull()){
                        buff[buffID] = buffSingle;
                    }
                    else
                    {
                        buff[buffID].asValueMap()["value"] = buff[buffID].asValueMap()["value"].asFloat() + buffSingle.asValueMap()["value"].asFloat();
                    }
                }
            }
		}
		return Value(buff);
	}
	void Talent::addTalent(int id){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		custom::LibDate d(NULL);
		sql << "REPLACE INTO `" << TABLE_USER_TALENT << "`  (`uid`, `talent_id`, `cdate`) VALUES ('" << _uid << "', '" << id << "', '" << d.datetime() << "') " << comment;
        _db->query(sql.str());
	}

	cocos2d::Value Talent::getUserTalentList()
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		//sql << "SELECT `t`.*, `u`.`cdate` FROM `" << TABLE_TALENT << "` `t` LEFT JOIN `" << TABLE_USER_TALENT << "` `u`  ON `t`.`talent_id` = `u`.`talent_id` ORDER BY `t`.`talent_id` ASC " << comment;
		sql << "SELECT * FROM `" << TABLE_USER_TALENT << "` ORDER BY `talent_id` ASC " << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value talent = getTalentByID(row.asValueMap()["talent_id"].asInt());
			talent.asValueMap()["complete"] = row.asValueMap()["uid"].asBool();
			row = talent;
		}
		return all;
	}
}
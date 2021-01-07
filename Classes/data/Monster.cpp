/****************************************************************************
      
*	@desc	怪物
*	@date	2015-1-27
*	@author	110101
   
*	@file	data/Monster.cpp
******************************************************************************/

#include "data/Monster.h"

USING_NS_CC;

namespace data {
	const std::string Monster::MONSTER_DEAD = "monsterDead";

	Monster* Monster::_instance = nullptr;

	Monster::Monster() :Event()
	{
		this->init();
	}
	Monster::~Monster()
	{
	}
	Monster* Monster::getInstance()
	{
		if (!_instance) {
			_instance = new Monster();
		}
		return _instance;
	}
    
    void Monster::destroyInstance(){
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Monster::init()
	{
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
		//更新表--
		_db->upgrade(TABLE_MONSTER, Table::getInstance()->getTableByName(TABLE_MONSTER), Table::getInstance()->getCSVByName(TABLE_MONSTER));//怪物表..
		_db->upgrade(TABLE_MONSTER_DROP, Table::getInstance()->getTableByName(TABLE_MONSTER_DROP), Table::getInstance()->getCSVByName(TABLE_MONSTER_DROP));//怪物掉落表..
		_db->upgrade(TABLE_STAGE_MONSTER, Table::getInstance()->getTableByName(TABLE_STAGE_MONSTER), Table::getInstance()->getCSVByName(TABLE_STAGE_MONSTER));//关卡怪物表..
	}
	Value Monster::getTowerMonster(int round){
        if(_preTowerRound!=round)
        {
            _preTowerRound = round;
            
            char comment[200];
            sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
            std::stringstream sql;
            int scope = round / 100 + 1;
            int min = 110000 + scope * 100;
            int max = scope < 10 ? min + 100 : 999999;
            sql << "SELECT * FROM `" << TABLE_MONSTER << "` WHERE SUBSTR(`mid`, 5, 2) NOT IN (13, 16) AND `mid` BETWEEN '" << min << "' AND '" << max << "' ORDER BY random() LIMIT 1";
            Value row = _db->getRow(sql.str());
            ValueMap map = row.asValueMap();
            map["mid"] = 0;
            float r = (float)round;
            float blood = ceil(80 + 80 * floor(r / 10) + r * pow(r, (0.6 + floor(r / 100) / 100)));
            float power = floor(40 + r * 1 + r * pow(r, 0.4));
            float speed = 3.4 - floor(r / 1500);
            float accuracy = 0.5 + (r * pow(r, 0.3)) / (1900 * pow(1900, 0.3));
            map["blood"] = blood;
            map["power"] = power;
            map["speed"] = speed;
            map["accuracy"] = accuracy;
            map["tower_round"] = round;
            _towerMonsterMap = map;
            return Value(map);
        }
        else
        {
            if(_towerMonsterMap != ValueMapNull)
            {
                return Value(_towerMonsterMap);
            }
        }
        return Value(ValueMapNull);
		
	}
	Value Monster::getMonsterByMID(int mid)
	{
		Value val = getMem(mid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_MONSTER << "` WHERE `mid` = '" << mid << "' LIMIT 1";
		val = _db->getRow(sql.str());
		val.asValueMap()["drop"] = getMonsterDropItems(mid);
		setMem(mid, val);
		return val;
	}
	Value Monster::getMonsterBySID(int sid){
		std::stringstream sql;
		sql << "SELECT `mid` FROM `" << TABLE_STAGE_MONSTER << "` WHERE `sid` = '" << sid << "' ORDER BY RANDOM() LIMIT 1";
		int mid = _db->getOne(sql.str()).asInt();
		return getMonsterByMID(mid);
	}
	Value Monster::getMonsterDropItems(int mid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_MONSTER_DROP << "` WHERE `mid` = '" << mid << "' AND `group` = 1";
		Value all = _db->getAll(sql.str());
        sql.clear();
        sql.str("");
		ValueVector items = ValueVectorNull;
		float seed = random(0.0f, 1.0f);
        float weight = 0.0f;
		for (Value row : all.asValueVector()){
			weight += row.asValueMap()["rate"].asFloat();
			if (seed < weight){
				Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
				item.asValueMap()["fit"] = row.asValueMap()["amount"];
				items.push_back(Value(item));
				break;
			}
		}
		sql << "SELECT * FROM `" << TABLE_MONSTER_DROP << "` WHERE `mid` = '" << mid << "' AND `group` = 0";
		all = _db->getAll(sql.str());
		seed = random(0.0f, 1.0f);
		for (Value row : all.asValueVector()){
			if (seed < row.asValueMap()["rate"].asFloat()){
				Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
				item.asValueMap()["fit"] = row.asValueMap()["amount"];
				item.asValueMap()["amount"] = row.asValueMap()["amount"];
				items.push_back(Value(item));
			}
		}
		return Value(items);
	}
}
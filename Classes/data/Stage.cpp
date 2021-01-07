/****************************************************************************
      
*	@desc	关卡
*	@date	2015-1-22
*	@author	110101
   
*	@file	data/Stage.cpp
******************************************************************************/

#include "Stage.h"
#include "data/Achievement.h"

USING_NS_CC;

namespace data {
	const std::string Stage::STAGE_CLEAR = "stageClear";
    const std::string Stage::FIELD_CHANGE = "fieldChange";

	Stage* Stage::_instance = nullptr;

	Stage::Stage() :Event()
	{
		this->init();
	}
	Stage::~Stage()
	{
	}
	Stage* Stage::getInstance()
	{
		if (!_instance) {
			_instance = new Stage();
		}
		return _instance;
	}
    void Stage::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Stage::init()
	{
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
		//更新表--
		_db->upgrade(TABLE_STAGE, Table::getInstance()->getTableByName(TABLE_STAGE), Table::getInstance()->getCSVByName(TABLE_STAGE));//关卡表..
		_db->upgrade(TABLE_FIELD, Table::getInstance()->getTableByName(TABLE_FIELD), Table::getInstance()->getCSVByName(TABLE_FIELD));//场地表..
        _db->upgrade(TABLE_FIELD_KEY, Table::getInstance()->getTableByName(TABLE_FIELD_KEY),Table::getInstance()->getCSVByName(TABLE_FIELD_KEY));//场地道具需求表..
		_db->upgrade(TABLE_FIELD_MONSTER, Table::getInstance()->getTableByName(TABLE_FIELD_MONSTER), Table::getInstance()->getCSVByName(TABLE_FIELD_MONSTER));//场地怪物表..
		_db->upgrade(TABLE_STAGE_FIELD, Table::getInstance()->getTableByName(TABLE_STAGE_FIELD), Table::getInstance()->getCSVByName(TABLE_STAGE_FIELD));//关卡场地表..
		_db->upgrade(TABLE_FIELD_REWARD, Table::getInstance()->getTableByName(TABLE_FIELD_REWARD), Table::getInstance()->getCSVByName(TABLE_FIELD_REWARD));//关卡庇护所表..
		_db->upgrade(TABLE_USER_STAGE, Table::getInstance()->getTableByName(TABLE_USER_STAGE));//用户关卡表..
		_db->upgrade(TABLE_USER_STAGE_MAP, Table::getInstance()->getTableByName(TABLE_USER_STAGE_MAP));//用户关卡地图表..
        _db->upgrade(TABLE_FIELD_REWARD, Table::getInstance()->getTableByName(TABLE_FIELD_REWARD), Table::getInstance()->getCSVByName(TABLE_FIELD_REWARD));//掉落物品表..
	}

	Value Stage::getUserStages()
	{
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_STAGE << "` `u`, `" << TABLE_STAGE << "` `s` WHERE `u`.`uid` = '" << _uid << "' AND `u`.`sid` = `s`.`sid` " << comment;
		Value all = _db->getAll(sql.str());
		if (all.asValueVector().size() == 0){
			sql.str(""); sql.clear();
			sql << "SELECT `sid` FROM `" << TABLE_STAGE << "` ORDER BY `sid` ASC LIMIT 1 " << comment;
			int sid = _db->getOne(sql.str()).asInt();
			addUserStage(sid);
			sql.str(""); sql.clear();
			sql << "SELECT * FROM `" << TABLE_USER_STAGE << "` `u`, `" << TABLE_STAGE << "` `s` WHERE `u`.`uid` = '" << _uid << "' AND `u`.`sid` = `s`.`sid` " << comment;
			all = _db->getAll(sql.str());
		}
		return all;
	}
    bool Stage::isUserStageFirstEnter(int sid)
    {
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        
        sql<<"SELECT first FROM "<<TABLE_USER_STAGE<<" WHERE sid = "<<sid<<comment;
        bool first = _db->getOne(sql.str()).asBool();
        sql.str("");
        if(first)
        {
            sql << "UPDATE `" << TABLE_USER_STAGE << "` SET `first` = "<< 0<<" WHERE `uid`='" << _uid << "' AND `sid` = '" << sid << "'" ;
            _db->query(sql.str());
        }
        
        return first;
    }
	void Stage::addUserStage(int sid){
		std::stringstream sql;
		sql << "REPLACE INTO `" << TABLE_USER_STAGE << "` (uid,sid,cdate) VALUES ('" << _uid << "','" << sid << "','" << User::getInstance()->getTimeDate() << "') ";
		_db->query(sql.str());
	}
	std::vector<std::vector<Value>> Stage::getStageMap(int sid){
		std::stringstream sql;
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_STAGE_MAP << "` WHERE `uid` = '" << _uid << "' AND `sid` = '" << sid << "' ";
		int num = _db->getOne(sql.str()).asInt();
		Value stage = getStageBySID(sid);
		int width = stage.asValueMap()["width"].asInt();
		int height = stage.asValueMap()["height"].asInt();
		if (num != width * height)
        {
			addUserStageMap(sid);
		}
		std::vector<std::vector<Value>> coordinate;
		for (int r = 0; r < height; r++){
			std::vector<Value> abscissa;
			sql.clear();
			sql.str("");
			sql << "SELECT * FROM `" << TABLE_USER_STAGE_MAP << "` WHERE `uid`='" << _uid << "' AND `sid` = '" << sid << "' AND `y` = '" << r << "' ORDER BY `x` ASC ";
			Value all = _db->getAll(sql.str());
			for (Value row : all.asValueVector())
            {
				int fid = row.asValueMap()["fid"].asInt();
				Value field = getFieldByFID(fid);
                Value field_key = getFieldKeyByFID(fid);
				row.asValueMap()["field"] = field;
                row.asValueMap()["field_key"] = field_key;
				abscissa.push_back(row);
			}
			coordinate.push_back(abscissa);
		}
		_coordinate = coordinate;
		return coordinate;
	}
	bool Stage::scanMap(int sid, int x, int y){
		if (x < 0) return false;
		if (y < 0) return false;
        if (y >= _coordinate.size()) return false;
		if (x >= _coordinate[y].size()) return false;
		Value &coord = _coordinate[y][x];
		if (coord.asValueMap()["fog"].asInt() == 1){
			coord.asValueMap()["fog"] = 0;
			std::stringstream sql;
			sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `fog` = '0' WHERE `uid`='" << _uid << "' AND `sid` = '" << sid << "' AND `y` = '" << y << "' AND `x` = '" << x << "' ";
			_db->query(sql.str());sql.str("");
            
            /* 检查nofog是否为0 ，不为0在没有操作 */
            sql<<"SELECT nofog FROM "<<TABLE_USER_STAGE<<" WHERE sid = "<<sid;
            int nofog = _db->getOne(sql.str()).asInt();sql.str("");
            if (nofog == 0)
            {
                /* 如果nofog为0，检查fog的个数是否为0。如果为0：添加探索度，nofog置1。1：表示在此之前fog已经被清理 */
                sql<<"SELECT COUNT(fog) FROM "<<TABLE_USER_STAGE_MAP<<" WHERE fog = 1";
                int fogNum = _db->getOne(sql.str()).asInt();sql.str("");
                if (fogNum == 0)
                {
                    Achievement::getInstance()->count(1);
                    //刷新nofog
                    sql<<"UPDATE "<<TABLE_USER_STAGE<<" SET nofog = "<<1<<" WHERE sid = "<<sid;
                    assert(_db->query(sql.str()));sql.str("");
                }
            }
			return true;
		}
		return false;
	}
	Value Stage::getStageMapByID(int id){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_STAGE_MAP << "` WHERE `id` = '" << id << "' LIMIT 1";
		return _db->getRow(sql.str());
	}
	Value Stage::getStageMapByCoord(int sid, int x, int y){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_STAGE_MAP << "` WHERE `sid` = '" << sid << "' AND `x` = '" << x << "' AND `y` = '" << y << "' LIMIT 1";
        Value row = _db->getRow(sql.str());
		row.asValueMap()["field"] = getFieldByFID(row.asValueMap()["fid"].asInt());
        row.asValueMap()["field_key"] = getFieldKeyByFID(row.asValueMap()["fid"].asInt());
		return row;
	}
	Value Stage::getStageFields(int sid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_STAGE_FIELD << "` WHERE `sid` = '" << sid << "' ORDER BY `coord_type` ASC, `amount` ASC " << comment;
		Value all = _db->getAll(sql.str());
		return all;
	}
	Value Stage::getStageBySID(int sid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__); 
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_STAGE << "` WHERE `sid` = '" << sid << "' LIMIT 1 " << comment;
		Value stage = _db->getRow(sql.str());
		return stage;
	}
	Value Stage::getFieldByFID(int fid){
		Value val = getMem(fid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_FIELD << "` WHERE `fid` = '" << fid << "' LIMIT 1 " << comment;
		Value field = _db->getRow(sql.str());
		Value monsters = getFieldMonsterByFID(fid);
		if (monsters.asValueVector().size() != 0) field.asValueMap()["monsters"] = monsters;
		Value items = getFieldReward(fid);
		if (items.asValueVector().size() != 0) field.asValueMap()["items"] = items;
		setMem(fid, field);
		return field;
	}
    Value Stage::getFieldKeyByFID(int fid)
    {
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        sql << "SELECT * FROM `" << TABLE_FIELD_KEY << "` WHERE `fid` = '" << fid<< "' " << comment;
        Value fieldkey = _db->getAll(sql.str());
        return fieldkey;
    }
	Value Stage::getPortal(int sid){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_STAGE_MAP << "` WHERE `sid` = '" << sid << "' AND `fid`='" << FIELD_PORTAL_ID << "' LIMIT 1 ";
		Value field = _db->getRow(sql.str());
		return field;
	}
	void Stage::fieldRest(int sid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `visit` = '" << FieldVisit::NORMAL_VISIT << "', `round` = 0  WHERE `sid`='" << sid << "' AND `visit`='" << FieldVisit::ALREADY_VISIT << "'" << comment;
		 _db->query(sql.str());
	}
	void Stage::fieldFailed(int sid, int x, int y){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `visit` = '" << FieldVisit::NORMAL_VISIT << "', `round` = 0 WHERE `fid` != 0 AND `sid`='" << sid << "' AND `x`='" << x << "' AND `y`='" << y << "'" << comment;
		_db->query(sql.str());
	}
	void Stage::fieldEnter(int sid, int x, int y){
		Value row = getStageMapByCoord(sid, x, y);
		fieldEnter(row.asValueMap()["id"].asInt());
	}
	void Stage::fieldEnter(int id){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `visit` = '" << FieldVisit::NORMAL_VISIT << "' WHERE `id`='" << id << "'" << comment;
		_db->query(sql.str());
	}
	bool Stage::fieldSiege(int id)
    {
		Value stageMap = getStageMapByID(id);
		int round = stageMap.asValueMap()["round"].asInt();
		Value field = getFieldByFID(stageMap.asValueMap()["fid"].asInt());
        round++;
		if (round == field.asValueMap()["monster"].asInt()) return true;
		std::stringstream sql;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `round` = '" << round << "' WHERE `id`='" << id << "'" << comment;
		_db->query(sql.str());
		return false;
	}
	bool Stage::fieldSiege(int sid, int x, int y){
		Value row = getStageMapByCoord(sid, x, y);
		return fieldSiege(row.asValueMap()["id"].asInt());
	}
	void Stage::fieldClear(int id){
//		Value field = getStageMapByID(id);
//		int type = field.asValueMap()["type"].asInt();
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
        /**
         *  临时修改
         */
        Value field = getStageMapByID(id);
        Value tempfield = getFieldByFID(field.asValueMap()["fid"].asInt());
        int type = tempfield.asValueMap()["type"].asInt();
		if (type == FieldType::DUNGEON || type == FieldType::GATE_BAR)
        {
			sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `visit` = '" << FieldVisit::NORMAL_VISIT << "', `round` = 0, `fid` = `fid` + 1 WHERE `id`='" << id << "'" << comment;
			_db->query(sql.str());
			Value &coord = _coordinate[field.asValueMap()["y"].asInt()][field.asValueMap()["x"].asInt()];
			coord.asValueMap()["fid"] = field.asValueMap()["fid"].asInt() + 1;
			dispatch(FIELD_CHANGE, &coord);
            
            //探索度计数..
            Achievement::getInstance()->count(Achievement::EXPLORE);
            //探索度事件触发
            Task::getInstance()->scanCheck(id);
		}
		else if (type == FieldType::SHELTER)
        {
            /**
             *  庇护所这里只更新了visitxinxi，因此没有更新图片
             */
			sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `visit` = '" << FieldVisit::ALREADY_VISIT << "' , `round` = 0 WHERE `id`='" << id << "'" << comment;
			_db->query(sql.str());
		}
		else if (type == FieldType::ARENA){
			sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `round` = 0 WHERE `id`='" << id << "'" << comment;
			_db->query(sql.str());
		}

	}
	void Stage::fieldClear(int sid, int x, int y){
		Value row = getStageMapByCoord(sid, x, y);
		fieldClear(row.asValueMap()["id"].asInt());
	}
	
	Value Stage::getFieldMonster(int fid, int round){
		Value monster;
		if (fid == FieldType::TOWER){
			monster = Monster::getInstance()->getTowerMonster(round);
		}else{
			char comment[200];
			sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
			std::stringstream sql;
			sql << "SELECT `mid` FROM `" << TABLE_FIELD_MONSTER << "` WHERE `fid`='" << fid << "' AND `sequence` = '" << round << "' LIMIT 1 ;" << comment;
			int mid = _db->getOne(sql.str()).asInt();
			monster = Monster::getInstance()->getMonsterByMID(mid);
		}
		return monster;
	}
	Value Stage::getFieldReward(int fid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_FIELD_REWARD << "` WHERE `fid`='" << fid << "'" << comment;
		Value all = _db->getAll(sql.str());
		for (Value &row : all.asValueVector())
		{
			Value item = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
			item.asValueMap()["amount"] = row.asValueMap()["amount"].asInt();
			row = item;
		}
		return all;
	}
	void Stage::addUserStageMap(int sid){
		ValueMap stage = getStageBySID(sid).asValueMap();
		std::vector<std::vector<int>> coordinate;
		int offsetX = stage["width"].asInt() / 2;
		int offsetY = stage["height"].asInt() / 2;
		std::stringstream sql;
		for (unsigned r = 0; r < stage["height"].asInt(); r++){
			sql.clear();
			sql.str("");
			sql << "REPLACE INTO `" << TABLE_USER_STAGE_MAP << "` (`uid`, `x`, `y`, `sid`, `fid`, `bgid`, `fog`) VALUES ";
			std::vector<int> abscissa;
			for (unsigned c = 0; c < stage["width"].asInt(); c++){
				abscissa.push_back(FIELD_NONE);
				if (c == 0){
				
				}
				else{
					sql << ",";
				}
				int bgid = sid + (c + r) % 2;
				int fog = StageFog::COORD_FUZZY;
				if (r > offsetY - StageFog::INIT_CLEAR && r < offsetY + StageFog::INIT_CLEAR && c > offsetX - StageFog::INIT_CLEAR && c < offsetX + StageFog::INIT_CLEAR) fog = StageFog::COORD_CLEAR;
				sql << "('" << _uid << "', '" << c << "', '" << r << "', '" << sid << "', '" << FIELD_NONE << "', '" << bgid << "', '" << fog << "')";
			}
			coordinate.push_back(abscissa);
			_db->query(sql.str());
		}
		Value stageFields = getStageFields(sid);
		for (Value row : stageFields.asValueVector())
		{
			ValueMap map = row.asValueMap();
			int coord0 = map["x"].asInt();
			int coord1 = map["y"].asInt();
			if (map["coord_type"].asInt() == COORD_TYPE_FIX){//固定坐标..
				int x = coord0 + offsetX;
				int y = coord1 + offsetY;
				coordinate[y][x] = map["fid"].asInt();
				addMapCoord(sid, map["fid"].asInt(), x, y);
				
			} else if (map["coord_type"].asInt() == COORD_TYPE_SCOPE){//随机坐标..
				//按照数量随机..
				for (size_t i = 0; i < map["amount"].asInt(); i++)
				{
					int x;
					int y;
					bool loop = false;
					int n = 0;
					//循环随机坐标，主要不能让副本有重复坐标..
					do{
						if (n > 100) break;//如果循环一定次数次还不能得到不重合坐标，就要强制跳出..
						int step = random(coord0, coord1);//随机步数..
						int gap = 0;
						if (step > offsetX){
							gap = step - offsetX;
						}
						x = random(gap, (step - gap));//随机x..
						y = step - x;//步数减去x就是y..
						x = (random(0.0f, 1.0f) < 0.5f) ? x + offsetX : offsetX - x;
						y = (random(0.0f, 1.0f) < 0.5f) ? y + offsetY : offsetY - y;
						loop = (coordinate[y][x] != FIELD_NONE);
						if (!loop && x > 0){
							loop = (coordinate[y][x - 1] != FIELD_NONE);
						}
						if (!loop && y > 0){
							loop = (coordinate[y - 1][x] != FIELD_NONE);
						}
						if (!loop && x < stage["weight"].asInt() - 1){
							loop = (coordinate[y][x + 1] != FIELD_NONE);
						}
						if (!loop && y < stage["height"].asInt() - 1){
							loop = (coordinate[y + 1][x] != FIELD_NONE);
						}
						n++;
					} while (loop);
					if (n < 100){
						coordinate[y][x] = map["fid"].asInt();
						addMapCoord(sid, map["fid"].asInt(), x, y);
					}
				}
			}
		}
	}
	void Stage::addMapCoord(int sid, int fid, unsigned x, unsigned y){
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_STAGE_MAP << "` SET `fid` = '" << fid << "', `visit` = '" << FieldVisit::NEVER_VISIT << "' WHERE `uid` = '" << _uid << "' AND `sid` = '" << sid << "' AND `x` = '" << x << "' AND `y` = '" << y << "' ";
		_db->query(sql.str());
	}
	cocos2d::Value Stage::getFieldMonsterByFID(int fid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_FIELD_MONSTER << "` WHERE `fid`='" << fid << "' ORDER BY `sequence` ASC";
		Value all = _db->getAll(sql.str());
		return all;
	}
    
}
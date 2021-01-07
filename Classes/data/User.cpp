/****************************************************************************
 
*	@desc	用户信息
*	@date	2014-12-12
*	@author	110101
   
*	@file	data/User.cpp
******************************************************************************/

#include "User.h"
#include "base/CCAsyncTaskPool.h"

USING_NS_CC;

namespace data {
const std::string User::GOLD_CHANGE = "userGoldChange";
const UserInfo User::DEFAULT_USER_DATA = { 1, "user", 0 };
const std::string User::TABLE_USER = "user";
const SettingInfo User::DEFAULT_SETTING_DATA = { 1, 50, 50, 0,110101, 1 ,false,false};
const std::string User::TABLE_SETTING = "user_setting";

User* User::_instance = nullptr;

User::User() :Event()
{
    this->init();
}
User::~User()
{
}
User* User::getInstance()
{
    if (!_instance) {
        _instance = new User();
    }
    return _instance;
}
    void User::destroyInstance()
    {
        if (_instance) {
            CC_SAFE_DELETE(_instance);
        }
    }
void User::init()
{
    initUserData();
    initSettingData();
    Value row = getUserData();
    _uid = row.asValueMap()["uid"].asInt();
	setGold(row.asValueMap()["gold"].asInt());
	setName(row.asValueMap()["name"].asString());
    setDate();
	setBGM(row.asValueMap()["bgm"].asInt());
	setSE(row.asValueMap()["se"].asInt());
	setFirstPlay(row.asValueMap()["first"].asBool());
    setGuideID(row.asValueMap()["cguide"].asInt());
    setGuide(row.asValueMap()["guide"].asBool());
    setGuidTextClose(row.asValueMap()["guidTxt"].asBool());
    setRMBShow(row.asValueMap()["rmbShow"].asBool());
}
void User::initUserData()
{
    _db = DBConnector::getInstance()->open();
    std::stringstream sql;
	std::string tablesql = Table::getInstance()->getTableByName(TABLE_USER);
	_db->upgrade(TABLE_USER, tablesql);
    sql << "SELECT COUNT(*) AS `num` FROM `" << User::TABLE_USER.c_str() << "` LIMIT 1";
    int num = _db->getOne(sql.str()).asInt();
    if (num == 0) {
        sql.clear();
        sql.str("");
        sql << "REPLACE INTO `" << User::TABLE_USER.c_str() << "` (`uid`, `name`, `gold`) VALUES ('" << User::DEFAULT_USER_DATA.uid << "','" << User::DEFAULT_USER_DATA.name.c_str() << "','" << User::DEFAULT_USER_DATA.gold << "')";
        _db->query(sql.str());
    }
}

void User::initSettingData()
{
    _db = DBConnector::getInstance()->open();
    std::stringstream sql;
    _db->upgrade(TABLE_SETTING, Table::getInstance()->getTableByName(TABLE_SETTING.c_str()));
    sql << "SELECT COUNT(*) AS `num` FROM `" << User::TABLE_SETTING.c_str() << "` LIMIT 1";
    int num = _db->getOne(sql.str()).asInt();
    if (num == 0) {
        sql.clear();
        sql.str("");
        sql << "REPLACE INTO `" << User::TABLE_SETTING.c_str() << "` (`uid`, `bgm`, `se`,`guide`,`cguide`, `first`,`guidTxt`,`rmbShow`) VALUES ('" << User::DEFAULT_SETTING_DATA.uid << "','" << User::DEFAULT_SETTING_DATA.bgm << "','" << User::DEFAULT_SETTING_DATA.se << "','" << User::DEFAULT_SETTING_DATA.guide << "','" << User::DEFAULT_SETTING_DATA.cguide << "','"<<User::DEFAULT_SETTING_DATA.first <<
            "','"<<User::DEFAULT_SETTING_DATA.guidTxt << "','"<<User::DEFAULT_SETTING_DATA.rmbshow<<"')";
        _db->query(sql.str());
    }
}

Value User::getUserData()
{
    std::stringstream sql;
    sql << "SELECT * FROM `" << User::TABLE_USER.c_str() << "` `u` , `" << User::TABLE_SETTING.c_str() << "` `s` WHERE `u`.uid = `s`.uid";
    return _db->getRow(sql.str());
}

void User::setGold(int gold)
{
    _gold = gold;
    bool res;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_USER.c_str() << "` SET `gold`='" << gold << "' WHERE `uid`=" << User::DEFAULT_USER_DATA.uid;
    res = _db->query(sql.str());
    if(res)
    {
        dispatch(User::GOLD_CHANGE, &gold);
    }
    
}
void User::getUserInit()
{
    int row = getGold();
    dispatch(User::GOLD_CHANGE, &row);
}
int User::getGold()
{
    return _gold;
}
bool User::goldPlus(int num)
{
    int gold = _gold + num;
    if (gold < 0)
        return false;
    this->setGold(gold);
    return true;
}
bool User::isGuidTextClose()
{
    return _guideText;
}
    
void User::setGuidTextClose(bool bRet)
{
    _guideText = bRet;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `guidTxt`='" << bRet << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    
    _db->query(sql.str());
}
void User::setGuideID(int guidID)
{
    _guideId    =   guidID;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `cguide`='" << guidID << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    
    _db->query(sql.str());
}
    
int User::getGuideID()
{
    return _guideId;
}
bool User::getRMBShow()
{
    return _rmbShow;
}
void User::setRMBShow(bool rmbshow)
{
    _rmbShow = rmbshow;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `rmbShow`='" << rmbshow << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    
    _db->query(sql.str());
    
}
bool User::goldReduce(unsigned num)
{
    int gold = _gold - num;
    if (gold < 0)
        return false;
    this->setGold(gold);
    return true;
}
void User::setName(std::string name)
{
    _name = name;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_USER.c_str() << "` SET `name`='" << name << "' WHERE `uid`=" << User::DEFAULT_USER_DATA.uid;
    _db->query(sql.str());
}
std::string User::getName()
{
    return _name;
}
int User::getUID()
{
    return _uid;
}
void User::setDate(time_t timestamp)
{
    char now[64];
    time_t t = time(&timestamp);
    struct tm* ttime;
    ttime = localtime(&t);
    strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
    std::string date;
    date = { now };
    this->setDate(date);
}
void User::setDate(std::string date)
{
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_USER.c_str() << "` SET `date`='" << date.c_str() << "' WHERE `uid`=" << User::DEFAULT_USER_DATA.uid;
    _db->query(sql.str());
}
void User::setDate()
{
    this->setDate(NULL);
}
std::string User::getDate()
{
    std::stringstream sql;
    sql << "SELECT `date` FROM `" << User::TABLE_USER.c_str() << "` WHERE `uid`=" << User::DEFAULT_USER_DATA.uid << " LIMIT 1";
    std::string date = _db->getOne(sql.str()).asString();
    return date;
}

void User::setBGM(int bgm)
{
    _bgm = bgm;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `bgm`='" << bgm << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}

int User::getBGM()
{
    return _bgm;
}

void User::setSE(int se)
{
    _se = se;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `se`='" << se << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}

int User::getSE()
{
    return _se;
}

void User::setSoundEffect(bool valid)
{
    _se = valid;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `se`='" << (int)_se << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}
bool User::getSoundEffect()
{
    return _se;
}
    
bool User::isAutoBattle()
{
    Value res;
    std::stringstream sql;
    sql << "SELECT `auto` FROM `" << User::TABLE_SETTING.c_str()<<"`";// << "` WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    res = _db->getOne(sql.str()).asInt();
    return res.asBool();
}

void User::setAutoBattle(bool autbat)
{
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `auto` = "<< autbat<<" WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}


void User::setFirstPlay(bool first)
{
    _first = first;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `first`='" << (int)first << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}
    
void User::setGuide(bool guide)
{
    _guide = guide;
    std::stringstream sql;
    sql << "UPDATE `" << User::TABLE_SETTING.c_str() << "` SET `guide`='" << (int)guide << "' WHERE `uid`=" << User::DEFAULT_SETTING_DATA.uid;
    _db->query(sql.str());
}
    
bool User::getGuide()
{
    return _guide;
}

int User::getFirstPlay()
{
    return _first;
}
std::string User::getTimeDate(time_t timestamp)
{
    char now[64];
    time_t t = time(&timestamp);
    struct tm* ttime;
    ttime = localtime(&t);
    strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
    std::string date(now);
    return date;
}
}
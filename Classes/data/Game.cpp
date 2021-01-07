/****************************************************************************
 
*	@desc	用户信息
*	@date	2014-12-16
*	@author	110101
   
*	@file	data/Game.cpp
******************************************************************************/

#include "Game.h"

USING_NS_CC;

namespace data {
const std::string Game::GOLD_CHANGE = "userGameGoldChange";
const std::string Game::ACT_CHANGE = "userGameActChange";
Game* Game::_instance = nullptr;

Game::Game() :Event()
{
    this->init();
}
Game::~Game()
{
}
Game* Game::getInstance()
{
    if (!_instance) {
        _instance = new Game();
    }
    return _instance;
}
    
    void  Game::destroyInstance(){
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
void Game::init()
{
    _uid = User::getInstance()->getUID(); /* 获取用户uid */
    _db = DBConnector::getInstance()->open();
    std::stringstream sql;
    _db->upgrade(TABLE_USER_GAME, Table::getInstance()->getTableByName(TABLE_USER_GAME)); /* 更新表 */
    sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_GAME << "` LIMIT 1";
    int num = _db->getOne(sql.str()).asInt();
    if (num == 0) {
        /** 如果空插入默认数据 **/
        sql.clear();
        sql.str("");
        sql << "REPLACE INTO `" << TABLE_USER_GAME << "` (`uid`) VALUES ('" << _uid << "')";
        _db->query(sql.str());
    }
}

Value Game::getDetail()
{
    std::stringstream sql;
    sql << "SELECT * FROM `" << TABLE_USER << "` `u` , `" << TABLE_USER_GAME << "` `g` WHERE `u`.uid = `g`.uid";
    return _db->getRow(sql.str());
}

void Game::setGold(int gold)
{
    _gold = gold;
    bool res;
    std::stringstream sql;
    sql << "UPDATE `" << TABLE_USER_GAME << "` SET `gold`='" << gold << "' WHERE `uid`=" << _uid;
    res = _db->query(sql.str());
    if(res)
    {
        dispatch(Game::GOLD_CHANGE, &gold);
    }
    
}
int Game::getGold()
{
    if (_gold == 0) {
        Value row = getDetail();
        _gold = row.asValueMap()["gold"].asInt();
    }
    return _gold;
}
bool Game::goldPlus(int num)
{
    int gold = _gold + num;
    if (gold < 0)
        return false;
    this->setGold(gold);
    return true;
}
bool Game::goldReduce(unsigned num)
{
    int gold = _gold - num;
    if (gold < 0)
        return false;
    this->setGold(gold);
    return true;
}

void Game::setAct(int act)
{
    _act = act;
    bool res;
    std::stringstream sql;
    sql << "UPDATE `" << TABLE_USER_GAME << "` SET `act`='" << act << "' WHERE `uid`=" << _uid;
    res = _db->query(sql.str());
    if(res)
    {
        dispatch(Game::ACT_CHANGE, &act);
    }
    
}
int Game::getAct()
{
    if (_act == 0) {
        Value row = getDetail();
        _act = row.asValueMap()["act"].asInt();
    }
    return _act;
}
bool Game::actPlus(int num)
{
    int act = _act + num;
    if (act < 0)
        return false;
    this->setAct(act);
    return true;
}
bool Game::actReduce(unsigned num)
{
    int act = _act - num;
    if (act < 0)
        return false;
    this->setAct(act);
    return true;
}

int Game::getUID()
{
    return _uid;
}
void Game::setDate(time_t timestamp)
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
void Game::setDate(std::string date)
{
    std::stringstream sql;
    sql << "UPDATE `" << Game::TABLE_USER_GAME << "` SET `date`='" << date << "' WHERE `uid`=" << _uid;
    _db->query(sql.str());
}
void Game::setDate()
{
    this->setDate(NULL);
}
std::string Game::getDate()
{
    std::stringstream sql;
    sql << "SELECT `date` FROM " << Game::TABLE_USER_GAME << "` WHERE `uid`=" << _uid << " LIMIT 1";
    std::string date = _db->getOne(sql.str()).asString();
    return date;
}

void Game::openAssisPosition(int p)
{
    std::stringstream sql;
    if (p == 1) {
        sql << "UPDATE " << Game::TABLE_USER_GAME << " SET assis_left = 1 WHERE assis_left = 0";
    }
    else if (p == 2) {
        sql << "UPDATE " << Game::TABLE_USER_GAME << " SET assis_right = 1 WHERE assis_right = 0";
    }
    else {
        return;
    }
    _db->query(sql.str());
}

int Game::getAssisOpen()
{
    std::stringstream sql;
    sql << "SELECT * FROM " << Game::TABLE_USER_GAME << " WHERE  uid = " << data::User::getInstance()->getUID();

    ValueMap row = _db->getRow(sql.str()).asValueMap();

    if (row["assis_right"].asInt() == 1 && row["assis_left"].asInt() == 1) {
        return assisOpenType::openAll;
    }
    else if (row["assis_right"].asInt() == 0 && row["assis_left"].asInt() == 0) {
        return assisOpenType::noOpen;
    }
    else {
        if (row["assis_left"].asInt() == 1) {
            return assisOpenType::openLeft;
        }
        else {
            return assisOpenType::openRight;
            ;
        }
    }
}
}
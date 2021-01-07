/****************************************************************************
 *	@desc	用户信息
 *	@date	2015-4-22
 *	@author	110101
     
 *	@file	data/Board.h
 ******************************************************************************/

#include "Board.h"
#include "GlobalMacros.h"

USING_NS_CC;

namespace data
{
    Board* Board::_instance = nullptr;
    
    const std::string Board::TABLE_USER_BOARD = "user_board";
    
    Board* Board::getInstance()
    {
        if (!_instance)
        {
            _instance = new Board();
            return _instance;
        }
        else
        {
            return _instance;
        }
    }
    
    void Board::destoryInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
    
    Board::Board()
    {
        _db = DBConnector::getInstance()->open();
		std::stringstream sql;
		_uid = User::getInstance()->getUID(); /* 获取用户uid */
		//更新表--
		_db->upgrade(TABLE_USER_BOARD, Table::getInstance()->getTableByName(TABLE_USER_BOARD));
    }
    
    Board::~Board()
    {
        
    }
    
	void Board::record(std::string msg){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		custom::LibDate d;
		sql << "INSERT INTO `" << TABLE_USER_BOARD << "` (`uid`, `content`, `cdate`) VALUES ('" << _uid << "', '" << msg << "','" << d.datetime() << "') " << comment;
		_db->query(sql.str());
	}
	Value Board::getHistory(int linage){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_BOARD << "` WHERE `uid` = '" << _uid << "' ORDER BY `cdate` DESC LIMIT " << linage;
		return _db->getAll(sql.str());
	}
}

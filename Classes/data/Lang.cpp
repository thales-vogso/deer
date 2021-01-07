/****************************************************************************
      
*	@desc	成就
*	@date	2015-2-5
*	@author	110101
   
*	@file	data/Achievement.cpp
******************************************************************************/

#include "Lang.h"

USING_NS_CC;

namespace data {

	Lang* Lang::_instance = nullptr;

	Lang::Lang()
	{
		this->init();
	}
	Lang::~Lang()
	{
	}
	Lang* Lang::getInstance()
	{
		if (!_instance) {
			_instance = new Lang();
		}
		return _instance;
	}
	void Lang::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Lang::init()
	{
		setLanguage(DEFAULT_LANGUAGE);
		_db = DBConnector::getInstance()->open();
		//更新表--
		_db->upgrade(TABLE_LANG, Table::getInstance()->getTableByName(TABLE_LANG), Table::getInstance()->getCSVByName(TABLE_LANG));//语言表..
        _db->upgrade(TABLE_TIPS, Table::getInstance()->getTableByName(TABLE_TIPS), Table::getInstance()->getCSVByName(TABLE_TIPS));//提示表..
	}
    
	void Lang::setLanguage(std::string language)
    {
		_language = language;
	}
	std::string Lang::getWord(std::string name)
    {
		Value val = getMem(name);
		if (!val.isNull()) return val.asValueMap()[_language].asString();
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_LANG << "` WHERE `en-us` = '" << name << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		setMem(name, val);
		return val.asValueMap()[_language].asString();
	}
	std::string Lang::getWord(int id)
    {
		Value val = getMem(id);
		if (!val.isNull()) return val.asValueMap()[_language].asString();
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_LANG << "` WHERE `id` = '" << id << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		setMem(id, val);
		return val.asValueMap()[_language].asString();
	}
    std::string Lang::getTipsByIID(int id)
    {
        //为了跟lang的id区分开来..
        Value val = getMem(id*10);
        if(!val.isNull()) return val.asValueMap()["summary"].asString();
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        sql << "SELECT * FROM `" << TABLE_TIPS << "` WHERE `iid` = '" << id << "' LIMIT 1 " << comment;
        val = _db->getRow(sql.str());
        setMem(id*10, val);
        return val.asValueMap()["summary"].asString();
    }
}
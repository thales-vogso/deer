/****************************************************************************
   
*	@desc	语言
*	@date	2015-6-1
*	@author	110101
   
*	@file	data/Lang.h
******************************************************************************/
#ifndef __DATA_LANG_H__
#define __DATA_LANG_H__

#include "cocos2d.h"
#include "user.h"

namespace data {

	class Lang : public custom::Event, custom::Memories {
	public:
		/**
		 *	初始化
		 */
		Lang();
		/**
		 *	析构函数，要清除有指针的内容
		 */
		virtual ~Lang();
		/**
		 *	获取实体
		 */
		static Lang* getInstance();
		/**
		 *	销毁
		 */
		static void destroyInstance();
		/**
		*	初始化
		*/
		void init();
		/**
		 *	设置语言
		 */
		void setLanguage(std::string language);
		/**
		 *	获取文字
		 *	@param	name	英文标示
		 *	@param	id	
		 *	@return	文字
		 */
		std::string getWord(std::string name);
		std::string getWord(int id);
        /**
         *	获取提示
         *	@param	id      道具id
         *	@return	文字
         */
        std::string getTipsByIID(int id);
	private:
		/**
		 *	自身
		 */
		static Lang* _instance;
		/**
		 *	数据库
		 */
		DBSqlite* _db;
		/**
		 *	语言
		 */
		std::string _language;
		/**
		 *	默认语言
		 */
		std::string const DEFAULT_LANGUAGE = "zh-cn";
		/**
		 *	语言
		 */
		std::string TABLE_LANG = "lang";
        
        std::string TABLE_TIPS = "tips";
	};
}
#endif // __DATA_LANG_H__
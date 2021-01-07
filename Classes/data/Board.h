/****************************************************************************
 *	@desc	用户信息
 *	@date	2015-4-22
 *	@author	110101
     
 *	@file	data/Board.h
 ******************************************************************************/

#ifndef __DATA_BOARD__
#define __DATA_BOARD__

#include "cocos2d.h"
#include "data/User.h"

namespace data
{
    class Board : public custom::Event
    {
    public:
        /**
         *  获取单例
         *
         *  @return board
         */
        static Board* getInstance();
        /**
         *  销毁单例
         */
        static void destoryInstance();
		/**
		 *	记录信息
		 */
		void record(std::string msg);
		/**
		 *	获取历史
		 */
		cocos2d::Value getHistory(int linage);
		cocos2d::Value getHistory(){
			return getHistory(DEFAULT_LINAGE);
		}
        
    CC_CONSTRUCTOR_ACCESS:
        /**
         *  构造
         */
        Board();
        
        /**
         *  析构
         */
        virtual ~Board();
        
        /**
         *  单例
         */
        static Board* _instance;
        
        /**
         *  数据库
         */
        DBSqlite *_db;
		/**
		 *	用户uid
		 */
		int _uid;
		/**
		 *	默认行数
		 */
		const int DEFAULT_LINAGE = 20;
        /**
         *  表名
         */
		static const std::string TABLE_USER_BOARD;


    };
}


#endif // __DATA_BOARD__

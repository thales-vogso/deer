/****************************************************************
 
*	@desc	道具数据
*	@date	2015-1-7
*	@author	110101
   
*	@file	data/Item.h
*************************************************************/
#ifndef __DATA_ITEM_H__
#define __DATA_ITEM_H__

#include "cocos2d.h"
#include "data/User.h"
#include "Task.h"
#include "base/CCAsyncTaskPool.h"

namespace data {

	class Item : public custom::Event, custom::Memories {
	public:
		enum ItemID
		{
			/**
			 *	金币
			 */
			COIN = 100101,
			/**
			 *	水晶
			 */
			CRYSTAL = 100102,
			/**
			 *	成就点数
			 */
			POINT = 100103,
			/**
			 *	太空食品
			 */
			FOOD = 110104,
			/**
			 *	破解器
			 */
			CRACKER_1 = 110701,
			CRACKER_2 = 110702,
			CRACKER_3 = 110703,
			CRACKER_4 = 110704,
			/**
			 *	复活道具
			 */
			REVIVE = 110112,
			/**
			 *	安葬道具
			 */
			BURIED = 110121,
			/**
			 *	身份识别牌
			 */
			IDENTITY = 110202,
        
			/**
			 *  虫洞道具
			 */
			WORMHOLE = 110206
		};
		/**
		*	初始化
		*/
		Item();
		/**
		*	析构函数，要清除有指针的内容
		*/
		virtual ~Item();
		/**
		*	获取实体
		*/
		static Item* getInstance();
		static void destroyInstance();
		/**
		*	初始化
		*/
		void init();
		/**
		*	根据iid获取道具
		*/
		cocos2d::Value getItemByIID(int iid);
		/**
		*	获取用户道具
		*/
		cocos2d::Value getUserItems();
		/**
		 *	获取仓库资源
		 */
		cocos2d::Value getResouces();
		/**
		*	增加用户道具
		*	@param	iid	道具id
		*	@amount	amount	数量
		*/
		void userItemPlus(int iid, int amount = 1);
		/* 用户道具添加完毕 */
		void userItemPlusEnd(void* param);
		/**
		*	减少用户道具
		*	@param	iid	道具id
		*	@amount	amount	数量
		*	@return	结果，比如减少的数量不足以扣
		*/
		bool userItemReduce(int iid, int amount = 1);
		/**
		 *	通过iid获取用户道具
		 *	@param	iid	道具iid
		 *	@return	道具数量
		 */
		cocos2d::Value getUserItemByIID(int iid);
		/**
		 *	通过iid获取用户道具数量
		 *	@param	iid	道具iid
		 *	@return	道具数量
		 */
		int getUserItemAmountByIID(int iid);
		/**
		*	获取用户制造
		*/
		cocos2d::Value getUserProducts();
		/**
		*	添加用户制造
		*/
		void addUserProduct(int iid);
		/**
		*	删除用户制造
		*/
		void delUserProduct(int iid);
		/**
		 *	用户制造初始
		 */
		void userProductInitial(int iid);
		/**
		 *	用户制造访问过
		 */
		void userProductVisited(int iid);
		/**
		 *	根据生产道具id获取合成方式
		 */
		cocos2d::Value getProduceByIID(int iid);
		/**
		*	制造
		*	@param	iid	 制造的道具
		*	@param	num	数量
		*/
		cocos2d::Value produce(int iid);
		cocos2d::Value produce(int iid, int num);
		/**
		 *	制造检测
		 */
		void produceCheck(int iid);
		/**
		 *	是否是一次性的制造道具
		 */
		bool isProduceDisposable(int iid);
		/**
		 *	炼金
		 */
		bool mint();
		//-----gather--------------------
		/**
		 *	获取用户采集
		 */
		cocos2d::Value getUserGather();
		/**
		 *	获取炼金数
		 */
		int getAlchemy();
		int getMint();
		/**
		 *	获取有效离线收益时间
		 */
		int getValidOutGainTime();
		//-----crusade--------------------
		/**
		 *	获取背包总大小
		 */
		int getPackage();
		/**
		 *	获取队伍总大小
		 */
		int getCarriage();
		/**
		 *	获取药效
		 */
		int getDrug();
		//-----shop--------------------
		/**
		 *	获取金钱
		 *	@return	金钱数量
		 */
		int getGold();
		/**
		 *	获取成就点数
		 */
		int getPoint();
		/**
		 *	根据商店id获取商品列表
		 *	@return	商店列表
		 */
		cocos2d::Value getShopByFID(int fid);
		cocos2d::Value getShop(){
			return getShopByFID(MANOR_SHOP_ID);
		}
		/**
		 *	购买道具
		 *	@param	fid	副本id
		 *	@param	iid	道具id
		 *	@param	amount	数量
		 *	@return	购买成功
		 */
		cocos2d::Value buy(int iid, int amount){
			return buy(MANOR_SHOP_ID, iid, amount);
		}
		cocos2d::Value buy(int iid){
			return buy(iid, 1);
		}
		cocos2d::Value buy(int fid, int iid, int amount);
		/**
		 *	购买道具
		 *	@param	fid	副本id
		 *	@param	iid	道具id
		 *	@param	amount	数量
		 *	@param	rmb	人民币
		 *	@return	购买成功
		 */
        cocos2d::Value cash(int fid, int iid, int amount);
        cocos2d::Value cash(int iid, int amount) {
			return cash(MANOR_SHOP_ID, iid, amount);
		}
		/**
		 *	卖道具
		 *	@param	iid	道具id
		 *	@param	amount	数量
		 */
		int sell(int iid, int amount = 1);
		/**
		 *	商店道具显示
		 *	@param	fid	商店id
		 *	@param	iid	道具id
		 */
		void shopItemShow(int fid, int iid);
		void shopItemShow(int iid);
		/**
		 *	商店道具隐藏
		 *	@param	fid	商店id
		 *	@param	iid	道具id
		 */
		void shopItemHide(int fid, int iid);
		/**
		*	金币改变事件
		*/
		static const std::string USER_ITEM_CHANGE;
    
		/**
		 *   生产事件名称
		 */
		static const std::string PRODUCE;
    
		/**
		 *	主商店id
		 */
		static const int MANOR_SHOP_ID = 100101;
    
	private:
		/**
		 *	自身
		 */
		static Item* _instance;
		/**
		 *	数据库
		 */
		DBSqlite* _db;
		/**
		 *	用户uid
		 */
		int _uid;
		/**
		*	道具表
		*/
		std::string TABLE_ITEM = "item";
		/**
		*	用户道具表
		*/
		std::string TABLE_USER_ITEM = "user_item";
		/**
		 *	 制造
		 */
		std::string TABLE_PRODUCE = "produce";
		/**
		 *	 用户制造
		 */
		std::string TABLE_USER_PRODUCT = "user_product";
		/**
		 *	 商店
		 */
		std::string TABLE_SHOP = "shop";
		/**
		*	 用户商店
		*/
		std::string TABLE_USER_SHOP = "user_shop";
		/**
		 *	 采集
		 */
		std::string TABLE_GATHER = "item_gather";
		/**
		 *	制币
		 */
		std::string TABLE_MINT = "item_mint";
		/**
		 *	背包
		 */
		std::string TABLE_PACKAGE = "item_package";
		/**
		 *	马车
		 */
		std::string TABLE_CARRIAGE = "item_carriage";
		/**
		 *	自动化工厂
		 */
		std::string TABLE_FACTORY = "item_factory";
		/**
		 *	药剂
		 */
		std::string TABLE_DRUG = "item_drug";
	};
}
#endif // __DATA_ITEM_H__
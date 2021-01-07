/****************************************************************************
     
 *	@desc	商店界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/StoreLayer.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_STORELAYER__
#define __DEER_STORELAYER__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "manor/LayerBase.h"
#include "core/custom/Caches.h"


class Shop : public cocos2d::Layer,
	public custom::Caches,
	public LayerBase
{
	class Item;
public:
	CREATE_FUNC(Shop);
    
    /**
     *  刷新列表
     */
    void updateList(int sid);
	void updateList();
    void freshList();
    void openShop();
    
	/**
	 *	显示批量购买弹窗
	 *	@param	item	道具数据
	 */
	void showBatchPop(cocos2d::Value item);
    /**
     *  含有延时的移除自身
     */
    void removeSelf() override;
    
    void updateProperty() override;
	/**
	 *	是否是主城商店
	 */
	bool isManor();
	/**
	 *	获取模板
	 */
	cocos2d::Node* getPattern(bool huge = false);
protected:
    bool init();
    
    void onEnter() override;
    
    void onExit() override;
    
    void showBoard();
    
    
	virtual ~Shop();
    
    /**
     *  返回按钮的回调
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void backReal();
    
    /**
     *  添加批量购买层
     *
     *  @param itemData 数据
     */
    virtual void addPurchaseBatchPop(cocos2d::ValueMap itemData);
    
    /**
     * 添加批量rmb购买层
     *
     *  @param itemData 数据
     */
    void addShopRmbBatchPop(cocos2d::ValueMap itemData);
    
    /**
     *  展示信息
     *
     *  @param std::string 信息
     */
    void showtext(std::string txt, cocos2d::Node* node);
    
    /**
     *  可购买事件
     */
    void onTaskComplete(cocos2d::EventCustom* e);
    
    void onItemChange(cocos2d::EventCustom* e);
private:  
    /**
     *  商店ID
     */
    int _sid = 0;
	/**
	*	列表容器
	*/
	cocos2d::ui::ListView* _list;
	/**
	*	底部遮罩
	*/
	cocos2d::ui::Layout* _bottomMask;
	/**
	*	间隔
	*/
	const float ITEM_SPACE = 50.0f;
	/**
	*	站位
	*/
	cocos2d::ui::Layout* _space;
	/**
	*	模块模板(小)
	*/
	cocos2d::ui::Layout* _itemPattern;
	/**
	*	模块模板(大)
	*/
	cocos2d::ui::Layout* _itemHugePattern;
	/**
	*  board尺寸改变
	*  @param e
	*/
	void onBoardResize(cocos2d::EventCustom* e);
	/**
	*	列表改变尺寸
	*/
	void listResize();
	/**
	*	根据id获取元素
	*/
	Shop::Item* getItemByID(int id);
    
    bool isUpdateList = false;
    
    bool isOpened = false;
    
    Node* view;
    
    cocos2d::Value preShopListData = cocos2d::Value();
};
class Shop::Item : public cocos2d::ui::Layout
{
public:
	CREATE_FUNC(Item);
	virtual ~Item();
	/**
	 *	设置数据
	 */
	static void setDelegate(Shop* param){
		_delegate = param;
	}
	/**
	 *	设置数据
	 */
	void setData(cocos2d::Value data);
	/**
	 *  获得iid
	 *  @return iid
	 */
	inline int getIID()
	{
		return _data.asValueMap()["iid"].asInt();
	}

	/**
	*  设置刷新状态
	*/
	inline void setUpdated(bool upDated)
	{
		_upDated = upDated;
	}

	inline bool isUpdated()
	{
		return _upDated;
	}

	/**
	*  更新显示
	*/
	void updateView(cocos2d::ValueMap data);
	

	bool _upDated;

protected:

	/**
	*  初始化
	*
	*  @return 初始化是否成功
	*/
	bool init();

private:
	/**
	*  购买按钮的回调
	*
	*  @param sender  按钮
	*  @param event 事件
	*/
	void buyButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event);
	void buyAction();

	/**
	*  信息按钮的回调
	*
	*  @param sender 按钮
	*  @param event  事件
	*/
	void infoBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event);
	void showinfo();

	/**
	*  rmb购买按钮的回调
	*
	*  @param sender 按钮
	*  @param event  事件
	*/
	void rmbBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event);

	/**
	 *	道具改变
	 */
	void onItemChange(cocos2d::EventCustom* e);
private:
	/**
	*  数据
	*/
	cocos2d::Value _data;

	cocos2d::EventListenerCustom *_listenerItem;

	/**
	*  商店层的代理
	*/
	static Shop *_delegate;
	/**
	*  克隆的显示层
	*/
	cocos2d::Node* _view = nullptr;
	/**
	*  长按动画的Tag值
	*/
	const int LongPressActionTag = 10;

	/**
	*  是否是长按
	*/
	bool _isLongPress;

	/**
	 *  道具iid
	 */
	int _iid;
    
    cocos2d::ui::LoadingBar* _itemBar = nullptr;

};


#endif /* defined(__deer__StoreLayer__) */

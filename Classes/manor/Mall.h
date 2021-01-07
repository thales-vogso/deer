/****************************************************************************
    
 *	@desc	商城
 *	@date	2015-3-29
 *	@author	110101
     
 *	@file	manor/Mall.h
 *  @modify	minamoto
 ******************************************************************************/

#ifndef __MONOR_MALL__
#define __MONOR_MALL__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "LayerBase.h"
#include "core/custom/Caches.h"

class Mall : public cocos2d::ui::Layout, public LayerBase, public custom::Caches
{
    class Item;
    
public:
	CREATE_FUNC(Mall);
    
    void removeSelf();
    void updateProperty();
	/**
	 *	获取模板克隆
	 */
	cocos2d::Node* getPattern();
protected:
    
    bool init();
	virtual ~Mall();
    
    /**
     *   返回按钮的回调
     */
    void gobackCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    /**
     *  执行返回草做
     */
    void backReal();
    
    /**
     *  刷新项目
     */
    void upItems();
	/**
	 *	通过id获取商城元件
	 */
	Item* getItemByID(int id);

    /**
     *  当玩家mall的数据改变时的回调
     *
     *  @param e 事件数据
     */
    void onMallChange(cocos2d::EventCustom *e);
    
    /**
     *  当玩家钻石的数据改变时的回调
     *
     *  @param e 事件数据
     */
    void onCrystalChange(cocos2d::EventCustom *e);
    /**
     *  展示确认弹层
     */
    void showSurePop(cocos2d::Value data);
	/***/
private:
	/**
	 *	列表容器
	 */
	cocos2d::ui::ScrollView* _list;
	/**
	 *	元件模型
	 */
	cocos2d::ui::Layout* _itemPattern;
    /**
     *  这个数组保存所有在界面上出现的item
     */
    cocos2d::Vector<Item*> _items;
	/**
	 *	高度间距
	 */
	const int ITEM_HEIGHT = 140;
    
    cocos2d::ui::Text* _rmbTxt;
    
};

class Mall::Item : public cocos2d::ui::Layout
{
public:
	CREATE_FUNC(Item);
	/**
	*	析构
	*/
	virtual ~Item();
    
	/**
	 *	设置代理
	 */
	static void setDelegate(Mall* param){
		_delegate = param;
	}
	/**
	 *	设置数据
	 */
	void setData(cocos2d::Value data);
	/**
	 *	获取数据
	 */
	cocos2d::Value getData(){
		return _data;
	}
	/**
	 *	获取id
	 */
	int getID(){
		return _data.asValueMap()["mall_id"].asInt();
	}
    /**
     *  更新显示
     */
    void upView(cocos2d::ValueMap data);
    
protected:
    
	bool init();
	/**
	 *	购买点击
	 */
	void buyBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
	/**
	 *	确定购买
	 */
	void buyReal();
	/**
	 *  克隆的显示层
	 */
	cocos2d::Node* _view;
	/**
	 *	数据
	 */
	cocos2d::Value _data;
    /**
     *  代理父级
     */
	static Mall* _delegate;
};


#endif // __MONOR_MALL__

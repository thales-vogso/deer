/****************************************************************************
    
 *	@desc	建造界面
 *	@date	2015-6-17
 *	@author	110101
    
 *	@file	manor/Build.h
 *  @modify	null
 ******************************************************************************/

#ifndef __MANOR_BUILD__
#define __MANOR_BUILD__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Caches.h"
#include "manor/LayerBase.h"
#include "guide/Guide.h"

class Build : public cocos2d::Layer, public custom::Caches,  public LayerBase{
	class Item;
public:
	CREATE_FUNC(Build);
	/**
	*  刷新列表
	*/
	void updateList();
	/**
	 *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
	 */
	void removeSelf() override;
    
    void updateProperty() override;

	cocos2d::Node* getPattern(bool huge = false);
protected:
    
	virtual ~Build();
    
    bool init();
	/**
	*  返回按钮的回调
	*/
	void backBtnClick(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
	void backExe();
	/**
	*  展示建筑的信息
	*/
	void showInfo(std::string text, cocos2d::Node* node);
	/**
	 *	重构
	 */
	void onEnter() override;
	void onExit() override;
	/**
	 *	显示board栏
	 */
	void showBoard();
	/**
	 *	运动间隔
	 */
	static const float ACT_DURATION;
    
    /**
     *  csb文件结点tag
     */
    static const int TAG;
    
private:
	/**
	 *	容器
	 */
	cocos2d::ui::Layout* _panel;
	/**
	*	列表容器
	*/
	cocos2d::ui::ScrollView* _list;
	/**
	 *	底部遮罩
	 */
	cocos2d::ui::Layout* _bottomMask;
    /**
     *  顶部遮罩
     */
    cocos2d::ui::Layout* _topMask;
    
	/**
	 *	旧坐标
	 */
	cocos2d::ValueMapIntKey _oldCoord;
	/**
	 *	间隔
	 */
	const float ITEM_SPACE = 30.0f;
    float _listInnerHeight = 0.0f;
	
	/**
	*	模块模板(小)
	*/
	cocos2d::ui::Layout* _itemPattern;
	/**
	*	模块模板(大)
	*/
	cocos2d::ui::Layout* _itemHugePattern;
	/**
	*  建筑界面的侦听器字典
	*/
	cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
	/**
	 *	列表顶部间距
	 */
	const float LIST_MARGIN = 188.0f;
    
    /**
     *  列表最高点
     */
    const float _listMaxHeight = 1055;
    const float _topMaskMaxHeight = 1005;  //28
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
	*  任务完成侦听
	*  @param e
	*/
	void onBuildChange(cocos2d::EventCustom* e);
	void onItemChange(cocos2d::EventCustom* e);
    
	/**
	*	根据id获取元素
	*/
	Build::Item* getItemByID(int id);
    
    /**
     *  新手引导手指
     */
    Guide* _guide;
    
    /**
     *  根据建筑item更新可点击按钮区域
     */
    void updateBtnsArea(Item* build);
    
    cocos2d::Value preBuildData = cocos2d::Value();
};
class Build::Item : public cocos2d::ui::Layout{
public:
	CREATE_FUNC(Item);
	/**
	*	析构
	*/
	virtual ~Item();
	/**
	*	初始化
	*/
	bool init();
	/**
	*	设置代理
	*/
	static void setDelegate(Build* param){
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
		return _data.asValueMap()["bid"].asInt();
	}
	/**
	 *	渐现
	 */
	void fadeIn();
    
    /**
     *  建筑id，用于新手引导
     */
    int _buildId;
    
private:
	/**
	*	代理父级
	*/
	static Build* _delegate;
	/**
	*	数据
	*/
	cocos2d::Value _data;
	/**
	*  克隆的显示层
	*/
	cocos2d::Node* _view = nullptr;
	/**
	 *	信息标签
	 */
	const int INFO_BTN_ACT = 12;
	const int BTNACT_TAG = 11;
	const float BTN_FADETIME = 0.3;
	/**
	 *	材料文字间隔
	 */
	const float MATERIAL_SPACE = 20.0f;
	/**
	*  触及回调
	*  @param s	名字按钮
	*  @param e	触摸类型
	*/
	void nameClick(cocos2d::Ref * s, cocos2d::ui::Button::TouchEventType e);
	/**
	*  建造
	*/
	void build();
	/**
	*  信息按钮的回调
	*  @param s	按钮
	*  @param e	触摸类型
	*/
	void infoClick(cocos2d::Ref * s, cocos2d::ui::Button::TouchEventType e);
	/**
	*   展示信息
	*/
	void showInfo();
    
};
#endif //__MANOR_BUILD__
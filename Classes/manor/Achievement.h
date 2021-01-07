/****************************************************************************
 *	@desc   成就界面
 *	@date	2015-1-15
 *	@author	110101
     
 *	@file	manor/Achievement.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_ACHIEVEMENT_H__
#define __DEER_ACHIEVEMENT_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "manor/LayerBase.h"

class Achievement : public cocos2d::Layer, public LayerBase
{
	class Item;
public:
	virtual ~Achievement();
    
	CREATE_FUNC(Achievement);
    
    virtual void removeSelf() override;
    virtual void updateProperty() override;
    
    /**
     *	更新元素
     */
    void upItems();
    
    void freshList();

protected:
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init();
    
    void onEnter() override;
    
    void showBoard();
    
private:

    /**
     *  返回按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 触摸事件类型
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType);
    void backReal();
    

	/**
	*  显示信息
	*
	*  @param text 信息
	*/
	void showInfo(std::string text, cocos2d::Node* node);


	/**
	*  滑动框变长
	*
	*  @param e
	*/
	void setListLong(cocos2d::EventCustom* e);

	/**
	*  滑动框变短
	*
	*  @param e
	*/
	void setListShort(cocos2d::EventCustom* e);

	/**
	*  模板字典
	*/
	cocos2d::Map<int, cocos2d::ui::Layout*> _itemModel;
	/**
	 *	 显示层
	 */
	cocos2d::Node* _view;
    
    /**
     *  成就点数文本框
     **/
    cocos2d::ui::TextBMFont* _pointTxt = nullptr;
    
    cocos2d::Value preAchievementData = cocos2d::Value();
public:
	/**
	 *	元件模型
	 */
	cocos2d::ui::Layout* _itemPattern;
	/**
	 *	间距
	 */
	int SPACE_H = 100;
    
    /**
     *  布局
     */
    int     THREE =3;
    float   TWO   =2.0;
    int     _rowNum = 0;
    
    /**
	*  用于监听滑动退出操作
	*/
	cocos2d::EventListenerTouchOneByOne* _touchlistener;

private:
    
    enum AchvmtType
    {
        kTrain = 210100,
        kGamble = 310100
    };
    
    
    

};
class Achievement::Item : public cocos2d::ui::Layout
{
public:
	/**
	*  实例创建函数
	*
	*  @param adata 数据
	*
	*  @return 实例
	*/
	static Item *create(cocos2d::Value adata, Achievement* delegate);

CC_CONSTRUCTOR_ACCESS:

	bool init(cocos2d::Value adata);
private:

	/**
	*  查看回调
	*
	*  @param sender 按钮
	*  @param tevent 触摸方式
	*/
	void infoCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType tevent);

	/**
	*  临时显示
	*/
	cocos2d::Label* _completed;

	/**
	*  成就信息
	*/
	cocos2d::Value _adata;

	/**
	*  代理
	*/
	Achievement *_delegate;
};
#endif //__DEER_ACHIEVEMENT_H__

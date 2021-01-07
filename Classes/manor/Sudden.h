/****************************************************************************
 
 *	@desc	随机事件
 *	@date	2015-5-20
 *	@author	110101
     
 *	@file	manor/Sudden.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_SUDDEN__
#define __DEER_SUDDEN__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"

class Sudden : public cocos2d::ui::Layout
{
public:
    static Sudden *create(cocos2d::ValueMap data);
    
protected:
    bool init(cocos2d::ValueMap data);
    
    virtual void onEnter();
    
    virtual ~Sudden();
    
	/**
	 *	确定
	 */
    void onConfirmClick(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void confirmExe();
	/**
	 *	人民币购买
	 */
    void onRMBClick(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void RMBExe();
	/**
	 *	取消
	 */
	void onCancelClick(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
	void cancelExe();
    
    cocos2d::ValueMap _data;
private:
	const float confirmBtnInitX = 325.0f;
	cocos2d::Node* _view;
};

#endif /* defined(__deer__Sudden__) */

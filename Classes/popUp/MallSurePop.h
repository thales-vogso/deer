/****************************************************************************
 *	@desc   商城的确认弹窗
 *	@date	2015-5-22
 *	@author	110101
     
 *	@file	popUp/MallSurePop.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_MALLSUREPOP__
#define __DEER_MALLSUREPOP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "manor/LayerNoTouchTrans.h"

/**
 *  这是一个商城中的商品购买确认的弹窗
 */
class MallSurePop : public cocos2d::ui::Layout , public LayerNoTouchTrans
{
public:
    /**
     *  create 创建方法
     *
     *  @param data mall中的商品的数据
     *
     *  @return 一个弹窗
     */
    static MallSurePop *create(cocos2d::ValueMap data);
    
    virtual void onEnter();
    
protected:
    
    /**
     *  initialize
     *
     *  @param data mall中的商品的数据
     *
     *  @return if this object initialized successful
     */
    bool init(cocos2d::ValueMap data);
    
    /**
     *  确认按钮的回调
     *
     *  @param sender 按钮
     *  @param t      事件
     */
    void onSureButton(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void sureReal();
    
    /**
     *  放弃按钮的回调
     *
     *  @param sender 按钮
     *  @param t      事件
     */
    void onQuitButton(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void quitReal();
    
    
    cocos2d::ValueMap _data;
};

#endif /* defined(__deer__MallSurePop__) */

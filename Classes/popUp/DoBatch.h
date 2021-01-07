/****************************************************************************
 *	@desc   批量操作
 *	@date	2015-5-27
 *	@author	110101
     
 *	@file	popUp/DoBatch.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_DOBATCH__
#define __DEER_DOBATCH__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "manor/LayerNoTouchTrans.h"
class PurchaseBatchPop : public cocos2d::ui::Layout , public LayerNoTouchTrans
{
public:
    static PurchaseBatchPop *create(cocos2d::ValueMap itemData);
    
CC_CONSTRUCTOR_ACCESS:
    bool init(cocos2d::ValueMap itemData);
    
    /**
     *  确定按钮的回调
     *
     *  @param sender 按钮
     *  @param type   事件
     */
    virtual void sureButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
    
    /**
     *  取消按钮的回调
     *
     *  @param sender 按钮
     *  @param type   事件
     */
    void cancelButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType type);
    
protected:
    /**
     *  按钮的字典
     */
    cocos2d::Map<std::string, cocos2d::ui::Widget*> _buttonMap;
    
    /**
     *  数据
     */
    cocos2d::ValueMap _itemData;
    /**
     *  购买数量
     */
    int _itemNum;
};

class SellBatchPop : public PurchaseBatchPop
{
public:
    static SellBatchPop *create(cocos2d::ValueMap idata);
protected:
    bool init(cocos2d::ValueMap idata);
    
    virtual void sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type);
};

class ProduceBatch : public PurchaseBatchPop
{
public:
    static ProduceBatch *create(cocos2d::ValueMap data);
protected:
    bool init(cocos2d::ValueMap data);
    
    virtual void sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type);
};


#endif /* defined(__deer__DoBatch__) */

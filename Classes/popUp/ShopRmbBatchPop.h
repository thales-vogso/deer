//
//  shopRmbBatchPop.h
//  deer
//
//  Created by xinhua on 15/6/12.
//
//

#ifndef __deer__shopRmbBatchPop__
#define __deer__shopRmbBatchPop__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "manor/LayerNoTouchTrans.h"
class ShopRmbBatchPop : public cocos2d::ui::Layout , public LayerNoTouchTrans
{
public:
    static ShopRmbBatchPop *create(cocos2d::ValueMap itemData);
    
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

#endif /* defined(__deer__shopRmbBatchPop__) */

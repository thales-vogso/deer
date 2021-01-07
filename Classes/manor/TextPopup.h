/****************************************************************************
  
 *	@desc	建造界面
 *	@date	2015-3-21
 *	@author	110101
     
 *	@file	manor/TextPopup.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_TEXTPOPUP__
#define __DEER_TEXTPOPUP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "popUp/BlackMask.h"

class TextPopup : public cocos2d::ui::Layout
{
public:
    virtual ~TextPopup();
    
    CREATE_FUNC(TextPopup);
    
    /**
     *  设置显示内容
     *
     *  @param txt 显示内容
     */
    void setTextInfo(std::string txt);
    
    /**
     *  添加不弹出区域
     *
     *  @param init
     */
    void setUnPopRect(cocos2d::Rect rect);
    
    
protected:
    bool init();
    
    void remove1(cocos2d::Ref*, cocos2d::ui::Layout::TouchEventType type);
    
    void remove2(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType t);
    
    cocos2d::EventListenerTouchOneByOne* _li;
    
    /**
     *  点击这个区域只关闭，不在打开。
     */
    cocos2d::Rect _rect;
};

#endif /* defined(__deer__TextPopup__) */

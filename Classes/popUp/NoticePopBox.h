/****************************************************************************
 *	@desc   信息弹窗
 *	@date	2015-2-28
 *	@author	110101
     
 *	@file	popUp/NoticePopBox.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_NOTICEPOPBOX__
#define __DEER_NOTICEPOPBOX__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class NoticePopBox : public cocos2d::ui::Layout
{
public:
    static NoticePopBox* create(std::string text);
    
CC_CONSTRUCTOR_ACCESS:
    bool init(std::string text);
    
private:
    void touchCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType tevet);
};

#endif /* defined(__deer__NoticePopBox__) */

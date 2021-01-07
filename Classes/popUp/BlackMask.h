/****************************************************************************
 *	@desc   背景遮罩
 *	@date	2015-3-6
 *	@author	110101
     
 *	@file	popUp/BlackMask.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_BLACKMASK__
#define __DEER_BLACKMASK__

#include <iostream>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class BlackMask : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(BlackMask);
CC_CONSTRUCTOR_ACCESS:
    
    virtual ~BlackMask();
    
    bool init();
};

#endif /* defined(__deer__BlaceMask__) */

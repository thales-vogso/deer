/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-2-4
 *	@author	110101
     
 *	@file	crusade/WildStore.h
 *   @modify	null
 ******************************************************************************/


#ifndef __DEER_WILDSTORE__
#define __DEER_WILDSTORE__


#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class WildStore : public cocos2d::ui::Layout
{
public:
    static WildStore *create(int sid);
    
CC_CONSTRUCTOR_ACCESS:
    bool init(int sid);
    
private:
    
    class WilStoreListItem : public cocos2d::ui::Layout
    {
    public:
//        static WilStoreListItem* create();
    };
};

#endif /* defined(__deer__WildStore__) */

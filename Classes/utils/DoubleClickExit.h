/****************************************************************************
 *	@desc   双击退出
 *	@date	2015-4-25
 *	@author	110101
     
 *	@file	utils/DoubleClickExit.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_COUBLECLICKEXIT__
#define __DEER_COUBLECLICKEXIT__
#include "cocos2d.h"

class DoubleClickExit : public cocos2d::Node
{
public:
    CREATE_FUNC(DoubleClickExit);
    
CC_CONSTRUCTOR_ACCESS:
    bool init();
    
    /**
     *  清除预退出标识位
     */
    void purgePrequit();
    
    /**
     *  预退出标识位
     */
    bool _prequit;
};

#endif /* defined(__deer__DoubleClickExit__) */

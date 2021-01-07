/****************************************************************************
 
 *	@desc	logo
 *	@date	2014-12-12
 *	@author	110101
 
 *	@file	Loading/UMLogo.h
 ******************************************************************************/

#ifndef __deer__UMLogo__
#define __deer__UMLogo__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "gateWay/GateWayDelegate.h"
class UMLogo : public cocos2d::ui::Layout
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(UMLogo);
private:
    
    bool init();
};
#endif /* defined(__deer__UMLogo__) */

/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-4-2
 *	@author	110101
     
 *	@file	crusade/GobackAsk.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_GOBACKASK__
#define __DEER_GOBACKASK__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GobackAsk : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(GobackAsk);
    
protected:
    bool init();
    virtual void onEnter();
    
    void sureBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void sureReal();
    
    void quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void quitReal();
    
    /**
     *  需要付费
     */
    bool _needPurchase;
    
    std::string _sureBtnName;
    std::string _quitBtnName;
    std::string _price;
};

#endif /* defined(__deer__GobackAsk__) */

//
//  CommenPopup.h
//  deer
//
//  Created by allyiaa on 15-4-10.
//
//

#ifndef __DEER_COMMENPOPUP__
#define __DEER_COMMENPOPUP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class VictoryPopup : public cocos2d::Layer
{
public:
    static VictoryPopup *create(cocos2d::ValueMap data);
    
protected:
    bool init(cocos2d::ValueMap data);
    
    void quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    
    
    cocos2d::ValueMap _data;
};

#include <iostream>

#endif /* defined(__deer__CommenPopup__) */

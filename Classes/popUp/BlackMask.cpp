/****************************************************************************
 *	@desc   信息弹窗
 *	@date	2015-3-6
 *	@author	110101
     
 *	@file	popUp/BlackMask.h
 *   @modify	null
 ******************************************************************************/
#include "BlackMask.h"
#include "utils/DeerUtils.h"
USING_NS_CC;
BlackMask::~BlackMask()
{
    
}

bool BlackMask::init()
{
    Layout::init();
    this->setContentSize(Director::getInstance()->getWinSize());
    this->setBackGroundColor(cocos2d::Color3B::BLACK);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setTouchEnabled(true);
    this->setPropagateTouchEvents(false);
    return true;
}
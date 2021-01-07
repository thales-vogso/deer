/****************************************************************************
 *	@desc   信息弹窗
 *	@date	2015-2-28
 *	@author	110101
     
 *	@file	popUp/NoticePopBox.cpp
 *   @modify	null
 ******************************************************************************/

#include "NoticePopBox.h"
USING_NS_CC;

NoticePopBox *NoticePopBox::create(std::string text)
{
    NoticePopBox* ret =new NoticePopBox();
    if (ret && ret->init(text))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool NoticePopBox::init(std::string text)
{
    if (!Layout::init())
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    
    this->setAnchorPoint(Vec2(0 ,0));
    this->setContentSize(winSize);
    this->setBackGroundColorType(BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B::BLACK);
    this->setOpacity(150);
    this->setTouchEnabled(true);

    ui::Text * content = ui::Text::create(text, "", 60);
    content->setPosition(winSize/2);
    content->setAnchorPoint(Vec2(0.5, 0.5));
    content->setTextAreaSize(Size(700, 700));
    content->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    content->setTextVerticalAlignment(cocos2d::TextVAlignment::TOP);
    content->ignoreContentAdaptWithSize(false);
    content->setTouchEnabled(true);
    content->addTouchEventListener(CC_CALLBACK_2(NoticePopBox::touchCallback, this));
    this->addChild(content);
    
    return true;
}

void NoticePopBox::touchCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType tevet)
{
    if (tevet == cocos2d::ui::Text::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}
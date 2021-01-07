/****************************************************************************
 *	@desc   选择弹窗
 *	@date	2015-1-28
 *	@author	110101
     
 *	@file   popUp/ChoicePopup.h
 *   @modify	null
 ******************************************************************************/

#include "ChoicePopup.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
USING_NS_CC;
USING_NS_DU;
bool ChoicePopup::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    /**
     *  添加遮罩
     */
    this->addChild(BlackMask::create());
    
    Size winSize = Director::getInstance()->getWinSize();
    
    this->setContentSize(winSize);
    this->setAnchorPoint(Vec2(0, 0));
    
    _sureCallback = nullptr;
    _quitCallback = nullptr;
    
    
    _sureBtn = ui::Text::create("sure", "", 60);
    _sureBtn->setPosition(Vec2(winSize.width * 0.3, winSize.height * 0.4));
    this->addChild(_sureBtn, 1, "sureButton");
    
//    auto
    
//    auto sureBtn = seekChildByNameWithRetType<ui::Text*>(this, "sureButton");
    _sureBtn->setTouchEnabled(true);
    _sureBtn->setTouchScaleChangeEnabled(true);
    _sureBtn->addTouchEventListener(CC_CALLBACK_2(ChoicePopup::sureBtnCall, this));
    
    _quitBtn = ui::Text::create("quit", "", 60);
    _quitBtn->setPosition(Vec2(winSize.width * 0.6, winSize.height * 0.4));
    this->addChild(_quitBtn, 1, "quitButton");
    
//    auto quitBtn = seekChildByNameWithRetType<ui::Text*>(this, "quitButton");
    _quitBtn->setTouchEnabled(true);
    _quitBtn->setTouchScaleChangeEnabled(true);
    _quitBtn->addTouchEventListener(CC_CALLBACK_2(ChoicePopup::quitBtnCall, this));
    
    auto title = ui::Text::create("", "", 60);
    title->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.7));
    this->addChild(title, 1, "Title");
    
    auto content = ui::Text::create("", "", 60);
    content->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.7));
    this->addChild(content, 1, "content");
    
    return true;
}

void ChoicePopup::setTitle(std::string title)
{
    seekChildByNameWithRetType<ui::Text*>(this, "Title")->setString(title);
}

void ChoicePopup::setContent(std::string text)
{
    seekChildByNameWithRetType<ui::Text*>(this, "content")->setString(text);
}

void ChoicePopup::setSureCallback(std::function<void (int)> callback)
{
    _sureCallback = callback;
}

void ChoicePopup::setQuitCallback(std::function<void (int)> callback)
{
    _quitCallback = callback;
}

void ChoicePopup::sureBtnCall(cocos2d::Ref *, cocos2d::ui::Text::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        _sureBtn->setTouchEnabled(false);
        if (_sureCallback)
        {
            _sureCallback(0);
        }
    }
}

void ChoicePopup::quitBtnCall(cocos2d::Ref *, cocos2d::ui::Text::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        _quitBtn->setTouchEnabled(false);
        if (_quitCallback)
        {
            _quitCallback(0);
        }
    }
}
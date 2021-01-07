/****************************************************************************
  
 *	@desc	建造界面
 *	@date	2015-3-21
 *	@author	110101
     
 *	@file	manor/TextPopup.h
 *  @modify	null
 ******************************************************************************/

#include "TextPopup.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"

USING_NS_CC;
USING_NS_DU;

TextPopup::~TextPopup()
{
//    _eventDispatcher->removeEventListener(_li);
}

bool TextPopup::init()
{
    if (!Layout::init())
    {
        return false;
    }
//    addChild(BlackMask::create());
    
    this->setContentSize(Director::getInstance()->getWinSize());
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("textpop.csb");
    this->addChild(view);
    ui::Helper::doLayout(this);
    
    auto panel = seekChildByNameWithRetType<ui::Layout*>(this, "Panel");
    panel->addTouchEventListener(CC_CALLBACK_2(TextPopup::remove2, this));
//    seekChildByNameWithRetType<ui::Layout*>(this, "Panel")->setSwallowTouches(true);
    
    this->setTouchEnabled(true);
    
    this->addTouchEventListener(CC_CALLBACK_2(TextPopup::remove1, this));
    return true;
}

void TextPopup::setUnPopRect(cocos2d::Rect rect)
{
    _rect = rect;
}

void TextPopup::remove1(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType type)
{
    if(type == ui::Layout::TouchEventType::BEGAN)
    {
        auto rect = seekChildByNameWithRetType<ui::Layout*>(this, "Panel")->getBoundingBox();
        auto bepos = getTouchBeganPosition();
        if (!rect.containsPoint(getTouchBeganPosition()) && !_rect.containsPoint(getTouchBeganPosition()))
        {
            this->removeFromParent();
        }
    }
    else if (type == ui::Layout::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}

void TextPopup::remove2(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType t)
{
    if (t == ui::Layout::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}

void TextPopup::setTextInfo(std::string txt)
{
    auto text = seekChildByNameWithRetType<ui::Text*>(this, "text");
    text->setString(txt);
}
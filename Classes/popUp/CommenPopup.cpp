//
//  CommenPopup.cpp
//  deer
//
//  Created by allyiaa on 15-4-10.
//
//

#include "CommenPopup.h"
#include "editor-support/cocostudio/CocoStudio.h"

#include "utils/DeerUtils.h"
USING_NS_CC;
USING_NS_DU;

VictoryPopup *VictoryPopup::create(cocos2d::ValueMap data)
{
    auto ret = new VictoryPopup();
    if (ret && ret->init(data))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return NULL;
    }
}

bool VictoryPopup::init(cocos2d::ValueMap data)
{
    if (!Layer::init())
    {
        return false;
    }
    auto win = Director::getInstance()->getWinSize();
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("buildingVictory.csb");
    
    view->setContentSize(win);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    _data = data;
    
    seekChildByNameWithRetType<ui::Text*>(view, "title")->setString(_data["name"].asString());;
    
    seekChildByNameWithRetType<ui::Text*>(view, "content")->setString(_data["masage_leave"].asString());
    
    seekChildByNameWithRetType<ui::Button*>(view, "quitBtn")->addTouchEventListener(CC_CALLBACK_2(VictoryPopup::quitBtnCall, this));
    return true;
}

void VictoryPopup::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Button*>(this, "quitBtn")->setTouchEnabled(false);
        CCLOG("去地图  ");
    }
}
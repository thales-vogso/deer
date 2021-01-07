//
//  shopRmbBatchPop.cpp
//  deer
//
//  Created by xinhua on 15/6/12.
//
//

#include "ShopRmbBatchPop.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "notice/Notice.h"
#include "data/Lang.h"
#include <stdio.h>
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;

ShopRmbBatchPop *ShopRmbBatchPop::create(cocos2d::ValueMap itemData)
{
    ShopRmbBatchPop *ret = new ShopRmbBatchPop();
    if (ret && ret->init(itemData))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return ret;
    }
}

bool ShopRmbBatchPop::init(cocos2d::ValueMap itemData)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _itemData = itemData;
    auto win = Director::getInstance()->getWinSize();
    
    addChild(BlackMask::create());
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("shop_rmbtanchuang.csb");
    auto viewRoot = seekChildByNameWithRetType<ui::Layout*>(view, "Panel");
    addChild(viewRoot);
    setContentSize(win);
    ui::Helper::doLayout(this);
    
    char titletxt[200];
    sprintf(titletxt,data::Lang::getInstance()->getWord(130302).c_str(),_itemData["name"].asString().c_str());
    seekChildByNameWithRetType<ui::Text*>(this, "title")->setString(titletxt);
    
    Value ten = Value(_itemData["rmb"].asFloat()*10);
    Value hundred = Value(_itemData["rmb"].asFloat()*100);
    Value thousand = Value(_itemData["rmb"].asFloat()*1000);
    
    char tenPrice[200];
    char huandredPrice[200];
    char thousandPrice[200];
    sprintf(tenPrice,seekChildByNameWithRetType<ui::Text*>(this, "tenPrice")->getString().c_str(),ten.asFloat());
    sprintf(huandredPrice,seekChildByNameWithRetType<ui::Text*>(this, "hundredPrice")->getString().c_str(),hundred.asFloat());
    sprintf(thousandPrice,seekChildByNameWithRetType<ui::Text*>(this, "tenPrice")->getString().c_str(),thousand.asFloat());
    
    seekChildByNameWithRetType<ui::Text*>(this, "tenPrice")->setString(tenPrice);
    seekChildByNameWithRetType<ui::Text*>(this, "hundredPrice")->setString(huandredPrice);
    seekChildByNameWithRetType<ui::Text*>(this, "thousandPrice")->setString(thousandPrice);
    
    seekChildByNameWithRetType<ui::Button*>(this, "tenBtn")->addTouchEventListener(CC_CALLBACK_2(ShopRmbBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Button*>(this, "hundredBtn")->addTouchEventListener(CC_CALLBACK_2(ShopRmbBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Button*>(this, "thousandBtn")->addTouchEventListener(CC_CALLBACK_2(ShopRmbBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Button*>(this, "quitBtn")->addTouchEventListener(CC_CALLBACK_2(ShopRmbBatchPop::cancelButtonCallback, this));
    
    return true;
}

void ShopRmbBatchPop::sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Button*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if (seekChildByNameWithRetType<ui::Button*>(this, "tenBtn") == sender)
        {
            auto ret = data::Item::getInstance()->cash(_itemData.at("iid").asFloat(), 10);
            if (ret.isNull())
            {
                Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130307));
            }
            else
            {
                char postTips[200];
                Value num = Value(10);
                sprintf(postTips, data::Lang::getInstance()->getWord(130308).c_str(),_itemData["name"].asString().c_str(),num.asString().c_str());
				Notice::getInstance()->postBoard(postTips);
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Button*>(this, "hundredBtn") == sender)
        {
            auto ret = data::Item::getInstance()->cash(_itemData.at("iid").asFloat(), 100);
            if (ret.isNull())
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130307));
            }
            else
            {
                char postTips[200];
                Value num = Value(100);
                sprintf(postTips, data::Lang::getInstance()->getWord(130308).c_str(),_itemData["name"].asString().c_str(),num.asString().c_str());
				Notice::getInstance()->postBoard(postTips);
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Button*>(this, "thousandBtn") == sender)
        {
            auto ret = data::Item::getInstance()->cash(_itemData.at("iid").asFloat(), 1000);
            if (ret.isNull())
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130307));
            }
            else
            {
                char postTips[200];
                Value num = Value(1000);
                sprintf(postTips, data::Lang::getInstance()->getWord(130308).c_str(),_itemData["name"].asString().c_str(),num.asString().c_str());
				Notice::getInstance()->postBoard(postTips);
            }
            removeFromParent();
        }
    }
}

void ShopRmbBatchPop::cancelButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Button*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        this->removeFromParent();
    }
}
/****************************************************************************
 *	@desc   批量操作
 *	@date	2015-5-27
 *	@author	110101
     
 *	@file	popUp/DoBatch.h
 *   @modify	null
 ******************************************************************************/

#include "DoBatch.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "notice/Notice.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;

PurchaseBatchPop *PurchaseBatchPop::create(cocos2d::ValueMap itemData)
{
    PurchaseBatchPop *ret = new PurchaseBatchPop();
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

bool PurchaseBatchPop::init(cocos2d::ValueMap itemData)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _itemData = itemData;
    auto win = Director::getInstance()->getWinSize();
    
    addChild(BlackMask::create());
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("PurchaseBatch.csb");
    auto viewRoot = seekChildByNameWithRetType<ui::Layout*>(view, "Panel");
    addChild(viewRoot);
    setContentSize(win);
    ui::Helper::doLayout(this);
    
    std::string titletxt = "请确定购买  " + _itemData["name"].asString() + "的数量..";
    
    seekChildByNameWithRetType<ui::Text*>(this, "title")->setString(titletxt);
    
    seekChildByNameWithRetType<ui::Text*>(this, "perPrice")->setString("单价:" + _itemData["price"].asString() + "金币");
    
    seekChildByNameWithRetType<ui::Layout*>(this, "tenBtnArea")->addTouchEventListener(CC_CALLBACK_2(PurchaseBatchPop::sureButtonCallback, this));
    
    
    seekChildByNameWithRetType<ui::Layout*>(this, "hundredBtnArea")->addTouchEventListener(CC_CALLBACK_2(PurchaseBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Layout*>(this, "thousandBtnArea")->addTouchEventListener(CC_CALLBACK_2(PurchaseBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Layout*>(this, "tenTousandBtnArea")->addTouchEventListener(CC_CALLBACK_2(PurchaseBatchPop::sureButtonCallback, this));
    
    seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->addTouchEventListener(CC_CALLBACK_2(PurchaseBatchPop::cancelButtonCallback, this));
    
    return true;
}

void PurchaseBatchPop::sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if (seekChildByNameWithRetType<ui::Layout*>(this, "tenBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->buy(_itemData.at("iid").asInt(), 10);
            if (ret.isNull())
            {
				Notice::getInstance()->postTips("金币不足. ");
            }
            else
            {
				Notice::getInstance()->postBoard("你购买了" + _itemData["name"].asString() + "X"+ret.asString());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "hundredBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->buy(_itemData.at("iid").asInt(), 100);
            if (ret.isNull())
            {
				Notice::getInstance()->postTips("金币不足. ");
            }
            else
            {
				Notice::getInstance()->postBoard("你购买了" + _itemData["name"].asString() + "X"+ret.asString());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "thousandBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->buy(_itemData.at("iid").asInt(), 1000);
            if (ret.isNull())
            {
				Notice::getInstance()->postTips("金币不足. ");
            }
            else
            {
				Notice::getInstance()->postBoard("你购买了" + _itemData["name"].asString() + "X"+ret.asString());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "tenTousandBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->buy(_itemData.at("iid").asInt(), 10000);
            if (ret.isNull())
            {
                Notice::getInstance()->postTips("金币不足. ");
            }
            else
            {
                Notice::getInstance()->postBoard("你购买了" + _itemData["name"].asString() + "X10000  ");
            }
            removeFromParent();
        }
    }
}

void PurchaseBatchPop::cancelButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        this->removeFromParent();
    }
}


SellBatchPop* SellBatchPop::create(cocos2d::ValueMap idata)
{
    auto ret = new SellBatchPop();
    if (ret && ret->init(idata))
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

bool SellBatchPop::init(cocos2d::ValueMap idata)
{
    if (!PurchaseBatchPop::init(idata))
    {
        return false;
    }
    
    std::string titletxt = "请确定卖出  " + _itemData["name"].asString() + "  的数量  ";
    seekChildByNameWithRetType<ui::Text*>(this, "title")->setString(titletxt);
    
    int amount =    data::Item::getInstance()->getUserItemAmountByIID(_itemData["iid"].asInt());
    if(amount < 10)
    {
        seekChildByNameWithRetType<ui::Text*>(this, "tentxt")->setString(Value(amount).asString());
    }
    
    return true;
}

void SellBatchPop::sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if (seekChildByNameWithRetType<ui::Layout*>(this, "tenBtnArea") == sender)
        {
            int ret = data::Item::getInstance()->sell(_itemData.at("iid").asInt(), 10);
            if (ret == 0)
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130309));
            }
            else
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130310).c_str(),_itemData["name"].asString().c_str(),ret);
				Notice::getInstance()->postBoard(word);
				Notice::getInstance()->postTips(word);
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "hundredBtnArea") == sender)
        {
            int ret = data::Item::getInstance()->sell(_itemData.at("iid").asInt(), 100);
            if (ret == 0)
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130309));
            }
            else
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130310).c_str(),_itemData["name"].asString().c_str(),ret);
				Notice::getInstance()->postBoard(word);
				Notice::getInstance()->postTips(word);
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "thousandBtnArea") == sender)
        {
            int ret = data::Item::getInstance()->sell(_itemData.at("iid").asInt(), 1000);
            if (ret == 0)
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130309));
            }
            else
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130310).c_str(),_itemData["name"].asString().c_str(),ret);
				Notice::getInstance()->postBoard(word);
				Notice::getInstance()->postTips(word);
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "tenTousandBtnArea") == sender)
        {
            int ret = data::Item::getInstance()->sell(_itemData.at("iid").asInt(), 10000);
            if (ret == 0)
            {
                Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130309));
            }
            else
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130310).c_str(),_itemData["name"].asString().c_str(),ret);
                Notice::getInstance()->postBoard(word);
                Notice::getInstance()->postTips(word);
            }
            removeFromParent();
        }
    }
}

ProduceBatch* ProduceBatch::create(cocos2d::ValueMap data)
{
    auto ret = new ProduceBatch();
    if (ret && ret->init(data))
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

bool ProduceBatch::init(cocos2d::ValueMap data)
{
    if (!PurchaseBatchPop::init(data))
    {
        return false;
    }
    
    std::string str = "请确定生产  " + _itemData["name"].asString() + "  的数量  ";
    //seekChildByNameWithRetType<ui::Text*>(this, "title")->setString(titletxt);
	getChildByName("Panel")->getChildByName<ui::Text*>("title")->setString(str);
	getChildByName("Panel")->getChildByName<ui::Text*>("perPrice")->setVisible(false);

    return true;
}

void ProduceBatch::sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        ((ui::Layout*)sender)->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if (seekChildByNameWithRetType<ui::Layout*>(this, "tenBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->produce(_itemData.at("iid").asInt(), 10);
            if (ret.asValueMap()["code"].asInt() == 0)
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130316).c_str(),ret.asValueMap()["num"].asInt(),_itemData["name"].asString().c_str());
				Notice::getInstance()->postBoard(word);
            }
            else
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130317).c_str());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "hundredBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->produce(_itemData.at("iid").asInt(), 100);
            if (ret.asValueMap()["code"].asInt() == 0)
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130316).c_str(),ret.asValueMap()["num"].asInt(),_itemData["name"].asString().c_str());
                Notice::getInstance()->postBoard(word);
            }
            else
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130317).c_str());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "thousandBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->produce(_itemData.at("iid").asInt(), 1000);
            if (ret.asValueMap()["code"].asInt() == 0)
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130316).c_str(),ret.asValueMap()["num"].asInt(),_itemData["name"].asString().c_str());
                Notice::getInstance()->postBoard(word);
                
            }
            else
            {
				Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130317).c_str());
            }
            removeFromParent();
        }
        else if (seekChildByNameWithRetType<ui::Layout*>(this, "tenTousandBtnArea") == sender)
        {
            auto ret = data::Item::getInstance()->produce(_itemData.at("iid").asInt(), 10000);
            if (ret.asValueMap()["code"].asInt() == 0)
            {
                char word[200];
                sprintf(word, data::Lang::getInstance()->getWord(130316).c_str(),10000,_itemData["name"].asString().c_str());
                Notice::getInstance()->postBoard(word);
                
            }
            else
            {
                Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(130317).c_str());
            }
            removeFromParent();
        }
    }
}

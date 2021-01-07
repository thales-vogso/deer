/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-4-2
 *	@author	110101
     
 *	@file	crusade/GobackAsk.cpp
 *   @modify	null
 ******************************************************************************/

#include "GobackAsk.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "data/item.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"
#include "notice/Notice.h"

USING_NS_CC;
USING_NS_DU;
bool GobackAsk::init(){
    if (!Layout::create())
    {
        return false;
    }
    
    //这里需要判断回程卷的数量..
    addChild(BlackMask::create());
    auto win = Director::getInstance()->getWinSize();
    
    auto csnode = CSLoader::getInstance()->createNodeWithFlatBuffersFile("goManor.csb");
    
    auto price = seekChildByNameWithRetType<ui::Text*>(csnode, "price");
    _price = price->getString();
    
    _needPurchase = !data::Item::getInstance()->getUserItemAmountByIID(data::Item::WORMHOLE);
   
    ui::Layout* view;
    if (_needPurchase)//没有虫洞..
    {
        view = seekChildByNameWithRetType<ui::Layout*>(csnode, "purchasePanel");
    }
    else
    {
        view = seekChildByNameWithRetType<ui::Layout*>(csnode, "commenPanel");
    }
    
    this->setContentSize(win);
    addChild(view);
    
    ui::Helper::doLayout(this);
    
    
    if (_needPurchase)//没有虫洞..
    {
        _sureBtnName = "sureBtnArea";
        _quitBtnName = "quitBtnArea";
    }
    else
    {
        _sureBtnName = "csureBtnArea";
        _quitBtnName = "cquitBtnArea";
    }
    
    seekChildByNameWithRetType<ui::Layout*>(this, _sureBtnName)->addTouchEventListener(CC_CALLBACK_2(GobackAsk::sureBtnCall, this));
    
    seekChildByNameWithRetType<ui::Layout*>(this, _quitBtnName)->addTouchEventListener(CC_CALLBACK_2(GobackAsk::quitBtnCall, this));
    
    return true;
}
void GobackAsk::onEnter()
{
    Layout::onEnter();
    addChild(Notice::getInstance()->getTips());
}
void GobackAsk::sureBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureReal();
    }
}

void GobackAsk::sureReal()
{
    if (_needPurchase)
    {
#if DEBUG_GOMANOR_FREE == 1
        data::Crusade::getInstance()->townPortal();
        Manor::goToManorScene();
        return;
//        Director::getInstance()->
#endif
        int crystal = data::Item::getInstance()->getUserItemAmountByIID(data::Item::ItemID::CRYSTAL);
        if (crystal >= Value(_price).asInt())
        {
            data::Item::getInstance()->userItemReduce(data::Item::ItemID::CRYSTAL, Value(_price).asInt());
            data::Crusade::getInstance()->townPortal();
            Manor::goToManorScene();
        }
        else
        {
            std::string str = "钻石数目不足";
            Notice::getInstance()->postTips(str);
        }
    }
    else
    {
        data::Item::getInstance()->userItemReduce(data::Item::WORMHOLE);
        data::Crusade::getInstance()->townPortal();
        Manor::goToManorScene();
    }
}

void GobackAsk::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitReal();
    }
}

void GobackAsk::quitReal()
{
    removeFromParent();
}
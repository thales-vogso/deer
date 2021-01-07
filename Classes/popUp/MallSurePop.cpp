/****************************************************************************
 *	@desc   商城的确认弹窗
 *	@date	2015-5-22
 *	@author	110101
     
 *	@file	popUp/MallSurePop.cpp
 *   @modify	null
 ******************************************************************************/

#include "MallSurePop.h"
#include "utils/DeerUtils.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "popUp/BlackMask.h"
#include "GlobalMacros.h"
#include "data/Mall.h"
#include "notice/Notice.h"
#include "sound/SoundManager.h"
#include "data/Lang.h"
#include "data/Item.h"
USING_NS_CC;
USING_NS_DU;
MallSurePop *MallSurePop::create(cocos2d::ValueMap data)
{
    auto pRet = new MallSurePop();
    if (pRet && pRet->init(data))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return pRet;
    }
}
void MallSurePop::onEnter()
{
    Layout::onEnter();
    addChild(Notice::getInstance()->getTips());
}
bool MallSurePop::init(cocos2d::ValueMap data)
{
    if (!Layout::init())
    {
        return false;
    }
    _data = data;
    addChild(BlackMask::create());
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("mallSurePop.csb");
    view->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    auto summary = seekChildByNameWithRetType<ui::Text*>(view, "summaryTxt");
    summary->setString(data["summary"].asString());
    
    auto cost = seekChildByNameWithRetType<ui::Text*>(view, "price");
	cost->setString(data::Lang::getInstance()->getWord(110102) + data["cost"].asString() + data::Lang::getInstance()->getWord(110104) );
    
    auto sureBtn = seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea");
    sureBtn->addTouchEventListener(CC_CALLBACK_2(MallSurePop::onSureButton, this));
    
    auto quitBtn = seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnAra");
    quitBtn->addTouchEventListener(CC_CALLBACK_2(MallSurePop::onQuitButton, this));
    
    return true;
}

void MallSurePop::onSureButton(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "sureBtnArea")->setTouchEnabled(false);
		SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureReal();
    }
}

void MallSurePop::sureReal()
{
  //  int crystal = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asInt();
    int crystal = data::Item::getInstance()->getUserItemAmountByIID(data::Item::ItemID::CRYSTAL);
    int costcrystal =_data["cost"].asInt();
    if (crystal >= costcrystal)
    {
        data::Item::getInstance()->userItemReduce(data::Item::ItemID::CRYSTAL, costcrystal);
        
        data::Mall::getInstance()->buy(_data["mall_id"].asInt());
        
        removeFromParent();
    }
    else
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "sureBtnArea")->setTouchEnabled(true);
        std::string str = "钻石数目不足";
        Notice::getInstance()->postTips(str);
    }
}

void MallSurePop::onQuitButton(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnAra")->setTouchEnabled(false);
		SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitReal();
    }
}

void MallSurePop::quitReal()
{
    removeFromParent();
//    Manor::getInstance()->showLayer(Manor::LayoutName::kMainLayer);
}
/****************************************************************
 *	@desc   战斗界面
 *	@date	2015-5-18
 *	@author	110101
     
 *	@file	crusade/CrusadeMapPopupManager.cpp
 *  @modify	null
 ***************************************************************/

#include "CrusadeMapPopupManager.h"
#include "crusade/EnterChoicepop.h"
#include "crusade/Cemetery.h"
#include "crusade/SettlementLayer.h"
#include "data/Stage.h"
#include "manor/Shop.h"
#include "crusade/WildTavern.h"
#include "crusade/TransGate.h"

#define TO_S(_var_) #_var_
#define popUpOrder 50
#define ASKPOPNAME "askpopname"

USING_NS_CC;
//USING_NS_DU;


CrusadeMapPopupManager* CrusadeMapPopupManager::create(CrusadeMap *target)
{
    auto ret = new CrusadeMapPopupManager();
    if (ret && ret->init(target))
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

bool CrusadeMapPopupManager::init(CrusadeMap *target)
{
    if (!Node::init())
    {
        return false;
    }
    
    _target = target;
    return true;
}

void CrusadeMapPopupManager::showEnterChoice(cocos2d::ValueMap data)
{
    auto pop = EnterChoicePop::create(data);
    pop->setDelegate(_target);
    this->addChild(pop, 0, ASKPOPNAME);
}

void CrusadeMapPopupManager::showBattleChoice()
{
    auto choicepop = BattleChoice::create();
    this->addChild(choicepop, popUpOrder);
    removeChildByName(ASKPOPNAME);
}

void CrusadeMapPopupManager::showCemetery()
{
    auto pop = Cemetery::create();
    this->addChild(pop, popUpOrder);
    removeChildByName(ASKPOPNAME);
}

void CrusadeMapPopupManager::showWildShop(ValueMap data)
{
    auto fdata = data::Crusade::getInstance()->fieldView().asValueMap();
	Shop* node = Shop::create();
	node->updateList(fdata.at("fid").asInt());
	addChild(node, popUpOrder);
    removeChildByName(ASKPOPNAME);
}

void CrusadeMapPopupManager::showWildHero()
{
    /**
     *  获取当前地图块的数据
     */
    auto fdata = data::Crusade::getInstance()->fieldView().asValueMap();
    WildTavern* wildTavern = WildTavern::create(fdata["fid"].asInt());
    wildTavern->showBoard();
    addChild(wildTavern,popUpOrder);
    removeChildByName(ASKPOPNAME);
}

void CrusadeMapPopupManager::showShelter()
{
    auto drops = data::Crusade::getInstance()->getDrops().asValueVector();
    addChild(SettlementLayer::create(drops), popUpOrder);
    removeChildByName(ASKPOPNAME);
}

void CrusadeMapPopupManager::showRecall()
{
    addChild(TransGate::create(), popUpOrder);
    removeChildByName(ASKPOPNAME);
}
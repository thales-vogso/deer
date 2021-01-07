/******************************************************
 *	@desc   地图事件管理
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	crusade/CrusadeMapEventManager.cpp
 *   @modify	null
 *******************************************************/

#include "CrusadeMapEventManager.h"
#include "crusade/CrusadeMap.h"
#include "data/crusade.h"
#include "battle/BattleLayer.h"
#include "manor/Manor.h"


#define TO_S(_var_) #_var_
USING_NS_CC;


CrusadeMapEventManager::~CrusadeMapEventManager()
{
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(encountListener)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(fieldEnterListner)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(clearlistener)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(acelistener)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(shelterlistener)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(packagechage)));
	//data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(fogClear)));
}

CrusadeMapEventManager * CrusadeMapEventManager::createWithTarget(CrusadeMap *target)
{
    auto ret = new CrusadeMapEventManager();
    if (ret && ret->initWithTarget(target))
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

bool CrusadeMapEventManager::initWithTarget(CrusadeMap *target)
{
    _target = target;
    
    //添加事件的侦听..
    auto encountListener = data::Crusade::getInstance()->addListener(data::Crusade::ENCOUNTER, CC_CALLBACK_1(CrusadeMapEventManager::onEncounter, this));
    addEventPool(encountListener,data::Crusade::getInstance());
    
    auto fieldEnterListner = data::Crusade::getInstance()->addListener(data::Crusade::FIELD_ENTER, CC_CALLBACK_1(CrusadeMapEventManager::onEnterField, this));
    addEventPool(fieldEnterListner,data::Crusade::getInstance());
    
    auto clearlistener = data::Crusade::getInstance()->addListener(data::Crusade::FIELD_CLEAR, CC_CALLBACK_1(CrusadeMap::onClearField, _target));
    addEventPool(clearlistener,data::Crusade::getInstance());
    
    auto acelistener = data::Crusade::getInstance()->addListener(data::Crusade::ACE, CC_CALLBACK_1(CrusadeMap::playerAce, _target));
    addEventPool(acelistener,data::Crusade::getInstance());
    
    auto nothinglistener = data::Crusade::getInstance()->addListener(data::Crusade::NOTHING_HAPPEN, CC_CALLBACK_1(CrusadeMap::moveNothingHappen, _target));
    addEventPool(nothinglistener,data::Crusade::getInstance());
    
    auto shelterlistener = data::Crusade::getInstance()->addListener(data::Crusade::SHELTER_DROP, CC_CALLBACK_1(CrusadeMapEventManager::onEnterShelter, this));
    addEventPool(shelterlistener,data::Crusade::getInstance());
    
	auto fogClear = data::Crusade::getInstance()->addListener(data::Crusade::FOG_CLEAR, CC_CALLBACK_1(CrusadeMapEventManager::onFogClear, this));
    addEventPool(fogClear,data::Crusade::getInstance());
    
    auto packagechage = data::Crusade::getInstance()->addListener(data::Crusade::PACKAGE_CHANGE, CC_CALLBACK_1(CrusadeMap::onPackageChange, _target));
    addEventPool(packagechage,data::Crusade::getInstance());
    
    auto itemChange = data::Item::getInstance()->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(CrusadeMap::onItemChange, _target));
    addEventPool(itemChange,data::Item::getInstance());

    return true;
}
void CrusadeMapEventManager::removeSelf()
{

}
void CrusadeMapEventManager::updateProperty()
{

}
void CrusadeMapEventManager::onEncounter(cocos2d::EventCustom *event)
{
    //遇到怪物，去战斗界面..
    _target->_touchListener->setEnabled(false);
    auto data =  (*(Value*)(event->getUserData()));
    _enconterData = data;
    _target->playWow();
    runAction(Sequence::create(DelayTime::create(0.6), CallFunc::create(CC_CALLBACK_0(CrusadeMapEventManager::enconterMonster,  this)),NULL));
}

void CrusadeMapEventManager::enconterMonster()
{
    if (_enconterData.asValueMap()["field"].asBool())
    {
        /**
         *  在建筑里面将建筑数据一起传送
         */
        //        _enconterData.asValueMap()["fdata"] = _enterData;
    }
    BattleLayer::gotoBattleScene(_enconterData.asValueMap());
}

void CrusadeMapEventManager::onEnterField(cocos2d::EventCustom *event)
{
    /**
     *  遇到敌人后0.5秒才响应进入事件，目的是让玩家的移动动画播放完毕。
     */
    Value data =(*(Value*)(event->getUserData()));
    _enterData = data.asValueMap();
    CCLOG("%s",_enterData["message_enter"].asString().c_str());
    runAction(Sequence::create(DelayTime::create(0.6), CallFunc::create(CC_CALLBACK_0(CrusadeMapEventManager::enterFieldAct, this)), NULL));
}

void CrusadeMapEventManager::enterFieldAct()
{
    int fieldType = _enterData["type"].asInt();
    //    bool key = _enterData["key"].asBool();
    
    if (fieldType == data::Stage::FieldType::PORTAL)
    {
        Manor::goToManorScene(Manor::GoManorType::kNone);
    }
    else
    {
        if (_enterData["visit"].asInt() != data::Stage::ALREADY_VISIT)
        {
            _target->getPopupManager()->showEnterChoice(_enterData);
        }
    }
    //    else if (fieldType == data::Stage::FieldType::GRAVE ||
    //             fieldType == data::Stage::FieldType::SHOP ||
    //             fieldType == data::Stage::FieldType::SHELTER ||
    //             fieldType == data::Stage::FieldType::DUNGEON)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::RECALL && key)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::TOWER && key)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::TAVERN)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::ARENA && key)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::GATE_BAR && key)
    //    {
    //        _target->getPopupManager()->showEnterChoice(_enterData);
    //    }
    //    else if (fieldType == data::Stage::FieldType::GATE_FREE && key)
    //    {
    //        auto pop = EnterChoicePop::create(_enterData);
    //        this->addChild(pop, popUpOrder, ASKPOPNAME);
    //    }
}

void CrusadeMapEventManager::onFogClear(cocos2d::EventCustom* e){
	ValueVector vec = (*(ValueVector*)(e->getUserData()));
	auto fogLayer = _target->_crusadeMap->getLayer("fogLayer");
	int fogfirst = fogLayer->getTileSet()->_firstGid;
	for (Value row : vec){
		int x = row.asValueMap()["x"].asInt();
		int y = data::Crusade::getInstance()->getStage()["height"].asInt() - 1 - row.asValueMap()["y"].asInt();
		fogLayer->setTileGID(fogfirst + 50, Vec2(x, y));
	}
    //检测迷雾是否被清除干净了..
    
}

void CrusadeMapEventManager::onEnterShelter(cocos2d::EventCustom *event)
{
    //这里要更新地图..
    auto fielddata = data::Crusade::getInstance()->fieldView().asValueMap();
    auto buildlayer = _target->_crusadeMap->getLayer("buildLayer");
    int mapHeight = _target->_crusadeMap->getMapSize().height;
    auto firstGid = buildlayer->getTileSet()->_firstGid;
    buildlayer->setTileGID(2 + firstGid, Vec2(fielddata["x"].asInt(), mapHeight -1- fielddata["y"].asInt()));
    _target->getPopupManager()->showShelter();
}

/****************************************************************************
 *	@desc   选择确认界面
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/EnterChoicePop.cpp
 *  @modify	null
 ******************************************************************************/

#include "EnterChoicePop.h"
#include "popUp/BlackMask.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "data/Crusade.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"
#include "guide/Guide.h"
#include "notice/Notice.h"

USING_NS_CC;
USING_NS_DU;

EnterChoicePop *EnterChoicePop::create(cocos2d::ValueMap data)
{
    auto ret = new EnterChoicePop();
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

bool EnterChoicePop::init(cocos2d::ValueMap data)
{
    if (!Layer::init())
    {
        return false;
    }
    
    _data = data;
    
    //添加触摸遮罩..
    this->addChild(BlackMask::create());
    //初始化显示部分..
    auto win = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("enterChoice.csb");
    view->setPosition(win/2);
    this->addChild(view);
    
    auto txt = seekChildByNameWithRetType<ui::Text*>(view, "summary");
    seekChildByNameWithRetType<ui::Text*>(view, "title")->setString(_data["name"].asString());
    txt->setString(_data["message_enter"].asString());
    Notice::getInstance()->postBoard(_data["message_enter"].asString());
    auto enterBtn = seekChildByNameWithRetType<ui::Layout*>(view, "enterBtnArea");
    enterBtn->addTouchEventListener(CC_CALLBACK_2(EnterChoicePop::sureCallback, this));
    
// 这里需要区分是不是第一层..
    CCLOG("第 %d 层..", _data["round"].asInt());
    if (!_data["key"].isNull() && _data["round"].asInt() == 0)
    {
        enterBtn->setVisible(_data["key"].asBool());
        auto entertxt = seekChildByNameWithRetType<ui::Text*>(view, "Text_1");
        entertxt->setVisible(_data["key"].asBool());
		view->getChildByName<ui::Button*>("enterBtn")->setVisible(_data["key"].asBool());
    }
    
    
    auto cancleBtn = seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea");
    cancleBtn->addTouchEventListener(CC_CALLBACK_2(EnterChoicePop::quitCallback, this));
    
    //新手引导..
    Guide* guide = Guide::create(data::Guid::BATTLE_SELECT_LAYER);
    guide->insertButton(data::Guid::BATTLE_SELECT_SURE, enterBtn);
    guide->insertButton(data::Guid::BATTLE_SELECT_CANCLE, cancleBtn);
    this->addChild(guide,10000);
    //如果在打钛矿，则试着指引..
    if(_data["fid"].asInt() == 110701)
    {
        data::Guid::getInstance()->showBattleSelectGuid();
    }
    return true;
}

void EnterChoicePop::sureCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "enterBtnArea")->setTouchEnabled(false);
        data::Guid::getInstance()->updateGuidInfo(data::Guid::BATTLE_SELECT_SURE);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        surecall();
        
    }
}

void EnterChoicePop::surecall()
{
    if (_delegate)
    {
        int ftype = _data["type"].asInt();
        if (ftype == data::Stage::FieldType::GRAVE)
        {
            _delegate->getPopupManager()->showCemetery();
        }
        else if (ftype == data::Stage::FieldType::TAVERN)
        {
            _delegate->getPopupManager()->showWildHero();
        }
        else if (ftype == data::Stage::FieldType::SHOP)
        {
            _delegate->getPopupManager()->showWildShop(_data);
        }
        else if (ftype == data::Stage::FieldType::SHELTER)
        {
            Vec2 vecpos;
            if (!_data["block"].isNull())
            {
                vecpos = Vec2(_data["block"].asValueMap()["x"].asInt(), _data["block"].asValueMap()["y"].asInt());
            }
            else
            {
                vecpos = Vec2(_data["x"].asInt(), _data["y"].asInt());
            }
            data::Crusade::getInstance()->fieldEnter(vecpos.x, vecpos.y);
        }
        else if (ftype == data::Stage::FieldType::DUNGEON ||
                 ftype == data::Stage::FieldType::ARENA ||
                 ftype == data::Stage::FieldType::TOWER)
        {
            _delegate->getPopupManager()->showBattleChoice();
        }
        else if (ftype == data::Stage::FieldType::GATE_FREE)
        {
            auto crudata = data::Crusade::getInstance()->getUserCrusade();
            int sid = crudata.asValueMap()["sid"].asInt();
            data::Crusade::getInstance()->depart(sid + 100);
//            //运行下一个新的地图场景..
            CrusadeMap::goToCrusadeMapScene(sid + 100);
        }
        else if (ftype == data::Stage::FieldType::RECALL)
        {
            _delegate->getPopupManager()->showRecall();
        }
        else
        {
            Vec2 vecpos;
            if (!_data["block"].isNull())
            {
                vecpos = Vec2(_data["block"].asValueMap()["x"].asInt(), _data["block"].asValueMap()["y"].asInt());
            }
            else
            {
                vecpos = Vec2(_data["x"].asInt(), _data["y"].asInt());
            }
            data::Crusade::getInstance()->fieldEnter(vecpos.x, vecpos.y);
        }
    }
}

void EnterChoicePop::quitCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        
        if (sender)
        {
            quitcall();
        }
        else
        {
            this->quitcall();
        }
    }
    
}

void EnterChoicePop::quitcall()
{
	this->removeFromParent();
}

/************************************************************/

BattleChoice *BattleChoice::create()
{
    auto ret = new BattleChoice();
    if (ret && ret->init())
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

bool BattleChoice::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    _fieldData = data::Crusade::getInstance()->fieldView().asValueMap();
    //添加触摸遮罩..
    this->addChild(BlackMask::create());
    //初始化显示部分..
    auto win = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("enterChoice.csb");
    view->setPosition(win/2);
    this->addChild(view);
    
    auto glanceData = data::Crusade::getInstance()->fieldView().asValueMap();
    if (!glanceData["monster"].isNull())
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        auto name = glanceData["field"].asValueMap()["name"].asString() + "第" + Value(glanceData["round"].asInt()+1).asString()+ "层  ";
#else
        auto name = glanceData["field"].asValueMap()["name"].asString() + "第  " + Value(glanceData["round"].asInt()+1).asString()+ "层  ";
#endif
        seekChildByNameWithRetType<ui::Text*>(view, "title")->setString(name);

        auto txt = seekChildByNameWithRetType<ui::Text*>(view, "summary");
        
        txt->setString(glanceData["monster"].asValueMap()["summary"].asString());
    }

    seekChildByNameWithRetType<ui::Text*>(this, "Text_1")->setString("  战斗  ");
    
    auto enterBtn = seekChildByNameWithRetType<ui::Layout*>(view, "enterBtnArea");
    enterBtn->addTouchEventListener(CC_CALLBACK_2(BattleChoice::sureCallback, this));
    
    auto cancleBtn = seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea");
    cancleBtn->addTouchEventListener(CC_CALLBACK_2(BattleChoice::quitCallback, this));
    
    //新手引导..
    Guide* guide = Guide::create(data::Guid::BATTLE_SELECT_LAYER2);
    guide->insertButton(data::Guid::BATTLE_CHOICE_SURE, enterBtn);
    guide->insertButton(data::Guid::BATTLE_CHOICE_CANCLE, cancleBtn);
    this->addChild(guide);
    //如果在打钛矿，则试着指引..
    if(glanceData["fid"].asInt() == 110701)
    {
        data::Guid::getInstance()->showBattleChoiceGuid();
    }
    
    SoundManager::getInstance()->stopMusicFadeOut();
    return true;
}

void BattleChoice::sureCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "enterBtnArea")->setTouchEnabled(false);
        data::Guid::getInstance()->updateGuidInfo(data::Guid::BATTLE_CHOICE_SURE);
        Vec2 vecpos;
        if (!_fieldData["block"].isNull())
        {
            vecpos = Vec2(_fieldData["block"].asValueMap()["x"].asInt(), _fieldData["block"].asValueMap()["y"].asInt());
        }
        else
        {
            vecpos = Vec2(_fieldData["x"].asInt(), _fieldData["y"].asInt());
        }
        data::Crusade::getInstance()->fieldEnter(vecpos.x, vecpos.y);
    }
}

void BattleChoice::quitCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->setTouchEnabled(false) ;
		if (_fieldData["block"].isNull())
			data::Crusade::getInstance()->fieldFailed(_fieldData["x"].asInt(), _fieldData["y"].asInt());
		else
			data::Crusade::getInstance()->fieldFailed(_fieldData["block"]);
		if (_quitCall)
        {
            _quitCall();
        }
        else
        {
			this->removeFromParent();
        }

    }
}

/**************建筑*************************/

#define QUITBTN_ACT 11

BuildClearPop *BuildClearPop::create()
{
    auto ret = new BuildClearPop();
    if (ret && ret->init())
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

bool BuildClearPop::init()
{
    if (!Layout::init())
    {
        return false;
    }
    _data = data::Crusade::getInstance()->fieldView().asValueMap();
    
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    auto csloader = CSLoader::getInstance();
    auto view = csloader->createNodeWithFlatBuffersFile("buildclear.csb");
    view->setContentSize(win);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    auto title = seekChildByNameWithRetType<ui::TextBMFont*>(this, "title");
    title->setString(_data["field"].asValueMap()["name"].asString());
    
    auto content = seekChildByNameWithRetType<ui::Text*>(this, "content");
    auto str = _data["field"].asValueMap()["message_leave"].asString();
    if(str.size() > 0)
    {
        content->setString(str);
        Notice::getInstance()->postBoard(str);
    }
    
    
    auto quitBtn = seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea");
    quitBtn->addTouchEventListener(CC_CALLBACK_2(BuildClearPop::quitBtnCall, this));
    
    return true;
}

void BuildClearPop::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if(t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->setTouchEnabled(false);
        quit();
    }
}

void BuildClearPop::quit()
{
    if (onSure)
    {
        onSure();
    }
}

/***********************传送门************************/

/****************************************************************************
 *	@desc   成就界面
 *	@date	2015-1-20
 *	@author	110101
     
 *	@file	manor/HeroChoicePopup.h
 *  @modify	null
 ******************************************************************************/

#include "HeroChoicePopup.h"
#include "utils/DeerUtils.h"
#include "data/Role.h"
#include "popUp/BlackMask.h"
#include "notice/Notice.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"
#include "data/Lang.h"

USING_NS_CC;
USING_NS_DU;

HeroChoicePopup *HeroChoicePopup::create(int rid)
{
    HeroChoicePopup *ret = new HeroChoicePopup();
    if (ret && ret->init(rid))
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

bool HeroChoicePopup::init(int rid)
{
    if (!Layout::init())
    {
        return false;
    }
    auto win = Director::getInstance()->getWinSize();
    
    addChild(BlackMask::create());
    
    auto CSNode = CSLoader::getInstance()->createNodeWithFlatBuffersFile("heroUpdate.csb");
    auto viewRoot = seekChildByNameWithRetType<ui::Layout*>(CSNode, "Panel");
    addChild(viewRoot);
    setContentSize(win);
    ui::Helper::doLayout(this);
    
    auto itemModel = seekChildByNameWithRetType<ui::Layout*>(this, "itemmodel");
    _model.pushBack(itemModel);//模版的第一个是item英雄item;;
    auto quitModel = seekChildByNameWithRetType<ui::Layout*>(this, "quitmodel");
    _model.pushBack(quitModel);//取消;;
    auto emptyModel = seekChildByNameWithRetType<ui::Layout*>(this, "itemmodel_0");
    _model.pushBack(emptyModel);
    
    auto backButton = seekChildByNameWithRetType<ui::Button*>(this, "quit");
    backButton->addTouchEventListener(CC_CALLBACK_2(HeroChoicePopup::backButtonCallback, this));
    
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    list->removeAllItems();
    list->pushBackCustomItem(_model.at(2));
    auto advanceHeroes = data::Role::getInstance()->getRoleByOriginRID(rid).asValueVector();
    for (auto iter = advanceHeroes.begin(); iter != advanceHeroes.end(); iter++)
    {
        auto item = itemModel->clone();
        auto name = seekChildByNameWithRetType<ui::Text*>(item, "heroname");
        name->setString((*iter).asValueMap()["name"].asString());

        auto trainBtn = seekChildByNameWithRetType<ui::Button*>(item, "targetHeroBtn");
        trainBtn->setOpacity(140);
        _buttonMap.insert((*iter).asValueMap()["rid"].asInt(), trainBtn);
        
        auto condition = seekChildByNameWithRetType<ui::Text*>(item, "condition");
        auto condata = data::Role::getInstance()->getTrainByRID((*iter).asValueMap()["rid"].asInt()).asValueMap();
        if (!condata["items"].isNull()) {
            std::stringstream sstr;
            for (auto &i : condata["items"].asValueVector())
            {
                sstr<<i.asValueMap()["name"].asString() << "x"<< i.asValueMap()["amount"].asString();
            }
            
            condition->setString(sstr.str());
        }
        else
        {
            condition->setVisible(false);
        }
        
        trainBtn->addTouchEventListener(CC_CALLBACK_2(HeroChoicePopup::trainToHero, this));
        list->pushBackCustomItem(item);
    }
  
    list->pushBackCustomItem(_model.at(1));
    auto size = list->getContentSize();
    list->setContentSize(Size(size.width, win.height - (SCREEN_HEIGHT - list->getPositionY())));

    return true;
}

void HeroChoicePopup::backButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType touchType)
{
    if (touchType == ui::Text::TouchEventType::ENDED || touchType == ui::Text::TouchEventType::CANCELED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        backReal();
    }
    //BTN_CALL_ACT(touchType, sender, sender, CC_CALLBACK_0(HeroChoicePopup::backReal, this))
}

void HeroChoicePopup::backReal()
{
    this->removeFromParent();
}

void HeroChoicePopup::trainToHero(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType touchType)
{
    auto btn = dynamic_cast<ui::Widget*>(sender);
    _touchBtn = btn;
//    if (touchType == ui::Button::TouchEventType::BEGAN)
//    {
//        btn->setOpacity(77);
//    }

//    if (touchType == ui::Text::TouchEventType::ENDED)
//    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
//        btn->runAction(FadeTo::create(0.3, 140));
//        runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create([=](){
//            this->trainToHeroAct();
//        }), NULL));
        
//        int advanceRid = _buttonMap.keys(sender).at(0);
//        auto retVal = data::Role::getInstance()->train(advanceRid).asValueMap();
//        auto ask  = RecruitAsk::create(data::Role::getInstance()->getRoleByRID(advanceRid).asValueMap());
//        ask->setDelegate(this);
//        addChild(ask);
        
        
//        if(retVal["code"].asInt() == 0)
//        {
//            this->removeFromParent();
//        }
//        else
//        {
//            CCLOG("资源不足");
//        }
//    }
    
    if (touchType == ui::Text::TouchEventType::ENDED || touchType == ui::Text::TouchEventType::CANCELED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        trainToHeroAct();
    }
    //BTN_CALL_ACT(touchType, sender, sender, CC_CALLBACK_0(HeroChoicePopup::trainToHeroAct, this))
}



void HeroChoicePopup::trainToHeroAct()
{
    if (_touchBtn)
    {
        int advanceRid = _buttonMap.keys(_touchBtn).at(0);
        //        auto retVal = data::Role::getInstance()->train(advanceRid).asValueMap();
        auto ask  = RecruitAsk::create(data::Role::getInstance()->getRoleByRID(advanceRid).asValueMap());
        ask->setDelegate(this);
        addChild(ask);
        _touchBtn = nullptr;
    }
}

/*****************确认弹窗***********************/

RecruitAsk::RecruitAsk()
:_delegate(nullptr)
{
    
}

RecruitAsk* RecruitAsk::create(cocos2d::ValueMap data)
{
    auto ret = new RecruitAsk();
    if (ret && ret->init(data))
    {
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}
void RecruitAsk::removeSelf()
{

}
void RecruitAsk::updateProperty()
{

}
bool RecruitAsk::init(cocos2d::ValueMap data)
{
    if(!Layout::init())
    {
        return false;
    }
    
    //新手引导手指..
    _guide = Guide::create(data::Guid::HERO_POP_LAYER);
    this->addChild(_guide,10000);
    
    auto win = Director::getInstance()->getWinSize();
    _data = data;
    
    addChild(BlackMask::create());
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("recruitAsk.csb");
    auto viewRoot = seekChildByNameWithRetType<ui::Layout*>(view, "Panel");
    addChild(viewRoot);
    setContentSize(win);
    ui::Helper::doLayout(this);
    
    std::string askTitle;
    std::string name = data["name"].asString();
    askTitle = askTitle + "训练一个  " + name + "  ";
    seekChildByNameWithRetType<ui::Text*>(view, "askTitle")->setString(askTitle);
    
    seekChildByNameWithRetType<ui::Text*>(view, "skillText")->setString("技能: " + data["skill"].asString());
    if (data["effect"].asValueMap()["name"].isNull() || data["effect"].asValueMap()["name"].getType() == Value::Type::NONE)
    {
        seekChildByNameWithRetType<ui::Text*>(view, "efficte")->setString("附加效果:无  ");
    }
    else
    {
        seekChildByNameWithRetType<ui::Text*>(view, "efficte")->setString("附加效果:" + data["effect"].asValueMap()["name"].asString());
    }
    
    std::string attribute;
    std::string power = data["power"].asString();
    if (data["power"].asFloat() < 0) {
        power.erase(power.begin());
        
    }
    attribute = "生命: " + data["life"].asString() + "威力: " + power + "速度: " + data["speed"].asString();
    seekChildByNameWithRetType<ui::Text*>(view, "attribute")->setString(attribute);
    
    seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea")->addTouchEventListener(CC_CALLBACK_2(RecruitAsk::sureBtnCall, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea")->setSwallowTouches(false);
    _guide->insertButton(data::Guid::HERO_POP_LAYER_BUTTON_SURE, seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea"));
    seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea")->addTouchEventListener(CC_CALLBACK_2(RecruitAsk::quitBtnCall, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea")->setSwallowTouches(false);
    _guide->insertButton(data::Guid::HERO_POP_LAYER_BUTTON_QUIT,seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea"));
    if (_data["rid"].asInt() != data::Role::BASE_ROLE_RID)
    {
        seekChildByNameWithRetType<ui::Text*>(view, "price")->setVisible(false);
    }
    
    return true;
}

void RecruitAsk::sureBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "sureBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureAct();
    }
}

void RecruitAsk::sureAct()
{
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::TRAINING_SURE);
    
    auto retVal = data::Role::getInstance()->train(_data["rid"].asInt()).asValueMap();
    if (retVal["code"].asInt() == 0)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBuildFinish);
        std::string name = _data["name"].asString();
        name = "训练了一个 " + name;
        Notice::getInstance()->postBoard(name);
        
        //训练成功手动显示下一个新手引导..
        data::Guid::getInstance()->showGuid(data::Guid::WAR);
    }
    else if (retVal["code"].asInt() == 1)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kLACK);
		std::stringstream sstr;
        for (auto &i : retVal["data"].asValueMap()["train"].asValueMap()["items"].asValueVector())
        {
			if (i.asValueMap()["lack"].asInt() < 0){
				sstr << "缺少: " << -i.asValueMap()["lack"].asInt() << "个  " << i.asValueMap()["name"].asString() <<"\r\n";
                auto tips = data::Lang::getInstance()->getTipsByIID(i.asValueMap()["iid"].asInt());
                sstr <<tips<<"\r\n";
			}
        }
        Notice::getInstance()->postTips(sstr.str(),Notice::kWarning);
    }
    if (_delegate)
    {
        _delegate->removeFromParent();
    }
    else
    {
        removeFromParent();
    }
}

void RecruitAsk::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitAct();
    }
}

void RecruitAsk::quitAct()
{
    if (_delegate)
    {
        _delegate->removeFromParent();
    }
    else
    {
        removeFromParent();
    }
}
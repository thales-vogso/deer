/****************************************************************************
     
 *	@desc	英雄界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/heroLayer.cpp
 *  @modify	null
 ******************************************************************************/

#include "HeroLayer.h"

#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "data/Role.h"
#include "manor/HeroChoicePopup.h"
#include "notice/Notice.h"
#include "manor/Manor.h"
#include "data/Guid.h"
#include "popUp/Summary.h"
#include "popUp/BlackMask.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"

#define TO_S(_var_) #_var_
#define LISTHIEGHT_OFFSET 50

USING_NS_CC;
USING_NS_DU;
#define RECRUIT_BTN_ACT 13
#define NOTIFICATION_Z 50
#define HERO_CHOICE_POP 55

HeroLayer::~HeroLayer()
{
    //data::Role::getInstance()->removeListener(_listenerMap.at(TO_S(trainListener)));
    //Notice::getInstance()->removeListener(_listenerMap.at(TO_S(foldEvent)));
	//Notice::getInstance()->removeListener(_listenerMap.at(TO_S(unfoldEvent)));
    LOGTYPENAME
    custom::LibDate::getRunTime();
}

bool HeroLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //新手引导指引..
    _guide = Guide::create(data::Guid::HERO_LAYER);
    this->addChild(_guide,10000);
    _guide->addFinishGuid();
    addChild(BlackMask::create());
    //初始化界面..
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("hero.csb");
    view->setContentSize(winSize);
    view->getChildByName<ui::Layout*>("Panel")->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //返回按钮..
    auto backButton = seekChildByNameWithRetType<ui::Layout*>(view, "gobackBtnAra");
    backButton->setTouchEnabled(true);
    backButton->addTouchEventListener(CC_CALLBACK_2(HeroLayer::backButtonCallback, this));
    _guide->insertButton(data::Guid::HERO_LAYER_BUTTON_BACK, backButton);
    
    //招募按钮..
    _recruitBtnArea = seekChildByNameWithRetType<ui::Layout*>(view, "recruitBtnArea");
    _recruitBtnArea->setSwallowTouches(true);
    _recruitBtnArea->setTouchEnabled(true);
    _recruitBtnArea->addTouchEventListener(CC_CALLBACK_2(HeroLayer::recruitBtnCallback, this));
    _guide->insertButton(data::Guid::HERO_LAYER_BUTTON_GETHERO, _recruitBtnArea);
    
    _recruitBtn     = seekChildByNameWithRetType<ui::Button*>(view, "recruitBtn");
    
    //初始化item模板..
    auto list = seekChildByNameWithRetType<ui::ListView*>(view, "list");
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "listitem"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "listitem_0"));
    list->removeAllItems();
    list->pushBackCustomItem(_itemModel.at(1));
    auto rolesData = data::Role::getInstance()->getUserRoles().asValueVector();
    int i = 0;
    for (auto itemData : rolesData)
    {
        auto item = HeroListItem::create(itemData.asValueMap(), this);
//        item->addListener(HeroListItem::TRAIN_HERO_CHOICE, CC_CALLBACK_1(HeroLayer::addTrainHeroPopup, this));
        seekChildByNameWithRetType<ui::ListView*>(this, "list")->pushBackCustomItem(item);
        _guide->insertButton(data::Guid::HERO_LAYER_HERO_NAME+i, item->_nameBtn);
        i++;
    }
    
    //设置天空大小，天空海阔你与我~~~
    auto starSky = view->getChildByName("Panel")->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winSize.width/2,(int)winSize.height*75/128));
    
    //添加训练事件的相应..
    auto trainListener = data::Role::getInstance()->addListener(data::Role::ROLE_TRAIN, CC_CALLBACK_1(HeroLayer::onTrainNewRole, this));
    addEventPool(trainListener, data::Role::getInstance());
    
    
    EventListenerCustom * foldEvent = Notice::getInstance()->addListener(Notice::FOLD, CC_CALLBACK_1(HeroLayer::unFold, this));
    addEventPool(foldEvent,Notice::getInstance());
    
	EventListenerCustom * unfoldEvent = Notice::getInstance()->addListener(Notice::UNFOLD, CC_CALLBACK_1(HeroLayer::unFold, this));
    addEventPool(unfoldEvent,Notice::getInstance());
    
    listResize();
    
    return true;
}
void HeroLayer::updateList()
{
    if(!_isUpdateList)
    {
        _isUpdateList = true;
        auto rolelist = data::Role::getInstance()->getUserRoles().asValueVector();
        if(preRoleListData != Value(rolelist)|| preRoleListData.asValueVector().size() != rolelist.size())
        {
            preRoleListData = Value(rolelist);
            auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
            auto items = list->getItems();
            items.erase(0);
            int j =0;
            for (auto &i : items)
            {
                auto item = dynamic_cast<HeroListItem*>(i);
                item->setUpdated(false);
                _guide->ereaseButton(data::Guid::HERO_LAYER_HERO_NAME+j);
            }
            
            int dataCount = 1;
            for (auto &i : rolelist)
            {
                auto iter = std::find_if(items.begin(), items.end(), [&](ui::Widget * item)->bool
                                         {
                                             HeroListItem * listitem = dynamic_cast<HeroListItem*>(item);
                                             return i.asValueMap()["rid"].asInt() == listitem->getRid();
                                         });
                if (iter == items.end())
                {
                    auto listitem = HeroListItem::create(i.asValueMap(), this);
                    list->insertCustomItem(listitem, dataCount);
                    _guide->insertButton(data::Guid::HERO_LAYER_HERO_NAME+dataCount-1, listitem->_nameBtn);
                }
                else
                {
                    auto listitem = dynamic_cast<HeroListItem*>((*iter));
                    listitem->updateView(i.asValueMap());
                    _guide->insertButton(data::Guid::HERO_LAYER_HERO_NAME+dataCount-1, listitem->_nameBtn);
                }
                
                ++dataCount;
            }
            
            /**
             *  删除没有数据更新的元素
             */
            for (auto &i : items)
            {
                auto item = dynamic_cast<HeroListItem*>(i);
                if (!item->isUpdated())
                {
                    list->removeItem(list->getIndex(item));
                }
            }
        }
        _isUpdateList = false;
    }
    
}

void HeroLayer::onEnter()
{
    Layer::onEnter();
    showBoard();
    
    
    /**
     * 招募按钮是否需要显示new
     */
    _showRecuitNew = !data::Task::getInstance()->BtnIsTouched(data::Task::BtnID::kRecruit);
    
    if (_showRecuitNew)
    {
        
        _recruitBtnArea->setOpacity(0);
        auto showAct = Sequence::create(FadeIn::create(0.7), CallFunc::create([=](){
                seekChildByName(_recruitBtnArea, "newdot")->setVisible(true);
        }), nullptr);
        
        _recruitBtnArea->runAction(showAct);
        _recruitBtn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 127.5), FadeTo::create(0.5, 255),NULL)));
    }
}

void HeroLayer::onExit()
{
    Layer::onExit();
    removeChildByName("tips");
}

void HeroLayer::showBoard()
{
    addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
	addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}
void HeroLayer::unFold(cocos2d::EventCustom *e)
{
    listResize();
}
void HeroLayer::listResize()
{
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    auto innerpos = list->getInnerContainer()->getPosition();
    list->setContentSize(Size(list->getContentSize().width, list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
    list->getInnerContainer()->setPosition(innerpos);
}

void HeroLayer::backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType)
{
    auto btn = dynamic_cast<Node*>(sender);
    if (touchType == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() - BACKBTN_ACT_DISTANCE);
    }
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
    if (touchType == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
        backReal();
    }
}

void HeroLayer::backReal()
{
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::TRAINING_BACK);
    
    removeChildByName("Textpop");
    Manor::getInstance()->showLayer(Manor::kMainLayer);
}

void HeroLayer::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(HeroLayer::removeFromParent, this)), NULL));
}

void HeroLayer::updateProperty()
{

}

void HeroLayer::recruitBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        //训练出最基础的英雄, 这里有待检查事件问题...
        recruit();
    }
}

void HeroLayer::recruit()
{
    data::Task::getInstance()->buttonCheck(data::Task::BtnID::kRecruit);
    _recruitBtnArea->stopAllActions();
    _recruitBtnArea->getChildByName("newdot")->setVisible(false);
    _recruitBtn->stopAllActions();
    _recruitBtn->setOpacity(255);
    if (!getChildByName("hero_gewala"))
    {
        auto rdata = data::Role::getInstance()->getRoleByRID(data::Role::BASE_ROLE_RID).asValueMap();
        addChild(RecruitAsk::create(rdata), HERO_CHOICE_POP, "hero_gawala");
    }
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::TRAINING_GET);
}

void HeroLayer::addTrainHeroPopup(int rid)
{
    auto advanceHeroes = data::Role::getInstance()->getRoleByOriginRID(rid).asValueVector();
    auto popup = HeroChoicePopup::create(rid);
    
    if (!getChildByName("hero_choice"))
    {
        this->addChild(popup, HERO_CHOICE_POP, "hero_choice");
    }
}

void HeroLayer::onTrainNewRole(cocos2d::EventCustom *event)
{
    this->updateList();
}

void HeroLayer::showInfo(std::string &info, Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(info,node);
        this->addChild(pop, 100, "Textpop");
    }
    
}

/*******************heroListItem*****************************/
std::string const HeroLayer::HeroListItem::TRAIN_HERO_CHOICE("trainHeroChoice");
#define BTN_ACT 11
#define INFO_BTN_ACT 12

HeroLayer::HeroListItem::~HeroListItem()
{
//    data::Role::getInstance()->removeListener(data::Role::ROLE_TRAIN);
    this->removeListener(HeroListItem::TRAIN_HERO_CHOICE);
}

HeroLayer::HeroListItem *HeroLayer::HeroListItem::create(ValueMap itemData, HeroLayer* delegate)
{
    HeroListItem* ret = new HeroListItem();
    ret->_delegate = delegate;
    if (ret && ret->init(itemData))
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

bool HeroLayer::HeroListItem::init(ValueMap itemData)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _itemData = itemData;
    _rid = _itemData["rid"].asInt();
    _updated = true;
    
    CCASSERT(!_delegate->_itemModel.empty(), "");
    auto view = _delegate->_itemModel.at(0)->clone();
    view->setPosition(Point::ZERO);
    this->setContentSize(view->getContentSize());
    this->addChild(view);
    
    //显示名字和条件..
    auto name = seekChildByNameWithRetType<ui::Text*>(this, "name");
    name->setString(itemData["name"].asString());
    
    _nameBtn = seekChildByNameWithRetType<ui::Layout*>(this, "nameback");
    _nameBtn->setTouchEnabled(true);
    _nameBtn->addTouchEventListener(CC_CALLBACK_2(HeroListItem::trainHeroCallback, this));
    
//    name->addTouchEventListener(CC_CALLBACK_2(HeroListItem::trainHeroCallback, this));
    seekChildByNameWithRetType<ui::Text*>(this, "skillinfo")->setString("技能:"+itemData["skill"].asString());
    
    seekChildByNameWithRetType<ui::Text*>(this, "lifeinfo")->setString("生命:"+itemData["life"].asString());
    seekChildByNameWithRetType<ui::Text*>(this, "powerInfo")->setString("威力:"+itemData["power"].asString());
    seekChildByNameWithRetType<ui::Text*>(this, "speedInfo")->setString("速度:"+itemData["speed"].asString());
    
    seekChildByNameWithRetType<ui::Layout*>(this, "infoBtnArea")->addTouchEventListener(CC_CALLBACK_2(HeroLayer::HeroListItem::infoBtnCall, this));
    
    //显示数量..
    if (itemData["amount"].asInt() > 0)
    {
        seekChildByNameWithRetType<ui::Text*>(this, "num")->setString(itemData["amount"].asString());
    }
    else
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "numtip")->setVisible(false);
    }
    
    //显示星级..
    for (auto i = 1; i < _itemData["level"].asInt(); i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(true);
    }
    for (auto i = _itemData["level"].asInt() + 1; i < 6 ; i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(false);
    }
    
    //事件注册..
//    _trainListener = EventListenerCustom::create(data::Role::ROLE_TRAIN, CC_CALLBACK_1(HeroListItem::onTrainEvent, this));
//    data::Role::getInstance()->addListener(_trainListener);
    return true;
}

int HeroLayer::HeroListItem::getRid()
{
    return _rid;
}

void HeroLayer::HeroListItem::infoBtnCall(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType touchType)
{
    if (touchType == ui::Layout::TouchEventType::BEGAN)
    {
        auto light = seekChildByNameWithRetType<ui::ImageView*>(this, "infolight");
        light->setVisible(true);light->setOpacity(255);
    }
    if (touchType == ui::Layout::TouchEventType::MOVED)
    {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(sender);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de < 20)
        {
            seekChildByNameWithRetType<ui::ImageView*>(this, "infolight")->setVisible(false);
        }
    }
    if (touchType == ui::Layout::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        seekChildByNameWithRetType<ui::ImageView*>(this, "infolight")->runAction(Sequence::create(FadeOut::create(0.3),  NULL) );
        HeroListItem::showInfo();
    }
    if (touchType == Layout::TouchEventType::CANCELED)
    {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(sender);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de < 25)
        {
//            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
            SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
            seekChildByNameWithRetType<ui::ImageView*>(this, "infolight")->runAction(Sequence::create(FadeOut::create(0.3), NULL) );
        }
    }
}

void HeroLayer::HeroListItem::showInfo()
{
    auto light = seekChildByNameWithRetType<ui::ImageView*>(this, "infolight");
    if (light)
    {
        light->setVisible(false);
    }
    
    std::string info =  _itemData["summary"].asString() + "\n";
    if (_itemData["effect"].isNull() || _itemData["effect"].asValueMap()["name"].getType() == Value::Type::NONE)
    {
        info = info + "效果: " + "无\n";
    }
    else
    {
        info = info + "效果: " + _itemData["effect"].asValueMap()["name"].asString() + "\n";
    }
    info = info +
    "技能: " + _itemData["skill"].asString() + "\n" +
    "生命: " + _itemData["life"].asString() + "  威力: " + _itemData["power"].asString() + "  速度: " + _itemData["speed"].asString(); + "\n";
    
    auto area = seekChildByNameWithRetType<ui::Layout*>(this, "nameback");
    //auto rect = area->getBoundingBox();
    //rect.origin = area->getWorldPosition();
    _delegate->showInfo(info, area);
}

void HeroLayer::HeroListItem::trainHeroCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED || touchType == ui::Button::TouchEventType::CANCELED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        trainHero();
    }
    //BTN_CALL_ACT(touchType, sender, sender, CC_CALLBACK_0(HeroLayer::HeroListItem::trainHero, this))
}

void HeroLayer::HeroListItem::trainHero()
{
    int dv = _itemData["rid"].asInt();
    if (dv == 000000)
    {
        auto ret = data::Role::getInstance()->train(100001).asValueMap();
        
        return;
    }
    _delegate->addTrainHeroPopup(dv);
}

//void HeroLayer::HeroListItem::onTrainEvent(cocos2d::EventCustom *event)
//{
////    int rid = _itemData["rid"].asInt();
////    auto tempData = data::Role::getInstance()->getUserRoleByRID(_itemData["rid"].asInt()).asValueMap();
////    seekChildByNameWithRetType<ui::Text*>(this, "num")->setString(tempData["amount"].asString());
//}

void HeroLayer::HeroListItem::updateView(cocos2d::ValueMap data)
{
    seekChildByNameWithRetType<ui::Text*>(this, "num")->setString(data["amount"].asString());
    _updated = true;
}
/****************************************************************************
 *	@desc	探险界面
 *	@date	2015-1-14
 *	@author	110101
     
 *	@file	manor/VentureLayer.cpp
 *  @modify	null
 ******************************************************************************/
#include "VentureLayer.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "manor/Achievement.h"
#include "manor/TalentLayer.h"
#include "crusade/CrusadeMap.h"
#include "data/Crusade.h"
#include "data/Item.h"
#include "popUp/ChoicePopup.h"
#include "manor/Ranking.h"
#include "manor/Manor.h"
#include "data/Task.h"
#include "GlobalMacros.h"
#include "core/custom/LibDate.h"
#include "manor/Translate.h"
#include "notice/Notice.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"

USING_NS_CC;
USING_NS_DU;

#define AchievementLayerName "AchievementLayer"
#define TalentLayerName "TalentLayer"
#define TO_S(_var_) #_var_
#define breadID 100103

VentureLayer::~VentureLayer()
{
	//data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(packageChangeListener)));
    //data::Crusade::getInstance()->removeListener(_listenerMap.at(TO_S(carrigeChangeListener)));
    //data::Role::getInstance()->removeListener(_listenerMap.at(TO_S(roleTarinListener)));
    //data::Task::getInstance()->removeListener(_listenerMap.at(TO_S(tasklistener)));
}

bool VentureLayer::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    /**
     *  新手引导
     */
    _guide = Guide::create(data::Guid::VENTURE_LAYER);
    this->addChild(_guide,10000);
    _guide->addFinishGuid();
    //初始化显示..
    Vec2 winSize = deerResouseSize;
    auto CSNode = CSLoader::getInstance()->createNodeWithFlatBuffersFile("crusadeTavern.csb");
    auto view = seekChildByNameWithRetType<ui::Layout*>(CSNode, "panel");
    view->retain();
    view->removeFromParent();
    this->addChild(view);
    view->release();
    this->setContentSize(Director::getInstance()->getWinSize());
    ui::Helper::doLayout(this);
    
    //初始化模板..
    auto list = seekChildByNameWithRetType<ui::ScrollView*>(view, "list");
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "item"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "item_0"));
    _itemModel.at(0)->removeFromParent();
    _itemModel.at(1)->removeFromParent();
    _itemModel.at(0)->retain();
    _itemModel.at(1)->retain();
    list->removeAllChildren();
    
    //初始化按钮..
    //成就按钮..
    auto achievementBtn = seekChildByNameWithRetType<ui::Layout*>(this, "achievementBtnArea");
    achievementBtn->addTouchEventListener(CC_CALLBACK_2(VentureLayer::achievementButtonCallback, this));
    achievementBtn->setTouchEnabled(true);
    _guide->insertButton(data::Guid::VENTURE_BUTTON_ACHIEVEMENT, achievementBtn);
    
    //天赋按钮..
    auto talentBtn = seekChildByNameWithRetType<ui::Layout*>(this, "talentBtnArea");
    talentBtn->addTouchEventListener(CC_CALLBACK_2(VentureLayer::tallentButtonCallback, this));
    _guide->insertButton(data::Guid::VENTURE_BUTTON_TALENT, talentBtn);
    
    //排行榜按钮..
    auto rankBtn = seekChildByNameWithRetType<ui::Layout*>(this, "rankBtnArea");
    rankBtn->addTouchEventListener(CC_CALLBACK_2(VentureLayer::rankingBtnCallback, this));
    _guide->insertButton(data::Guid::VENTURE_BUTTON_RANK, rankBtn);
    
    //出征按钮..
    auto ventureBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "crusadeBtnArea");
    ventureBtnArea->addTouchEventListener(CC_CALLBACK_2(VentureLayer::goVentureButtonCallback, this));
    ventureBtnArea->setSwallowTouches(false);
    ventureBtnArea->setTouchEnabled(true);
    _guide->insertButton(data::Guid::VENTURE_BUTTON_VENTURE, ventureBtnArea);
    
    auto ventureBtn = seekChildByNameWithRetType<Node*>(this, "crusadenode");
    ventureBtn->setVisible(false);
    
    //初始化时刷新按钮..
    updateBtns();
    
    //返回按钮..
    seekChildByNameWithRetType<ui::Layout*>(view, "gobacktBtnArea")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::Layout*>(view, "gobacktBtnArea")->addTouchEventListener(CC_CALLBACK_2(VentureLayer::backButtonCallback, this));
    _guide->insertButton(data::Guid::VENTURE_BUTTON_BACK, seekChildByNameWithRetType<ui::Layout*>(view, "gobacktBtnArea"));
    
    this->upDateItems();
    //设置天空大小，天空海阔你与我
    auto starSky = view->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winSize.x/2,(int)winSize.y*75/128));
    
    //刷新显示..
    this->onRoleChange(nullptr);
    this->onPackageChange(nullptr);
    
    //添加事件的监听使得队伍人数和背包容量可以实时变化..
    EventListenerCustom * carrigeChangeListener = data::Crusade::getInstance()->addListener(data::Crusade::CARRIAGE_CHANGE, CC_CALLBACK_1(VentureLayer::onRoleChange, this));
    addEventPool(carrigeChangeListener, data::Crusade::getInstance());
    
    EventListenerCustom * packageChangeListener = data::Crusade::getInstance()->addListener(data::Crusade::PACKAGE_CHANGE, CC_CALLBACK_1(VentureLayer::onPackageChange, this));
    addEventPool(packageChangeListener, data::Crusade::getInstance());
    
    EventListenerCustom * roleTarinListener = data::Crusade::getInstance()->addListener(data::Role::ROLE_TRAIN, CC_CALLBACK_1(VentureLayer::onRoleTrain, this));
    addEventPool(roleTarinListener, data::Crusade::getInstance());
    
    
    /**
     *  游戏事件侦听
     */
    EventListenerCustom * tasklistener = data::Task::getInstance()->addListener(data::Task::TASK_COMPLETE, std::bind(&VentureLayer::onTaskCompleted, this, std::placeholders::_1));
    addEventPool(tasklistener, data::Task::getInstance());
    
    return true;
}
void VentureLayer::removeSelf()
{
    
}
void VentureLayer::updateProperty()
{

}
void VentureLayer::onEnter()
{
    Layout::onEnter();
}

void VentureLayer::showBoard()
{
	addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
	addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}

void VentureLayer::runButtonShowCheck()
{
    if (!_btnToRunShowAct.empty())
    {
        runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(VentureLayer::runButtonShowAct, this)), NULL));
    }
}

void VentureLayer::runButtonShowAct()
{
    if (!_btnToRunShowAct.empty())
    {
        auto btn = _btnToRunShowAct.front();
        btn->setOpacity(0);
        btn->setVisible(true);
        seekChildByName(btn, "newdot")->setVisible(true);
        btn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 127.5),FadeTo::create(0.5, 255), NULL)));
        _btnToRunShowAct.eraseObject(btn);
    }
}

void VentureLayer::onTaskCompleted(EventCustom* e)
{
    auto data = static_cast<Value*>(e->getUserData());
    for (auto &i : (*data).asValueMap()["result"].asValueVector())
    {
        if (i.asValueMap()["type"].asInt() == data::Task::BUTTON)
        {
            if (i.asValueMap()["type_id"].asInt() == data::Task::kCrusade)
            {
                _btnToRunShowAct.pushBack(seekChildByName(this, "crusadenode"));
            }
        }
    }
}

void VentureLayer::updateBtns()
{
    auto btns = data::Task::getInstance()->getButtons();
    for (auto &i : btns)
    {
        if (i == kAchievemtnBtn)
        {
            //seekChildByNameWithRetType<Node*>(this, "achievementnode")->setVisible(true);
        }
        else if (i == kCrusadeBtn)
        {
            auto btn = seekChildByNameWithRetType<Node*>(this, "crusadenode");
            btn->setVisible(true);
            
            
            if (!data::Task::getInstance()->BtnIsTouched(data::Task::BtnID::kCrusade))
            {
                btn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 127.5),FadeTo::create(0.5, 255), NULL)));
                
                seekChildByName(btn, "newdot")->setVisible(true);

            }

            
        }
        else if (i == kRankBtn)
        {
            //seekChildByNameWithRetType<Node*>(this, "ranknode")->setVisible(true);
        }
        else if (i == kTalentBtn)
        {
            //seekChildByNameWithRetType<Node*>(this, "talentnode")->setVisible(true);
        }
    }
}

void VentureLayer::backButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    auto btn = dynamic_cast<ui::Layout*>(sender);
    if (touchType == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() + 20);
    }
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() - 20);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
    else if (touchType == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() - 20);
        backReal();
    }
}

void VentureLayer::backReal()
{
    data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_BACK);
    Manor::getInstance()->showLayer(Manor::kMainLayer);
}

void VentureLayer::rankingBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        rankingBtnCall();
    }
}

void VentureLayer::rankingBtnCall()
{
     Manor::getInstance()->showLayer(Manor::kRank);
}

void VentureLayer::achievementButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (TouchEventType::ENDED == touchType)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        achievementBtnCall();
    }
}

void VentureLayer::achievementBtnCall()
{
    data::Task::getInstance()->buttonCheck(kAchievemtnBtn);
    Manor::getInstance()->showLayer(Manor::kAchivement);
}

void  VentureLayer::tallentButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (TouchEventType::ENDED == touchType)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        tallentBtnCall();
    }
}

void VentureLayer::tallentBtnCall()
{
    Manor::getInstance()->showLayer(Manor::kTalent);
}

void VentureLayer::goVentureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (TouchEventType::ENDED == touchType)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        goVenture();
    }
}

void VentureLayer::goVenture()
{
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_VENTURE);
    
    data::Task::getInstance()->buttonCheck(data::Task::kCrusade);
    auto btn = seekChildByName(this, "crusadenode");
    btn->stopAllActions();
    btn->setOpacity(255);
    seekChildByName(btn, "newdot")->setVisible(false);
    //这里要检验队伍..
    int roleNum = data::Crusade::getInstance()->getCarriageCurrent();
    
    int totalrole = 0;
    auto userRole = data::Role::getInstance()->getUserRoles().asValueVector();
    for (auto &i : userRole)
    {
        totalrole += i.asValueMap().at("amount").asInt();
    }
    
    int breadNum = data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD);
    int itemNum = data::Crusade::getInstance()->getPackageCurrent();
    
    if (totalrole == 0)
    {
		Notice::getInstance()->postTips("现在一个士兵都没有!\n请先去训练界面招募一个士兵吧!  ");
    }
    else if (roleNum == 0)
    {
		Notice::getInstance()->postTips("探险队伍一个人也没有!\n请先增加探险人数! ");
    }
    else if (roleNum > 6 /*roleTotal*/)
    {
		Notice::getInstance()->postTips("人数过多  ");
    }
    else if (data::Item::getInstance()->getUserItemAmountByIID(data::Item::FOOD) == 0)
    {
		Notice::getInstance()->postTips("现在一个太空能量都没有!\n请先去生产界面产出一些太空能量吧! ");
    }
    else if (breadNum == 0)
    {
		Notice::getInstance()->postTips("必须携带太空能量才可以出发!\n请先增加携带数量!  ");
    }
    else if (itemNum > data::Crusade::getInstance()->getPackageTotal())
    {
		Notice::getInstance()->postTips("背包容量太小  ");
    }
    else
    {
        data::Crusade::getInstance()->depart();
//        CrusadeMap::goToCrusadeMapScene();
        Manor::getInstance()->exitManorToMap();
    }
}

void VentureLayer::onRoleChange(cocos2d::EventCustom *event)
{
    int current = data::Crusade::getInstance()->getCarriageCurrent();
    int total = data::Crusade::getInstance()->getCarriageTotal();
    seekChildByNameWithRetType<ui::Text*>(this, "carriageNum")->setString(Value(current).asString()+"/"+Value(total).asString());
    seekChildByNameWithRetType<ui::Text*>(this, "carriageNum")->stopAllActions();
    seekChildByNameWithRetType<ui::Text*>(this, "carriageNum")->runAction(Sequence::create(ScaleTo::create(0.1, 1.3),ScaleTo::create(0.1, 1), NULL));
}

void VentureLayer::onPackageChange(cocos2d::EventCustom *event)
{
    int current = data::Crusade::getInstance()->getPackageSpace();
    int total = data::Crusade::getInstance()->getPackageTotal();
    seekChildByNameWithRetType<ui::Text*>(this, "packageNum")->setString(Value(current).asString()+"/"+Value(total).asString());
    seekChildByNameWithRetType<ui::Text*>(this, "packageNum")->stopAllActions();
    seekChildByNameWithRetType<ui::Text*>(this, "packageNum")->runAction(Sequence::create(ScaleTo::create(0.1, 1.3),ScaleTo::create(0.1, 1), NULL));
}

void VentureLayer::onRoleTrain(cocos2d::EventCustom *event)
{
    this->upDateItems();
    //这里需要显示新手引导..
    data::Guid::getInstance()->showGuid();
}

void VentureLayer::upDateItems()
{
    if(!_isUpdateList)
    {
        _isUpdateList = true;
        auto list = seekChildByNameWithRetType<ui::ScrollView*>(this, "list");
        list->removeAllChildren();
        
        auto itemList = data::Crusade::getInstance()->getPackage();
        auto roleList = data::Crusade::getInstance()->getCarriage();
        int rowNum = (int)itemList.asValueVector().size()+(int)roleList.asValueVector().size();
        int spaceH = _itemModel.at(0)->getContentSize().height;
        list->setInnerContainerSize(Size(list->getInnerContainerSize().width,(rowNum+1)*spaceH+_itemModel.at(1)->getContentSize().height));
        
        Size listSize = list->getInnerContainerSize();
        auto item = _itemModel.at(1)->clone();
        list->addChild(item);
        item->setPositionY(listSize.height-_itemModel.at(1)->getContentSize().height);
        
        int nMaxZoder = 1000;
        int j = 0;
        for (auto i : itemList.asValueVector())
        {
            auto item = CrusadeListItem::create(i.asValueMap(), this);
            item->setZOrder(nMaxZoder--);
            list->addChild(item);
            float y = listSize.height-_itemModel.at(1)->getContentSize().height- spaceH*j - spaceH;
            item->setPositionY(y);
            //item->setPositionX(item->getContentSize().width/2.0f);
            _guide->insertButton(item->idDecrease, seekChildByNameWithRetType<Layout*>(item, "decreaseBtnArea"));
            _guide->insertButton(item->idAdd, seekChildByNameWithRetType<Layout*>(item, "increaseBtnArea"));
            j++;
        }
        
        for (auto i : roleList.asValueVector())
        {
            auto item = CrusadeListItem::create(i.asValueMap(), this);
            item->setZOrder(nMaxZoder--);
            list->addChild(item);
            float y = listSize.height-_itemModel.at(1)->getContentSize().height- spaceH*j - spaceH;
            item->setPositionY(y);
            //item->setPositionX(item->getContentSize().width/2.0f);
            _guide->insertButton(item->idDecrease, seekChildByNameWithRetType<Layout*>(item, "decreaseBtnArea"));
            _guide->insertButton(item->idAdd, seekChildByNameWithRetType<Layout*>(item, "increaseBtnArea"));
            j++;
        }
        
        
        /**
         *  其他刷新
         */
        this->onRoleChange(nullptr);
        this->onPackageChange(nullptr);
        _isUpdateList = false;
    }
    
}

/**************************listitem**********************/
VentureLayer::CrusadeListItem::~CrusadeListItem()
{
    //data::Crusade::getInstance()->removeListener(_lisMap.at(TO_S(numberchange)));
}

VentureLayer::CrusadeListItem * VentureLayer::CrusadeListItem::create(cocos2d::ValueMap itemData, VentureLayer* delegate)
{
    CrusadeListItem *ret = new CrusadeListItem();
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

bool VentureLayer::CrusadeListItem::init(cocos2d::ValueMap itemData)
{
    if (!Layout::init())
    {
        return false;
    }
    _longPressedIncrease = false;
    _longPressedDecrease = false;
    
    _itemData = itemData;
    
    //初始化显示..
    CCASSERT(!_delegate->_itemModel.empty(), "");
    auto view = _delegate->_itemModel.at(0)->clone();
    this->setContentSize(view->getContentSize());
    this->addChild(view);
    view->setPosition(Point::ZERO);
    
    seekChildByNameWithRetType<ui::Text*>(view, "name")->setString(_itemData["name"].asString());
    
    std::string fitStr;
    MAP_CANT_FIND(itemData, "current");
    auto fit = itemData["current"].asInt();
    if (fit == 0) fitStr = "0"; else fitStr = itemData["current"].asString();
    seekChildByNameWithRetType<ui::Text*>(view, "number")->setString(fitStr);
    
    seekChildByNameWithRetType<ui::Layout*>(view, "increaseBtnArea")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::Layout*>(view, "increaseBtnArea")->addTouchEventListener(CC_CALLBACK_2(CrusadeListItem::increaseButtonCallback, this));

    seekChildByNameWithRetType<ui::Layout*>(view, "decreaseBtnArea")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::Layout*>(view, "decreaseBtnArea")->addTouchEventListener(CC_CALLBACK_2(CrusadeListItem::decreaseButtonCallback, this));

    
    //添加事件..
    if (!itemData["iid"].isNull())
    {
        idAdd = itemData["iid"].asInt();
        idDecrease = itemData["iid"].asInt()*10;
        _itemType = ItemType::kItem;
        EventListenerCustom* numberchange = data::Crusade::getInstance()->addListener(data::Crusade::PACKAGE_CHANGE, CC_CALLBACK_1(CrusadeListItem::onNumberChange, this));
        addEventPool(numberchange, data::Crusade::getInstance());
    }
    else if (!itemData["rid"].isNull())
    {
        idAdd = itemData["rid"].asInt();
        idDecrease = itemData["rid"].asInt()*10;
        
        _itemType = ItemType::kRole;
        EventListenerCustom* numberchange = data::Crusade::getInstance()->addListener(data::Crusade::CARRIAGE_CHANGE, CC_CALLBACK_1(CrusadeListItem::onNumberChange, this));
        addEventPool(numberchange, data::Crusade::getInstance());
    }
    
    //根据数据库中的内容显示数量..
    //    this->onNumberChange(nullptr);
    
    return true;
}
void VentureLayer::CrusadeListItem::removeSelf()
{

}
void VentureLayer::CrusadeListItem::updateProperty()
{

}
void VentureLayer::CrusadeListItem::increaseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType eventType)
{
    if (eventType == ui::Button::TouchEventType::BEGAN)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setVisible(true);
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setOpacity(255);
        
        //这里需要运行长按检测,需要一个标识位这个位在 LONG_PRESSED_TIME 之后执行
        auto longPressAct = Sequence::create(DelayTime::create(LONG_PRESSED_TIME), CallFunc::create([this](){this->_longPressedIncrease = true;
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd, true);}), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::increaseFast, this)), nullptr);
        longPressAct->setTag(11);
        runAction(longPressAct);
    }
    if (eventType == ui::Button::TouchEventType::MOVED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setVisible(false);
    }
    if (eventType == ui::Text::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->stopAllEffects();
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd);
        CallFunc* actCall = nullptr;
        if(!_longPressedIncrease)
        {
            actCall = CallFunc::create(CC_CALLBACK_0(CrusadeListItem::increase, this));
        }
        else
        {
            if (_itemType == kRole)
            {
                data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_SOLDIGER_ADD);
            }
            else if (_itemType == kItem)
            {
                data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_ADD);
            }
        }
        
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->runAction(Sequence::create(FadeOut::create(0.3), actCall, NULL) );
        stopAllActionsByTag(11);
        _longPressedIncrease = false;
        _fastOpreateIndex = 0;
    }
    else if (eventType == ui::Button::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->stopAllEffects();
        stopAllActionsByTag(11);
        _longPressedIncrease = false;
        _fastOpreateIndex = 0;
        if (_itemType == kRole)
        {
            data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_SOLDIGER_ADD);
        }
        else if (_itemType == kItem)
        {
            data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_ADD);
        }
    }
}
void VentureLayer::CrusadeListItem::playFastAddMusic()
{

}
void VentureLayer::CrusadeListItem::increase()
{
    if (_itemType == kRole)
    {
        int rid = _itemData["rid"].asInt();
        int retcode = data::Crusade::getInstance()->carriageIn(rid);
        if (retcode == data::Crusade::INOUT_OK)
        {
            
        }
        else if (retcode == data::Crusade::IN_ERROR_OVER)
        {
			Notice::getInstance()->postTips("人数已满  ");
        }
        else if (retcode == data::Crusade::IN_ERROR_SHOTAGE)
        {
			Notice::getInstance()->postTips("没有更多  ");
        }
        //更新新手引导状态..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_SOLDIGER_ADD);
    }
    else if (_itemType == kItem)
    {
        int retcode = data::Crusade::getInstance()->packageIn(_itemData["iid"].asInt());
        if (retcode == data::Crusade::INOUT_OK)
        {
            
        }
        else if (retcode == data::Crusade::IN_ERROR_OVER)
        {
			Notice::getInstance()->postTips("背包已满  ");
        }
        else if (retcode == data::Crusade::IN_ERROR_SHOTAGE)
        {
			Notice::getInstance()->postTips("没有更多资源了  ");
        }
        //更新新手引导状态..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::VENTURE_ADD);
    }
    
}

void VentureLayer::CrusadeListItem::increaseFast()
{
    if(_delegate->result == ValueMapNull)
    {
        _fastOpreateIndex++;
        int retcode;
        if (_itemType == kRole)
        {
            int rid = _itemData["rid"].asInt();
            if(_fastOpreateIndex <= 5)
            {
                retcode = data::Crusade::getInstance()->carriageIn(rid);
            }
            else if(_fastOpreateIndex > 5 && _fastOpreateIndex <= 8)
            {
                retcode = data::Crusade::getInstance()->carriageIn(rid,_fastOpreateIndex);
            }
            else if(_fastOpreateIndex >8)
            {
                retcode = data::Crusade::getInstance()->carriageIn(rid,8);
            }
            
        }
        else if (_itemType == kItem)
        {
            if(_fastOpreateIndex <= 5)
            {
                retcode = data::Crusade::getInstance()->packageIn(_itemData["iid"].asInt());
            }
            else if(_fastOpreateIndex > 5 && _fastOpreateIndex <= 8)
            {
                retcode = data::Crusade::getInstance()->packageIn(_itemData["iid"].asInt(),_fastOpreateIndex);
            }
            else if(_fastOpreateIndex >8)
            {
                retcode = data::Crusade::getInstance()->packageIn(_itemData["iid"].asInt(),8);
            }
            
        }
        
        if (_longPressedIncrease&&retcode== data::Crusade::INOUT_OK)
        {
            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::increaseFast, this)), nullptr));
        }
        else
        {
            if(_fastOpreateIndex == 1)
            {
                _fastOpreateIndex = 0;
                SoundManager::getInstance()->stopAllEffects();
            }
            else if(_fastOpreateIndex > 1)
            {
                _fastOpreateIndex = 0;
                runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::increaseFast, this)), nullptr));
            }
            
        }
    }
}

void  VentureLayer::CrusadeListItem::decreaseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType eventType)
{
    if (eventType == ui::Button::TouchEventType::BEGAN)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setVisible(true);
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setOpacity(255);
        auto longPressAct = Sequence::create(DelayTime::create(LONG_PRESSED_TIME), CallFunc::create([this](){this->_longPressedDecrease = true;SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd, true);}), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::decreaseFast, this)), nullptr);
        longPressAct->setTag(11);
        runAction(longPressAct);
    }
    else if (eventType == ui::Button::TouchEventType::MOVED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setVisible(false);
    }
    else if (eventType == ui::Text::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->stopAllEffects();
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd);
        CallFunc *actCall = nullptr;
        if(!_longPressedDecrease)
        {
            actCall = CallFunc::create(CC_CALLBACK_0(CrusadeListItem::decrease, this));
        }
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->runAction(Sequence::create(FadeOut::create(0.3), actCall, NULL) );
        stopAllActionsByTag(11);
        _longPressedDecrease = false;
    }
    else if (eventType == ui::Text::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->stopAllEffects();
        stopAllActionsByTag(11);
        _longPressedDecrease = false;
    }
}

void VentureLayer::CrusadeListItem::decrease()
{
    if (_itemType == kRole)
    {
        int rid = _itemData["rid"].asInt();
        int retcode = data::Crusade::getInstance()->carriageOut(rid);
        
        if (retcode == data::Crusade::INOUT_OK)
        {
            
        }
        else if (retcode == data::Crusade::OUT_ERROR_SHOTAGE)
        {
			Notice::getInstance()->postTips("没有战士了 ");
        }
    }
    else if (_itemType == kItem)
    {
        int retcode = data::Crusade::getInstance()->packageOut(_itemData["iid"].asInt());
        
        if (retcode == data::Crusade::INOUT_OK)
        {
            
        }
        else if (retcode == data::Crusade::OUT_ERROR_SHOTAGE)
        {
			Notice::getInstance()->postTips("背包中没有了 ");
        }
    }
}

void VentureLayer::CrusadeListItem::decreaseFast()
{
    _fastOpreateIndex++;
    int retcode;
    if (_itemType == kRole)
    {
        int rid = _itemData["rid"].asInt();
        if(_fastOpreateIndex <= 5)
        {
            retcode = data::Crusade::getInstance()->carriageOut(rid);
        }
        else if(_fastOpreateIndex > 5&&_fastOpreateIndex <= 8)
        {
            retcode = data::Crusade::getInstance()->carriageOut(rid,_fastOpreateIndex);
        }
        else
        {
            retcode = data::Crusade::getInstance()->carriageOut(rid,8);
        }
        
    }
    else if (_itemType == kItem)
    {
        if(_fastOpreateIndex <= 5)
        {
            retcode = data::Crusade::getInstance()->packageOut(_itemData["iid"].asInt());
        }
        else if(_fastOpreateIndex > 5&&_fastOpreateIndex <= 8)
        {
            retcode = data::Crusade::getInstance()->packageOut(_itemData["iid"].asInt(),_fastOpreateIndex);
        }
        else
        {
            retcode = data::Crusade::getInstance()->packageOut(_itemData["iid"].asInt(),8);
        }
        
    }
    if (_longPressedDecrease&&retcode == data::Crusade::INOUT_OK)
    {
        runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::decreaseFast, this)), nullptr));
    }
    else
    {
        if(_fastOpreateIndex == 1)
        {
            _fastOpreateIndex = 0;
            SoundManager::getInstance()->stopAllEffects();
        }
        else if(_fastOpreateIndex > 1)
        {
            _fastOpreateIndex = 0;
            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CrusadeListItem::decreaseFast, this)), nullptr));
        }
    }
}

void VentureLayer::CrusadeListItem::onNumberChange(cocos2d::EventCustom *event)
{
    if (_itemType == kRole)
    {
        int amount = data::Crusade::getInstance()->getCarriageCurrent(_itemData["rid"].asInt());
        seekChildByNameWithRetType<ui::Text*>(this, "number")->setString(Value(amount).asString());
    }
    else if (_itemType == kItem)
    {
        auto amount = data::Crusade::getInstance()->getPackageCurrent(_itemData["iid"].asInt());
        seekChildByNameWithRetType<ui::Text*>(this, "number")->setString(Value(amount).asString());
    }
}

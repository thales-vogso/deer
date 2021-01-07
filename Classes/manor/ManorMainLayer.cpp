/****************************************************************************
     
 *	@desc   收集界面的选项
 *	@date	2014-12-12
 *	@author	110101
     
 *	@file	manor/ManorMainLayer.h
 *  @modify	NULL
 ******************************************************************************/

#include "ManorMainLayer.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "manor/CollectionLayer.h"
#include "manor/Resouce.h"
#include "manor/HeroLayer.h"
#include "manor/Produce.h"
#include "manor/Manor.h"
#include "data/Achievement.h"
#include "utils/LoadingAction.h"
#include "data/Task.h"
#include "data/Item.h"
#include "notice/Notice.h"
#include "data/Guid.h"
#include "data/Lang.h"

#include "utils/DeerUtils.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"

#include "RmbBuyLayer.h"

USING_NS_CC;
USING_NS_DU;

#define GET_BUTTON_1(_var1_) \
auto _var1_ = DeerUtils::seekChildByNameWithRetType<ui::Button*>(this, #_var1_);

#define TO_S(_var_) #_var_

const std::string ManorMainLayer::MAIN_VIEW_NAME_CS("MainLayer");

const std::string ManorMainLayer::TURN_COLLECTION("TurnToCollection");

const std::string ManorMainLayer::TURN_VENTURE("TurnToVenture");

ManorMainLayer::ManorMainLayer()
:_btnsInMain({data::Task::kAlchemyBtn,data::Task::kBuildBtn,data::Task::kWarehouseBtn,data::Task::kStoreBtn, data::Task::kProduceBtn, data::Task::kHeroBtn, data::Task::kCrystalBtn, data::Task::kSettingsBtn, data::Task::kCollectionBtn, data::Task::kVentureBtn})//这里初始化要按照从小到大的顺序，这样可以省去之后的求交集运算和求并集运算之前的排序操作..
{
    /**
     *  按钮的位置是动态的，所以我们在初始的时候初始化一个数组来保存位置
     */
    _buttonPosData[1] = std::vector<Vec2>({Vec2(360, 840)});
    _buttonPosData[2] = std::vector<Vec2>({Vec2(274, 840), Vec2(446, 840)});
    _buttonPosData[3] = std::vector<Vec2>({Vec2(188, 840), Vec2(360, 840), Vec2(532, 840)});
    _buttonPosData[4] = _buttonPosData[3];_buttonPosData[4].push_back(Vec2(360, 750));
    _buttonPosData[5] = _buttonPosData[3];_buttonPosData[5].push_back(Vec2(274, 750));_buttonPosData[5].push_back(Vec2(446, 750));
    _buttonPosData[6] = _buttonPosData[3];
    _buttonPosData[6].push_back(Vec2(188, 750));
    _buttonPosData[6].push_back(Vec2(360, 750));
    _buttonPosData[6].push_back(Vec2(532, 750));
    _buttonPosData[7] = _buttonPosData[6];_buttonPosData[7].push_back(Vec2(360, 662));
    _buttonPosData[8] = _buttonPosData[6];_buttonPosData[8].push_back(Vec2(274, 662));_buttonPosData[8].push_back(Vec2(446, 662));
}

ManorMainLayer::~ManorMainLayer()
{
    //data::Task::getInstance()->removeListener(_listenMap.at(TO_S(tasklistener)));
    //Notice::getInstance()->removeListener(_listenMap.at(TO_S(fold)));
}

ManorMainLayer * ManorMainLayer::create()
{
    auto ret = new ManorMainLayer();
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
bool ManorMainLayer::init()
{
    if (!Layout::init())
    {
        return false;
    }
    _btnInited = false;
    
    /**
     *  新手引导
     */
    _guide = Guide::create(data::Guid::MAIN_LAYER);
    this->addChild(_guide,10000);
    _guide->addFinishGuid();
    Vec2 winSize = Director::getInstance()->getWinSize();
    this->setContentSize(Size(winSize));
    //载入主界面--
    auto CSNode = CSLoader::getInstance()->createNodeWithFlatBuffersFile("manor.csb");
    addChild(CSNode);
    
    _goldTxt = CSNode->getChildByName("Panel")->getChildByName("userData")->getChildByName<ui::Text*>("goldTxt");
    _foodTxt = CSNode->getChildByName("Panel")->getChildByName("userData")->getChildByName<ui::Text*>("foodTxt");
    _rmbTxt  = CSNode->getChildByName("Panel")->getChildByName("userData")->getChildByName<ui::Text*>("rmbTxt");
    
    std::string  goldNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::COIN).asValueMap()["amount"].asString();
    if(goldNum.size() == 0)
    {
        goldNum = "0";
    }
    _goldTxt->setString(goldNum);
    std::string  foodNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::FOOD).asValueMap()["amount"].asString();
    if(foodNum.size() == 0)
    {
        foodNum = "0";
    }
    _foodTxt->setString(foodNum);
    std::string  rmbNum  = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
    if(rmbNum.size() == 0)
    {
        rmbNum = "0";
    }
	_rmbTxt->setString(rmbNum);
    
    _rmbBtn = CSNode->getChildByName("Panel")->getChildByName("userData")->getChildByName<ui::Button*>("rmbBuy");
    _rmbBtn->setPositionX(_rmbTxt->getPositionX()+_rmbTxt->getContentSize().width*_rmbTxt->getScaleX()+_rmbBtn->getContentSize().width/2.0f*_rmbBtn->getScaleX()+20);
//	_rmbBtn->setTouchEnabled(true);
    _rmbBuyBtn = _rmbBtn->getChildByName<cocos2d::ui::Layout*>("rmbBuyBtn");
	_rmbBuyBtn->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::rmbBuyBtnCall, this));
    
    auto starSky = CSNode->getChildByName("particlesRoot")->getChildByName<ParticleSystemQuad*>("deerbackparticle");
    starSky->setPosVar(Vec2(winSize.x/2,(int)winSize.y*75/128));
    
    _startAction = CSLoader::getInstance()->createTimeline("manor.csb");
 
    CSNode->setContentSize(Size(winSize));
    ui::Helper::doLayout(this);
    
    CSNode->runAction(_startAction);
    
    this->addCallbackForButton();
    
    /**
     *  游戏事件
     */
    EventListenerCustom* tasklistener = data::Task::getInstance()->addListener(data::Task::TASK_COMPLETE, std::bind(&ManorMainLayer::onTaskCompleted, this, std::placeholders::_1));
    addEventPool(tasklistener, data::Task::getInstance());
    
    
    data::Item* item = data::Item::getInstance();
    EventListenerCustom* listener = item->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(ManorMainLayer::itemChange, this));
    addEventPool(listener, item);

    
    return true;
}
void ManorMainLayer::itemChange(cocos2d::EventCustom* e)
{
    static int preGoldNum = 0;
    static int preFoodNum = 0;
    static int preRmbNum = 0;
    std::string  goldNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::COIN).asValueMap()["amount"].asString();
    if(goldNum.size() == 0)
    {
        goldNum = "0";
    }
    _goldTxt->setString(goldNum);
    std::string  foodNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::FOOD).asValueMap()["amount"].asString();
    if(foodNum.size() == 0)
    {
        foodNum = "0";
    }
    _foodTxt->setString(foodNum);
    std::string  rmbNum  = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
    if(rmbNum.size() == 0)
    {
        rmbNum = "0";
    }
    _rmbTxt->setString(rmbNum);
    if(preGoldNum != Value(goldNum).asInt())
    {
        preGoldNum = Value(goldNum).asInt();
        _goldTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.5),ScaleTo::create(0.1, 1), NULL));
    }
    if(preFoodNum != Value(foodNum).asInt())
    {
        preFoodNum = Value(foodNum).asInt();
        _foodTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.5),ScaleTo::create(0.1, 1), NULL));
    }
    if(preRmbNum != Value(rmbNum).asInt())
    {
        preRmbNum = Value(rmbNum).asInt();
        _rmbTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.5),ScaleTo::create(0.1, 1), NULL));
    }
    _rmbBtn->setPositionX(_rmbTxt->getPositionX()+_rmbTxt->getContentSize().width*_rmbTxt->getScaleX()+_rmbBtn->getContentSize().width/2.0f*_rmbBtn->getScaleX()+20);
}
void ManorMainLayer::removeSelf()
{

}
void ManorMainLayer::updateProperty()
{

}
void ManorMainLayer::frameCall(cocostudio::timeline::Frame *f)
{
    if (_startAction->getCurrentFrame() == 250)
    {
        initShowUnTouchState();
    }
}

void ManorMainLayer::onBoardFold(cocos2d::EventCustom* e)
{
	ValueMap param = *(ValueMap*)(e->getUserData());
	if (param["fold"].asBool()){
		_startAction->setTimeSpeed(4);
		_startAction->gotoFrameAndPlay(300, 430, false);
		
	}else{
		_startAction->setTimeSpeed(2);
		_startAction->gotoFrameAndPlay(520, 540, false);
	}
}


void ManorMainLayer::onEnter()
{
    ui::Layout::onEnter();
    showBoard();
    runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create(CC_CALLBACK_0(ManorMainLayer::playStartAnimation, this)), NULL));
    data::Task::getInstance()->baseCheck();
    outLineGainCheck();
}

void ManorMainLayer::outLineGainCheck()
{
    /**
     * 检查离线收获..
     */
    auto ret = data::Building::getInstance()->gain().asValueMap();
    
    std::stringstream sstr;
    for (auto &i : ret)
    {
        if (i.second.asValueMap()["amount"].asInt() > 0 )
        {
			sstr << data::Lang::getInstance()->getWord(120104) << " : " << i.second.asValueMap()["name"].asString() << "x" << i.second.asValueMap()["amount"].asInt() << "," << data::Lang::getInstance()->getWord(120105) << ": " << i.second.asValueMap()["total"].asInt() << "\n";
        }
    }
    if (sstr.str().size() > 0)
    {
		std::string str = data::Lang::getInstance()->getWord(120106) + ":\n" + sstr.str();
		Notice::getInstance()->postTips(str);
		Notice::getInstance()->postBoard(str);
    }
}

void ManorMainLayer::update(float delta)
{
    ui::Layout::update(delta);
    
    
    if (getWorldPosition() == Point::ZERO&& (!_startAction->isPlaying() || _startAction->getCurrentFrame() ==  150))
    {
        runButtonShowAct();
        if (!_btnInited)
        {
            initShowUnTouchState();
        }
    }
}

void ManorMainLayer::showBoard()
{
    addChild(Notice::getInstance()->getBoard(true), Notice::BOARD_Z_ORDER);
    addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER);
}

void ManorMainLayer::addCallbackForButton()
{
    int MaxZorder = 1000;
    //----------炼金-------------
    _alchemyBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "alchemyBtnArea");
    _alchemyBtnArea->setTouchEnabled(true);
    _alchemyBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::alchemyButtonCallback, this));
    
    _alchemyBtn = seekChildByNameWithRetType<Node*>(this, "alchemyBtn");
    _alchemyBtn->setVisible(false);
    _alchemyBtn->setZOrder(MaxZorder--);
    
    //----------建造按钮---------
    _buildBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "buildBtnArea");
    _buildBtnArea->setTouchEnabled(true);
    _buildBtnArea->setSwallowTouches(false);
    _buildBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::buildButtonCallback, this));
    
    _buildBtn = seekChildByNameWithRetType<Node*>(this, "buildBtn");
    _buildBtn->setVisible(false);
    _buildBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_buildBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kBuildBtn));
    
    //-----------仓库按钮-------
    _warehouseBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "warehouseBtnArea");
    _warehouseBtnArea->setTouchEnabled(true);
    _warehouseBtnArea->setSwallowTouches(false);
    _warehouseBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::warehouseButtonCallback, this));
    
    _warehouseBtn = seekChildByNameWithRetType<Node*>(this, "warehouseBtn");
    _warehouseBtn->setVisible(false);
    _warehouseBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_warehouseBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kWarehouseBtn));
    
    //------------商店------------
    _storeBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "storeBtnArea");
    _storeBtnArea->setTouchEnabled(true);
    _storeBtnArea->setSwallowTouches(false);
    _storeBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::storeButtonCallback, this));

    _storeBtn = seekChildByNameWithRetType<Node*>(this, "storeBtn");
    _storeBtn->setVisible(false);
    _storeBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_storeBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kStoreBtn));
    
    //------------水晶按钮--------
    _crystalBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "crystalBtnArea");
    _crystalBtnArea->setTouchEnabled(true);
    _crystalBtnArea->setSwallowTouches(false);
    _crystalBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::crystalBtnCall, this));
    
    _crystalBtn = seekChildByNameWithRetType<Node*>(this, "crystalBtn");
    _crystalBtn->setVisible(false);
    _crystalBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_crystalBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kCrystalBtn));
   
    //------------制作----------
    _practiceBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "produceBtnArea");
    _practiceBtnArea->setTouchEnabled(true);
    _practiceBtnArea->setSwallowTouches(false);
    _practiceBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::practiceButtonCallback, this));

    _practiceBtn = seekChildByNameWithRetType<Node*>(this, "produceBtn");
    _practiceBtn->setVisible(false);
    _practiceBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_practiceBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kProduceBtn));

    //------------英雄----------
    _heroBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "heroBtnArea");
    _heroBtnArea->setTouchEnabled(true);
    _heroBtnArea->setSwallowTouches(false);
    _heroBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::heroButtonCallback, this));

    _heroBtn = seekChildByNameWithRetType<Node*>(this, "heroBtn");
    _heroBtn->setVisible(false);
    _heroBtn->setZOrder(MaxZorder--);
    
    seekChildByNameWithRetType<ui::ImageView*>(_heroBtn, "newdot")->setVisible(!data::Task::getInstance()->BtnIsTouched(data::Task::kHeroBtn));

    

    //-----------设置按钮--------
    _settingBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "settingsBtnArea");
    _settingBtnArea->setTouchEnabled(true);
    _settingBtnArea->setSwallowTouches(false);
    _settingBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::settingsButtonCall, this));

    _settingBtn = seekChildByNameWithRetType<Node*>(this, "settingsBtn");
    _settingBtn->setVisible(false);
    _settingBtn->setZOrder(MaxZorder--);

    
    //收集按钮..
    _collectionBtnArea = DeerUtils::seekChildByNameWithRetType<ui::Layout*>(this, "collectionTextArea");
    _collectionBtnArea->setTouchEnabled(true);
    _collectionBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::collectionButtonCallback, this));
    
    _collectionBtnArea->setVisible(false);

    //探险按钮..
    _ventureBtnArea = DeerUtils::seekChildByNameWithRetType<ui::Layout*>(this, "ventureTextArea");
    _ventureBtnArea->setTouchEnabled(true);
    _ventureBtnArea->addTouchEventListener(CC_CALLBACK_2(ManorMainLayer::ventureButtonCallback, this));

    _ventureBtnArea->setVisible(false);
    
    
#if DG_SHOW_BTN_AREA == 1
    SET_LAYOUT_SHOW(_alchemyBtnArea);
    SET_LAYOUT_SHOW(_storeBtnArea);
    SET_LAYOUT_SHOW(_buildBtnArea);
    SET_LAYOUT_SHOW(_practiceBtnArea);
    SET_LAYOUT_SHOW(_heroBtnArea);
    SET_LAYOUT_SHOW(_warehouseBtnArea);
    SET_LAYOUT_SHOW(_settingBtnArea);
    SET_LAYOUT_SHOW(_crystalBtnArea);
    SET_LAYOUT_SHOW(_collectionBtnArea);
    SET_LAYOUT_SHOW(_ventureBtnArea);
#endif
    
    /**
     *  我们需要把这8个按钮放进_sortButtons里面，因为出征按钮和采集按钮不需要排列位置
     */
//    if (!_sortButttons.empty())
//    {
        _sortButttons.insert(data::Task::kAlchemyBtn, _alchemyBtn);
        _sortButttons.insert(data::Task::kBuildBtn, _buildBtn);
        _sortButttons.insert(data::Task::kWarehouseBtn, _warehouseBtn);
        _sortButttons.insert(data::Task::kStoreBtn, _storeBtn);
        _sortButttons.insert(data::Task::kProduceBtn, _practiceBtn);
        _sortButttons.insert(data::Task::kHeroBtn, _heroBtn);
        _sortButttons.insert(data::Task::kCrystalBtn, _crystalBtn);
        _sortButttons.insert(data::Task::kSettingsBtn, _settingBtn);
//    }
    
    _btnsMapInMain = _sortButttons;
    _btnsMapInMain.insert(data::Task::kCollectionBtn, _collectionBtnArea);
    _btnsMapInMain.insert(data::Task::kVentureBtn, _ventureBtnArea);
    
    _guide->insertButton(data::Task::kAlchemyBtn, _alchemyBtnArea);
    _guide->insertButton(data::Task::kBuildBtn, _buildBtnArea);
    _guide->insertButton(data::Task::kWarehouseBtn, _warehouseBtnArea);
    _guide->insertButton(data::Task::kStoreBtn, _storeBtnArea);
    _guide->insertButton(data::Task::kProduceBtn, _practiceBtnArea);
    _guide->insertButton(data::Task::kHeroBtn, _heroBtnArea);
    _guide->insertButton(data::Task::kCrystalBtn, _crystalBtnArea);
    _guide->insertButton(data::Task::kSettingsBtn, _settingBtnArea);
    _guide->insertButton(data::Task::kCollectionBtn, _collectionBtnArea);
    _guide->insertButton(data::Task::kVentureBtn, _ventureBtnArea);
    
    updateBtns();
}

void ManorMainLayer::updateBtns()
{
    /**
     *  首先应当检测是否有需要显示的按钮(这里必须找出属于这个页面的按钮 )
     *  算法中通过c++库求交集算法实现
     */
    auto allBtns = data::Task::getInstance()->getButtons();
    std::sort(allBtns.begin(), allBtns.end());
    std::vector<int> btnToShow(MAX(allBtns.size(), _btnsInMain.size())); //需要显示的按钮的id放在这里..
    auto iter = std::set_intersection(allBtns.begin(), allBtns.end(), _btnsInMain.begin(), _btnsInMain.end(), btnToShow.begin());
    btnToShow.resize(iter- btnToShow.begin());
    
    //如果有需要显示的按钮，就将需要显示的按钮设为可见..
    for (int i : btnToShow)
    {
        auto btn = _btnsMapInMain.at(i);
        btn->setVisible(true);
    }
    
    //按钮的位置排序
    sortBtns();
}


void ManorMainLayer::sortBtns()
{
    std::vector<int> haveShown;
    for (auto &i: _sortButttons)
    {
        if (i.second->isVisible())
        {
            haveShown.push_back(i.first);
        }
    }
    std::sort(haveShown.begin(), haveShown.end());
    int n = haveShown.size();
    for (int i(0); i < n; ++i)
    {
        _sortButttons.at(haveShown.at(i))->setPosition(_buttonPosData.at(n).at(i));
    }
}
void ManorMainLayer::storeButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        storeBtnCall();
    }
}
void ManorMainLayer::storeBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kStoreBtn);
    Manor::getInstance()->showLayer(Manor::kStore);
    seekChildByNameWithRetType<ui::ImageView*>(_storeBtn,"newdot")->setVisible(false);
    _storeBtn->stopAllActions();
    _storeBtn->setOpacity(255);
}

void ManorMainLayer::alchemyButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kAlchemy);
        //使得能量按钮在1秒中之内不可以点击, 还没有添加冷却动画..
        auto button = dynamic_cast<ui::Layout*>(sender);
        button->setEnabled(false);
        //炼金...
        data::Item::getInstance()->mint();
        //事件检测..
        data::Task::getInstance()->buttonCheck(data::Task::kAlchemyBtn);
        
        auto load = LoadingAction::create(float(kMINT) / 1000);
        Sequence *seq = Sequence::create(DelayTime::create(float(kMINT) / 1000), CallFunc::create([this](){
            seekChildByNameWithRetType<ui::Layout*>(this, "alchemyBtnArea")->setEnabled(true);}), NULL);
        button->runAction(seq);
        auto alloading = seekChildByNameWithRetType<ui::LoadingBar*>(this, "alchemyLoading");
        alloading->setVisible(true);
        alloading->runAction(Sequence::create(load, CallFunc::create([=]()
        {
            seekChildByNameWithRetType<ui::LoadingBar*>(this, "alchemyLoading")->setVisible(false);
        }), NULL));
        
        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::GOLD);
        alchemyBtnCall();
    }
}

void ManorMainLayer::alchemyBtnCall()
{
    _alchemyBtn->stopAllActions();
    _alchemyBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kAlchemyBtn);
}

void ManorMainLayer::buildButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        buildBtnCall();
    }
}
void ManorMainLayer::buildBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kBuildBtn);
    Manor::getInstance()->showLayer(Manor::kBuildings);
    seekChildByNameWithRetType<ui::ImageView*>(_buildBtn,"newdot")->setVisible(false);
    _buildBtn->stopAllActions();
    _buildBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kBuildBtn);
    
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_BUID);
}

void ManorMainLayer::practiceButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        practiceBtnCall();
    }
}

void ManorMainLayer::practiceBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kProduceBtn);
    Manor::getInstance()->showLayer(Manor::kMake);
    seekChildByNameWithRetType<ui::ImageView*>(_practiceBtn,"newdot")->setVisible(false);
    _practiceBtn->stopAllActions();
    _practiceBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kProduceBtn);
}

void ManorMainLayer::heroButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        heroBtnCall();
    }
}

void ManorMainLayer::heroBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kHeroBtn);
    Manor::getInstance()->showLayer(Manor::kHero);
    seekChildByNameWithRetType<ui::ImageView*>(_heroBtn,"newdot")->setVisible(false);
    _heroBtn->stopAllActions();
    _heroBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kHeroBtn);
    
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_TRAINING);
}

void ManorMainLayer::warehouseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if (!Manor::getInstance()->isWarehouseOpen())
        {
            warehouseBtnCall();
        }
    }
}
void ManorMainLayer::warehouseBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kWarehouseBtn);
    seekChildByNameWithRetType<ui::ImageView*>(_warehouseBtn,"newdot")->setVisible(false);
    if (!Manor::getInstance()->isWarehouseOpen())
    {
        Manor::getInstance()->showLayer(Manor::kWarehouse);
    }
    _warehouseBtn->stopAllActions();
    _warehouseBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kWarehouseBtn);
}

void ManorMainLayer::settingsButtonCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        settingsBtnCall();
    }
}
void ManorMainLayer::settingsBtnCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kSettingsBtn);
    Manor::getInstance()->showLayer(Manor::kSettings);
    _settingBtn->stopAllActions();
    _settingBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kSettingsBtn);
}

void ManorMainLayer::crystalBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        crystalCall();
    }
}

void ManorMainLayer::crystalCall()
{
    data::Task::getInstance()->buttonCheck(data::Task::kCrystalBtn);
    Manor::getInstance()->showLayer(Manor::kCrystal);
    seekChildByNameWithRetType<ui::ImageView*>(seekChildByNameWithRetType<Node*>(this, "crystalBtn"),"newdot")->setVisible(false);
    _crystalBtn->stopAllActions();
    _crystalBtn->setOpacity(255);
    stopBtnShowAct(data::Task::kCrystalBtn);
}

void ManorMainLayer::ventureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    auto area = dynamic_cast<ui::Layout*>(sender);
    auto img = seekChildByNameWithRetType<ui::ImageView*>(area, "Image_1");
    auto txt = seekChildByNameWithRetType<ui::Text*>(area, "Text_13");
    
    if (touchType == ui::Layout::TouchEventType::BEGAN)
    {
        img->setPositionX(25);
        txt->setPositionX(82);
    }
    else if (touchType == ui::Button::TouchEventType::MOVED)
    {
        auto box = area->getBoundingBox();
        box.size = box.size * 1.33;
        if (box.containsPoint(area->getTouchMovePosition()))
        {
            img->setPositionX(25);
            txt->setPositionX(82);
        }
        else
        {
            img->setPositionX(45);
            txt->setPositionX(102);
        }
    }
    else if (touchType == ui::Button::TouchEventType::ENDED)
    {
        img->setPositionX(45);
        txt->setPositionX(102);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        Manor::getInstance()->shutLayer(Manor::kWarehouse);
        /**
         *  事件检测
         *  1、按钮事件的检测..
         *  2、成就检测..
         */
        data::Task::getInstance()->buttonCheck(data::Task::kVentureBtn);
        data::Task::getInstance()->acheivementCheck();
        
        /**
         *  翻页事件..
         */
        Manor::getInstance()->showLayer(Manor::kVenture);
        
        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_VENTURE);
    }
    else if ( touchType == ui::Button::TouchEventType::CANCELED)
    {
        img->setPositionX(45);
        txt->setPositionX(102);
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        Manor::getInstance()->shutLayer(Manor::kWarehouse);
        /**
         *  事件检测
         *  1、按钮事件的检测..
         *  2、成就检测..
         */
        data::Task::getInstance()->buttonCheck(data::Task::kVentureBtn);
        data::Task::getInstance()->acheivementCheck();
        
        /**
         *  翻页事件..
         */
        Manor::getInstance()->showLayer(Manor::kVenture);
        
        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_VENTURE);
    }
}

void ManorMainLayer::collectionButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    auto area = dynamic_cast<ui::Layout*>(sender);
    auto img = area->getChildByName("Image_2");
    auto txt = area->getChildByName("Text_14");
    if (touchType == ui::Layout::TouchEventType::BEGAN)
    {
        img->setPositionX(175);
        txt->setPositionX(118);
    }
    else if (touchType == ui::Button::TouchEventType::MOVED)
    {
        auto box = area->getBoundingBox();
        box.origin = area->getParent()->convertToWorldSpace(box.origin);
        box.size = box.size * 1.33;
        if (box.containsPoint(area->getTouchMovePosition()))
        {
            img->setPositionX(175);
            txt->setPositionX(118);
        }
        else
        {
            img->setPositionX(155);
            txt->setPositionX(98);
        }
    }
    else if (touchType == ui::Button::TouchEventType::ENDED)
    {
        img->setPositionX(155);
        txt->setPositionX(98);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        
        data::Task::getInstance()->buttonCheck(data::Task::kCollectionBtn);
        Manor::getInstance()->shutLayer(Manor::kWarehouse);
        Manor::getInstance()->showLayer(Manor::kCollection);
        
        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_COLLECTION);
    }
    else if (touchType == ui::Button::TouchEventType::CANCELED)
    {

        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_COLLECTION);
            
        img->setPositionX(175);
        txt->setPositionX(118);
            
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
            
        data::Task::getInstance()->buttonCheck(data::Task::kCollectionBtn);
        Manor::getInstance()->shutLayer(Manor::kWarehouse);
        Manor::getInstance()->showLayer(Manor::kCollection);
    }
}

/*---------------事件------------*/
void ManorMainLayer::onTaskCompleted(cocos2d::EventCustom *e)
{
    auto data = static_cast<Value*>(e->getUserData());
    
    for (auto &i : (*data).asValueMap()["result"].asValueVector())
    {
        if (i.asValueMap()["type"].asInt() == data::Task::TaskType::BUTTON)
        {
            int btnid = i.asValueMap()["type_id"].asInt();
            auto btnkey = _sortButttons.keys();
            if (std::count(btnkey.begin(), btnkey.end(), btnid))
            {
                
                auto btn = _sortButttons.at(btnid);
                btn->setVisible(true);
                btn->setOpacity(0);
                sortBtns();
                _btnsToRunShowAct.push_back(btnid);
                
            }
            else if (btnid == data::Task::kCollectionBtn || btnid == data::Task::kVentureBtn)
            {
                _btnsMapInMain.at(btnid)->setVisible(true);
            }
            if (btnid == data::Task::kVentureBtn)
            {
                //fingerCheck();
                data::Guid::getInstance()->showGuid();
            }
            if (btnid == data::Task::kCollectionBtn)
            {
                //fingerCheck();
                data::Guid::getInstance()->showGuid();
            }
        }
        else if (i.asValueMap()["type"].asInt() == data::Task::TaskType::BUILD)
        {
            auto ndot = seekChildByName(_btnsMapInMain.at(data::Task::kBuildBtn), "newdot");
            if (!ndot->isVisible()) ndot->setVisible(true);
            data::Task::getInstance()->setBtnShowNew(data::Task::kBuildBtn);

            data::Guid::getInstance()->showGuid();
        }
        else if (i.asValueMap()["type"].asInt() == data::Task::TaskType::SHOP_SHOW)
        {
            auto ndot = seekChildByName(_btnsMapInMain.at(data::Task::kStoreBtn), "newdot");
            if (!ndot->isVisible()) ndot->setVisible(true);
            data::Task::getInstance()->setBtnShowNew(data::Task::kStoreBtn);
        }
        else if (i.asValueMap()["type"].asInt() == data::Task::TaskType::ITEM_PRODUCE)
        {
            auto ndot = seekChildByName(_btnsMapInMain.at(data::Task::kProduceBtn), "newdot");
            if (!ndot->isVisible()) ndot->setVisible(true);
            data::Task::getInstance()->setBtnShowNew(data::Task::kProduceBtn);
        }
    }
}


void ManorMainLayer::playStartAnimation()
{
    _startAction->setTimeSpeed(1);
    _startAction->play("startanimation", false);
    scheduleUpdate();
}

void ManorMainLayer::runButtonShowAct()
{
    float fadeinTime = 0.7;
    while (!_btnsToRunShowAct.empty())
    {
        auto btnid = _btnsToRunShowAct.front();
        auto btn = _sortButttons.at(_btnsToRunShowAct.front());
        auto btnback = seekChildByName(_sortButttons.at(_btnsToRunShowAct.front()), "btnback");
        auto showAct = Sequence::create(FadeTo::create(fadeinTime, 255), CallFunc::create([=](){
            this->showUnTouchState(btnid);
        }), NULL);
        
        btnback->runAction(showAct);
        btn->runAction(Sequence::create(FadeTo::create(fadeinTime, 255),nullptr));
        _btnsToRunShowAct.erase(_btnsToRunShowAct.begin());
    }
}

void ManorMainLayer::stopBtnShowAct(int btnid)
{
    auto iter = std::find(_btnsToRunShowAct.begin(), _btnsToRunShowAct.end(), btnid);
    if (iter != _btnsToRunShowAct.end())
    {
        _btnsToRunShowAct.erase(iter);
    }
    auto btn = _btnsMapInMain.at(btnid);
    if (btn != nullptr)
    {
        btn->stopAllActions();
        btn->getChildByName("btnback")->stopAllActions();
    }
}

void ManorMainLayer::initShowUnTouchState()
{
    auto allBtns = data::Task::getInstance()->getButtons();
    std::sort(allBtns.begin(), allBtns.end());
    std::vector<int> btnToShow(MAX(allBtns.size(), _btnsInMain.size())); //需要显示的按钮的id放在这里..
    auto iter = std::set_intersection(allBtns.begin(), allBtns.end(), _btnsInMain.begin(), _btnsInMain.end(), btnToShow.begin());
    btnToShow.resize(iter- btnToShow.begin());
    for (auto &i : btnToShow)
    {
        if(!data::Task::getInstance()->BtnIsTouched(i))
            showUnTouchState(i);
    }
    //新手引导显示
    data::Guid::getInstance()->showGuid();
    
    _btnInited = true;
}

void ManorMainLayer::showUnTouchState(int btnid)
{
    auto btn = _sortButttons.at(btnid);
    if (btn == nullptr)
    {
        return;
    }
    if (seekChildByName(btn, "btnback")->getOpacity() < 255)
    {
        seekChildByName(btn, "btnback")->setOpacity(255);
    }

    //auto par = ParticleSystemQuad::create("building_click.plist");
    //par->setPosition(0, 0);
    //par->setName("par");
    btn->stopAllActions();
    btn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 127.5),FadeTo::create(0.5, 255), NULL)));
    //btn->addChild(par, -1);
}

int ManorMainLayer::getWareHouseBtnPosType()
{
    if (_sortButttons.at(data::Task::kWarehouseBtn)->getPositionX() < 300)
    {
        return -1;
    }
    else if (_sortButttons.at(data::Task::kWarehouseBtn)->getPositionX() > 400)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void ManorMainLayer::rmbBuyBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t)
{
	if (t == ui::Button::TouchEventType::ENDED)
	{
		auto rmbBuy = RmbBuyLayer::create();
		rmbBuy->setPosition(Vec2(0, 0));
		addChild(rmbBuy, 1000);
	//	SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
	//	crystalCall();
	}
}

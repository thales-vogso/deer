/****************************************************************************
 *	@desc	主界面
 *	@date	2015-1-15
 *	@author	110101
     
 *	@file	manor/Manor.cpp
 *  @modify	null
 ******************************************************************************/

#include "Manor.h"

#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"

#include "manor/CollectionLayer.h"
#include "manor/Resouce.h"
#include "manor/HeroLayer.h"
#include "manor/Produce.h"
#include "manor/Shop.h"
#include "manor/Build.h"
#include "manor/ManorMainLayer.h"
#include "manor/VentureLayer.h"
#include "popUp/ChoicePopup.h"

#include "data/Achievement.h"
#include "notice/Notice.h"
#include "manor/Achievement.h"
#include "manor/Talent.h"
#include "manor/Ranking.h"
#include "manor/Settings.h"
#include "manor/Mall.h"
#include "data/Task.h"
#include "loading/Loading.h"
#include "crusade/CrusadeMap.h"
#include "manor/LayerBase.h"
#include "popUp/TouchEffect.h"
#include "GlobalMacros.h"
#include "core/custom/LibDate.h"

#include "manor/Sudden.h"

#include "manor/Translate.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"
#include "manor/HeroChoicePopup.h"

/**
 *  test
 */
//#include "CombatBase.h"

USING_NS_CC;
USING_NS_DU;

#define LAYOUTORDER 50
#define FINGER_Z 40
#define MAINLAYER_NAME "ManorMainLayer"
#define VENTUR_NAME "ventureLayer"
#define COLLECTION_NAME "collectionLayer"
#define WAREHOUSE_NAME "warehouseLayer"
/**
 *  三个主要的界面显示时 _viewNode节点的位置
 */
#define SHOW_MONAR_POS_X (0)
#define SHOW_VENTURE_POS_X (deerResouseSize.width)
#define SHOW_COLLCTION_POS_X (-deerResouseSize.width)
#define SHOW_LEFTLEFT_POS_X (deerResouseSize.width * 2)

#define RIGHT_POS_X (deerResouseSize.width)
#define CENTER_POS_X (0)
#define LEFT_POS_X (-deerResouseSize.width)
#define LEFT_LEFT_POS_X (-deerResouseSize.width * 2)

#define VENTURE_LAYER_BTNID 111000
#define COLLECTON_LAYER_BTNID 110900

#define SHOW_ACTION_TAG 15
#define TURN_ACT_INTERVAL 0.2

#define TOUCHEFFECT_Z 155

using ui::Widget;

const int Manor::callGainIntval(15);

Manor *Manor::_instance = nullptr;

Manor *Manor::getInstance(Manor::GoManorType type)
{
    if (!_instance)
    {
        _instance = Manor::create(type);
    }
    return _instance;
};

void Manor::destory()
{
    if (_instance)
    {
        _instance->removeFromParent();
        delete _instance;
        _instance = nullptr;
    }
}

Manor *Manor::create(Manor::GoManorType type)
{
    Manor *ret = new Manor();
    if(ret && ret->init(type))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return ret;
    }
}

Manor::Manor()
:_prequite(false)
,_releaseRefCount(-1)
{
    
}

Manor::~Manor()
{
    _eventDispatcher->removeEventListener(_touchLi);

    _instance = nullptr;
    
    //Notice::destoryInstance();
    
    data::Task::getInstance()->removeListener(_suddenLi);
    
    LOGTYPENAME; custom::LibDate::getRunTime();
    
    Director::getInstance()->purgeCachedData();
    
}

Scene* Manor::createScene(Manor::GoManorType type)
{
    auto scene = Scene::create();
    auto layer = Manor::getInstance(type);
    scene->addChild(layer);
    return scene;
}

void Manor::goToManorScene(GoManorType type)
{
    if (Director::getInstance()->getRunningScene())
    {
        if (type == Manor::kNone)
        {
            SoundManager::getInstance()->stopMusic();
            SoundManager::getInstance()->playMusic(SoundManager::MusicType::kManor);
            Director::getInstance()->replaceScene(TransitionFade::create(2, Manor::createScene(type)));
        }
        else
        {
            SoundManager::getInstance()->stopMusic();
            SoundManager::getInstance()->playMusic(SoundManager::MusicType::kManor);
            Director::getInstance()->replaceScene(Manor::createScene(type));
        }
    }
    else
    {
        Director::getInstance()->runWithScene(Manor::createScene(type));
    }
}
bool Manor::init(Manor::GoManorType type)
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    //显示初始化..
    _viewsNode = Node::create();
    this->addChild(_viewsNode);
    
    //主城界面..
    auto manorLayer = ManorMainLayer::create();
    manorLayer->setPosition(Vec2(0, 0));
    _viewsNode->addChild(manorLayer, 0, MAINLAYER_NAME);
    
    //探险界面..
    auto ventureLayer = VentureLayer::create();
    ventureLayer->setPosition(Vec2(-deerResouseSize.width, 0));
    _viewsNode->addChild(ventureLayer, 0 , VENTUR_NAME);
    
    //采集界面..
    auto collectionLayer = CollectionLayer::create();
    collectionLayer->setPosition(Vec2(deerResouseSize.width, 0));
    _viewsNode->addChild(collectionLayer, 0, COLLECTION_NAME);
    
    recoverGuideLayer();
    
    //初始化最高控制层的触摸..
    _touchLi = EventListenerTouchOneByOne::create();
    _touchLi->onTouchBegan =[=](Touch* touch, Event * event)->bool
    {
        return true;
    };
    
    _touchLi->onTouchEnded =[=](Touch* touch, Event * event)
    {
        if(!data::Guid::getInstance()->isNeedClearData())//已完成强制引导后可用..
        {
            if (touch->getLocation().x - touch->getStartLocation().x > deerResouseSize.width/3)
            {
                if (_viewsNode->getPosition().x < deerResouseSize.width - 1)
                {
                    /**
                     *  通过判断主页面控制器（Manor）的位置来确定显示的页面是哪个，进而判断移动到哪个界面..
                     */
                    auto posx = _viewsNode->getPositionX();
                    if (posx == SHOW_MONAR_POS_X)
                    {
                        auto btns = data::Task::getInstance()->getButtons();
                        auto ret = std::find(btns.begin(), btns.end(), data::Task::BtnID::kVentureBtn);
                        if (ret != btns.end())
                        {
                            Manor::showLayer(kVenture);
                        }
                    }
                    else if (posx == SHOW_COLLCTION_POS_X)
                    {
                        Manor::showLayer(kMainLayer);
                    }
                }
            }
            else if (touch->getLocation().x - touch->getStartLocation().x < - deerResouseSize.width/3)
            {
                if (_viewsNode->getPosition().x > -deerResouseSize.width + 1)
                {
                    /**
                     *  通过判断主页面控制器（Manor）的位置来确定显示的页面是哪个，进而判断移动到哪个界面..
                     */
                    auto posx = _viewsNode->getPositionX();
                    if (posx == SHOW_MONAR_POS_X)
                    {
                        auto btns = data::Task::getInstance()->getButtons();
                        auto ret = std::find(btns.begin(), btns.end(), data::Task::BtnID::kCollectionBtn);
                        if (ret != btns.end())
                        {
                            Manor::showLayer(kCollection);
                        }
                    }
                    else if (posx == SHOW_VENTURE_POS_X)
                    {
                        Manor::showLayer(kMainLayer);
                    }
                    else if (posx == SHOW_LEFTLEFT_POS_X)
                    {
                        Manor::showLayer(kVenture);
                    }
                }
            }
        }
        
    };

    _touchLi->setEnabled(true);
    _eventDispatcher->addEventListenerWithFixedPriority(_touchLi, -1);
    
    
    _suddenLi = EventListenerCustom::create(data::Task::SUDDEN_OCCUR, CC_CALLBACK_1(Manor::showSudden, this));
    data::Task::getInstance()->addListener(_suddenLi);
    
    //安卓返回键监听..
    
	androidKeyinit();
    
    this->addChild(TouchEffect::create(), 254);
    
    return true;
}
void Manor::recoverGuideLayer()
{
    if(!data::Guid::getInstance()->isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        int layerType = data::Guid::getInstance()->getGuideLayer();
        if(layerType == data::Guid::MAIN_LAYER)
        {
            showLayer(kMainLayer);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::BUID_LAYER)
        {
            showLayer(kBuildings);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::COLLECTION_LAYER)
        {
            showLayer(kCollection);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::VENTURE_LAYER)
        {
            showLayer(kVenture);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::HERO_LAYER)
        {
            showLayer(kHero);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::HERO_POP_LAYER)
        {
            showLayer(kHero);
            
            auto rdata = data::Role::getInstance()->getRoleByRID(data::Role::BASE_ROLE_RID).asValueMap();
            ((HeroLayer*)_layoutMap.at(kHero))->addChild(RecruitAsk::create(rdata), 55, "hero_gawala");
            
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
        else if(layerType == data::Guid::RESOUCE_LAYER)
        {
            showLayer(kWarehouse);
            //必要时候要手动显示新手引导..
            data::Guid::getInstance()->showGuid();
        }
    }
    
}
void Manor::onEnter()
{
    Layer::onEnter();
	showSuddenAfferInterval();
}

void Manor::exitManorToMap()
{
    _releaseRefCount = _layoutMap.size();
    
    addChild(Translate::create(), 101);
    
    /**
     * 然后释放缓存的页面
     */
    _layoutMap.clear();
    _viewsNode->removeChildByName(MAINLAYER_NAME);
    _viewsNode->removeChildByName(COLLECTION_NAME);
    _viewsNode->removeChildByName(WAREHOUSE_NAME);
    
    runAction(Sequence::create(DelayTime::create(1), CallFunc::create([](){
        Director::getInstance()->replaceScene(Translate::getTransLateScene());
    }), NULL));
    
}

void Manor::showSuddenAfferInterval()
{
	this->stopActionByTag(SUDDEN_ACTION_TAG);
	Action* suddenAction = Sequence::create(DelayTime::create(data::Task::SUDDEN_INTERVAL), CallFunc::create([=](){
        data::Task::getInstance()->suddenCheck();
    }), NULL, NULL);
	suddenAction->setTag(SUDDEN_ACTION_TAG);
	this->runAction(suddenAction);
}

void Manor::showSudden(EventCustom *e)
{
    auto edata = (Value*)e->getUserData();
    if (!getChildByName("sudden"))
    {
        addChild(Sudden::create((*edata).asValueMap()), LAYOUTORDER + 20, "sudden");
    }
}

void Manor::turnToCollection(cocos2d::EventCustom *unusedEvent)
{
    auto btns = data::Task::getInstance()->getButtons();
    
    auto iter = std::find(btns.begin(), btns.end(), COLLECTON_LAYER_BTNID);
    if (iter != btns.end())
    {
        _viewsNode->stopAllActions();
        _viewsNode->runAction(MoveTo::create(TURN_ACT_INTERVAL, Vec2(-deerResouseSize.width, 0)));
    }
	showSuddenAfferInterval();
}

void Manor::turnToVenture(cocos2d::EventCustom *unusedEvent)
{
    /**
     *  进入改界面的时候，需要[成就检测]和［天赋检测］来刷新按钮..
     */
    data::Task::getInstance()->acheivementCheck();
    data::Task::getInstance()->talentCheck();
    
    auto btns = data::Task::getInstance()->getButtons();
    
    auto iter = std::find(btns.begin(), btns.end(), VENTURE_LAYER_BTNID);
    
    seekChildByNameWithRetType<VentureLayer*>(_viewsNode, VENTUR_NAME)->upDateItems();
    if (iter != btns.end())
    {
        _viewsNode->stopAllActions();
        _viewsNode->runAction(MoveTo::create(0.2, Vec2(deerResouseSize.width, 0)));
    }
	showSuddenAfferInterval();
}

void Manor::turnToLeft()
{
    _viewsNode->stopAllActions();
    auto turnLeftAct = MoveTo::create(TURN_ACT_INTERVAL, Vec2(RIGHT_POS_X, 0));
    _viewsNode->runAction(turnLeftAct);
}

void Manor::turnToLeftLeft()
{
    _viewsNode->stopAllActions();
    _viewsNode->runAction(MoveTo::create(TURN_ACT_INTERVAL, Vec2(SHOW_LEFTLEFT_POS_X, 0)));
}

void Manor::turnToRight()
{
    _viewsNode->stopAllActions();
    _viewsNode->runAction(MoveTo::create(TURN_ACT_INTERVAL, Vec2(LEFT_POS_X, 0)));
}

void Manor::turnToCenter()
{
    _viewsNode->stopAllActions();
    _viewsNode->runAction(Sequence::create(MoveTo::create(TURN_ACT_INTERVAL, Vec2(CENTER_POS_X, 0)), DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(Manor::runButtonShowAct, this)),NULL));
}

void Manor::gain(float dt)
{
    auto gainRet = data::Building::getInstance()->gain().asValueMap();
}

void Manor::preInitCheck(int name)
{
    if (name == kCollection || name == kVenture || name == kMainLayer)
    {
        return;
    }
    if(_layoutMap.at(name) == nullptr)
    {
        switch (name)
        {
            case kBuildings:
            {
				auto lay = Build::create();
				_layoutMap.insert(kBuildings, lay);
				break;
            }
            case kWarehouse:
            {
                auto lay = Resouce::create();
                _layoutMap.insert(kWarehouse, lay);
                lay->setVisible(false);
                _viewsNode->getChildByName(MAINLAYER_NAME)->addChild(lay, LAYOUTORDER+1, WAREHOUSE_NAME);
                break;
            }
            case kStore:
            {
                 auto lay = Shop::create();
				 lay->updateList();
                _layoutMap.insert(kStore, lay);
                break;
            }
            case kMake:
            {
                auto lay = Produce::create();
                _layoutMap.insert(kMake, lay);
                break;
            }
            case kHero:
            {
                auto lay = HeroLayer::create();
                _layoutMap.insert(kHero, lay);
                break;
            }
            case kAchivement:
            {
				auto lay = Achievement::create();
                _layoutMap.insert(kAchivement, lay);
                break;
            }
            case kTalent:
            {
                auto lay = Talent::create();
                _layoutMap.insert(kTalent, lay);
                break;
            }
            case kRank:
            {
                auto lay = Ranking::create();
                _layoutMap.insert(kRank, lay);
                break;
            }
            default:
                break;
        }
    }
}


void Manor::showLayer(int name)
{
    if (currentlay == name && currentlay != kWarehouse)
    {
        return;
    }
    preInitCheck(name);
    shutCurrentLayer();
    
    if (name == kCollection)
    {
        auto col = _viewsNode->getChildByName<CollectionLayer*>(COLLECTION_NAME);
        col->showBoard();
        col->runButtonShowCheck();
        col->show();
        data::Task::getInstance()->buttonCheck(data::Task::BtnID::kCollectionBtn);
        turnToRight();
        //col->runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(CollectionLayer::showFinger, col)), NULL));/* showFinger();*/
    }
    else if (name == kMainLayer)
    {
        _viewsNode->getChildByName<ManorMainLayer*>(MAINLAYER_NAME)->showBoard();
        turnToCenter();
    }
    else if (name == kVenture)
    {
        auto ven = _viewsNode->getChildByName<VentureLayer*>(VENTUR_NAME);
        ven->upDateItems();
        ven->showBoard();
        ven->runButtonShowCheck();
        data::Task::getInstance()->buttonCheck(data::Task::BtnID::kVentureBtn);
        //_viewsNode->getChildByName<ManorMainLayer*>(MAINLAYER_NAME)->removeFinger(data::Task::BtnID::kVentureBtn);
        turnToLeft();
    }
    else
    {
        for (auto &i : _layoutMap)
        {
            auto lay = (Node*)i.second;
            if (i.first == name)
            {
                if (name == kWarehouse)
                {
                    lay->setVisible(true);
                    auto res = (Resouce*)lay;
                    auto mainlayer = _viewsNode->getChildByName<ManorMainLayer*>(MAINLAYER_NAME);
                    res->open(mainlayer->getWareHouseBtnPosType());
                }
                else if (name == kBuildings)
                {
                    if (!_viewsNode->getChildByTag(kBuildings)) {
                        auto bdgLay = dynamic_cast<Build*>(_layoutMap.at(kBuildings));
                        bdgLay->setPosition(Vec2(RIGHT_POS_X, 0));
                        _viewsNode->addChild(bdgLay, LAYOUTORDER, kBuildings);
                        bdgLay->updateList();
                        turnToRight();
                    }
                }
                else if (name == kMake)
                {
                    if (!_viewsNode->getChildByTag(kMake))
                    {
                        auto proLay = dynamic_cast<Produce*>(_layoutMap.at(kMake));
                        proLay->setPosition(Vec2(RIGHT_POS_X, 0));
                        _viewsNode->addChild(proLay, LAYOUTORDER);
                        proLay->openProduce();
                        turnToRight();
                    }
                }
                else if (name == kHero)
                {
                    if (!_viewsNode->getChildByTag(kHero))
                    {
                        auto herolay = dynamic_cast<HeroLayer*>(_layoutMap.at(kHero));
                        herolay->setPosition(Vec2(RIGHT_POS_X, 0));
                        _viewsNode->addChild(herolay, LAYOUTORDER);
                        herolay->updateList();
                        turnToRight();
                    }
                }
                else if (name == kStore)
                {
                    if (!_viewsNode->getChildByTag(kStore))
                    {
                        auto stoLay = dynamic_cast<Shop*>(_layoutMap.at(kStore));
                        stoLay->setPosition(Vec2(RIGHT_POS_X, 0));
                        _viewsNode->addChild(stoLay, LAYOUTORDER, kStore);
                        stoLay->openShop();
                        turnToRight();
                    }
                }
                else if (name == kAchivement)
                {
                    if (!_viewsNode->getChildByTag(kAchivement))
                    {
						auto achLay = dynamic_cast<Achievement*>(_layoutMap.at(kAchivement));
                        achLay->setPosition(Vec2(LEFT_LEFT_POS_X, 0));
                        _viewsNode->addChild(achLay, LAYOUTORDER);
                        achLay->freshList();
                        turnToLeftLeft();
                    }
                }
                else if (name == kRank)
                {
                    if (!_viewsNode->getChildByTag(kRank))
                    {
                        auto rankLay = dynamic_cast<Ranking*>(_layoutMap.at(kRank));
                        rankLay->setPosition(Vec2(LEFT_LEFT_POS_X, 0));
                        _viewsNode->addChild(rankLay, LAYOUTORDER);
                        rankLay->showBoard();
                        turnToLeftLeft();
                    }
                }
                else if (name == kTalent)
                {
                    if (!_viewsNode->getChildByTag(kTalent))
                    {
                        auto talentLay = dynamic_cast<Talent*>(_layoutMap.at(kTalent));
                        talentLay->setPosition(Vec2(LEFT_LEFT_POS_X, 0));
                        _viewsNode->addChild(talentLay, LAYOUTORDER);
                        talentLay->updateItems();
                        turnToLeftLeft();
                    }
                }
            }
            else
            {
                
            }
        }
        if (name == kSettings)
        {
            if (!_viewsNode->getChildByName("settingsLayer"))
            {
                auto set = Settings::create();
                set->setPosition(Vec2(RIGHT_POS_X, 0));
                _viewsNode->addChild(set, LAYOUTORDER, "settingsLayer");
                turnToRight();
            }
        }
        else if (name == kCrystal)
        {
            if (!_viewsNode->getChildByName("mall"))
            {
                auto cry = Mall::create();
                cry->setPosition(Vec2(RIGHT_POS_X, 0));
                _viewsNode->addChild(cry, LAYOUTORDER, "mall");
                turnToRight();
            }
        }
    }
    currentlay = (LayoutName)name;
	showSuddenAfferInterval();
}

void Manor::shutLayer(int name)
{
    preInitCheck(name);
    if (name == kWarehouse)
    {
        auto lay = (Resouce*)_layoutMap.at(name);
        lay->shut();
    }
    _touchLi->setEnabled(true);
}

void Manor::shutCurrentLayer()
{
    auto cur = dynamic_cast<LayerBase*>(_layoutMap.at(currentlay));
    if (cur && currentlay != LayoutName::kWarehouse)
    {
        cur->removeSelf();
    }
    if (currentlay == kSettings || currentlay == kCrystal)
    {
        auto settings = _viewsNode->getChildByName<Settings*>("settingsLayer");
        if (settings) settings->removeSelf();
        auto crystal =  _viewsNode->getChildByName<Mall*>("mall");
        if (crystal) crystal->removeSelf();
    }
}

void Manor::setTouchChangeEnable(bool enable)
{
    _touchLi->setEnabled(enable);
}

void Manor::androidKeyinit(){
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK){
            if (_prequite)
            {
                Director::getInstance()->end();
            }
            else{
                _prequite = true;
                Notice::getInstance()->postTips("再按一次退出  ");
            }
            if (_prequite)
            {
                this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(Manor::purgePrequit, this)), NULL));
            }
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Manor::purgePrequit()
{
    _prequite = false;
}

void Manor::removeFinger()
{
    /*auto finger  = (FingerAction*)_viewsNode->getChildByName("finger");
    if (finger)
    {
        finger->ActStop();
    }*/
}

void Manor::playStartAnimation()
{
    ManorMainLayer* mlayer = (ManorMainLayer*)_viewsNode->getChildByName(MAINLAYER_NAME);
    mlayer->playStartAnimation();
}

void Manor::runButtonShowAct()
{
    ManorMainLayer* mlayer = (ManorMainLayer*)_viewsNode->getChildByName(MAINLAYER_NAME);
    mlayer->runButtonShowAct();
}

bool Manor::isWarehouseOpen()
{
    preInitCheck(kWarehouse);
    auto warehouse = (Resouce*)_layoutMap.at(kWarehouse);
    return warehouse->isOpened();
}

void Manor::dataTest()
{
    //测试使用初始化添加金币5000..
    if(data::Item::getInstance()->getGold() < 5000000 )
    {
        data::Item::getInstance()->userItemPlus(100101, 5000000);
        data::Item::getInstance()->userItemPlus(110105, 5000000);
        data::Item::getInstance()->userItemPlus(110106, 1000000);
        data::Item::getInstance()->userItemPlus(110103, 10000);
        data::Item::getInstance()->userItemPlus(110104, 10000);
//        data::Item::getInstance()->userItemPlus(110107, 1);
//        data::Item::getInstance()->userItemPlus(110108, 1);
//        data::Item::getInstance()->userItemPlus(110109, 9);
//        data::Item::getInstance()->userItemPlus(110110,1);
//        data::Item::getInstance()->userItemPlus(110111, 1);
//        data::Item::getInstance()->userItemPlus(110112, 1);
//        data::Item::getInstance()->userItemPlus(110113, 1);
//        data::Item::getInstance()->userItemPlus(110114, 1);
//        data::Item::getInstance()->userItemPlus(110115, 1);
//        data::Item::getInstance()->userItemPlus(110116, 1);
//        
//        
//        data::Building::getInstance()->addUserBuild(110501);
//        data::Building::getInstance()->addUserBuild(110601);
//        data::Building::getInstance()->addUserBuild(110701);
//        data::Building::getInstance()->addUserBuild(110801);
//        data::Building::getInstance()->addUserBuild(110901);
    }
    data::Talent::getInstance()->addTalent(data::Talent::BuffId::kLifeEnhance);
    data::Talent::getInstance()->addTalent(data::Talent::BuffId::kResistant);
    data::Talent::getInstance()->addTalent(data::Talent::BuffId::kAttackFirst);
}

/****************************************************************************
     
 *	@desc	采集界面
 *	@date	2015-1-12
 *	@author	110101
     
 *	@file	manor/Collection.h
 *  @modify	null
 ******************************************************************************/

#include "CollectionLayer.h"
#include "utils/DeerUtils.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "data/Building.h"
#include "notice/Notice.h"
#include "utils/LoadingAction.h"
#include "data/Task.h"
#include "data/Lang.h"
#include "GlobalMacros.h"
#include "data/Guid.h"
#include "popUp/Summary.h"
#include "manor/Manor.h"
#include "sound/SoundManager.h"
#include "data/Achievement.h"

USING_NS_CC;
USING_NS_DU;
#define TO_S(_var_) #_var_


CollectionLayer::CollectionLayer()
:_inTimer(false)
,_runTimeActNextFram(false)
{
    
}

CollectionLayer::~CollectionLayer()
{
    _itemModel.clear();
    _btnToRunShowAct.clear();
}

CollectionLayer *CollectionLayer::create()
{
    auto ret = new CollectionLayer();
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

bool CollectionLayer::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    //新手引导手指..
    _guide = Guide::create(data::Guid::COLLECTION_LAYER);
    this->addChild(_guide,10000);
    _guide->addFinishGuid();
    //载入收集界面--
    auto collectionView = seekChildByNameWithRetType<ui::Layout*>(CSLoader::getInstance()->createNodeWithFlatBuffersFile("base.csb"), "Panel");
    collectionView->retain();
    collectionView->removeFromParent();
    collectionView->setContentSize(winSize);
    this->addChild(collectionView);
    collectionView->release();
    this->setContentSize(winSize);
    ui::Helper::doLayout(this);
    
    //这个界面的触摸设置--
    this->setTouchEnabled(true);
    this->setSwallowTouches(true);
    
    auto wokerTotal = data::Building::getInstance()->getWorkTotal();
    auto wokerFree = data::Building::getInstance()->getWorkFree();
    auto wokerCurNum = seekChildByNameWithRetType<ui::TextBMFont*>(this, "workCurNum");
    wokerCurNum->setString(Value(wokerFree).asString());
    if(wokerFree>0)
    {
        wokerCurNum->setColor(Color3B::RED);
    }
    else
    {
        wokerCurNum->setColor(DeerUtils::hexToColor3B("#E2E4A5"));
    }
    auto wokerTolNum = seekChildByNameWithRetType<ui::TextBMFont*>(this, "workTolNum");
    wokerTolNum->setString("/"+Value(wokerTotal).asString());
    if(wokerFree<10)
    {
        wokerTolNum->setPositionX(wokerCurNum->getPositionX()+_fontWidth);
    }
    else if(wokerFree >= 10 && wokerFree < 100)
    {
        wokerTolNum->setPositionX(wokerCurNum->getPositionX()+_fontWidth*2);
    }
    else if(wokerFree>= 100 && wokerFree < 1000)
    {
        wokerTolNum->setPositionX(wokerCurNum->getPositionX()+_fontWidth*3);
    }
    
    
    
    //为返回按钮添加回调--
    auto gobackBtn = seekChildByNameWithRetType<ui::Layout*>(this, "gobackArea");
    gobackBtn->addTouchEventListener(CC_CALLBACK_2(CollectionLayer::backButtonCallback, this));
    _backBtn = seekChildByNameWithRetType<ui::ImageView*>(gobackBtn, "backBtn");
    _backBtnX = _backBtn->getPositionX();
    
    _guide->insertButton(data::Guid::COLLECTION_BUTTON_BACK,gobackBtn);
    
    //采集按钮..
    /**我们需要检测这个按钮是否是否已经解锁, 如果按钮已经解锁，我门需要知道这个按钮是否被点击过。如果点未点击过则要现实未点击状态**/
    _gatherBtnArea = seekChildByNameWithRetType<ui::Layout*>(this, "collectionBtnArea");
    _gatherBtn = seekChildByNameWithRetType<ui::Button*>(this, "collectionBtn");
    _gatherBtnArea->setEnabled(true);
    _gatherBtnArea->addTouchEventListener(CC_CALLBACK_2(CollectionLayer::gatherButtonCallback, this));
    _guide->insertButton(data::Guid::COLLECTION_BUTTON_GATHER,_gatherBtnArea);
    //展示吸引点击动画..
    auto par = ParticleSystemQuad::create("building_click.plist");
    par->setPosition(95, 34);
    _gatherBtn->addChild(par, -1, "par");
    
    _gatherLoadBar = seekChildByNameWithRetType<ui::LoadingBar*>(this, "collectionLoading");
    
    auto btns = data::Task::getInstance()->getButtons();
    if (std::find(btns.begin(), btns.end(), data::Task::kCollect) != btns.end())
    {
        _gatherBtn->setVisible(true);
        
        if (!data::Task::getInstance()->BtnIsTouched(data::Task::kCollect))
        {
            
            _gatherBtn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5, 127.5),FadeTo::create(0.5, 255), NULL)));
            seekChildByName(_gatherBtn, "newdot")->setVisible(true);
        }
    }
    else
    {
        _gatherBtn->setVisible(false);
    }
    
    //初始化itemModel
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(this, "item"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(this, "item_0"));
    _itemModel.at(0)->removeFromParent();
    _itemModel.at(1)->removeFromParent();
    _itemModel.at(0)->retain();
    _itemModel.at(1)->retain();
    auto list = seekChildByNameWithRetType<ui::ScrollView*>(this, "list");
    
    Size win = Director::getInstance()->getWinSize();
    //list->setContentSize(Size(list->getContentSize().width,list->getContentSize().height*win.height/SCREEN_HEIGHT));
    
    auto gainListData = data::Building::getInstance()->getUserGainList().asValueVector();
    
    int rowNum = (int)gainListData.size();
    int spaceH = _itemModel.at(0)->getContentSize().height;
    list->setInnerContainerSize(Size(list->getInnerContainerSize().width,(rowNum+1)*spaceH+_itemModel.at(1)->getContentSize().height));
    
    Size listSize = list->getInnerContainerSize();
    list->removeAllChildren();
    auto item = _itemModel.at(1)->clone();
    list->addChild(item);
    item->setPositionY(listSize.height-_itemModel.at(1)->getContentSize().height/2.0f);
    
    int nMaxZoder = 1000;
    int i = 0;
    for (auto listItemData : gainListData)
    {
        auto viewItem = CollectionItem::create(listItemData.asValueMap(), this);
        _guide->insertButton(viewItem->robotDecreseId, seekChildByNameWithRetType<Layout*>(viewItem, "decreaseBtnArea"));
        _guide->insertButton(viewItem->robotNameId, seekChildByNameWithRetType<Layout*>(viewItem, "nameBtnArea"));
        _guide->insertButton(viewItem->robotId, seekChildByNameWithRetType<Layout*>(viewItem, "increaseBtnArea"));
        
        viewItem->setZOrder(nMaxZoder--);
        list->addChild(viewItem);
        float y = listSize.height-_itemModel.at(1)->getContentSize().height- spaceH*i - spaceH/2.0f;
        viewItem->setPositionY(y);
        viewItem->setPositionX(viewItem->getContentSize().width/2.0f);
        i++;
    }
    
    
    
    this->checkTimerAble(nullptr);
    
    auto isShowAll = data::Building::getInstance()->getWorkTotal() > 0;
    if (isShowAll)
    {
        
    }
    else
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "workImage")->setVisible(false);
        seekChildByNameWithRetType<ui::ScrollView*>(this, "list")->setVisible(false);
    }
    //设置天空大小，天空海阔你与我~~~
    auto starSky =  collectionView->getChildByName<ParticleSystemQuad*>("Particle_1") ;
    starSky->setPosVar(Vec2(winSize.width/2,(int)winSize.height*75/128));
    
    /**事件侦听工人的改变和建筑物的改变**/
    EventListenerCustom* workerChange = data::Building::getInstance()->addListener(data::Building::WORKER_CHANGE, CC_CALLBACK_1(CollectionLayer::onWorkerChange, this));
    addEventPool(workerChange, data::Building::getInstance());
    
    auto buildListener = data::Building::getInstance()->addListener(data::Building::BUILD, CC_CALLBACK_1(CollectionLayer::updateItems, this));
    addEventPool(buildListener, data::Building::getInstance());
    
    /**
     事件侦听Task的完成，这里最主要的目的是为了侦听收集按钮的解锁，所以
     1、在初始时如果收集按钮已经解锁就没必要侦听了
     2、在收集按钮解锁后就可以结束侦听了
     **/
    auto taskLi = data::Task::getInstance()->addListener(data::Task::TASK_COMPLETE, CC_CALLBACK_1(CollectionLayer::onTaskCompleted, this));
    addEventPool(taskLi, data::Task::getInstance());
    
    
    
    
    return true;
}
void CollectionLayer::removeSelf()
{

}
void CollectionLayer::updateProperty()
{

}
void CollectionLayer::show()
{
    int wokerFree = data::Building::getInstance()->getWorkFree();
    if(wokerFree >= _tipsFreeWorkerNum)
    {
        
        auto act  = CallFunc::create([=](){
			char s[100];
			sprintf(s, data::Lang::getInstance()->getWord(130323).c_str(), wokerFree);
			std::stringstream str;
            str<<std::string(s)<<"\n"<<data::Lang::getInstance()->getWord(130324);
            Notice::getInstance()->postTips(str.str());
        });
        this->runAction(Sequence::create(DelayTime::create(0.5), act,NULL));
    }
}
/************************************/

void CollectionLayer::onEnter()
{
    Layout::onEnter();
    scheduleUpdate();
}

void CollectionLayer::update(float delta)
{
    Layout::update(delta);
    
    if (_runTimeActNextFram)
    {
        _runTimeActNextFram = false;
        runTimeAction();
    }
}

void CollectionLayer::onTaskCompleted(cocos2d::EventCustom *e)
{
    auto edata = (Value*)e->getUserData();
    auto data = (*edata).asValueMap();
    auto taskResult = data["result"].asValueVector();
    for(auto & i : taskResult)
    {
        if (i.asValueMap()["type"].asInt() == data::Task::TaskType::BUTTON &&
            i.asValueMap()["type_id"].asInt() == data::Task::kCollect)
        {
            /**我们将按钮加入到这个向量中，等待玩家进入这个界面以后在开始播放显示动画**/
            _btnToRunShowAct.pushBack(seekChildByName(this, "collectionBtn"));
        }
    }
}

void CollectionLayer::showBoard()
{
    addChild(Notice::getInstance()->getBoard(), 50);
    addChild(Notice::getInstance()->getTips(), 52);
}

void CollectionLayer::runButtonShowCheck()
{
    if (!_btnToRunShowAct.empty())
    {
        runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(CollectionLayer::runButtonShowAct, this)), NULL));
    }
}

void CollectionLayer::runButtonShowAct()
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

void CollectionLayer::backButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType eventType)
{
    auto btn = dynamic_cast<ui::Layout*>(sender);
    auto img = _backBtn;
    if (eventType == ui::Button::TouchEventType::BEGAN)
    {
        img->setPositionX(_backBtnX - 20);
    }
    else if (eventType == ui::Button::TouchEventType::MOVED)
    {
        if (btn->getBoundingBox().containsPoint(btn->getTouchMovePosition()))
        {
            img->setPositionX(_backBtnX - 20);
        }
        else
        {
            img->setPositionX(_backBtnX);
        }
    }
    else if (eventType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        img->setPositionX(_backBtnX);
        backReal();
    }
    else if (eventType == ui::Button::TouchEventType::CANCELED)
    {

        img->setPositionX(_backBtnX);
        backReal();
    }
}

void CollectionLayer::backReal()
{
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_BACK);
    
    Manor::getInstance()->showLayer(Manor::kMainLayer);
}

void CollectionLayer::showAll()
{
    seekChildByNameWithRetType<ui::Layout*>(this, "workImage")->setVisible(true);
    seekChildByNameWithRetType<ui::ScrollView*>(this, "list")->setVisible(true);
}

void CollectionLayer::gatherButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        //更新新手引导..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_GATHER);
    
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kCollection);
        _gatherLoadBar->setVisible(true);
        auto button = (ui::Layout*)sender;
        button->setEnabled(false);
        auto act1 = LoadingAction::create(kGATHER/1000);
        auto act2 = CallFunc::create([=]()
        {
            _gatherBtnArea->setEnabled(true);
            _gatherLoadBar->setVisible(false);
            //展示吸引点击动画..
            if(!_gatherBtn->getChildByName("par"))
            {
                auto par = ParticleSystemQuad::create("building_click.plist");
                par->setPosition(95, 34);
                _gatherBtn->addChild(par, -1, "par");
            }
            
            
        });
        _gatherLoadBar->runAction(Sequence::create(act1, act2, NULL));
        
        
        //调用数据采集..
        auto gatherList = data::Building::getInstance()->gather().asValueVector();
        std::stringstream sstr;
        for (auto &i : gatherList)
        {
            int total = data::Item::getInstance()->getUserItemAmountByIID(i.asValueMap()["iid"].asInt());
            sstr<<"采集了 : "<<i.asValueMap()["name"].asString()<<"x"<<i.asValueMap()["amount"].asInt()<<"，共: "<<total<<"\n";
        }
        Notice::getInstance()->postTips(sstr.str());
        //挖矿计数..
        data::Achievement::getInstance()->count(data::Achievement::DIG);
        data::Task::getInstance()->buttonCheck(data::Task::kCollect);
        seekChildByName(_gatherBtn, "newdot")->setVisible(false);
        _gatherBtn->removeChildByName("par");
        _gatherBtn->stopAllActions();
        _gatherBtn->setOpacity(255);
    }
}

void CollectionLayer::onWorkerChange(cocos2d::EventCustom *event)
{
    auto wokerButton = seekChildByNameWithRetType<ui::TextBMFont*>(this, "workCurNum");
    wokerButton->stopAllActions();
    wokerButton->runAction(Sequence::create(ScaleTo::create(0.1, 1.3*1.5),ScaleTo::create(0.1, 1*1.5),NULL));
    auto wokerTotal = data::Building::getInstance()->getWorkTotal();
    
    auto wokerFree = data::Building::getInstance()->getWorkFree();
    wokerButton->setString(Value(wokerFree).asString());
    if(wokerFree>0)
    {
        wokerButton->setColor(Color3B::RED);
    }
    else
    {
        wokerButton->setColor(DeerUtils::hexToColor3B("#E2E4A5"));
    }
    
    
    auto workTolNum =   seekChildByNameWithRetType<ui::TextBMFont*>(this, "workTolNum");
    workTolNum->setString("/" + Value(wokerTotal).asString());
    if(wokerFree<10)
    {
        workTolNum->setPositionX(wokerButton->getPositionX()+_fontWidth);
    }
    else if(wokerFree >= 10 && wokerFree < 100)
    {
        workTolNum->setPositionX(wokerButton->getPositionX()+_fontWidth*2);
    }
    else if(wokerFree>= 100 && wokerFree < 1000)
    {
        workTolNum->setPositionX(wokerButton->getPositionX()+_fontWidth*3);
    }
    
	auto list = seekChildByNameWithRetType<ui::ScrollView*>(this, "list");

    auto listItems = list->getChildren();
    //add an empty item need ignore it 
    listItems.erase(0);
	auto gainListData = data::Building::getInstance()->getUserGainList().asValueVector();
    
	for (auto listItemData : gainListData)
	{
        auto iter = std::find_if(listItems.begin(), listItems.end(), [&](Node* item){
            int wid = dynamic_cast<CollectionItem*>(item)->getWid();
            return wid == listItemData.asValueMap()["wid"].asInt();
        });
        
        if (iter != listItems.end())
        {
            dynamic_cast<CollectionItem*>(*iter)->updateView(listItemData.asValueMap());
        }
	}
    
    if (wokerTotal > 0)
    {
        showAll();
    }
}

void CollectionLayer::updateItems(cocos2d::EventCustom *event)
{
    if(!_isUpdateList)
    {
        _isUpdateList = true;
        auto gainListData = data::Building::getInstance()->getUserGainList().asValueVector();
        if(_preGainListData != Value(gainListData) || _preGainListData.asValueVector().size() != gainListData.size())
        {
            _preGainListData = gainListData;
            auto collectList = seekChildByNameWithRetType<ui::ScrollView*>(this, "list");
            
            int rowNum = (int)gainListData.size();
            int spaceH = _itemModel.at(0)->getContentSize().height;
            collectList->setInnerContainerSize(Size(collectList->getInnerContainerSize().width,(rowNum+1)*spaceH+_itemModel.at(1)->getContentSize().height));
            
            Size listSize = collectList->getInnerContainerSize();
            collectList->removeAllChildren();
            auto item = _itemModel.at(1)->clone();
            collectList->addChild(item);
            item->setPositionY(listSize.height-_itemModel.at(1)->getContentSize().height/2.0f);
            
            int nMaxZoder = 1000;
            int i = 0;
            for (auto listItemData : gainListData)
            {
                auto viewItem = CollectionItem::create(listItemData.asValueMap(), this);
                _guide->insertButton(viewItem->robotDecreseId, seekChildByNameWithRetType<Layout*>(viewItem, "decreaseBtnArea"));
                _guide->insertButton(viewItem->robotNameId, seekChildByNameWithRetType<Layout*>(viewItem, "nameBtnArea"));
                _guide->insertButton(viewItem->robotId, seekChildByNameWithRetType<Layout*>(viewItem, "increaseBtnArea"));
                
                
                viewItem->setZOrder(nMaxZoder--);
                collectList->addChild(viewItem);
                float y = listSize.height-_itemModel.at(1)->getContentSize().height- spaceH*i - spaceH/2.0f;
                viewItem->setPositionY(y);
                viewItem->setPositionX(viewItem->getContentSize().width/2.0f);
                i++;
            }
        }
        //更新新手引导
        data::Guid::getInstance()->showGuid();
        _isUpdateList = false;
    }
    
}

void CollectionLayer::checkTimerAble(cocos2d::EventCustom *event)
{
    if (_inTimer == false)
    {
        auto gainListData = data::Building::getInstance()->getUserGainList().asValueVector();
        if (true /*gainListData.size() != 0*/)
        {
            _inTimer = true;
            runTimeAction();
        }
    }
}

void CollectionLayer::runTimeAction()
{
    auto interval = data::Building::getInstance()->getGainInterval();
    auto timeact = TimerAction::create(interval);
    CallFunc *gainCall = CallFunc::create(CC_CALLBACK_0(CollectionLayer::gainCall, this));
    auto seq = Sequence::create(timeact, gainCall, NULL);
    auto produceTimer = seekChildByNameWithRetType<ui::TextBMFont*>(this, "producetime");
    produceTimer->runAction(seq);
}

void CollectionLayer::gainCall()
{
    auto ret = data::Building::getInstance()->gain().asValueMap();

    std::stringstream sstr;
    for (auto &i : ret)
    {
        if (i.second.asValueMap()["amount"].asInt() > 0 )
        {
			sstr<< data::Lang::getInstance()->getWord(120104) << " : " << i.second.asValueMap()["name"].asString() << "x" << i.second.asValueMap()["amount"].asInt() << ", " << data::Lang::getInstance()->getWord(120105) << ": " << i.second.asValueMap()["total"].asInt() << "\n";
        }
    }
    if (sstr.str().size() > 0)
    {
        Notice::getInstance()->postTips(sstr.str());
    }
    
    /**
     *  运行结束以后会自动再次运行倒计时；
     */
    _runTimeActNextFram = true;
}

void CollectionLayer::showInfo(std::string txt, cocos2d::Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(txt,node);
        this->addChild(pop, 100, "Textpop");
    }
}


/***************************************************TimerAction************************************************/

CollectionLayer::TimerAction * CollectionLayer::TimerAction::create(float time)
{
    TimerAction *ret = new TimerAction();
    if (ret && ret->init(time))
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

bool CollectionLayer::TimerAction::init(float time)
{
    if (!ActionInterval::initWithDuration(time))
    {
        return false;
    }
    _time = time;
    return true;
}

void CollectionLayer::TimerAction::update(float t)
{
    auto tar =  (ui::TextBMFont*)_target;
    int time = _time - t * _duration;
    if (!t) --time;
    std::stringstream sstr;
	//sstr << data::Lang::getInstance()->getWord(120102);
    if (time < 10)
    {
		sstr << "00:0" << time;
    }
    else
    {
		sstr << "00:" << time;
    }
    tar->setString(sstr.str());
}

/******************************************************************************************************/

using namespace ui;

CollectionLayer::CollectionItem::~CollectionItem()
{
    //data::Building::getInstance()->removeListener(_buildWorkerChange);
}

CollectionLayer::CollectionItem* CollectionLayer::CollectionItem::create(ValueMap itemData, CollectionLayer* delegate)
{
    CollectionItem *ret = new CollectionItem();
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

bool CollectionLayer::CollectionItem::init(ValueMap itemData)
{
    if (!Widget::init()) {
        return false;
    }
    _longPressedIncrease = false;
    _longPressedReduce = false;
    
    _itemData = itemData;
    
    CCASSERT(!_delegate->_itemModel.empty(), "");
    auto view = _delegate->_itemModel.at(0)->clone();
    view->setPosition(Point::ZERO);
    this->setContentSize(view->getContentSize());
    this->addChild(view);
    
    int wid = _itemData["wid"].asInt();
	seekChildByNameWithRetType<ui::TextBMFont*>(this, "name")->setString(_itemData["name"].asString());
    
    robotId = wid;
    robotDecreseId =wid*10;
    robotNameId    =wid*100;
    
    auto showInfoBtn = seekChildByNameWithRetType<ui::Layout*>(this, "nameBtnArea");
    showInfoBtn->addTouchEventListener(CC_CALLBACK_2(CollectionItem::showWorkerInfo, this));
    showInfoBtn->setTouchEnabled(true);
    
    updateView(itemData);

    auto increaseButton = seekChildByNameWithRetType<Layout*>(view, "increaseBtnArea");
    increaseButton->addTouchEventListener(CC_CALLBACK_2(CollectionItem::increaseCallback, this));
    
    auto reduceButton = seekChildByNameWithRetType<Layout*>(view, "decreaseBtnArea");
    reduceButton->addTouchEventListener(CC_CALLBACK_2(CollectionItem::reduceCallback, this));
    
    return true;
}

void CollectionLayer::CollectionItem::updateView(cocos2d::ValueMap data)
{
    _itemData = data;
    auto workerAmount = seekChildByNameWithRetType<ui::TextBMFont*>(this, "number");
    workerAmount->setString(_itemData["amount"].asString());
    
    auto product = seekChildByNameWithRetType<ui::TextBMFont*>(this, "product");
	std::string str = "";
	str += _itemData["gather"].asValueMap()["name"].asString();
    if(_itemData["gather"].asValueMap()["amount"].asInt() >=0)
    {
        str += " ";
    }
	str += _itemData["gather"].asValueMap()["amount"].asString();
    product->setString(str);
    
    if (_itemData["gather"].asValueMap()["amount"].asInt() < 0)
    {
        product->setColor(Color3B::RED);
        workerAmount->setColor(Color3B::RED);
    }
    else
    {
        //这个地方的色值要整理出来..
        product->setColor(Color3B(0xE2, 0xE4, 0xA5));
        workerAmount->setColor(Color3B(0xD3, 0xD5, 0xE0));
    }
}

void CollectionLayer::CollectionItem::showWorkerInfo(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType t)
{
    if (t == ui::Text::TouchEventType::ENDED)
    {
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_NAME_CLICK);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPopUp);
        auto area = dynamic_cast<ui::Layout*>(sender);
        
        _delegate->showInfo(_itemData["summary"].asString(), area);
    }
}

void CollectionLayer::CollectionItem::increaseCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == Button::TouchEventType::BEGAN)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setVisible(true);
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setOpacity(255);
        auto longPressAct = Sequence::create(DelayTime::create(LONG_PRESSED_TIME), CallFunc::create([this](){this->_longPressedIncrease = true;
            playFastAddEffect();
            }),CallFunc::create(CC_CALLBACK_0(CollectionItem::increaseFast, this)), NULL);
        longPressAct->setTag(11);
        runAction(longPressAct);
    }
    else if (touchType == Button::TouchEventType::MOVED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->setVisible(false);
    }
    else if (touchType == Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->stopAllEffects();
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd);
        CallFunc * actCall = nullptr;
        if(!_longPressedIncrease)
        {
            actCall = CallFunc::create(CC_CALLBACK_0(CollectionItem::increase, this));
        }
        else
        {
            data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_ADD);
        }
        
        seekChildByNameWithRetType<ui::ImageView*>(this, "inlight")->runAction(Sequence::create(FadeOut::create(0.3), actCall, NULL) );
        stopAllActionsByTag(11);
        _longPressedIncrease = false;
        _fastOpreateIndex = 0;
    }
    else if (touchType == Button::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->stopAllEffects();
        stopAllActionsByTag(11);
        _longPressedIncrease = false;
        _fastOpreateIndex = 0;
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_ADD);
    }
}
void CollectionLayer::CollectionItem::increase()
{
    //修改数据库，并更新显示--
    if(data::Building::getInstance()->workerPlus(_itemData["wid"].asInt()))
    {
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_ADD);
        CCLOG("添加工人成功..");
    }
    else
    {
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_ADD);
        Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(120101), Notice::kWarning);
    }
}

void CollectionLayer::CollectionItem::increaseFast()
{
    if (_longPressedIncrease)
    {
        _fastOpreateIndex ++;
        bool rsultCode = false;
        if(_fastOpreateIndex <= 5)
        {
            rsultCode = data::Building::getInstance()->workerPlus(_itemData["wid"].asInt());
        }
        else if(_fastOpreateIndex > 5 && _fastOpreateIndex <= 8)
        {
            rsultCode = data::Building::getInstance()->workerPlus(_itemData["wid"].asInt(),_fastOpreateIndex);
        }
        else if(_fastOpreateIndex >8)
        {
            rsultCode = data::Building::getInstance()->workerPlus(_itemData["wid"].asInt(),8);
        }
        
        if(rsultCode)
        {
            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CollectionItem::increaseFast, this)), nullptr));
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
                runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CollectionItem::increaseFast, this)), nullptr));
            }
        }

    }
    else
    {
        _fastOpreateIndex = 0;
        SoundManager::getInstance()->stopAllEffects();
    }
    
}

void CollectionLayer::CollectionItem::reduceCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == Button::TouchEventType::BEGAN)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setVisible(true);
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setOpacity(255);
        auto longPressAct = Sequence::create(DelayTime::create(LONG_PRESSED_TIME), CallFunc::create([this](){this->_longPressedReduce = true;
            playFastAddEffect();}),CallFunc::create(CC_CALLBACK_0(CollectionItem::reduceFast, this)), NULL);
        longPressAct->setTag(11);
        runAction(longPressAct);
    }
    else if (touchType == Button::TouchEventType::MOVED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->setVisible(false);
    }
    else if (touchType == Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->stopAllEffects();
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd);
        CallFunc * actCall = nullptr;
        if(!_longPressedReduce)
        {
            actCall = CallFunc::create(CC_CALLBACK_0(CollectionItem::reduce, this));
        }
        else
        {
            data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_REDUCE);
        }
        seekChildByNameWithRetType<ui::ImageView*>(this, "delight")->runAction(Sequence::create(FadeOut::create(0.3), actCall, NULL));
        stopAllActionsByTag(11);
        _longPressedReduce = false;
    }
    else if (touchType == Button::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->stopAllEffects();
        stopAllActionsByTag(11);
        _longPressedReduce = false;
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_REDUCE);
    }
}

void CollectionLayer::CollectionItem::reduce()
{
    //修改数据库， 并更新显示--
    if (data::Building::getInstance()->workerReduce(_itemData["wid"].asInt()))
    {
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_REDUCE);
        CCLOG("减少工人成功..");
    }
    else
    {
        data::Guid::getInstance()->updateGuidInfo(data::Guid::COLLECTION_ADD);
        CCLOG("减少工人失败..");
    }
    
}
void CollectionLayer::CollectionItem::playFastAddEffect()
{
    SoundManager::getInstance()->stopAllEffects();
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kFastAdd, true);
}
void CollectionLayer::CollectionItem::reduceFast()
{
    //未进行新手引导可用..
    if (_longPressedReduce)
    {
        _fastOpreateIndex ++;
        bool rsultCode = false;
        if(_fastOpreateIndex <= 5)
        {
            rsultCode = data::Building::getInstance()->workerReduce(_itemData["wid"].asInt());
        }
        else if(_fastOpreateIndex > 5 && _fastOpreateIndex <= 8)
        {
            rsultCode = data::Building::getInstance()->workerReduce(_itemData["wid"].asInt(),_fastOpreateIndex);
        }
        else if(_fastOpreateIndex >8)
        {
            rsultCode = data::Building::getInstance()->workerReduce(_itemData["wid"].asInt(),8);
        }
        
        if(rsultCode)
        {
            runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CollectionItem::reduceFast, this)), nullptr));
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
                runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create(CC_CALLBACK_0(CollectionItem::reduceFast, this)), nullptr));
            }
        }
        
    }
    else
    {
        _fastOpreateIndex = 0;
        SoundManager::getInstance()->stopAllEffects();
    }
}

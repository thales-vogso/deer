/****************************************************************************
   
*	@desc	制造界面
*	@date	2015-06-30
*	@author	wzw
    
*	@file	manor/Produce.h
*  @modify	minamoto
******************************************************************************/

#include "Produce.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "data/Item.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "data/Task.h"
#include "utils/ButtonAction.h"
#include "popUp/DoBatch.h"
#include "GlobalMacros.h"
#include "notice/Notice.h"
#include "popUp/Summary.h"
#include "sound/SoundManager.h"
#include "data/Lang.h"


#define LISTHIEGHT_OFFSET 50
USING_NS_CC;

Produce::~Produce()
{
    
}

const float Produce::ACT_DURATION = 1.0f;

bool Produce::init()
{
    if (!Layer::init())
    {
        return false;
    }
    //autorelease();
    //添加黑色背景..
    auto mask = BlackMask::create();
    addChild(mask);
    
    auto winSize = Director::getInstance()->getWinSize();
    
    //载入界面..
    Node* view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("produce.csb");
    
    _panel = view->getChildByName<ui::Layout*>("Panel_1");
    view->setContentSize(winSize);
    _panel->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    //返回按钮..
    ui::Layout* backButton = _panel->getChildByName<ui::Layout*>("gobackBtnArea");
    backButton->setTouchEnabled(true);
    backButton->setSwallowTouches(false);
    backButton->addTouchEventListener(CC_CALLBACK_2(Produce::backBtnClick, this));
    //底部遮罩..
    _bottomMask = _panel->getChildByName<ui::Layout*>("Image_1_0");
    _topmask    = _panel->getChildByName<ui::Layout*>("Image_1");
    //列表容器..
    _list = _panel->getChildByName<ui::ListView*>("list");
    
    //模块模板(小)..
    _itemPattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("listitem");
    _itemPattern->retain();
    //模块模板(大)..
    _itemHugePattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("widthlistitem");
    _itemHugePattern->retain();
    //设置子元素代理..
    Item::setDelegate(this);
    //当故事内容打开或者关上是，调整listView的长度..
	Notice* notice = Notice::getInstance();
	EventListenerCustom * foldEvent = notice->addListener(Notice::FOLD, CC_CALLBACK_1(Produce::onBoardResize, this));
	addEventPool(foldEvent, notice);
    
    //设置天空大小，天空海阔你与我~~~
    auto starSky = _panel->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winSize.width/2,winSize.height*75/128));
    
    /**
     *  事件注册（完成任务）
     */
	data::Task* task = data::Task::getInstance();
	EventListenerCustom* completeTask = task->addListener(data::Task::TASK_COMPLETE, CC_CALLBACK_1(Produce::onTaskCompleted, this));
	addEventPool(completeTask, task);
    
    /* 更新项目列表 */
    ui::Layout* emptyItem =   dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("listitem_0");
    _list->removeAllItems();
    _list->pushBackCustomItem(emptyItem);
    return true;
}
void Produce::updateList()
{
    auto items = _list->getItems();
    items.erase(0);
    
    Value all = data::Item::getInstance()->getUserProducts();
    if(_preUserProduct != all || _preUserProduct.asValueVector().size() != all.asValueVector().size())
    {
        _preUserProduct = all;
        
        /**由于可能出现玩家制作减少的情况所以必须添加减少item的检查,该算法使用求差集运算 **/
        std::vector<int> itemsid(items.size());
        for (int i(0); i< items.size(); ++i)
        {
            itemsid.at(i) = dynamic_cast<Item*>(items.at(i))->getID();
        }
        
        std::vector<int> pid(all.asValueVector().size());
        for (int i(0); i< all.asValueVector().size(); ++i)
        {
            pid.at(i) = all.asValueVector().at(i).asValueMap()["iid"].asInt();
        }
        
        std::vector<int> v(MAX(itemsid.size(), pid.size()));
        std::sort(itemsid.begin(), itemsid.end());
        std::sort(pid.begin(), pid.end());
        auto it = std::set_difference(itemsid.begin(), itemsid.end(), pid.begin(), pid.end(), v.begin());
        v.resize(it - v.begin());
        
        /**
         *  这里反向使用迭代器，目的是为了在删除某一项目时，不扰乱剩余的迭代器的指向。
         */
        for (auto iter = items.end()-1; iter != items.begin()-1; --iter)
        {
            auto iid = dynamic_cast<Item*>(*iter)->getID();
            if (std::any_of(v.begin(), v.end(), [&](int i)->bool{return i == iid;}))
                _list->removeItem(iter - items.begin());
        }
        
        for (Value row : all.asValueVector())
        {
            Item* item = getItemByID(row.asValueMap()["iid"].asInt());
            item->setData(row);
            if (item->getParent()){
                
            } else{
                _list->pushBackCustomItem(item);
            }
        }
        //更新提醒信息..
        auto tips = DeerUtils::seekChildByNameWithRetType<ui::Layout*>(this, "tip");
        tips->setVisible(true);
        if(_list->getPositionY() != tips->getPositionY())
        {
            _list->setPositionY(tips->getPositionY());
            _list->setContentSize(Size(_list->getContentSize().width,_list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
            _list->setInnerContainerSize(_list->getContentSize());
            _topmask->setPositionY(_list->getPositionY()-21);
        }
        
        //军需所是否完成..
        if(data::Building::getInstance()->getUserBuildByBID(110401).asValueMap()["complete"].asBool())
        {
            tips->setVisible(false);
            if(_list->getPositionY() != _listMaxHeight)
            {
                _list->setPositionY(tips->getPositionY()+49);
                _list->setContentSize(Size(_list->getContentSize().width,_list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
                _list->setInnerContainerSize(_list->getContentSize());
                _topmask->setPositionY(_list->getPositionY()-21);
            }
        }
    }
    
    
}
void Produce::backBtnClick(cocos2d::Ref* s, cocos2d::ui::Button::TouchEventType e)
{
    auto btn = dynamic_cast<Node*>(s);
    if (e == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() - BACKBTN_ACT_DISTANCE);
    }
    else if (e == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backExe();
    }
    else if (e == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
    }
}

void Produce::backExe()
{
    _isOpened = false;
    Manor::getInstance()->showLayer(Manor::kMainLayer);
}
void Produce::addBatchPop(cocos2d::ValueMap data)
{
    auto pop = ProduceBatch::create(data);
    addChild(pop, PURCHASE_BATCH_Z);
}
void Produce::removeSelf()
{
    auto removeme = Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL);
    removeme->setTag(199);
    runAction(removeme);
}
void Produce::updateProperty()
{

}
void Produce::onTaskCompleted(cocos2d::EventCustom *e)
{
    /**
     *  这里需要添加事件筛选，只在必要的时候刷新列表，可以节约消耗
     */
    if (_isOpened)
    {
        this->updateList();
    }
}
void Produce::showInfo(std::string text, Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(text, node);
        this->addChild(pop, 100, "Textpop");
    }
    
    
}

void Produce::onEnter()
{
    Layer::onEnter();
    this->stopActionByTag(199);
    showBoard();
}
void Produce::openProduce()
{
    _isOpened = true;
    updateList();
}
void Produce::showBoard()
{
	addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
    //添加通知板..
	addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}
void Produce::onExit()
{
    Layer::onExit();
    removeChildByName("tips");
}

void Produce::onBoardResize(EventCustom* e){
    listResize();
}
void Produce::listResize()
{
	_list->setContentSize(Size(_list->getContentSize().width, _list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
    _list->setInnerContainerSize(_list->getContentSize());
    _bottomMask->setPositionY(_list->getBottomBoundary());
}

Produce::Item* Produce::getItemByID(int id)
{
    void* p = getMem(id);
    if (p == nullptr)
    {
        p = new Item();
        setMem(id, p);
    }
    return (Produce::Item*)p;
}
Node* Produce::getPattern(bool huge){
    cocos2d::ui::Layout* patten = huge ? _itemHugePattern : _itemPattern;
    return dynamic_cast<Node*>(patten->clone());
}
/************************Item*********************/
Produce* Produce::Item::_delegate = nullptr;

Produce::Item::Item()
:LongPressActionTag(10)
{
    
}
Produce::Item::~Item()
{
    //if (_lisMap.find(TO_S(itemlistener)) != _lisMap.end()) data::Item::getInstance()->removeListener(_lisMap.at(TO_S(itemlistener)));
}
void Produce::Item::removeSelf()
{
    auto removeme = Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL);
    removeme->setTag(199);
    runAction(removeme);
}
void Produce::Item::updateProperty()
{

}
bool Produce::Item::init()
{
    if (!Layout::init())
    {
        return false;
    }
    return true;
}
void Produce::Item::setData(Value data){
    _data = data;
    ValueMap map = _data.asValueMap();
    //通过字数选择item还是itemHuge..
    if (_view) removeChild(_view);
    if (map["name"].asString().size() <= 12){
        _view = _delegate->getPattern();
    }
    else{
        _view = _delegate->getPattern(true);
    }
    //添加item..
    Size size = _view->getContentSize();
    this->setContentSize(size);
    this->addChild(_view);
    _view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _view->setPosition(Vec2(size.width / 2, size.height /2));

    auto txtModel = _view->getChildByName("consumePanel")->getChildByName<ui::Text*>("consume");
    
    /**材料显示模块初始化**/
    auto materialData = _data.asValueMap().at("produce").asValueVector();
    std::string materialText;
    Node* last = nullptr;
    for (auto iter = materialData.begin(); iter != materialData.end(); ++iter)
    {
        auto txtView = (ui::Text*)txtModel->clone();
        txtView->setString((*iter).asValueMap()["name"].asString() + "x"+(*iter).asValueMap()["amount"].asString() + " ");
        _view->getChildByName<ui::Layout*>("consumePanel")->addChild(txtView);
        
        if ((*iter).asValueMap()["shortage"].asBool() == true)
        {
            txtView->setTextColor(DeerUtils::ShotageColor);
        }
        else
        {
            txtView->setTextColor(DeerUtils::EnoughColor);
        }
        
        if (last){
            float x = last->getContentSize().width + last->getPositionX() + MATERIAL_SPACE;
            if(x+txtView->getContentSize().width >= _view->getChildByName("consumePanel")->getContentSize().width)
            {
                txtView->setPosition(Vec2(0,MATERIAL_SPACEH));
            }
            else
            {
                txtView->setPosition(Vec2(x,last->getPositionY()));
            }
            
        }else{
            txtView->setPositionX(0);
        }
        last = txtView;
        
        _consumes.pushBack(txtView);
        
    }
    txtModel->removeFromParent();
    
    
    /* 修改名称 */
    auto nameText = _view->getChildByName<ui::Text*>("name");
    std::string nametxt = _data.asValueMap()["name"].asString();
    
    if (_data.asValueMap()["iid"].asInt() != 111221 && nametxt.size() > 12 )
    {
        nametxt.insert(12, "\n");
    }
    else if (_data.asValueMap()["iid"].asInt() == 111221)
    {
        nametxt.insert(13, "\n");
    }
    nameText->setString(nametxt);
    
    /**
     *  小红点
     */
    if(data::Item::getInstance()->getUserItemByIID(getID()).asValueMap()["amount"].asInt() >0)
    {
        _view->getChildByName<ui::ImageView*>("newdot")->setVisible(false);//_data.asValueMap()["new"].asBool()
    }
    else
    {
        _view->getChildByName<ui::ImageView*>("newdot")->setVisible(true);
    }
    
    
    
    //显示星级..
    for (auto i = 1; i < _data.asValueMap()["level"].asInt(); i++)
    {
        _view->getChildByName<ui::ImageView*>("star"+Value(i).asString())->setVisible(true);
    }
    for (auto i = _data.asValueMap()["level"].asInt() + 1; i < 6 ; i++)
    {
        _view->getChildByName<ui::ImageView*>("star"+Value(i).asString())->setVisible(false);
    }
    
    //按钮初始化..
    auto nameBtn = _view->getChildByName<ui::Layout*>("namebackArea");
    nameBtn->addTouchEventListener(CC_CALLBACK_2(Item::nameClick, this));
    nameBtn->setPropagateTouchEvents(false);
    nameBtn->setSwallowTouches(true);
	_view->getChildByName("nameback")->setOpacity(140);
    
    //信息按钮的回调..
    _view->getChildByName("infoBtn")->getChildByName<ui::Layout*>("infoBtnArea")->addTouchEventListener(CC_CALLBACK_2(Item::infoClick, this));
    
    /**
     *  监听道具变化
     */
	data::Item* item = data::Item::getInstance();
	EventListenerCustom* listener = item->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(Item::updateMaterial, this));
	addEventPool(listener, item);
    
}
void Produce::Item::updateMaterial(EventCustom* e)
{
    if(!_updateList)
    {
        _updateList = true;
        auto item = (Value*)e->getUserData();
        int iid = item->asValueMap()["iid"].asInt();
        ValueVector &con = _data.asValueMap().at("produce").asValueVector();
        auto iter = std::find_if(con.begin(), con.end(), [=](Value item)->bool{
            return iid == item.asValueMap()["iid"].asInt();
        });
        if (iter != con.end())
        {
            if (item->asValueMap()["total"].asInt() >= (*iter).asValueMap()["amount"].asInt())
            {
                _consumes.at(iter - con.begin())->setTextColor(DeerUtils::EnoughColor);
                (*iter).asValueMap()["shortage"] = false;
            }
            else
            {
                _consumes.at(iter - con.begin())->setTextColor(DeerUtils::ShotageColor);
                (*iter).asValueMap()["shortage"] = true;
            }
        }
        /**
         *  小红点
         */
        if(data::Item::getInstance()->getUserItemByIID(getID()).asValueMap()["amount"].asInt() >0)
        {
            _view->getChildByName<ui::ImageView*>("newdot")->setVisible(false);
        }
        else
        {
            _view->getChildByName<ui::ImageView*>("newdot")->setVisible(true);
        }
        _updateList = false;
    }
    
}
void Produce::Item::nameClick(cocos2d::Ref *s, cocos2d::ui::Text::TouchEventType e)
{
    if (e == ui::Text::TouchEventType::ENDED || e == ui::Text::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
    }
    if (e == ui::Text::TouchEventType::BEGAN)
    {
        /**
         *  刚开始将长按的触摸设置为false(应为不确定触摸的类型)
         */
        _longTouching = false;
        
        /**
         *  长按执行延时回调
         */
		if (!data::Item::getInstance()->isProduceDisposable(_data.asValueMap()["iid"].asInt())){
			auto delay = DelayTime::create(1);
			CallFunc *call = CallFunc::create([=]()
			{
				/**
				*  这里将长按标志位设为1
				*/
				_longTouching = true;

				_view->getChildByName("nameback")->setOpacity(140);
				//auto newdot = _view->getChildByName("newdot");
				//if (newdot->isVisible())
				//{
					//newdot->setVisible(false);
				//}
				_delegate->addBatchPop(_data.asValueMap());

			});
			auto seq = Sequence::create(delay, call, NULL);
			seq->setTag(LongPressActionTag);
			this->runAction(seq);
		}

    }
    if (e == Widget::TouchEventType::MOVED)
    {
        auto btn = dynamic_cast<ui::Widget*>(s);
        
        if ((btn->getTouchMovePosition() - btn->getTouchBeganPosition()).length()> 20)
        {
            /**
             *  如果移动距离大于了20停止长按检测
             */
            this->stopAllActionsByTag(LongPressActionTag);
            /**
             *  把触摸传给上层容器
             */
            btn->setPropagateTouchEvents(true);
        }
    }
    if (e == Widget::TouchEventType::ENDED || e == Widget::TouchEventType::CANCELED)
    {
        /**
         *  如果调用了end，就停止长按键回调
         */
        stopAllActionsByTag(LongPressActionTag);
        dynamic_cast<ui::Widget*>(s)->setPropagateTouchEvents(false);
        
        /**
         *  这段内容当批量界面已经弹出的时候调用的，目的是不再执行单个购买操作
         */
        if (_longTouching)
        {
            _longTouching = false;
            return;
        }
        build();
    }
    
    //BTN_CALL_ACT(e, _view->getChildByName<ui::ImageView*>("nameback"), s, CC_CALLBACK_0(Item::build, this));
}
void Produce::Item::build()
{
    /** 点击当下去掉新标记 **/
    //auto newdot = _view->getChildByName("newdot");
    //if (newdot->isVisible())
    //{
        //newdot->setVisible(false);
    //}
    
    auto retInfo = data::Item::getInstance()->produce(getID()).asValueMap();
    if (retInfo.at("code").asInt() == 1)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kLACK);
        auto data = retInfo.at("data");
        std::stringstream str;
        for(auto mapItem : data.asValueMap())
        {
            str<<"缺少: "<<mapItem.second.asValueMap()["name"].asString()<<"\n";
            auto tips = data::Lang::getInstance()->getTipsByIID(mapItem.second.asValueMap()["iid"].asInt());
            str<<tips<<"\n";
        }
		Notice::getInstance()->postTips(str.str(),Notice::kWarning);
    }
    else if (retInfo.at("code").asInt() == 0)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBuildFinish);
		Notice::getInstance()->postBoard("制作了: " + _data.asValueMap()["name"].asString() + "X1");
		if (data::Item::getInstance()->isProduceDisposable(getID())){
			this->removeFromParent();
		}
        _delegate->updateList();
    }
}

void Produce::Item::infoClick(cocos2d::Ref *s, cocos2d::ui::Button::TouchEventType e)
{
    ui::ImageView* infolight = _view->getChildByName("infoBtn")->getChildByName<ui::ImageView*>("infolight");
    if (e == ui::Button::TouchEventType::BEGAN)
    {
        infolight->stopAllActionsByTag(INFO_BTN_ACT);
        infolight->setOpacity(255);
        infolight->setVisible(true);
    }
    else if (e == ui::Button::TouchEventType::MOVED)
    {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(s);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de > 20)
        {
            infolight->setVisible(false);
        }
    }
    if (e == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPopUp);
        auto infoBtnAct = FadeOut::create(0.3);
        infoBtnAct->setTag(INFO_BTN_ACT);
        infolight->runAction(infoBtnAct);
        Item::showInfo();
    }
    
    if (e == ui::Button::TouchEventType::CANCELED)
    {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(s);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de < 20)
        {
            SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPopUp);
            auto infoBtnAct = FadeOut::create(0.3);
            infoBtnAct->setTag(INFO_BTN_ACT);
            infolight->runAction(infoBtnAct);
            runAction(Sequence::create(DelayTime::create(0.3), NULL));
        }
    }
}
void Produce::Item::showInfo()
{
    auto light = _view->getChildByName("infoBtn")->getChildByName<ui::ImageView*>("infolight");
    if (light)
    {
        light->setVisible(false);
        light->setOpacity(255);
    }
    if (_delegate)
    {
        auto area = _view->getChildByName<ui::ImageView*>("nameback");
        _delegate->showInfo(_data.asValueMap()["summary"].asString(), area);
    }
}
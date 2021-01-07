/****************************************************************************
  
 *	@desc	建造界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/Build.h
 *  @modify	null
 ******************************************************************************/

#include "editor-support/cocostudio/CocoStudio.h"
#include "GlobalMacros.h"
#include "utils/DeerUtils.h"
#include "data/Building.h"
#include "manor/Manor.h"
#include "manor/TextPopup.h"
#include "Build.h"
#include "popUp/BlackMask.h"
#include "popUp/Summary.h"
#include "notice/Notice.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"
#include "data/Lang.h"
#include "data/Task.h"

USING_NS_CC;
const int Build::TAG = 100;

Build::~Build()
{
	
}

const float Build::ACT_DURATION = 1.0f;

bool Build::init()
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
    Node* view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("build.csb");
    
	_panel = view->getChildByName<ui::Layout*>("Panel");
	view->setContentSize(winSize);
	this->addChild(view);
	ui::Helper::doLayout(this);
	//返回按钮..
	ui::Layout* backButton = _panel->getChildByName<ui::Layout*>("gobackBtnArea");
	backButton->setTouchEnabled(true);
	backButton->setSwallowTouches(false);
	backButton->addTouchEventListener(CC_CALLBACK_2(Build::backBtnClick, this));
	//底部遮罩..
	_bottomMask = view->getChildByName("Panel")->getChildByName<ui::Layout*>("bottomMask");
    _topMask    = view->getChildByName("Panel")->getChildByName<ui::Layout*>("topmask");
	//列表容器..
	_list = view->getChildByName("Panel")->getChildByName<ui::ScrollView*>("scrollView");
	
	//模块模板(小)..
	_itemPattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("item");
	_itemPattern->retain();
	//模块模板(大)..
	_itemHugePattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("itemHuge");
	_itemHugePattern->retain();
	//设置子元素代理..
	Item::setDelegate(this);
	//当故事内容打开或者关上是，调整listView的长度..
	Notice* notice = Notice::getInstance();
	EventListenerCustom * foldEvent = notice->addListener(Notice::FOLD, CC_CALLBACK_1(Build::onBoardResize, this));
	addEventPool(foldEvent, notice);
    
	/* 更新项目列表 */
	_list->removeAllChildren();
	_list->addChild(Layer::create(), 1, "layer");
    //新手引导小手..
    _guide = Guide::create(data::Guid::BUID_LAYER);
    this->addChild(_guide,10000);
    _guide->addFinishGuid();
    _guide->insertButton(data::Guid::BUILD_BUTTON_BACK, backButton);
    
    //设置天空大小，天空海阔你与我~~~
    auto starSky =  _panel->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winSize.width/2,(int)winSize.height*75/128));
    
    return true;
}
void Build::updateList()
{
    Value all = data::Building::getInstance()->getUserBuildList();
    if(preBuildData != all || preBuildData.asValueVector().size() != all.asValueVector().size())
    {
        preBuildData = all;
        
        Layer* layer = dynamic_cast<Layer*>(_list->getChildByName("layer"));
        layer->setContentSize(Size(layer->getContentSize().width,0));
        layer->setPosition(Vec2::ZERO);
        
        float x = 0;
        float y = 0;
        int nZoderMax = 1000;
        for (Value row : all.asValueVector())
        {
            Item* item = getItemByID(row.asValueMap()["bid"].asInt());
            item->setData(row);
            
            //设置层级
            item->setZOrder(nZoderMax--);
            
            y -= (item->getContentSize().height + ITEM_SPACE);
            if (item->getParent()){
                item->runAction(Sequence::create(MoveTo::create(ACT_DURATION, Vec2(x, y)), NULL, NULL));
                //新手引导按钮存储..
                updateBtnsArea(item);
            } else{
                layer->addChild(item);
                item->setPosition(Vec2(x, y));
                item->fadeIn();
                
                //新手引导按钮存储..
                updateBtnsArea(item);
            }
        }
        _listInnerHeight = -y+ITEM_SPACE;
        listResize();
        
        //更新提示信息..
        auto  tips = DeerUtils::seekChildByNameWithRetType<ui::Layout*>(this, "tips");
        tips->setVisible(false);
        //兵营是否建造完成..
        if(data::Building::getInstance()->getUserBuildByBID(111701).asValueMap()["complete"].asBool())
        {
            tips->setVisible(true);
            if(_list->getPositionY() != tips->getPositionY())
            {
                _list->setPositionY(tips->getPositionY());
                float distanceY = _listMaxHeight- tips->getPositionY();
                Size innerSize = _list->getInnerContainerSize();
                _list->setContentSize(Size(_list->getContentSize().width, _list->getContentSize().height-distanceY));
                _list->setInnerContainerSize(innerSize);
                _bottomMask->setPositionY(_list->getBottomBoundary());
                _topMask->setPositionY(_topMaskMaxHeight-28);
                Layer* layer = dynamic_cast<Layer*>(_list->getChildByName("layer"));
                layer->setPositionY(_list->getInnerContainerSize().height);
            }
            
        }
        if(all.asValueVector().size() > 12)
        {
            tips->setVisible(false);
            if(_list->getPositionY() != _listMaxHeight)
            {
                _list->setPositionY(_listMaxHeight);
                
                float distanceY = _listMaxHeight- tips->getPositionY();
                Size innerSize = _list->getInnerContainerSize();
                _list->setContentSize(Size(_list->getContentSize().width, _list->getContentSize().height+distanceY));
                _list->setInnerContainerSize(innerSize);
                _bottomMask->setPositionY(_list->getBottomBoundary());
                _topMask->setPositionY(_topMaskMaxHeight);
                Layer* layer = dynamic_cast<Layer*>(_list->getChildByName("layer"));
                layer->setPositionY(_list->getInnerContainerSize().height);
            }
            
        }
    }
	
}
void Build::updateBtnsArea(Item* build)
{
    if(build->_buildId >= data::Building::SUPPORT_MIN && build->_buildId <= data::Building::SUPPORT_MAX)//庇护所..
    {
        _guide->insertButton(data::Guid::BUILD_BUTTON_SHELTER, build->getChildByTag(TAG)->getChildByName<ui::Layout*>("nameBackArea"));
    }
    else
    {
        _guide->insertButton(build->_buildId, build->getChildByTag(TAG)->getChildByName<ui::Layout*>("nameBackArea"));
    }
}
void Build::backBtnClick(cocos2d::Ref* s, cocos2d::ui::Button::TouchEventType e)
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
        backExe();
	}
}

void Build::backExe()
{
    //更新新手状态..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::BUID_BACK);
    
	Manor::getInstance()->showLayer(Manor::kMainLayer);
}

void Build::removeSelf()
{
	auto removeme = Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL);
	removeme->setTag(199);
	runAction(removeme);
}
void Build::updateProperty()
{

}

void Build::showInfo(std::string text, Node* node)
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

void Build::onEnter()
{
	Layer::onEnter();
	this->stopActionByTag(199);
	showBoard();
}

void Build::showBoard()
{
	addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
	//添加通知板..
	addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}
void Build::onExit()
{
	Layer::onExit();
	removeChildByName("tips");
}

void Build::onBoardResize(EventCustom* e){
	listResize();
}
void Build::listResize()
{
	float height = _list->getWorldPosition().y - Notice::getInstance()->getBoardHeight();
	_list->setContentSize(Size(_list->getContentSize().width, height));
    if(_listInnerHeight < _list->getContentSize().height)
    {
        _list->setInnerContainerSize(Size(_list->getContentSize().width, _list->getContentSize().height));
    }
    else
    {
        _list->setInnerContainerSize(Size(_list->getContentSize().width, _listInnerHeight));
    }
	_bottomMask->setPositionY(_list->getBottomBoundary());
    Layer* layer = dynamic_cast<Layer*>(_list->getChildByName("layer"));
    layer->setPositionY(_list->getInnerContainerSize().height);
}
void Build::onBuildChange(EventCustom* e){
	Value* data = (Value*)e->getUserData();
	bool flag = false;
	for (Value &i : data->asValueMap()["cause"].asValueVector())
	{
		if (i.asValueMap()["type"].asInt() == data::Task::BUILD)
		{
			if (!getChildByName("sudden"))
			{
				flag = true;
				break;
			}
		}
	}
	if (flag){
		updateList();
		data::Guid::getInstance()->showGuid();
	}
}

Build::Item* Build::getItemByID(int id)
{
	void* p = getMem(id);
	if (p == nullptr)
	{
		p = Item::create();
		setMem(id, p);
	}
	return (Build::Item*)p;
}
Node* Build::getPattern(bool huge){
	cocos2d::ui::Layout* patten = huge ? _itemHugePattern : _itemPattern;
	return dynamic_cast<Node*>(patten->clone());
}
/************************Item*********************/
Build* Build::Item::_delegate = nullptr;

Build::Item::~Item()
{
	//if (_lisMap.find(TO_S(itemlistener)) != _lisMap.end()) data::Item::getInstance()->removeListener(_lisMap.at(TO_S(itemlistener)));
}

bool Build::Item::init()
{
	if (!Layout::init())
	{
		return false;
	}
	return true;
}
void Build::Item::setData(Value data){
	_data = data;
	ValueMap map = _data.asValueMap();
	//通过字数选择item还是itemHuge..
	if (_view &&_view->getReferenceCount() > 0)
    {
        removeChild(_view);
    }
	if (map["name"].asString().size() <= 12){
		_view = _delegate->getPattern();
		_view->getChildByName<ui::ImageView*>("supportback")->setVisible(false);
	}
	else{
		_view = _delegate->getPattern(true);
	}
	//添加item..
	//_view->setPosition(Vec2(0, 0));
	Size size = _view->getContentSize();
	this->setContentSize(size);
	this->addChild(_view);
    _view->setTag(TAG);
	_view->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_view->setPosition(Vec2(size.width / 2, size.height /2));
	//设置建筑名字..
	_view->getChildByName<ui::Text*>("name")->setString(map["name"].asString());
    _buildId = map["bid"].asInt();
	//是否新..
	_view->getChildByName<ui::ImageView*>("newdot")->setVisible(!map["complete"].asBool());// map["new"].asBool()
	//庇护所显示数量..
	if (map["isSupport"].asBool() && map["amount"].asInt() > 0){
		_view->getChildByName<ui::ImageView*>("supportback")->setVisible(true);
		_view->getChildByName("supportback")->getChildByName<ui::Text*>("supportnum")->setString(map["amount"].asString());
	}else{
		_view->getChildByName("materialPanel")->getChildByName<ui::Text*>("material")->setPositionX(0);
	}
	//如果建筑物完成显示默认文字，没有完成要显示所有的建造材料..
	if (map["complete"].asBool()){
		_view->getChildByName("materialPanel")->getChildByName<ui::Text*>("material")->setTextColor(DeerUtils::ShotageColor);
	}
	else{
		//建造需要的材料..
		ui::Layout* material = _view->getChildByName<ui::Layout*>("materialPanel");
		ui::Text* pattern = material->getChildByName<ui::Text*>("material");
		Node* last = nullptr;
		for (Value& i : map["build"].asValueVector()){
			ui::Text* text;
			if (last){
				text = dynamic_cast<ui::Text*>(pattern->clone());
				float x = last->getContentSize().width + last->getPositionX() + MATERIAL_SPACE;
				text->setPositionX(x);
				material->addChild(text);
			}else{
				text = pattern;
			}
			last = text;
			text->setString(i.asValueMap()["name"].asString() + "x" + i.asValueMap()["amount"].asString());
			if (i.asValueMap()["shortage"].asBool()) text->setTextColor(DeerUtils::ShotageColor);
		}
		//名称点击，触发建造..
		ui::Layout* nameBtn = _view->getChildByName<ui::Layout*>("nameBackArea");
		nameBtn->setSwallowTouches(true);
		nameBtn->setPropagateTouchEvents(false);
		nameBtn->addTouchEventListener(CC_CALLBACK_2(Item::nameClick, this));
	}

	//信息按钮的回调注册..
	ui::Layout* infoBtn = _view->getChildByName("InfoBtn")->getChildByName<ui::Layout*>("InfoBtnArea");
	infoBtn->addTouchEventListener(CC_CALLBACK_2(Item::infoClick, this));
}

void Build::Item::nameClick(cocos2d::Ref *s, cocos2d::ui::Text::TouchEventType e)
{
	if (e == ui::Text::TouchEventType::ENDED || e == ui::Text::TouchEventType::CANCELED)
	{
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
	}
	if (e == Widget::TouchEventType::MOVED)
	{
		auto btn = dynamic_cast<ui::Widget*>(s);

		if ((btn->getTouchMovePosition() - btn->getTouchBeganPosition()).length()> 20)
		{
			/**
			*  把触摸传给容器
			*/
			btn->setPropagateTouchEvents(true);
		}
	}
	if (e == Widget::TouchEventType::ENDED||e == Widget::TouchEventType::CANCELED)
	{
		dynamic_cast<ui::Widget*>(s)->setPropagateTouchEvents(false);
        build();
	}
	//BTN_CALL_ACT(e, _view->getChildByName<ui::ImageView*>("nameBack"), s, CC_CALLBACK_0(Item::build, this));
}
void Build::Item::build()
{
	//点击去除new标记..
	//_view->getChildByName<ui::ImageView*>("newdot")->setVisible(false);
	int bid = _data.asValueMap().at("bid").asInt();
	auto buildRet = data::Building::getInstance()->build(bid);
	if (buildRet.asValueMap().at("code").asInt() == 1)
	{
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kLACK);
		std::stringstream str;
		for (auto mapItem : buildRet.asValueMap().at("data").asValueMap())
		{
			if (mapItem.second.asInt() < 0)
			{
                str << "缺少: ";
				auto itemdata = data::Item::getInstance()->getItemByIID(Value(mapItem.first).asInt());
				str << itemdata.asValueMap()["name"].asString() << "X" << -mapItem.second.asInt() << "\n";
                auto tips = data::Lang::getInstance()->getTipsByIID(Value(mapItem.first).asInt());
                str << tips<<"\n";
			}
		}
		Notice::getInstance()->postTips(str.str(),Notice::kWarning);
	}
	else if (buildRet.asValueMap().at("code").asInt() == 2)
	{
		CCLOG("已经拥有..");
	}
	else if (buildRet.asValueMap().at("code").asInt() == 0)
	{
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBuildFinish);
		if (_data.asValueMap().at("isSupport").asBool())
        {
            if(_data.asValueMap().at("bid").asInt() == data::Building::BuildingID::SUPPORT_MAX)
            {
                
            }
            else
            {
                _delegate->getItemByID(bid)->removeFromParent();
            }
            
        }
		_delegate->updateList();
        //更新新手引导..
        data::Guid::getInstance()->showGuid();
	}
}

void Build::Item::infoClick(cocos2d::Ref *s, cocos2d::ui::Button::TouchEventType e)
{
	ui::ImageView* infolight = _view->getChildByName("InfoBtn")->getChildByName<ui::ImageView*>("infolight");
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
		}
	}
}
void Build::Item::showInfo()
{
	auto light = _view->getChildByName("InfoBtn")->getChildByName<ui::ImageView*>("infolight");
	if (light)
	{
		light->setVisible(false);
		light->setOpacity(255);
	}
	if (_delegate)
	{
        auto area = _view->getChildByName<ui::ImageView*>("nameBack");
		_delegate->showInfo(_data.asValueMap()["summary"].asString(), area);
	}
}
void Build::Item::fadeIn(){
	//_view->setScale(0.1f);
	//_view->runAction(Sequence::create(ScaleTo::create(ACT_DURATION, 1.0f), NULL, NULL));
	//直接设置透明度不起作用，需要分批设置..
	for (auto node : _view->getChildren()){
		node->setOpacity(0);
		node->runAction(FadeIn::create(1));
	}
	
}

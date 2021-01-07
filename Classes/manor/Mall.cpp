/************************************************************
    
 *	@desc	商城
 *	@date	2015-3-29
 *	@author	110101
     
 *	@file	manor/Mall.h
 *  @modify	minamoto
 *****************************************************/

#include "Mall.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "GlobalMacros.h"
#include "data/Mall.h"
#include "popUp/MallSurePop.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"
#include "utils/DeerUtils.h"

USING_NS_CC;

Mall::~Mall()
{
}

bool Mall::init()
{
    if (!Layout::init())
    {
        return false;
    }
    addChild(BlackMask::create());
    
    
    
    auto win = Director::getInstance()->getWinSize();
    setContentSize(win);
    //setTouchEnabled(true);
    //setBackGroundColor(Color3B::BLACK);
    //setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    
    Node* view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("mall.csb");
    view->setContentSize(win);
    addChild(view);
    ui::Helper::doLayout(this);
    
	ui::Layout* goback = view->getChildByName("Panel")->getChildByName<ui::Layout*>("backBtnArea");
    goback->addTouchEventListener(CC_CALLBACK_2(Mall::gobackCall, this));
    
    _rmbTxt  = view->getChildByName("Panel")->getChildByName<ui::Text*>("rmbTxt");
    std::string  rmbNum  = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
    if(rmbNum.size() == 0)
    {
        rmbNum = "0";
    }
    _rmbTxt->setString(rmbNum);
    
	_itemPattern = view->getChildByName("Panel")->getChildByName<ui::Layout*>("item");
	_itemPattern->retain();
	_itemPattern->removeFromParent();

	Item::setDelegate(this);
    
	_list = view->getChildByName("Panel")->getChildByName<ui::ScrollView*>("scroll");
	_list->addChild(Layer::create(),1, "layer");
    upItems();
    
    //设置天空大小,天空海阔你与我~~~
    auto starSky = view->getChildByName("Panel")->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(win.width/2,win.height*75/128));
    
    EventListenerCustom* e = data::Mall::getInstance()->addListener(data::Mall::USER_MALL_CHANGE, CC_CALLBACK_1(Mall::onMallChange, this));
    
    EventListenerCustom* e2 = data::Mall::getInstance()->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(Mall::onCrystalChange, this));

//	EventListenerCustom* e3 = data::Mall::getInstance()->addListener(data::User::GOLD_CHANGE, CC_CALLBACK_1(Mall::ongoldChange, this));
    
	addEventPool(e,data::Mall::getInstance());
    addEventPool(e2,data::Item::getInstance());
//	addEventPool(e3, data::User::getInstance());
    return true;
}

void Mall::upItems()
{
    Value all = data::Mall::getInstance()->getMallList();
        
    Layer* layer = dynamic_cast<Layer*>(_list->getChildByName("layer"));
    layer->setPosition(Vec2::ZERO);
    int i = 0;
    float colgap = _list->getContentSize().width / 2;
    for (Value row : all.asValueVector()){
        auto item = getItemByID(row.asValueMap()["mall_id"].asInt());
        if (item->getParent() == nullptr){
            layer->addChild(item);
        }
        item->setData(row);
        float x = (i % 2) * colgap;
        float y = (i / 2) * ITEM_HEIGHT;
        item->setPosition(Vec2(x, -y));
        i++;
    }
}

Mall::Item* Mall::getItemByID(int id){
	void* p = getMem(id);
	if (p == nullptr){
		p = Item::create();
		static_cast<Item*>(p)->setDelegate(this);
		setMem(id, p);
	}
	return (Mall::Item*)p;
}
Node* Mall::getPattern(){
	return dynamic_cast<Node*>(_itemPattern->clone());
}
void Mall::gobackCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    auto btn = dynamic_cast<Node*>(sender);
    if (t == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() -BACKBTN_ACT_DISTANCE);
    }
    if (t == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
		SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
    else if(t == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
    }
}

void Mall::backReal()
{
    Manor::getInstance()->showLayer(Manor::kMainLayer);
}

void Mall::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL));
}

void Mall::updateProperty()
{

}

void Mall::onMallChange(cocos2d::EventCustom *e)
{
	Value mall = *(Value*)e->getUserData();
	if (mall.asValueMap()["parent_mall_id"].asInt() > 0){
		getItemByID(mall.asValueMap()["mall_id"].asInt())->removeFromParent();
	}
	upItems();
}

void Mall::showSurePop(cocos2d::Value data)
{
    addChild(MallSurePop::create(data.asValueMap()), 1);
}
void Mall::onCrystalChange(cocos2d::EventCustom *e)
{
    std::string  rmbNum  = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
    if(rmbNum.size() == 0)
    {
        rmbNum = "0";
    }
    _rmbTxt->setString(rmbNum);
}



/************************Item*********************/
Mall* Mall::Item::_delegate = nullptr;

Mall::Item::~Item()
{
	//if (_lisMap.find(TO_S(itemlistener)) != _lisMap.end()) data::Item::getInstance()->removeListener(_lisMap.at(TO_S(itemlistener)));
}

bool Mall::Item::init()
{
	if (!Layout::init())
	{
		return false;
	}
	_view = _delegate->getPattern();
	setContentSize(_view->getContentSize());
	addChild(_view);
	_view->getChildByName<ui::Button*>("buyBtn")->addTouchEventListener(CC_CALLBACK_2(Item::buyBtnCall, this));
	return true;
}

void Mall::Item::setData(cocos2d::Value data)
{
    _data = data;
	ValueMap map = data.asValueMap();
    
	auto buyBtn = _view->getChildByName<ui::Button*>("buyBtn");
	buyBtn->setOpacity(MANOR_BUTTON_OPACITY);
	_view->getChildByName<ui::Text*>("name")->setString(map["name"].asString());
//	_view->getChildByName<ui::Text*>("price")->setString(data::Lang::getInstance()->getWord(110101) + " " + map["cost"].asString());
    _view->getChildByName<ui::Text*>("price")->setString(map["cost"].asString());
    //加载图片..
	_view->getChildByName<ui::ImageView*>("icon")->loadTexture(map["img"].asString());
    
    if (map["complete"].asBool())
    {
        buyBtn->setTouchEnabled(false);
		_view->getChildByName<ui::Text*>("price")->setVisible(false);
        _view->getChildByName<ui::Text*>("crystalicon")->setVisible(false);
    }
	_view->getChildByName<ui::ImageView*>("ownflag")->setVisible(map["complete"].asBool());
}

void Mall::Item::buyBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if(t == ui::Button::TouchEventType::ENDED)
    {
        buyReal();
    }
    
    //BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Item::buyReal, this));
}

void Mall::Item::buyReal()
{
    if (_delegate)
    {
		_delegate->showSurePop(_data);
    }
}
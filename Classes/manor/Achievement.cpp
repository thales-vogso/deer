/****************************************************************************
                  UMGame
 *	@desc   成就界面
 *	@date	2015-1-15
 *	@author	110101
     
 *	@file	manor/Achievement.h
 *  @modify	minamoto
 ******************************************************************************/

#include "Achievement.h"
#include "data/Achievement.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
//#include "manor/TextPopup.h"
#include "data/Lang.h"
#include "GlobalMacros.h"
#include "notice/Notice.h"
#include "popUp/Summary.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;

Achievement::~Achievement()
{
}

bool Achievement::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->addChild(BlackMask::create());
    _touchlistener = EventListenerTouchOneByOne::create();
    _touchlistener->setSwallowTouches(false);
    _touchlistener->setEnabled(false);
    _touchlistener->onTouchBegan =[=](Touch* touch, Event*event)->bool{return true;};
    _touchlistener->onTouchMoved =[=](Touch* touch, Event*event){
        float delta = touch->getLocation().x - touch->getStartLocation().x;
        if (delta < -deerResouseSize.width /3)
        {
            _touchlistener->setEnabled(false);
            MoveTo * movAct = MoveTo::create(1, Vec2(-deerResouseSize.width, 0));
			CallFunc *calFun = CallFunc::create(std::bind(&Achievement::removeFromParent, this));
            this->runAction(Sequence::create(movAct, calFun, NULL));
        }
        else if (delta > deerResouseSize.width /3)
        {
            _touchlistener->setEnabled(false);
            MoveTo * movAct = MoveTo::create(1, Vec2(deerResouseSize.width, 0));
			CallFunc *calFun = CallFunc::create(std::bind(&Achievement::removeFromParent, this));
            this->runAction(Sequence::create(movAct, calFun, NULL));
        }
    };
    _eventDispatcher->addEventListenerWithFixedPriority(_touchlistener, -1);

//    Point temv[3] = {Vec2(111, 166), Vec2(406, 166), Vec2(702, 166)};
    std::vector<Vec2> pos({Vec2(111, 166), Vec2(406, 166), Vec2(702, 166)});
    
    //初始化显示..
    _view = CSLoader::createNode("achievement.csb");
    _view->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(_view);
    ui::Helper::doLayout(_view);
    //总成就点数..
	_pointTxt= _view->getChildByName("panel")->getChildByName("tip")->getChildByName<ui::TextBMFont*>("pointText");
	_pointTxt->setString(Value(data::Item::getInstance()->getPoint()).asString());
    //返回按钮..
	auto backArea = _view->getChildByName("panel")->getChildByName<ui::Layout*>("gobackBtnArea");
    backArea->setTouchEnabled(true);
    backArea->setSwallowTouches(false);
	backArea->addTouchEventListener(CC_CALLBACK_2(Achievement::backButtonCallback, this));
    
    //初始化模板..
    _itemPattern = _view->getChildByName("panel")->getChildByName("scroll")->getChildByName<ui::Layout*>("item");
	_itemPattern->removeFromParent();
	_itemPattern->retain();
	
    
    Size win = Director::getInstance()->getWinSize();
    ui::ScrollView* scroll = _view->getChildByName("panel")->getChildByName<ui::ScrollView*>("scroll");
    scroll->setContentSize(Size(scroll->getContentSize().width,scroll->getContentSize().height*win.height/SCREEN_HEIGHT));
    
    return true;
}
void Achievement::upItems()
{
    auto all = data::Achievement::getInstance()->getUserAchievements();
    if(preAchievementData!= all || preAchievementData.asValueVector().size() != all.asValueVector().size())
    {
        preAchievementData = all;
        ui::ScrollView* scroll = _view->getChildByName("panel")->getChildByName<ui::ScrollView*>("scroll");
        scroll->removeAllChildren();
        
        int rowNum = (int)all.asValueVector().size()/THREE;
        if(rowNum != _rowNum)
        {
            _rowNum = rowNum;
            scroll->setInnerContainerSize(Size(scroll->getInnerContainerSize().width,(_rowNum+1)*SPACE_H+_itemPattern->getContentSize().height/2.0));
        }
        auto size = scroll->getInnerContainerSize();
        for (int i = 0; i < all.asValueVector().size(); i++){
            Value row = all.asValueVector()[i];
            Item* item = Item::create(row, this);
            scroll->addChild(item);
            int r = i / THREE;
            int c = i % THREE;
            float x1 = (float)c*size.width /TWO;
            float x2 = _itemPattern->getContentSize().width*1.3;
            item->setPositionX(x1 + (1-c)*x2/TWO);
            int y = size.height - r * SPACE_H - SPACE_H;
            item->setPositionY(y);
        }
        _pointTxt->setString(Value(data::Item::getInstance()->getPoint()).asString());
    }
    
}
void Achievement::freshList()
{
    upItems();
    ui::ScrollView* scroll = _view->getChildByName("panel")->getChildByName<ui::ScrollView*>("scroll");
    scroll->scrollToTop(0.1,false);
}
void Achievement::onEnter()
{
    Layer::onEnter();
    //showBoard();
}

void Achievement::showBoard()
{
    this->addChild(Notice::getInstance()->getBoard(), 50);
}

void Achievement::backButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if (touchType == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
}

void Achievement::backReal()
{
    Manor::getInstance()->showLayer(Manor::kVenture);
}

void Achievement::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL));
}

void Achievement::updateProperty()
{

}

void Achievement::showInfo(std::string text, cocos2d::Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(text,node);
        this->addChild(pop, 100, "Textpop");
    }
    
    
}


/*********************    item     *******************************/

Achievement::Item *Achievement::Item::create(cocos2d::Value adata, Achievement *delegate)
{
    Item *ret = new Item();
    ret->_delegate = delegate;
    if (ret && ret->init(adata))
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

bool Achievement::Item::init(cocos2d::Value adata)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _adata = adata;
    
    //初始显示..
	ui::Widget* view = _delegate->_itemPattern->clone();
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
	ui::TextBMFont* achm = view->getChildByName<ui::TextBMFont*>("name");
    achm->setString(_adata.asValueMap()["name"].asString());
	ui::Button* btn = view->getChildByName<ui::Button*>("btnback");
	btn->setTouchEnabled(true);
	btn->addTouchEventListener(CC_CALLBACK_2(Item::infoCallback, this));
	achm->setColor(DeerUtils::hexToColor3BByLevel(_adata.asValueMap()["level"].asInt()));
    
	ui::ImageView* compeleticon = view->getChildByName<ui::ImageView*>("compeleticon");
	if (_adata.asValueMap()["complete"].asBool()) compeleticon->setVisible(true);
	else compeleticon->setVisible(false);
    
	for (int i = 1; i <= 5; i++){
		ui::ImageView* star = view->getChildByName<ui::ImageView*>("star" + Value(i).asString());
		if (_adata.asValueMap()["level"].asInt() >= i){
			star->setVisible(true);
		}
		else{
			star->setVisible(false);
		}
	}
    return true;
}

void Achievement::Item::infoCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType tevent)
{
    if (tevent == ui::Text::TouchEventType::ENDED)
    {
		SoundManager::getInstance()->playEffect(SoundManager::EffectType::kPopUp);
		std::string txt = _adata.asValueMap()["summary"].asString();
		if ( _adata.asValueMap()["isCounter"].asBool())
        {
			txt = txt + "\n" + "当前进度  " + _adata.asValueMap()["counter"].asString();
        }
        txt = txt + "\n" + "完成后可得成就点数  "+_adata.asValueMap()["point"].asString();
        auto area = seekChildByNameWithRetType<ui::ImageView*>(this, "btnback");
        _delegate->showInfo(txt , area);
    }
}


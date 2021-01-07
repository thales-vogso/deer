/****************************************************************************
     
 *	@desc	商店界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/StoreLayer.cpp
 *  @modify	null
 ******************************************************************************/

#include "Shop.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "data/Item.h"
#include "manor/Manor.h"
//#include "manor/TextPopup.h"
#include "data/Task.h"
#include "GlobalMacros.h"
#include "popUp/DoBatch.h"
#include "popUp/ShopRmbBatchPop.h"
#include "notice/Notice.h"
#include "popUp/Summary.h"
#include "sound/SoundManager.h"
#include "utils/LoadingAction.h"


USING_NS_CC;

#define PURCHASE_BATCH_Z 55

Shop::~Shop()
{
}


bool Shop::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //触摸隔离..
    addChild(BlackMask::create());
    
    //初始化view
    Size winSize = Director::getInstance()->getWinSize();
    view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("shop.csb");
    view->setContentSize(winSize);
    view->getChildByName<ui::Layout*>("panel")->setContentSize(winSize);
    
	addChild(view);
    ui::Helper::doLayout(view);
    
    //添加返回按钮的功能..
	auto backButton = view->getChildByName("panel")->getChildByName<ui::Layout*>("gobackBtnArea");
    backButton->setTouchEnabled(true);
    backButton->setSwallowTouches(false);
    backButton->addTouchEventListener(CC_CALLBACK_2(Shop::backButtonCallback, this));
	//底部遮罩..
	_bottomMask = view->getChildByName("panel")->getChildByName<ui::Layout*>("bottomMask");
	//列表容器..
	_list = view->getChildByName("panel")->getChildByName<ui::ListView*>("list");
	Item::setDelegate(this);
	//站位..
	ui::Layout* space = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("space");
	//模块模板(小)..
	_itemPattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("item");
	_itemPattern->retain();
	//模块模板(大)..
	_itemHugePattern = dynamic_cast<Node*>(_list)->getChildByName<ui::Layout*>("itemHuge");
	_itemHugePattern->retain();
	//清空元素
	_list->removeAllItems();
	_list->pushBackCustomItem(space);
    //设置天空大小,天空海阔你与我~~~~
    auto starSky = view->getChildByName("panel")->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(winSize.width/2,winSize.height*75/128));
    
    /**
     *  事件注册（list长短变化）
     */
	Notice* notice = Notice::getInstance();
	EventListenerCustom * foldListener = notice->addListener(Notice::FOLD, CC_CALLBACK_1(Shop::onBoardResize, this));
	addEventPool(foldListener, notice);
    
	data::Task* task = data::Task::getInstance();
	EventListenerCustom* taskListener = task->addListener(data::Task::TASK_COMPLETE, CC_CALLBACK_1(Shop::onTaskComplete, this));
	addEventPool(taskListener, task);
    data::Item* item = data::Item::getInstance();
    EventListenerCustom* itemListener = item->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(Shop::onItemChange, this));
    addEventPool(itemListener, item);
    
    
    return true;
}

void Shop::onEnter()
{
    Layer::onEnter();
    showBoard();
    //重置大小..
    listResize();
}

void Shop::onExit()
{
    Layer::onExit();
    removeChildByName("tips");
}

void Shop::showBoard()
{
	addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
	addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}
void Shop::openShop()
{
    isOpened = true;
}
void Shop::onBoardResize(EventCustom* e)
{
	listResize();
}
void Shop::listResize(){
	auto innerpos = _list->getInnerContainer()->getPosition();
	_list->setContentSize(Size(_list->getContentSize().width, _list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
	_list->getInnerContainer()->setPosition(innerpos);
	_bottomMask->setPosition(Vec2(_bottomMask->getPositionX(), _list->getBottomBoundary() + _bottomMask->getContentSize().height / 2));
}


void Shop::backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType touchType)
{
    auto btn = dynamic_cast<Node*>(sender);
    if (touchType == ui::Button::TouchEventType::BEGAN)
    {
        btn->setPositionX(btn->getPositionX() - BACKBTN_ACT_DISTANCE);
    }
    else if (touchType == ui::Button::TouchEventType::ENDED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
    else if (touchType == ui::Button::TouchEventType::CANCELED)
    {
        btn->setPositionX(btn->getPositionX() + BACKBTN_ACT_DISTANCE);
    }
}

void Shop::backReal()
{
    isOpened = false;
    removeChildByName("Textpop");
    if (_sid != data::Item::MANOR_SHOP_ID)
    {
        this->removeFromParent();
    }
    else
    {
        Manor::getInstance()->showLayer(Manor::kMainLayer);
    }
}

void Shop::removeSelf()
{
	runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(Shop::removeFromParent, this)), NULL));
}
void Shop::updateProperty()
{

}
void Shop::addPurchaseBatchPop(cocos2d::ValueMap itemData)
{
    auto pop = PurchaseBatchPop::create(itemData);
    this->addChild(pop, PURCHASE_BATCH_Z);
}

void Shop::addShopRmbBatchPop(cocos2d::ValueMap itemData)
{
    auto pop = ShopRmbBatchPop::create(itemData);
    this->addChild(pop, PURCHASE_BATCH_Z);
}

void Shop::showtext(std::string txt, cocos2d::Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(txt,node);
        this->addChild(pop, 52, "Textpop");
    }
    
}

void Shop::onTaskComplete(EventCustom* e)
{
    if (true)
    {
        updateList(_sid);
    }
}
void Shop::onItemChange(cocos2d::EventCustom* e)
{
    if(isOpened)
    {
        updateList(_sid);
    }
}

void Shop::updateList(int sid)
{
    if(!isUpdateList)
    {
        isUpdateList = true;
        
        _sid = sid;
        //野外商店不出现长按提示..
        if(_sid == 0 || _sid == data::Item::MANOR_SHOP_ID)
        {
            view->getChildByName("panel")->getChildByName("tips")->setVisible(true);
        }
        else
        {
            view->getChildByName("panel")->getChildByName("tips")->setVisible(false);
        }
        Value all = data::Item::getInstance()->getShopByFID(_sid);
        if(preShopListData != all|| preShopListData.asValueVector().size() != all.asValueVector().size())
        {
            preShopListData = all;
            for (Value row : all.asValueVector())
            {
                Item* item = getItemByID(row.asValueMap()["iid"].asInt());
                item->setData(row);
                
                if (item->getParent()){
                    
                }
                else{
                    _list->pushBackCustomItem(item);
                }
            }
            auto items = _list->getItems();
            for(int i = items.size()-1 ; i > 0;i--)
            {
                ((Item*)items.at(i))->setUpdated(false);
            }
        }
        isUpdateList = false;
    }
	

}
void Shop::freshList()
{
    if(!isUpdateList)
    {
        isUpdateList = true;
        
        //野外商店不出现长按提示..
        if(_sid == 0 || _sid == data::Item::MANOR_SHOP_ID)
        {
            view->getChildByName("panel")->getChildByName("tips")->setVisible(true);
        }
        else
        {
            view->getChildByName("panel")->getChildByName("tips")->setVisible(false);
        }
        Value all = data::Item::getInstance()->getShopByFID(_sid);
        for (Value row : all.asValueVector())
        {
            Item* item = getItemByID(row.asValueMap()["iid"].asInt());
            item->setData(row);
            
            if (item->getParent()){
                
            }
            else{
                _list->pushBackCustomItem(item);
            }
        }
        //如果没有更新就删除掉..
        auto items = _list->getItems();
        for(int i = items.size()-1 ; i > 0;i--)
        {
            if(!((Item*)items.at(i))->isUpdated())
            {
                _list->removeItem(i);
            }
            else
            {
                ((Item*)items.at(i))->setUpdated(false);
            }
        }
        
        isUpdateList = false;
    }
}
void Shop::updateList(){
	if (_sid == 0){
		updateList(data::Item::MANOR_SHOP_ID);
	}
	else{
		updateList(_sid);
	}
}
Shop::Item* Shop::getItemByID(int id)
{
	void* p = getMem(id);
	if (p == nullptr)
	{
		p = Item::create();
		setMem(id, p);
	}
	return (Shop::Item*)p;
}
Node* Shop::getPattern(bool huge){
	cocos2d::ui::Layout* patten = huge ? _itemHugePattern : _itemPattern;
	return dynamic_cast<Node*>(patten->clone());
}
bool Shop::isManor(){
	return _sid == data::Item::MANOR_SHOP_ID;
}
/********************listitem类******************************/

#define BTN_ACT 11
#define INFO_BTN_ACT 12

Shop* Shop::Item::_delegate = nullptr;

Shop::Item::~Item()
{
    //data::Item::getInstance()->removeListener(_listenerItem);
}



bool Shop::Item::init()
{
    if (!Layout::init())
    {
        return false;
    }
	//_listenerItem = data::Item::getInstance()->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(Item::onItemChange, this));
    return true;
}
void Shop::Item::setData(Value data){
	_data = data;
	ValueMap map = data.asValueMap();
	if (_view) removeChild(_view);
	//通过模版初始化显示..
	if (map["name"].asString().size() <= 12)
	{
        _view = _delegate->getPattern();
        _itemBar = _view->getChildByName<ui::LoadingBar*>("itemBar");
        _itemBar->setPercent(0);
	}
	else
	{
		_view = _delegate->getPattern(true);
        _itemBar = _view->getChildByName<ui::LoadingBar*>("hugeItemBar");
        _itemBar->setPercent(0);
        
	}
	//rmb购买
	ui::Button* rmbBtn = _view->getChildByName<ui::Button*>("rmbBtn");
    if (map["rmb"].asFloat() <= 0.0f || !data::User::getInstance()->getRMBShow())
	{
		rmbBtn->setTouchEnabled(false);
		rmbBtn->setVisible(false);
        rmbBtn->getChildByName<ui::Layout*>("btnArea")->setTouchEnabled(false);
	}
    else
    {
        rmbBtn->setTouchEnabled(true);
        rmbBtn->setVisible(true);
        rmbBtn->getChildByName<ui::Layout*>("btnArea")->setTouchEnabled(true);
    }
	
	rmbBtn->getChildByName<ui::Layout*>("btnArea")->addTouchEventListener(CC_CALLBACK_2(Item::rmbBtnCallback, this));

	_view->getChildByName("nameback")->setOpacity(140);

	_view->setPosition(Point::ZERO);
	this->setContentSize(_view->getContentSize());
	this->addChild(_view);

	_iid = map["iid"].asInt();

	//显示商品名字和价格..
	ui::Text* name = _view->getChildByName<ui::Text*>("name");
	name->setString(map["name"].asString());
	_view->getChildByName<ui::Text*>("price")->setString("金币 x" + map["price"].asString());

	/**
	* 红点显示
	*/
	ui::ImageView* newdot = _view->getChildByName<ui::ImageView*>("newdot");
    int itemNum = data::Item::getInstance()->getUserItemByIID(_iid).asValueMap()["amount"].asInt();
    if( itemNum <= 0 && _delegate->isManor())
    {
        newdot->setVisible(true);
    }
    else
    {
        newdot->setVisible(false);
    }


	//刷新价格颜色..
	if (map["shortage"].asBool())
	{
		_view->getChildByName<ui::Text*>("price")->setTextColor(DeerUtils::ShotageColor);
	}
	else
	{
		_view->getChildByName<ui::Text*>("price")->setTextColor(DeerUtils::EnoughColor);
	}

	//显示星级..
	for (int i = 1; i <= 5; i++){
		ui::ImageView* star = _view->getChildByName<ui::ImageView*>("star" + Value(i).asString());
		if (map["level"].asInt() >= i){
			star->setVisible(true);
		}
		else{
			star->setVisible(false);
		}
	}

	/**
	*  rmb购买状态
	*/
	if (map["rmb"].asFloat() <= 0.0f)
	{
		rmbBtn->setTouchEnabled(false);
		rmbBtn->setVisible(false);
	}


	_upDated = true;

	//注册商品按钮回调..
	_view->getChildByName<ui::Layout*>("namebackArea")->addTouchEventListener(CC_CALLBACK_2(Item::buyButtonCallback, this));

	//查看商品信息..
	_view->getChildByName("infoBtn")->getChildByName<ui::Layout*>("infoBtnArea")->addTouchEventListener(CC_CALLBACK_2(Item::infoBtnCallback, this));
}

void Shop::Item::onItemChange(cocos2d::EventCustom *e)
{
   // auto item = (Value*)e->getUserData();
    //int iid = item->asValueMap()["iid"].asInt();
    
    //if (iid == data::Item::COIN)
    //{
        //if (_data.asValueMap()["price"].asInt() > item->asValueMap()["total"].asInt()) {
			_delegate->updateList();
        //}
        
    //}
}


void Shop::Item::buyButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType event)
{
    /**
     *  声音部分
     */
    if (event == ui::Widget::TouchEventType::ENDED || event == ui::Widget::TouchEventType::CANCELED)
    {
		SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
    }
    
    if (event == Widget::TouchEventType::BEGAN)
    {
        
		if (_data.asValueMap()["space"].asInt() != -1 && _delegate->isManor()){
			_isLongPress = false;

			auto delay = DelayTime::create(0.3);
			CallFunc *call = CallFunc::create([=]()
			{
				_isLongPress = true;

				/**
				*  去掉新的状态
				*/
				_view->getChildByName("nameback")->setOpacity(140);
				//_view->getChildByName<ui::ImageView*>("newdot")->setVisible(false);
				/**
				*  弹出批量处理弹窗
				*/
				_delegate->addPurchaseBatchPop(_data.asValueMap());
			});
			auto seq = Sequence::create(delay, call, NULL);
			seq->setTag(LongPressActionTag);
			this->runAction(seq);
            auto load = LoadingAction::create(0.3);
            _itemBar->runAction(Sequence::create(load, CallFunc::create([=]()
                                                                           {
                                                                               _itemBar->setPercent(0);
                                                                           }), NULL));
		}

    }
    if (event == Widget::TouchEventType::MOVED)
    {
        auto btn = dynamic_cast<ui::Widget*>(sender);

        if ((btn->getTouchMovePosition() - btn->getTouchBeganPosition()).length() > 20)
        {
            /**
             *  停止长按回调
             */
            this->stopActionByTag(LongPressActionTag);
            _itemBar->stopAllActions();
            _itemBar->setPercent(0);
            /**
             *  向父容器扩散触摸时间
             */
            btn->setPropagateTouchEvents(true);
        }
    }
    if (event == Widget::TouchEventType::ENDED)
    {
        stopAllActionsByTag(LongPressActionTag);
        _itemBar->stopAllActions();
        _itemBar->setPercent(0);
        dynamic_cast<ui::Widget*>(sender)->setPropagateTouchEvents(false);
        
        if (_isLongPress)
        {
            _isLongPress = false;
            return;
        }
        buyAction();
    }
    else if (event == Widget::TouchEventType::CANCELED)
    {
        this->stopActionByTag(LongPressActionTag);
        _itemBar->stopAllActions();
        _itemBar->setPercent(0);
        dynamic_cast<ui::Widget*>(sender)->setPropagateTouchEvents(false);
        
        /**
         *  这段内容当批量界面已经弹出的时候调用的，目的是不再执行单个购买操作
         */
        if (_isLongPress)
        {
            _isLongPress = false;
            return;
        }
        buyAction();
    }
	//BTN_CALL_ACT(event, _view->getChildByName("nameback"), sender, CC_CALLBACK_0(Item::buyAction, this))
}

void Shop::Item::buyAction()
{
	//_view->getChildByName<ui::ImageView*>("newdot")->setVisible(false);
    
    Value ret;
    
    auto fdata = data::Crusade::getInstance()->fieldView();
    auto crusdata = data::Crusade::getInstance()->getUserCrusade();
    if (crusdata.asValueMap()["depart"].asBool() == true)
    {
        ret = data::Item::getInstance()->buy(fdata.asValueMap()["fid"].asInt(), _data.asValueMap()["iid"].asInt(), 1);
    }
    else
    {
		ret = data::Item::getInstance()->buy(_data.asValueMap()["iid"].asInt());
    }
    
    if (!ret.isNull())
    {
		std::string str = "你购买了: " + _data.asValueMap()["name"].asString() + "X" + "1";
		Notice::getInstance()->postBoard(str);
		Notice::getInstance()->postTips(str);
        _delegate->freshList();
    }
    else
    {
		Notice::getInstance()->postTips("金币不足  ", Notice::TipsType::kWarning);
    }
}

void Shop::Item::infoBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType event)
{
    auto infolight = _view->getChildByName("infoBtn")->getChildByName<ui::ImageView*>("infolight");
    if (event == ui::Widget::TouchEventType::BEGAN)
    {
        infolight->stopActionByTag(INFO_BTN_ACT);
        infolight->setOpacity(255);
        infolight->setVisible(true);
    }
    if (event == ui::Widget::TouchEventType::MOVED)
    {
//        auto btn = dynamic_cast<ui::Widget*>(sender);
        
//        if ((btn->getTouchMovePosition() - btn->getTouchBeganPosition()).length()>20) {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(sender);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de > 20)
        {
        
            infolight->setVisible(false);
        }
    }
    if (event == Widget::TouchEventType::ENDED || event == Widget::TouchEventType::CANCELED)
    {
        
//            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
            auto fade = FadeOut::create(0.3);
            fade->setTag(INFO_BTN_ACT);
            infolight->runAction(fade);
            Item::showinfo();
        
    }
    if (event == Widget::TouchEventType::CANCELED)
    {
        auto btn = dynamic_cast<cocos2d::ui::Widget*>(sender);
        float de = btn->getTouchBeganPosition().distance(btn->getTouchMovePosition());
        if (de < 20)
        {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
            SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
            auto fade = FadeOut::create(0.3);
            fade->setTag(INFO_BTN_ACT);
            infolight->runAction(fade);
            //runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create(CC_CALLBACK_0(Item::showinfo, this)), NULL));
        }
    }

}
void Shop::Item::rmbBtnCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event)
{

    if (event == Widget::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        _delegate->addShopRmbBatchPop(_data.asValueMap());
    }
}
void Shop::Item::showinfo()
{
	auto area = _view->getChildByName<ui::ImageView*>("nameback");
    auto rect = area->getBoundingBox();
    rect.origin = area->getWorldPosition();
    _delegate->showtext(_data.asValueMap()["summary"].asString(), area);
}

/****************************************批量购买弹窗************************************************/


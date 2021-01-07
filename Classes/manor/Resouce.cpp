/****************************************************************************
  
 *	@desc	资源界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/ResouceLayer.cpp
 *  @modify	null
 ******************************************************************************/

#include "Resouce.h"
#include "utils/DeerUtils.h"
#include "data/Item.h"
#include "data/Lang.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "data/Guid.h"
#include "popUp/Summary.h"
#include "GlobalMacros.h"
#include "utils/LoadingAction.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;
#define TO_S(_var_) #_var_
#define POP_Z 70
#define LONGPRESSLAYER "longpresslayer"

Resouce::Resouce()
:_isOpened(false)
{
    
}

Resouce::~Resouce()
{
    //data::Item::getInstance()->removeListener(_limap.at(TO_S(itemChangeEvent)));
    LOGTYPENAME
}
void Resouce::removeSelf()
{
    
}
void Resouce::updateProperty()
{

}
bool Resouce::init()
{
    if (!ui::Layout::init())
    {
        return false;
    }
    /**
     *  新手引导
     */
    _guide = Guide::create(data::Guid::RESOUCE_LAYER);
    this->addChild(_guide,10000);
    //_guide->addFinishGuid();
    Size winSize = Director::getInstance()->getWinSize();
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("cangku.csb");
    this->setContentSize(winSize);
    this->addChild(view);
    
    view->setPosition(Vec2(winSize.width/2, 0));
    ui::Helper::doLayout(view);
    
    auto back = seekChildByNameWithRetType<ui::ImageView*>(view, "back");
    auto backSize = back->getContentSize();
    back->setContentSize(Size(backSize.width, (winSize.height - 520 * 1.33)/1.33));
    
    auto arr = seekChildByNameWithRetType<Sprite*>(view, "tiparrow");
    arr->setAnchorPoint(Vec2(0.5, 0.4));
    arr->setNormalizedPosition(Vec2(0.5, 1));
    
    auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
    auto scrollSize = scroll->getContentSize();
    scroll->setContentSize(Size(scrollSize.width, (winSize.height - 560 * 1.33)/1.33));
    
    _animation = CSLoader::getInstance()->createTimeline("cangku.csb");
    view->runAction(_animation);
    
    //储存item模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(this, "item"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(this, "widthitem"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(this, "textItem"));
    _itemModel.at(0)->retain();
    _itemModel.at(1)->retain();
    _itemModel.at(2)->retain();
    
    scroll->removeAllChildren();
    updateitems();
    
    //EventListenerCustom* itemChangeEvent = data::Item::getInstance()->addListener(data::Item::USER_ITEM_CHANGE_LIST, CC_CALLBACK_1(Resouce::onChange, this));
    //addEventPool(itemChangeEvent, data::Item::getInstance());
    
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t, Event* e)->bool
    {
        if (isVisible())
        {
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [=](Touch * t, Event *e)
    {
        //更新新手引导状态..
        data::Guid::getInstance()->updateGuidInfo(data::Guid::WAREHOUSE_EMPTY);
        
        Manor::getInstance()->shutLayer(Manor::kWarehouse);
    };
    listener->setEnabled(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _guide->setTouchListener(listener);
    _guide->setCangkuBack(back);
    
    return true;
}
void Resouce::onChange(cocos2d::EventCustom *)
{
    if (data::Guid::getInstance()->getCurrentGuidId() != data::Guid::WAREHOUSE_USE2) {
        //引导卖出过程中暂时中断刷新..
        updateitems();
    }
}
Resouce::ResouceItem* Resouce::getItemByID(int resourceId)
{
    void* p = getMem(resourceId);
    if (p == nullptr)
    {
        p = ResouceItem::create(this);
        setMem(resourceId, p);
    }
    return (Resouce::ResouceItem*)p;
}
void Resouce::updateitems()
{
    if(!_isUpadteList)
    {
        _isUpadteList = true;
        bool textClose = data::User::getInstance()->isGuidTextClose();
        auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
        
        auto allItemsData = data::Item::getInstance()->getResouces().asValueVector();
        if(preReouceData != Value(allItemsData)||preReouceData.asValueVector().size() != allItemsData.size())
        {
            preReouceData = Value(allItemsData);
            _guide->clearButton();
            
            
            vov.clear();
            vov.resize(2);
            
            int itemDex = 1;
            for (auto itemData : allItemsData)
            {
                auto item = getItemByID(itemData.asValueMap()["iid"].asInt());
                item->setData(itemData);
                if(itemDex%2 == 1)
                {
                    vov.at(0).pushBack(item);
                }
                else{
                    vov.at(1).pushBack(item);
                }
                itemDex++;
            }
            //计算需要的滚动条高度..
            float h1 = 0;
            for (auto &i : vov.at(0))
            {
                h1 += i->getContentSize().height;
            }
            
            float h2 = 0;
            for (auto &i : vov.at(1))
            {
                h2 += i->getContentSize().height;
            }
            if(!textClose)
            {
                h1 = h1 + _itemModel.at(2)->getContentSize().height;
                h2 = h2+ _itemModel.at(2)->getContentSize().height;
            }
            
            
            scroll->setInnerContainerSize(Size(scroll->getInnerContainerSize().width, MAX(h1, h2)));
            
            auto starth(scroll->getInnerContainerSize().height);
            int index = data::Guid::RESOURCE_BUTTON_FIRST;
            _canSaleFirstIndex = -1;
            
            addGuideText();
            
            if(!textClose)
            {
                starth = scroll->getInnerContainerSize().height-_itemModel.at(2)->getContentSize().height;
            }
            else
            {
                starth = scroll->getInnerContainerSize().height;
            }
            int zorderMax = 1000;
            for (auto &i : vov.at(0))
            {
                i->setAnchorPoint(Vec2(0.5, 1));
                i->setPosition(Vec2(150, starth));
                i->setZOrder(zorderMax--);
                if(i->getParent())
                {
                    
                }
                else
                {
                    scroll->addChild(i);
                    scroll->setPassFocusToChild(true);
                }
                starth -= i->getContentSize().height;
                if(((ResouceItem*)i)->_data["price"].asInt()>0 && _canSaleFirstIndex == -1)
                {
                    _canSaleFirstIndex = index;
                    _guide->setCangkuCanSaleIndex(index);
                }
                _guide->insertButton(index, i->getChildByName<ui::Layout*>("item"));
                index++;
            }
            if(!textClose)
            {
                starth = scroll->getInnerContainerSize().height-_itemModel.at(2)->getContentSize().height;
            }
            else
            {
                starth = scroll->getInnerContainerSize().height;
            }
            
            for (auto &i : vov.at(1))
            {
                i->setAnchorPoint(Vec2(0.5, 1));
                i->setPosition(Vec2(400, starth));
                i->setZOrder(zorderMax--);
                if(i->getParent())
                {
                    
                }
                else
                {
                    scroll->addChild(i);
                    scroll->setPassFocusToChild(true);
                }
                
                starth -= i->getContentSize().height;
                if(((ResouceItem*)i)->_data["price"].asInt()>0 && _canSaleFirstIndex == -1)
                {
                    _canSaleFirstIndex = index;
                    _guide->setCangkuCanSaleIndex(index);
                }
                _guide->insertButton(index, i->getChildByName<ui::Layout*>("item"));
                index++;
            }
            removeUnusedItems();
        }
        _isUpadteList = false;
    }
}
void Resouce::removeUnusedItems()
{
    bool textClose = data::User::getInstance()->isGuidTextClose();
    auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
    Vector<Node*> scrollChildren = scroll->getChildren();
    
    //删除引导卖出文本..
    if(textClose && _textShow != textClose)
    {
        _textShow = textClose;
        if(scrollChildren.size() >= 1)
        {
            scroll->removeChild(scrollChildren.at(0));
            scrollChildren = scroll->getChildren();
        }
    }
    int beginIndex;
    if(textClose)
    {
        beginIndex = 1;
    }
    else
    {
        beginIndex = 0;
    }
    
    for(auto iter = scrollChildren.end()-1; iter != scrollChildren.begin()-beginIndex; --iter)
    {
            auto iid = dynamic_cast<ResouceItem*>(*iter)->getResourceId();
            if(isCanBeRemove(iid))
            {
                scroll->removeChild(*iter);
                setMem(iid, nullptr);
            }
    }
}
bool Resouce::isCanBeRemove(int resourceId)
{
    auto allItemsData = data::Item::getInstance()->getResouces().asValueVector();
    for(int i = 0; i < allItemsData.size();i++)
    {
        if(allItemsData[i].asValueMap()["iid"].asInt() == resourceId)
        {
            return false;
        }
    }
    return true;
}
void Resouce::addGuideText()
{
    bool textClose = data::User::getInstance()->isGuidTextClose();
    auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
    auto starth(scroll->getInnerContainerSize().height);
    //如果未曾删除过就添加textItem..
    if(!textClose&&_textShow != textClose)
    {
        _textShow = textClose;
        auto textItem = _itemModel.at(2)->clone();
        textItem->setAnchorPoint(Vec2(0.5,1));
        textItem->setPosition(Vec2(272.5,starth));
        scroll->addChild(textItem);
        starth -= textItem->getContentSize().height;
    }
    if(!textClose)
    {
        if(data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::WAREHOUSE_USE2))
        {
            auto closeBtnArea = seekChildByNameWithRetType<ui::Button*>(this, "closeBtn")->getChildByName<ui::Layout*>("closeBtnArea");
            closeBtnArea->addTouchEventListener(CC_CALLBACK_2(Resouce::textCloseCallback, this));
            auto closeBtn = seekChildByNameWithRetType<ui::Button*>(this, "closeBtn");
            closeBtn->setVisible(true);
        }
        else
        {
            auto closeBtn = seekChildByNameWithRetType<ui::Button*>(this, "closeBtn");
            closeBtn->setVisible(false);
        }
    }
    
}
void Resouce::textCloseCallback(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType t)
{
    data::User::getInstance()->setGuidTextClose(true);
    updateitems();
}
void Resouce::open(int type)
{
    updateitems();
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo(data::Guid::MAIN_WAREHOUSE);
    if (type == -1)
    {
        seekChildByNameWithRetType<Sprite*>(this, "tiparrow")->setNormalizedPosition(Vec2(0.2, 1));
    }else if (type == 1)
    {
        seekChildByNameWithRetType<Sprite*>(this, "tiparrow")->setNormalizedPosition(Vec2(0.8, 1));
    }
    _animation->play("openanimation", false);
    auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
    auto children = scroll->getChildren();
    for (auto i: children)
    {
        i->runAction(FadeIn::create(0.15));
    }
    //如果有出现第一个可以卖出的商品 show guide..
    if(_canSaleFirstIndex != -1)
    {
        if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::WAREHOUSE_USE2))
        {
            data::Guid::getInstance()->showGuid(data::Guid::WAREHOUSE_USE2);
        }
    }
    
    _isOpened = true;
    CCLOG("setopened true");
}

void Resouce::shut()
{
    _animation->play("shutanimation", false);
    auto scroll = seekChildByNameWithRetType<ui::ScrollView*>(this, "scroll");
    auto children = scroll->getChildren();
    for (auto i: children)
    {
        i->runAction(FadeOut::create(0.15));
    }
    runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([this](){
        this->setVisible(false);
        this->_isOpened = false;
    }), NULL));
}

void Resouce::showSell(cocos2d::ValueMap idata)
{
    this->getParent()->addChild(SellBatchPop::create(idata), POP_Z, LONGPRESSLAYER);
}

void Resouce::showSellDeny(cocos2d::ValueMap idata)
{
    this->getParent()->addChild(SellDeny::create(idata), POP_Z, LONGPRESSLAYER);
}

void Resouce::showInfo(std::string txt, cocos2d::Node* node,float nodeScale)
{
    if (this->getChildByName(LONGPRESSLAYER))
    {
        this->removeChildByName(LONGPRESSLAYER);
    }
    else
    {
        auto pop = Summary::create(txt,node,nodeScale);
        this->addChild(pop, 100,LONGPRESSLAYER);
    }
}

/*********************resouceItem********************************/

#define ITME_BTN_ACT 11

#define LONG_TOUCH_TIME 0.3
#define LONG_TOCUH_CHECK 21
Resouce::ResouceItem::~ResouceItem()
{
    //data::Item::getInstance()->removeListener(_listenerMap.at(TO_S(itemChangeEvent)));
}

Resouce::ResouceItem* Resouce::ResouceItem::create(Resouce* delegate)
{
    ResouceItem* ret = new ResouceItem();
    ret->_delegate = delegate;
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

bool Resouce::ResouceItem::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    
    return true;
}
int Resouce::ResouceItem::getResourceId()
{
    return _resouceIID;
}
void Resouce::ResouceItem::setData(Value data)
{
    _longtouched = false;
    if(_data != data.asValueMap())
    {
        _data = data.asValueMap();
        
        //是否需要重新添加节点..
        if(_resouceIID != data.asValueMap()["iid"].asInt())
        {
            _resouceIID = data.asValueMap()["iid"].asInt();
            if(_view)
            {
                _view->removeFromParent();
            }
            
            if (_data["name"].asString().size() > 12)
            {
                _view = (Layout*)_delegate->_itemModel.at(1)->clone();
                _loadingBar = _view->getChildByName<ui::LoadingBar*>("widthBar");
            }
            else
            {
                _view = (Layout*)_delegate->_itemModel.at(0)->clone();
                _loadingBar = _view->getChildByName<ui::LoadingBar*>("Bar");
            }
            this->addChild(_view);
        }
        
        _loadingBar->setVisible(false);
        _view->setName("item");
        _view->setTouchEnabled(true);
        this->setContentSize(_view->getContentSize());
        _view->setPosition(Point::ZERO);
        
        ui::Text* resouceName = _view->getChildByName<ui::Text*>("name");
        resouceName->setString(_data.at("name").asString());
        
        ui::Text* resNum = _view->getChildByName<ui::Text*>("price");
        resNum->setString(Value(_data.at("amount")).asString());
        _view->setTouchEnabled(true);
        _view->addTouchEventListener(CC_CALLBACK_2(ResouceItem::touchCallback, this));
    }
    

}
void Resouce::ResouceItem::removeSelf()
{

}
void Resouce::ResouceItem::updateProperty()
{

}
void Resouce::ResouceItem::touchCallback(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType t)
{
    if (t == ui::Layout::TouchEventType::BEGAN)
    {
        _longtouched = false;
        auto longact = Sequence::create(DelayTime::create(LONG_TOUCH_TIME), CallFunc::create([this](){this->_longtouched = true;}), CallFunc::create(CC_CALLBACK_0(ResouceItem::longTouchCheck, this)), nullptr);
        longact->setTag(11);
        runAction(longact);
        
        _loadingBar->setVisible(true);
        auto load = LoadingAction::create(LONG_TOUCH_TIME);
        _loadingBar->runAction(Sequence::create(load, CallFunc::create([=]()
                                                                     {
                                                                         _loadingBar->setVisible(false);
                                                                     }), NULL));
    }
    
    if (t == ui::Layout::TouchEventType::MOVED)
    {
        ui::Widget* self = (ui::Widget*)sender;
        if ( self->getTouchMovePosition().distance(self->getTouchBeganPosition()) > 20)
        {
            stopAllActionsByTag(11);
            _longtouched = false;
            _loadingBar->setVisible(false);
            _loadingBar->stopAllActions();
        }
    }
    else if (t == ui::Layout::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        
        if (!_longtouched)
        {
            actCall = CC_CALLBACK_0(ResouceItem::touchReal,this);
        }
        stopAllActionsByTag(11);
        _longtouched = false;
        _loadingBar->setVisible(false);
        _loadingBar->stopAllActions();
        if(_delegate->getChildByName(LONGPRESSLAYER))
        {
            actCall = CC_CALLBACK_0(ResouceItem::touchReal,this);
        }
    }
    else if (t == ui::Layout::TouchEventType::CANCELED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        stopAllActionsByTag(11);
        _longtouched = false;
        _loadingBar->setVisible(false);
        _loadingBar->stopAllActions();
    }
    
    BTN_CALL_ACT(t, sender, sender, actCall)
}

void Resouce::ResouceItem::touchReal()
{
    if (_delegate)
    {
        //更新新手引导..
        if(data::Guid::getInstance()->getCurrentGuidId() != data::Guid::WAREHOUSE_USE2)
        {
            data::Guid::getInstance()->updateGuidInfo(data::Guid::WAREHOUSE_SOURCE);
            _delegate->showInfo(_data["summary"].asString(), _view,1.333);
        }
    }
}

void Resouce::ResouceItem::longTouchCheck()
{

    if(data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::WAREHOUSE_USE1)|| !DEBUG_SHOW_GUID)
    {
        if (_data["price"].asInt() > 0)
        {
            if (_delegate)
            {
                if(data::Guid::getInstance()->getCurrentGuidId() == data::Guid::WAREHOUSE_USE2)
                {
                    data::Guid::getInstance()->updateGuidInfo(data::Guid::WAREHOUSE_SOURCE);
                }
                _delegate->showSell(_data);
            }
        }
        else
        {
            if (_delegate)
            {
                _delegate->showSellDeny(_data);
            }
        }
    }
}

void Resouce::ResouceItem::onResouseChange(cocos2d::EventCustom *event)
{
    auto itemCount = data::Item::getInstance()->getUserItemAmountByIID(_resouceIID);
    _view->getChildByName<ui::Text*>("price")->setString(Value(itemCount).asString());
}

void Resouce::ResouceItem::setOpacity(GLubyte opacity)
{
    Layout::setOpacity(opacity);
    _view->setOpacity(opacity);
    auto children = _view->getChildren();
    for (auto i : children)
    {
        i->setOpacity(opacity);
    }
}


/************************sellBatchPop**************************/

SellDeny* SellDeny::create(cocos2d::ValueMap idata)
{
    auto ret = new SellDeny();
    if (ret && ret->init(idata))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return ret;
    }
}

bool SellDeny::init(cocos2d::ValueMap idata)
{
    if (!Layout::init())
    {
        return false;
    }
    _data = idata;
    auto win = Director::getInstance()->getWinSize();
    
    addChild(BlackMask::create());
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("selldeny.csb");
    auto viewRoot = seekChildByNameWithRetType<ui::Layout*>(view, "Panel");
    addChild(viewRoot);
    setContentSize(win);
    ui::Helper::doLayout(this);
    
    char word[200];
    sprintf(word, data::Lang::getInstance()->getWord(130311).c_str(),_data["name"].asString().c_str());
    seekChildByNameWithRetType<ui::Text*>(view, "name")->setString(word);
    
    seekChildByNameWithRetType<ui::Layout*>(view, "sureBtnArea")->addTouchEventListener(CC_CALLBACK_2(SellDeny::sureButtonCallback, this));
    
    return true;
}

void SellDeny::sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureBtnCall();
    }
}

void SellDeny::sureBtnCall()
{
    this->removeFromParent();
}

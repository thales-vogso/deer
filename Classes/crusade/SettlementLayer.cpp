/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	cursad/BattleLayer.h
 *   @modify	null
 ******************************************************************************/

#include "SettlementLayer.h"
#include "utils/DeerUtils.h"
#include "data/Monster.h"
#include "data/Crusade.h"
#include "data/Lang.h"
#include "popUp/BlackMask.h"
#include "battle/BattleLayer.h"
#include "notice/Notice.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"
#include "guide/Guide.h"

USING_NS_CC;
USING_NS_DU;

std::string const SettlementLayer::EXIT_FROM_SETTLEMENT("ExitFromSettlement");

SettlementLayer::SettlementLayer()
:_delegate(nullptr)
{
    
}

SettlementLayer::~SettlementLayer()
{
    this->removeListener(SettlementLayer::EXIT_FROM_SETTLEMENT);
    
    //当战利品界面结束推出是需要清空掉落数据..
    data::Crusade::getInstance()->dropsClear();
}



SettlementLayer *SettlementLayer::create(cocos2d::ValueVector drops)
{
    SettlementLayer *ret = new SettlementLayer();
    if (ret && ret->init(drops))
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

bool SettlementLayer::init(cocos2d::ValueVector drops)
{
    if (!Layout::init())
    {
        return false;
    }
    this->addChild(BlackMask::create());
    SoundManager::getInstance()->stopMusic();
    auto winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("spoils.csb");
    view->setContentSize(winSize);
    seekChildByNameWithRetType<ui::Layout*>(view, "Panel_1")->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //item模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "dropItem"));
    
    this->updatePackCapacity();
    
    auto packlist = seekChildByNameWithRetType<ui::ListView*>(view, "packagelist");
    auto targetSize = packlist->getContentSize();
    targetSize.height = targetSize.height + winSize.height - deerResouseSize.height;
    packlist->setContentSize(targetSize);
    auto listBack = seekChildByNameWithRetType<ui::ImageView*>(view, "listBack");
    listBack->setContentSize(Size(listBack->getContentSize().width,targetSize.height+18));
    
    auto droplist = seekChildByNameWithRetType<ui::ListView*>(view, "droplist");
    droplist->setContentSize(targetSize);
    
    seekChildByNameWithRetType<ui::Layout*>(view, "shutdownArea")->addTouchEventListener(CC_CALLBACK_2(SettlementLayer::shutDownCallback, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "getallArea")->addTouchEventListener(CC_CALLBACK_2(SettlementLayer::getAllCallback, this));
    
    //初始化临时的package和Drop数据
    auto packeagedata = data::Crusade::getInstance()->getPackage(true).asValueVector();
    for(auto &i : packeagedata)
    {
        i.asValueMap()["amount"] = i.asValueMap()["current"];
    }
    _spiolsData["package"] = packeagedata;
    _spiolsData["drop"] = drops;
    
    this->upDateItems();
    
	this->addChild(Notice::getInstance()->getTips());
    
    /**
     *  新手引导
     */
    Guide* guide = Guide::create(data::Guid::DROP_LAYER);
    guide->insertButton(data::Guid::DROP_LAYER_BACK,  seekChildByNameWithRetType<ui::Layout*>(view, "shutdownArea"));
    guide->insertButton(data::Guid::DROP_LAYER_GETAAL, seekChildByNameWithRetType<ui::Layout*>(view, "getallArea"));
    this->addChild(guide);
    
    //需要手动显示新手引导..
    data::Guid::getInstance()->showGuid(data::Guid::DROP_GET);
    
    return true;
}

SettlementLayer * SettlementLayer::create(int mid)
{
    SettlementLayer *ret = new SettlementLayer();
    if (ret && ret->init(mid))
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

bool SettlementLayer::init(int mid)
{
    if (!Layout::init())
    {
        return false;
    }
    _mid = mid;
    
    //添加遮罩..
    this->addChild(BlackMask::create());
    SoundManager::getInstance()->stopMusic();
    auto winSize = deerResouseSize;
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("spoils.csb");
    view->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //item模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "dropItem"));

    this->updatePackCapacity();
    
    seekChildByNameWithRetType<ui::Layout*>(view, "shutdownArea")->addTouchEventListener(CC_CALLBACK_2(SettlementLayer::shutDownCallback, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "getallArea")->addTouchEventListener(CC_CALLBACK_2(SettlementLayer::getAllCallback, this));
    
#if DG_SHOW_BTN_AREA == 1
    SET_LAYOUT_SHOW(seekChildByNameWithRetType<ui::Layout*>(view, "shutdownArea"))
    SET_LAYOUT_SHOW(seekChildByNameWithRetType<ui::Layout*>(view, "getallArea"))
#endif
    
    //初始化临时的package和Drop数据
    _spiolsData["package"] = data::Crusade::getInstance()->getPackage().asValueVector();
    _spiolsData["drop"] = data::Monster::getInstance()->getMonsterDropItems(_mid).asValueVector();
    
    this->upDateItems();
    
	this->addChild(Notice::getInstance()->getTips());
    
    /**
     *  新手引导
     */
    Guide* guide = Guide::create(data::Guid::DROP_LAYER);
    guide->insertButton(data::Guid::DROP_LAYER_BACK,  seekChildByNameWithRetType<ui::Layout*>(view, "shutdownArea"));
    guide->insertButton(data::Guid::DROP_LAYER_GETAAL, seekChildByNameWithRetType<ui::Layout*>(view, "getallArea"));
    this->addChild(guide);
    
    
    //需要手动显示新手引导..
    data::Guid::getInstance()->showGuid(data::Guid::DROP_GET);
    
    return true;
}

void SettlementLayer::updatePackCapacity()
{
    //背包容量显示..
    auto capacityTxt = seekChildByNameWithRetType<ui::Text*>(this, "capacity");
    auto crusadInstace = data::Crusade::getInstance();
    std::string str = capacityTxt->getString();
    std::string coo = "0123456789/";
    auto pos = std::find_first_of(str.begin(), str.end(), coo.begin(), coo.end());
    str = str.substr(0, pos-str.begin());
    data::Crusade::getInstance()->getPackageSpace();
    
    capacityTxt->setString(str + Value(crusadInstace->getPackageSpace()).asString() + "/" + Value(crusadInstace->getPackageTotal()).asString());
}

void SettlementLayer::shutDownCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType eventType)
{
    if (eventType == ui::Text::TouchEventType::ENDED)
    {
        shutDown();
    }
}

void SettlementLayer::shutDown()
{
    auto dropList = seekChildByNameWithRetType<ui::ListView*>(this, "droplist");
    for (auto &i : dropList->getItems())
    {
        auto itemdata = dynamic_cast<ListItem*>(i)->getData();
        int iid = itemdata["iid"].asInt();
        //重要道具没有拾取..
		if(itemdata["space"].asInt() == 0 && iid != data::Item::COIN && iid != data::Item::CRYSTAL && iid != data::Item::POINT)
        {
			Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(220101));
            return;
        }
    }
    if (onShut)
    {
        onShut();
    }
    else
    {
        removeFromParent();
    }
}

void SettlementLayer::getAllCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if(type == ui::Layout::TouchEventType::ENDED)
    {
        getAll();
    }
}

void SettlementLayer::getAll()
{
    //更新新手引导..
    data::Guid::getInstance()->updateGuidInfo();
    
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
    auto drop = seekChildByNameWithRetType<ui::ListView*>(this, "droplist");
    auto list = drop->getItems();
    
    bool showtips = false;
    for (auto &i : list)
    {
        auto item = (ListItem*)i;
        int remain = data::Crusade::getInstance()->getPackageTotal() - data::Crusade::getInstance()->getPackageSpace();
        int perspace = item->getData().at("space").asInt();
        
        if (perspace > remain)
        {
            showtips = true;
        }
        
        int amount = item->getAmount();
        
        while (amount && perspace * amount > remain)
        {
            --amount;
        }
        if (amount > 0)
        {
            transfer(item, amount);
        }
    }
    //背包已满..
    if (showtips)
    {
		Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(220102));
    }
}



void SettlementLayer::upDateItems()
{
    auto dropList = seekChildByNameWithRetType<ui::ListView*>(this, "droplist");
    dropList->removeAllItems();
    auto backPackList = seekChildByNameWithRetType<ui::ListView*>(this, "packagelist");
    backPackList->removeAllItems();
    
    //掉落物品..
    for (auto i : _spiolsData["drop"].asValueVector())
    {
        if (i.asValueMap()["amount"].asInt() > 0)
        {
            auto item = ListItem::create(i.asValueMap(), this);
            item->_place = ListItem::ItemPlace::kDropheap;
            dropList->pushBackCustomItem(item);
        }
    }
    //背包物品..
    for (auto i : _spiolsData["package"].asValueVector())
    {
        if (i.asValueMap()["amount"].asInt() > 0)
        {
            auto item = ListItem::create(i.asValueMap(), this);
            item->_place = ListItem::ItemPlace::kPackage;
            backPackList->pushBackCustomItem(item);
        }
    }
}

void SettlementLayer::transfer(SettlementLayer::ListItem* item, int amount)
{
    auto dropView = seekChildByNameWithRetType<ui::ListView*>(this, "droplist");
    auto packageView = seekChildByNameWithRetType<ui::ListView*>(this, "packagelist");
    
    /**
     *  丢弃物品
     */
    if (item->_place == SettlementLayer::ListItem::ItemPlace::kPackage)
    {
        int iid = item->getiid();
        
        item->decrease(amount);
        int operateResult =  data::Crusade::getInstance()->packageOut(iid, amount);
        if(operateResult == data::Crusade::INOUT_OK)
        {
            //丢弃物品成功..
            auto itemData =  data::Item::getInstance()->getItemByIID(iid);
            //拾取成功..
            std::stringstream str;
            str<<"你丢弃了:";
            str<< itemData.asValueMap()["name"].asString();
            Notice::getInstance()->postTips(str.str());
        }
        else
        {
            //丢弃物品失败..
            Notice::getInstance()->postTips("丢弃失败");
        }
        
        auto droplist = dropView->getItems();
        auto findret = std::find_if(droplist.begin(), droplist.end(), [=](ui::Widget* i)->bool
        {
            auto item = (SettlementLayer::ListItem*)i;
            return item->getiid() == iid;
        });
        if (findret != droplist.end())
        {
            auto aim = (SettlementLayer::ListItem*)(*findret);
            aim->increase(amount);
        }
        else
        {
            auto newitem = ListItem::create(item->getData(), this);
            newitem->setAmount(amount);
            newitem->_place = ListItem::ItemPlace::kDropheap;
            dropView->pushBackCustomItem(newitem);
        }
        
        if (item->getAmount() <= 0)
        {
            packageView->removeItem(packageView->getIndex(item));
        }
    }
    
    
    /**
     *  拾取物品
     */
    if (item->_place == SettlementLayer::ListItem::ItemPlace::kDropheap)
    {
        int iid = item->getiid();
        auto ret = data::Crusade::getInstance()->packageInByPickup(iid, amount);
        if (ret == data::Crusade::INOUT_OK)
        {
            item->decrease(amount);
            auto packagelist = packageView->getItems();
            auto findret = std::find_if(packagelist.begin(), packagelist.end(), [=](ui::Widget* i)->bool{
                auto item = (SettlementLayer::ListItem*)i;
                return item->getiid() == iid;
            });
            if (findret != packagelist.end())
            {
                auto aim = (SettlementLayer::ListItem*)(*findret);
                aim->increase(amount);
            }
            else
            {
                auto newitem = ListItem::create(item->getData(), this);
                newitem->setAmount(amount);
                newitem->_place = ListItem::ItemPlace::kPackage;
                packageView->pushBackCustomItem(newitem);
            }
            
            if (item->getAmount() <= 0)
            {
                dropView->removeItem(dropView->getIndex(item));
            }
            auto itemData =  data::Item::getInstance()->getItemByIID(iid);
            //拾取成功..
            std::stringstream str;
            str<<"你拾取了:";
            str<< itemData.asValueMap()["name"].asString();
            Notice::getInstance()->postTips(str.str());
        }
        else if (ret == data::Crusade::IN_ERROR_OVER)
        {
			//背包已满..
			Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(220102));
        }
    }
    
    this->updatePackCapacity();
}

/**************************************************listItem*******************************************************/

#define ITEM_BTN_ACT 11
SettlementLayer::ListItem *SettlementLayer::ListItem::create(ValueMap itemData, SettlementLayer* delegate)
{
    ListItem* ret = new ListItem();
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

bool SettlementLayer::ListItem::init(ValueMap itemData)
{
    if (!Widget::init())
    {
        return false;
    }
    
    _itemData = itemData;
    
    //初始化显示..
    CCASSERT(!_delegate->_itemModel.empty(), "");
    auto view = _delegate->_itemModel.at(0)->clone();
    this->setContentSize(view->getContentSize());
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
    seekChildByName(view, "backimage")->setOpacity(140);

    seekChildByNameWithRetType<ui::Text*>(view, "name")->setString(_itemData["name"].asString());
    seekChildByNameWithRetType<ui::Text*>(view, "number")->setString(_itemData["amount"].asString());
    
    view->setTouchEnabled(true);
    view->addTouchEventListener(CC_CALLBACK_2(ListItem::choiceButtonCallback, this));
    
    return true;
}

int SettlementLayer::ListItem::getiid()
{
    return _itemData["iid"].asInt();
}

void SettlementLayer::ListItem::choiceButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if(type == ui::Text::TouchEventType::ENDED)
    {
        choiceIt();
    }
}

void SettlementLayer::ListItem::choiceIt()
{
    _delegate->transfer(this);
}

void SettlementLayer::ListItem::setAmount(int amount)
{
    _itemData["amount"] = amount;
    seekChildByNameWithRetType<ui::Text*>(this, "number")->setString(Value(amount).asString());
}

int SettlementLayer::ListItem::getAmount()
{
    return _itemData["amount"].asInt();
}

void SettlementLayer::ListItem::decrease(int amount)
{
    _itemData["amount"] = _itemData["amount"].asInt() - amount;
    seekChildByNameWithRetType<ui::Text*>(this, "number")->setString(_itemData["amount"].asString());
}

void SettlementLayer::ListItem::increase(int amount)
{
    _itemData["amount"] = _itemData["amount"].asInt() + amount;
    seekChildByNameWithRetType<ui::Text*>(this, "number")->setString(_itemData["amount"].asString());
}
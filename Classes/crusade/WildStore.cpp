/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-2-4
 *	@author	110101
     
 *	@file	crusade/WildStore.h
 *   @modify	null
 ******************************************************************************/

#include "WildStore.h"
#include "utils/DeerUtils.h"
#include "data/item.h"
#include "popUp/BlackMask.h"
//#include "manor/TextPopup.h"
#include "popUp/Summary.h"

USING_NS_CC;
USING_NS_DU;

WildStore *WildStore::create(int sid)
{
    WildStore *ret = new WildStore();
    if (ret && ret->init(sid))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return NULL;
    }
}

bool WildStore::init(int sid)
{
    if (!Layout::init())
    {
        return false;
    }
    addChild(BlackMask::create());
    
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("store.csb");
    view->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //添加返回按钮的功能..
//    auto backButton = seekChildByNameWithRetType<ui::Layout*>(view, "gobackBtnArea");
//    backButton->setTouchEnabled(true);
//    backButton->setSwallowTouches(false);
//    backButton->addTouchEventListener(CC_CALLBACK_2(StoreLayer::backButtonCallback, this));
//    
//    //初始化item模板..
//    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
//    setListLong(nullptr);
//    
//    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "panel"));
//    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(list, "widthPanel"));
//    
//    //清空list
//    list->removeAllItems();
//    
//    ValueVector shopData = ValueVectorNull;
//    if (sid == -1)
//    {
//        shopData = data::Item::getInstance()->getShop().asValueVector();
//    }
//    else
//    {
//        shopData = data::Item::getInstance()->getShopBySid(sid).asValueVector();
//    }
//    
//    for (auto &i : shopData)
//    {
//        auto item = StoreListItem::create(i.asValueMap(), this);
//        list->pushBackCustomItem(item);
//    }
//    
//    /**
//     *  添加通知板
//     */
//    this->addChild(NotificationManager::getInstance()->getView(), 50);
//    this->addChild(NotificationManager::getInstance()->getTipsView(), 50);
//    
//    EventListenerCustom * foldEvent = EventListenerCustom::create(NotificationManager::FOLD, CC_CALLBACK_1(StoreLayer::setListLong, this));
//    NotificationManager::getInstance()->addListener(foldEvent);
//    _listenerMap.insert(C_TO_S(foldEvent), foldEvent);
//    
//    EventListenerCustom * unfoldEvent = EventListenerCustom::create(NotificationManager::UNFOLD, CC_CALLBACK_1(StoreLayer::setListShort, this));
//    NotificationManager::getInstance()->addListener(unfoldEvent);
//    _listenerMap.insert(C_TO_S(unfoldEvent), unfoldEvent);
    
    return true;
}
/****************************************************************************
*	@Copyright(c)	2014,青岛UMGame
*	@desc	购买钻石界面
*	@date	2015-
*	@author	120102
*	@file	
*	@modify	null
******************************************************************************/
#include "RmbBuyLayer.h"
#include "popUp/BlackMask.h"
#include "notice/Notice.h"
#include "data/Item.h"
#include "editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;
RmbBuyLayer::~RmbBuyLayer()
{
    
    _LayoutVec.clear();
    _eventDispatcher->removeCustomEventListeners(data::Item::USER_ITEM_CHANGE);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    delete newiosbridge;
    _eventDispatcher->removeCustomEventListeners(IOSiAPBridge::PURCHASEOVER);
    _eventDispatcher->removeCustomEventListeners(IOSiAPBridge::PURCHASESUCCESS);
    _eventDispatcher->removeCustomEventListeners(IOSiAPBridge::PURCHASEFAIL);
 //   _eventDispatcher->removeCustomEventListeners(IOSiAPBridge::RECOVERSUCCESS);
    _eventDispatcher->removeCustomEventListeners(IOSiAPBridge::PURCHASEVERIFY);
    
#endif
}

void RmbBuyLayer::onEnter()
{
    Layout::onEnter();
    addChild(Notice::getInstance()->getTips());
}

bool RmbBuyLayer::init()
{
	if (!Layout::init())
	{
		return false;
	}
    addChild(BlackMask::create());
    auto win = Director::getInstance()->getWinSize();
	Node* view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("rmbmall.csb");
	view->setContentSize(win);
    setContentSize(win);
    addChild(view);
	ui::Helper::doLayout(this);
    
    //设置天空大小,天空海阔你与我~~~
    auto starSky = view->getChildByName("Panel")->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(win.width/2,win.height*75/128));
    
    _rmbTxt  = view->getChildByName("Panel")->getChildByName<ui::Text*>("rmbTxt");
	_rmbimage = view->getChildByName("Panel")->getChildByName<ui::ImageView*>("rmbImage");
    std::string  rmbNum  = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
    if(rmbNum.size() == 0)
    {
        rmbNum = "0";
    }
    _rmbTxt->setString(rmbNum);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	_rmbTxt->setVisible(false);
	_rmbimage->setVisible(false);
#endif
	//各个购买选项..
    //50元宝...
	auto fiftyrmb = view->getChildByName("Panel")->getChildByName("item1")->getChildByName<ui::Layout*>("buyBtn");
    fiftyrmb->setTag(50);
	fiftyrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //100元宝...
    auto onehundredrmb = view->getChildByName("Panel")->getChildByName("item2")->getChildByName<ui::Layout*>("buyBtn");
    onehundredrmb->setTag(100);
    onehundredrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //500元宝...
    auto fivehundredrmb = view->getChildByName("Panel")->getChildByName("item3")->getChildByName<ui::Layout*>("buyBtn");
    fivehundredrmb->setTag(500);
    fivehundredrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //1000元宝...
    auto onethousandrmb = view->getChildByName("Panel")->getChildByName("item5")->getChildByName<ui::Layout*>("buyBtn");
    onethousandrmb->setTag(1000);
    onethousandrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //5000元宝...
    auto fivethousandrmb = view->getChildByName("Panel")->getChildByName("item6")->getChildByName<ui::Layout*>("buyBtn");
    fivethousandrmb->setTag(5000);
    fivethousandrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //10000元宝...
    auto onemillionrmb = view->getChildByName("Panel")->getChildByName("item7")->getChildByName<ui::Layout*>("buyBtn");
    onemillionrmb->setTag(10000);
    onemillionrmb->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::BuyBtnCall, this));
    //
	//返回按钮...
	auto goback = view->getChildByName("Panel")->getChildByName<ui::Layout*>("backBtnArea");
	goback->addTouchEventListener(CC_CALLBACK_2(RmbBuyLayer::gobackCall, this));
    
    _LayoutVec.push_back(fiftyrmb);
    _LayoutVec.push_back(onehundredrmb);
    _LayoutVec.push_back(fivehundredrmb);
    _LayoutVec.push_back(onethousandrmb);
    _LayoutVec.push_back(fivethousandrmb);
    _LayoutVec.push_back(onemillionrmb);
    _LayoutVec.push_back(goback);
//    auto crystallistener = [=](EventCustom* e)
//    {
//        std::string rmbNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
//        if (rmbNum.size() == 0)
//        {
//            rmbNum = "0";
//        }
//        _rmbTxt->setString(rmbNum);
//    };
//    _eventDispatcher->addCustomEventListener(data::Item::USER_ITEM_CHANGE, crystallistener);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto purchaseoverlistener = [=] (EventCustom* e)
    {
        auto translate = this->getChildByName("translate");
        if(translate)
        {
            translate->removeFromParent();
            translate = nullptr;
        }
      //
    };
    _eventDispatcher->addCustomEventListener(IOSiAPBridge::PURCHASEOVER, purchaseoverlistener);
    
    auto purchasesuccesslistener = [=] (EventCustom* e)
    {
        auto translate = this->getChildByName("translateverify");
        if(translate)
        {
            translate->removeFromParent();
            translate = nullptr;
        }
        std::string str = "购买成功!";
        Notice::getInstance()->postTips(str);
        SetIsTouchEnable(true);
      //  }
        
    };
    _eventDispatcher->addCustomEventListener(IOSiAPBridge::PURCHASESUCCESS, purchasesuccesslistener);
    
    auto purchasefaillistener = [=] (EventCustom* e)
    {
        auto translate = this->getChildByName("translateverify");
        if(translate)
        {
            translate->removeFromParent();
            translate = nullptr;
        }
        std::string str = "购买失败!";
        Notice::getInstance()->postTips(str);
        SetIsTouchEnable(true);
     //   }

    };
    _eventDispatcher->addCustomEventListener(IOSiAPBridge::PURCHASEFAIL, purchasefaillistener);
    
    auto purchaseverifylistener = [=] (EventCustom* e)
    {
        auto size = Director::getInstance()->getWinSize();
        translate = RmbBuyTranslate::create(size, "[请等待，正在验证...]");
        translate->setTag(100);
        addChild(translate, 10000000, "translateverify");
     //   std::string str = "正在验证!";
     //   Notice::getInstance()->postTips(str);
    };
    _eventDispatcher->addCustomEventListener(IOSiAPBridge::PURCHASEVERIFY, purchaseverifylistener);
    
    newiosbridge = new IOSiAPBridge();
#endif

	data::Item* item = data::Item::getInstance();
	EventListenerCustom*listener = item->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(RmbBuyLayer::itemChange, this));
	addEventPool(listener, item);
	return true;
}

void RmbBuyLayer::itemChange(cocos2d::EventCustom* e)
{
	std::string  rmbNum = data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
	if (rmbNum.size() == 0)
	{
		rmbNum = "0";
	}
	_rmbTxt->setString(rmbNum);
}
void RmbBuyLayer::gobackCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
	if (t == cocos2d::ui::Button::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void RmbBuyLayer::BuyBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    
	if (t == cocos2d::ui::Button::TouchEventType::ENDED)
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SetIsTouchEnable(false);
        
        translate = RmbBuyTranslate::create(Director::getInstance()->getWinSize(),"[请等待，正在发送购买请求...]");
        translate->setTag(10000);
        addChild(translate, 100000, "translate");
#endif
        int tag = ((Node*)sender)->getTag();
        if (tag == 50)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            newiosbridge->requestProducts(50);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
         //   data::User::getInstance()->goldPlus(50);
		//	data::Item::getInstance()->getUserItemByIID(data::Item::ItemID::CRYSTAL).asValueMap()["amount"].asString();
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 50);
#endif
        }
        else if (tag == 100)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
           
            newiosbridge->requestProducts(100);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 100);
#endif
        }
        else if (tag == 500)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            newiosbridge->requestProducts(500);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
          //  data::User::getInstance()->goldPlus(500);
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 500);
#endif
        }
        else if (tag == 1000)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            newiosbridge->requestProducts(1000);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
         //   data::User::getInstance()->goldPlus(1000);
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 1000);
#endif
        }
        else if (tag == 5000)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
           
            newiosbridge->requestProducts(5000);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
          //  data::User::getInstance()->goldPlus(5000);
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 5000);
#endif
        }
        else if (tag == 10000)
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            newiosbridge->requestProducts(10000);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
         //   data::User::getInstance()->goldPlus(10000);
			data::Item::getInstance()->userItemPlus((data::Item::ItemID::CRYSTAL), 10000);
#endif
        }

	}
}
void RmbBuyLayer::SetIsTouchEnable(bool enable)
{
    if (enable == true)
    {
        for (int i = 0; i < _LayoutVec.size(); i++)
        {
            _LayoutVec.at(i)->setTouchEnabled(true);
        }
    }
    else
    {
        for (int i = 0; i < _LayoutVec.size(); i++)
        {
            _LayoutVec.at(i)->setTouchEnabled(false);
        }
    }
}
void RmbBuyLayer::removeSelf()
{
    
}
void RmbBuyLayer::updateProperty()
{
    
}


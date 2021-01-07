/****************************************************************************
 *	@desc   战斗界面
 *	@date	2015-2-27
 *	@author	110101
     
 *	@file	crusade/WildTavern.h
 *  @modify	null
 ******************************************************************************/

#include "WildTavern.h"
#include "data/Role.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "notice/Notice.h"
//#include "manor/TextPopup.h"
#include "popUp/Summary.h"
#include "sound/SoundManager.h"
#include "data/Lang.h"

USING_NS_CC;
USING_NS_DU;

WildTavern::~WildTavern()
{
    
}

WildTavern * WildTavern::create(int tavernid)
{
    WildTavern * ret = new WildTavern();
    if (ret && ret->init(tavernid))
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
    return ret;
}

bool WildTavern::init(int tavernid)
{
    if (!Widget::init())
    {
        return false;
    }
    /**
     *  载入界面
     */
    /* 临时 */
    addChild(BlackMask::create());
    
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("wildTarven.csb");
    view->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    auto backBtn = seekChildByNameWithRetType<ui::Layout*>(this, "backBtnArea");
    backBtn->setTouchEnabled(true);
    backBtn->addTouchEventListener(CC_CALLBACK_2(WildTavern::backBtnCall, this));
    
    //保存模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "listitem"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "listitem_0"));
    
    auto list = seekChildByNameWithRetType<ui::ListView*>(view, "list");
    list->removeAllItems();
    list->pushBackCustomItem(_itemModel.at(1));
    auto herodata = data::Role::getInstance()->getTavern(tavernid).asValueVector();
    for (auto &i : herodata)
    {
        auto item = WildTavern::TavernListItem::create(i, this);
        list->pushBackCustomItem(item);
    }
    
    //当故事内容打开或者关上是，调整listView的长度..
    Notice* notice = Notice::getInstance();
    EventListenerCustom * foldEvent = notice->addListener(Notice::FOLD, CC_CALLBACK_1(WildTavern::onBoardResize, this));
    addEventPool(foldEvent, notice);
    
    return true;
}
void WildTavern::removeSelf()
{
    
}
void WildTavern::updateProperty()
{
    
}
void WildTavern::showBoard()
{
    addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
    //添加通知板..
    addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
    
    listResize();
}
void WildTavern::onBoardResize(cocos2d::EventCustom* e)
{
    listResize();
}
void WildTavern::listResize()
{
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    auto innerpos = list->getInnerContainer()->getPosition();
    list->setContentSize(Size(list->getContentSize().width, list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
    list->getInnerContainer()->setPosition(innerpos);
}
void WildTavern::showInfo(std::string text,cocos2d::Node* node)
{
    if(this->getChildByName("TextPop"))
    {
        this->removeChildByName("TextPop");
    }
    else
    {
        auto pop = Summary::create(text,node);
        this->addChild(pop, 50,"TextPop");
    }
}

void WildTavern::backBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touch)
{
    if (touch == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "backBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
}

void WildTavern::backReal()
{
    this->removeFromParent();
}

void WildTavern::showPurchase(cocos2d::Value data)
{
    auto lay =  WildTavern::PurchasePop::create(data);
    lay->setPosition(Point::ZERO);
    this->addChild(lay, 100);
}

/*************************************************************************************************/

WildTavern::TavernListItem * WildTavern::TavernListItem::create(cocos2d::Value rdata, WildTavern *delegate)
{
    WildTavern::TavernListItem *ret = new TavernListItem();
    ret->_delegate = delegate;
    if (ret && ret->init(rdata))
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

bool WildTavern::TavernListItem::init(cocos2d::Value rdata)
{
    if (!Widget::init())
    {
        return false;
    }
    _rdata = rdata;
    
    auto view = _delegate->_itemModel.at(0)->clone();
    
    view->setPosition(Point::ZERO);
    this->setContentSize(view->getContentSize());
    this->addChild(view);
    
    auto items = _rdata.asValueMap()["train"].asValueMap()["items"].asValueVector();
    
    _rdata.asValueMap()["condition"] = Value(items);
    seekChildByNameWithRetType<ui::Text*>(this, "name")->setString(rdata.asValueMap().at("name").asString());
    
    std::stringstream conditon;
//    conditon<<"金币 x"<< rdata.asValueMap().at("price").asString();
    for (auto &i : items)
    {
        conditon<<i.asValueMap()["name"].asString()<<i.asValueMap()["amount"].asString();
    }
    seekChildByNameWithRetType<ui::Text*>(this, "price")->setString(conditon.str());
    
    auto infoBtn = seekChildByNameWithRetType<ui::Layout*>(this, "infoBtnArea");
    infoBtn->setTouchEnabled(true);
    infoBtn->addTouchEventListener(CC_CALLBACK_2(TavernListItem::infoBtnCall, this));
    
    auto nameBtn = seekChildByNameWithRetType<ui::Layout*>(this, "namebackArea");
    nameBtn->setTouchEnabled(true);
    nameBtn->addTouchEventListener(CC_CALLBACK_2(TavernListItem::purchaseBtnCall, this));
    
    
//    Label* price = Label::createWithSystemFont(_rdata.asValueMap()["price"].asString(), "", 60);
//    price->setPosition(Vec2(800, 100));
//    this->addChild(price);
//    if (_rdata.asValueMap()["price"].asString().empty())
//    {
//        price->setString("There is no data");
//    }
    
//    ui::Text *info = ui::Text::create("information", "", 60);
//    info->setPosition(Vec2(1200, 100));
//    this->addChild(info);
//    info->setTouchEnabled(true);
//    info->addTouchEventListener(CC_CALLBACK_2(TavernListItem::infoBtnCall, this));
    
    return true;
}

void WildTavern::TavernListItem::infoBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType tevent)
{
    if (tevent == ui::Button::TouchEventType::ENDED)
    {
        std::string text;
        text = _rdata.asValueMap()["summary"].asString();
        
        if (_rdata.asValueMap()["effect"].isNull() || _rdata.asValueMap()["effect"].asValueMap()["name"].getType() == Value::Type::NONE)
        {
            text = text + "\n效果:无 \n";
        }
        else
        {
            text = text + "\n效果: " + _rdata.asValueMap()["effect"].asValueMap()["name"].asString() + "\n";
        }
        text = text +
        "技能: " + _rdata.asValueMap()["skill"].asString() + "\n" + "生命: " + _rdata.asValueMap()["life"].asString() + "  威力: " + _rdata.asValueMap()["power"].asString() + "  速度: " + _rdata.asValueMap()["speed"].asString(); + "\n";
        auto infoBtn = seekChildByNameWithRetType<ui::ImageView*>(this, "nameback");
        _delegate->showInfo(text,infoBtn);
    }
}

void WildTavern::TavernListItem::purchaseBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType tevent)
{
    if (tevent == ui::Button::TouchEventType::ENDED)
    {
        _delegate->showPurchase(_rdata);
    }
}

/***************************************PurchasePop****************************************/


WildTavern::PurchasePop *WildTavern::PurchasePop::create(cocos2d::Value data)
{
    auto ret = new PurchasePop();
    if (ret && ret->init(data))
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

bool WildTavern::PurchasePop::init(cocos2d::Value data)
{
    if(!Layout::init())
    {
        return false;
    }
    
    _data = data;
    addChild(BlackMask::create());
    
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("wildTarvenPur.csb");
    view->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    auto title = "招募一个  " + _data.asValueMap()["name"].asString() + "?  ";
    seekChildByNameWithRetType<ui::Text*>(view, "title")->setString(title);
    
    std::string text1 = "技能: "; text1 = text1 + _data.asValueMap()["skill"].asString();
    seekChildByNameWithRetType<ui::Text*>(view, "text1")->setString(text1);
    
    
    std::string text2 = "生命: "+_data.asValueMap()["life"].asString() + "威力: "+_data.asValueMap()["power"].asString()
    +"速度: "+_data.asValueMap()["speed"].asString();
    seekChildByNameWithRetType<ui::Text*>(view, "text2")->setString(text2);
    
    seekChildByNameWithRetType<ui::Button*>(view, "sureBtn")->addTouchEventListener(CC_CALLBACK_2(PurchasePop::sureBtnCall, this));
    seekChildByNameWithRetType<ui::Button*>(view, "quitBtn")->addTouchEventListener(CC_CALLBACK_2(PurchasePop::quitBtnCall, this));
    
    return true;
}

void WildTavern::PurchasePop::sureBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType tevent)
{
    if (tevent == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        sureReal();
    }
    //BTN_CALL_ACT(tevent, sender, sender, CC_CALLBACK_0(PurchasePop::sureReal, this));
}

void WildTavern::PurchasePop::sureReal()
{
    Value ret = data::Role::getInstance()->train(_data.asValueMap()["rid"].asInt());
    if (ret.asValueMap()["code"].asInt() == 0)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBuildFinish);
		Notice::getInstance()->postTips(_data.asValueMap()["name"].asString() + "+1");
	}
	else{
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kLACK);
		std::stringstream sstr;
		for (auto &i : ret.asValueMap()["data"].asValueMap()["train"].asValueMap()["items"].asValueVector())
		{
			if (i.asValueMap()["lack"].asInt() <= 0){
				sstr << "缺少: " << -i.asValueMap()["lack"].asInt() << "个  " << i.asValueMap()["name"].asString() << "\r\n";
                auto tips = data::Lang::getInstance()->getTipsByIID(i.asValueMap()["iid"].asInt());
                sstr <<tips<<"\r\n";
			}
		}
		Notice::getInstance()->postTips(sstr.str(),Notice::kWarning);
	}
	removeFromParent();
}

void WildTavern::PurchasePop::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType tevent)
{
    if (tevent == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitReal();
    }
    //BTN_CALL_ACT(tevent, sender, sender, CC_CALLBACK_0(PurchasePop::quitReal, this));
}

void WildTavern::PurchasePop::quitReal()
{
    removeFromParent();
}
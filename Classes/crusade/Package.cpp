/****************************************************************************
 *	@desc   背包
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/Package.cpp
 *  @modify	null
 ******************************************************************************/

#include "Package.h"
#include "popUp/BlackMask.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "data/crusade.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"
#include "popUp/Summary.h"

USING_NS_CC;
USING_NS_DU;

bool Package::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //添加遮罩..
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    
    //初始化界面..
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("package.csb");
    view->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(view);
    ui::Helper::doLayout(this);
    
    //返回按钮..
    seekChildByNameWithRetType<ui::Layout*>(view, "backBtnArea")->addTouchEventListener(CC_CALLBACK_2(Package::backBtnCallback, this));
    
    //初始化item模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "item"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "item_0"));
    
    //清空list
    auto list = seekChildByNameWithRetType<ui::ListView*>(view, "list");
    list->removeAllItems();
    list->pushBackCustomItem(_itemModel.at(1));
    
    //添加背包数据..
    auto packData = data::Crusade::getInstance()->getPackage(true).asValueVector();
    for (auto &i : packData)
    {
        auto item = ListItem::create(i.asValueMap(), this);
        list->pushBackCustomItem(item);
    }
    
    return true;
}

void Package::backBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        this->removeFromParent();
    }
}

void Package::showInfo(std::string text, cocos2d::Node* node)
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

/****************item模板*******************/

Package::ListItem * Package::ListItem::create(cocos2d::ValueMap data, Package *delegate)
{
    auto ret = new ListItem();
    ret->_delegate = delegate;
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

bool Package::ListItem::init(cocos2d::ValueMap data)
{
    if (!Layout::init())
    {
        return false;
    }
    _data = data;
    
    //初始化显示..
    CC_ASSERT(!_delegate->_itemModel.empty());
    auto view = _delegate->_itemModel.front()->clone();
    this->setContentSize(view->getContentSize());
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
    seekChildByName(view, "nameback")->setOpacity(140);
    
    seekChildByNameWithRetType<ui::Text*>(view, "itemname")->setString(_data["name"].asString());
    seekChildByNameWithRetType<ui::Text*>(view, "itemNum")->setString(_data["current"].asString());
    seekChildByNameWithRetType<ui::Layout*>(view, "infoBtnArea")->addTouchEventListener(CC_CALLBACK_2(ListItem::infoBtnCallback, this));
    
    //显示星级..
    for (auto i = 1; i < _data["level"].asInt(); i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(true);
    }
    for (auto i = _data["level"].asInt() + 1; i < 6 ; i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(false);
    }
    
    return true;
}

void Package::ListItem::infoBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        auto area = seekChildByNameWithRetType<ui::ImageView*>(this, "nameback");
        _delegate->showInfo(_data["summary"].asString(), area);
        //CCLOG("%s", _data["summary"].asString().c_str());
    }
}
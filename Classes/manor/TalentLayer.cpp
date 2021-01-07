/****************************************************************************
 *	@desc   天赋界面
 *	@date	2015-2-6
 *	@author	110101
     
 *	@file	manor/TalentLayer.cpp
 *  @modify	null
 ******************************************************************************/

#include "TalentLayer.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "data/Talent.h"
#include "manor/Manor.h"
#include "notice/Notice.h"

#include "popUp/Summary.h"
#include "GlobalMacros.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;
#define TO_S(_var_) #_var_
#define LISTHIEGHT_OFFSET 50

TalentLayer::~TalentLayer()
{
	//Notice::getInstance()->removeListener(_liMap.at(TO_S(foldEvent)));
	//Notice::getInstance()->removeListener(_liMap.at(TO_S(unfoldEvent)));
}

bool TalentLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //初始化界面..
    Size winSize = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("talent.csb");
    view->setContentSize(winSize);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    //获取item的模板..
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "panel"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "widthpanel"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "panel_0"));

    
    updateItems();
    
    //返回按钮的处理..
    auto backButton = seekChildByNameWithRetType<ui::Layout*>(this, "goBackBtnArea");
    backButton->setTouchEnabled(true);
    backButton->setSwallowTouches(false);
    backButton->addTouchEventListener(CC_CALLBACK_2(TalentLayer::backButtonCallback, this));
    
    //添加通知页..
    
	EventListenerCustom * foldEvent = Notice::getInstance()->addListener(Notice::FOLD, CC_CALLBACK_1(TalentLayer::setListLong, this));
    addEventPool(foldEvent,Notice::getInstance());
    
	EventListenerCustom * unfoldEvent = Notice::getInstance()->addListener(Notice::UNFOLD, CC_CALLBACK_1(TalentLayer::setListShort, this));
    addEventPool(unfoldEvent,Notice::getInstance());
    
    setListLong(nullptr);
    
    return true;
}

void TalentLayer::updateItems()
{
    //清空list添加item..
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    list->removeAllItems();
    list->pushBackCustomItem(_itemModel.at(2));
    auto talList = data::Talent::getInstance()->getUserTalentList().asValueVector();
    for (auto &i : talList)
    {
        auto item = TalentListItem::create(i.asValueMap(), this);
        list->pushBackCustomItem(item);
    }
}

void TalentLayer::onEnter()
{
    Layer::onEnter();
    showBoard();
}

void TalentLayer::showBoard()
{
	addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
}

void TalentLayer::setListLong(cocos2d::EventCustom *e)
{
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    auto innerpos = list->getInnerContainer()->getPosition();
	list->setContentSize(Size(list->getContentSize().width, list->getWorldPosition().y - Notice::getInstance()->getBoardHeight()));
    list->getInnerContainer()->setPosition(innerpos);
}

void TalentLayer::setListShort(cocos2d::EventCustom *e)
{
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    auto innerpos = list->getInnerContainer()->getPosition();
    list->setContentSize(Size(list->getContentSize().width, list->getWorldPosition().y - LISTHIEGHT_OFFSET));
    list->getInnerContainer()->setPosition(innerpos);
    auto bottomMask = seekChildByNameWithRetType<ui::ImageView*>(this, "Image_1_0");
    bottomMask->setPositionY(list->getBottomBoundary());
}

void TalentLayer::backButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
}

void TalentLayer::backReal()
{
    removeChildByName("Textpop");
    Manor::getInstance()->showLayer(Manor::kVenture);
}

void TalentLayer::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(TalentLayer::removeFromParent, this)), NULL));
}
void TalentLayer::updateProperty()
{

}
void TalentLayer::showInfo(std::string &info, cocos2d::Node* node)
{
    if (this->getChildByName("Textpop"))
    {
        this->removeChildByName("Textpop");
    }
    else
    {
        auto pop = Summary::create(info,node);
        this->addChild(pop, 100, "Textpop");
    }
}

/***************************************talentListItem*******************************************/

TalentLayer::TalentListItem::TalentListItem()
:_delegate(nullptr)
{
    
}

TalentLayer::TalentListItem *TalentLayer::TalentListItem::create(cocos2d::ValueMap itemData, TalentLayer* delegate)
{
    TalentListItem *ret = new TalentListItem();
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
        return ret;
    }
}

bool TalentLayer::TalentListItem::init(cocos2d::ValueMap itemData)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _itemData = itemData;
    
    //初始化显示部分..
    CCASSERT(!_delegate->_itemModel.empty(), "");
    ui::Widget* view;
    if (_itemData["name"].asString().size() <= 12)
    {
        view = _delegate->_itemModel.front()->clone();
    }
    else
    {
        view = _delegate->_itemModel.at(1)->clone();
    }
    
    this->setContentSize(view->getContentSize());
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
    seekChildByNameWithRetType<ui::ImageView*>(view, "nameBack")->setOpacity(140);
    seekChildByNameWithRetType<ui::Text*>(this, "name")->setString(itemData["name"].asString());
    seekChildByNameWithRetType<ui::Text*>(this, "summary")->setString(itemData["summary"].asString());
    
    //info 按钮..
    auto infoBtn = seekChildByNameWithRetType<ui::Layout*>(this, "infoBtnArea");
    infoBtn->addTouchEventListener(std::bind(&TalentListItem::tipsCallback, this, std::placeholders::_1, std::placeholders::_2));
    
    return true;
}

void TalentLayer::TalentListItem::tipsCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        std::string str = _itemData["name"].asString() + "\n" + _itemData["summary"].asString();
        
        auto area = seekChildByNameWithRetType<ui::ImageView*>(this, "nameBack");
        _delegate->showInfo(str, area);
    }
}
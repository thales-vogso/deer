/****************************************************************************
 *	@desc   墓园
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	crusade/Cemetery.h
 *  @modify	null
 ******************************************************************************/

#include "Cemetery.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/Deerutils.h"
#include "popUp/BlackMask.h"
#include "data/Role.h"
#include "notice/Notice.h"
//#include "manor/TextPopup.h"
#include "popUp/Summary.h"
#include "GlobalMacros.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"

USING_NS_CC;
USING_NS_DU;

#define POP_Z 100
bool Cemetery::init()
{
    if (!Layer::init())
    {
        return false;
    }
    //触摸隔离..
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("cemetery.csb");
    view->setContentSize(win);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    seekChildByNameWithRetType<ui::Layout *>(view, "gobackBtnArea")->addTouchEventListener(CC_CALLBACK_2(Cemetery::backCallback, this));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "item"));
    _itemModel.pushBack(seekChildByNameWithRetType<ui::Layout*>(view, "item_0"));
    
    updateItems();
    
    //设置天空大小，天空海阔你与我~~~~
    auto starSky = view->getChildByName("Panel_1")->getChildByName<ParticleSystemQuad*>("Particle_1");
    starSky->setPosVar(Vec2(win.width/2,(int)win.height*75/128));
    
    return true;
}

void Cemetery::onEnter()
{
    Layer::onEnter();
	this->addChild(Notice::getInstance()->getBoard(), Notice::BOARD_Z_ORDER);
	this->addChild(Notice::getInstance()->getTips(), Notice::TIPS_Z_ORDER, "tips");
}

void Cemetery::onExit()
{
    Layer::onExit();
}

void Cemetery::updateItems()
{
    auto list = seekChildByNameWithRetType<ui::ListView*>(this, "list");
    list->removeAllItems();
    list->pushBackCustomItem(_itemModel.at(1)->clone());
    auto data = data::Role::getInstance()->getDeadRoles().asValueVector();
    for (auto &i : data)
    {
        if (i.asValueMap()["dead"].asInt() > 0)
        {
            auto item = Cemetery::ListItem::create(i.asValueMap(), this);
            list->pushBackCustomItem(item);
        }
    }
}

void Cemetery::backCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout *>(this, "gobackBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backReal();
    }
}

void Cemetery::backReal()
{
    this->removeFromParent();
}

void Cemetery::showInfo(std::string &info,cocos2d::Node* node)
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
void Cemetery::revival(cocos2d::ValueMap data)
{
    auto rp = RevivalChoice::create(data);
    rp->setDelegate(this);
    this->addChild(rp, POP_Z);
}

/*****************************************listitem**********************************************/

Cemetery::ListItem* Cemetery::ListItem::create(cocos2d::ValueMap data, Cemetery *delegate)
{
    auto ret = new Cemetery::ListItem();
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

bool Cemetery::ListItem::init(cocos2d::ValueMap data)
{
    if (!Layout::init())
    {
        return true;
    }
    
    _data = data;
    //初始化显示..
    auto view = _delegate->_itemModel.front()->clone();
    this->setContentSize(view->getContentSize());
    view->setPosition(Point::ZERO);
    this->addChild(view);
    
    seekChildByNameWithRetType<ui::TextBMFont*>(view, "nameTxt")->setString(_data["role"].asValueMap()["name"].asString());
    seekChildByNameWithRetType<ui::TextBMFont*>(view, "number")->setString("x" + _data["dead"].asString());
    
	seekChildByNameWithRetType<ui::TextBMFont*>(view, "life")->setString(data::Lang::getInstance()->getWord(110201) + ": " + _data["role"].asValueMap()["life"].asString());
    
	seekChildByNameWithRetType<ui::TextBMFont*>(view, "skill")->setString(data::Lang::getInstance()->getWord(110202) + ": " + _data["role"].asValueMap()["skill"].asString());
    
	seekChildByNameWithRetType<ui::TextBMFont*>(view, "power")->setString(data::Lang::getInstance()->getWord(110203) + ": " + _data["role"].asValueMap()["power"].asString());
    
	seekChildByNameWithRetType<ui::TextBMFont*>(view, "speed")->setString(data::Lang::getInstance()->getWord(110204) + ": " + _data["role"].asValueMap()["speed"].asString());
    
    //星级..
    for (auto i = 1; i < _data["role"].asValueMap()["level"].asInt(); i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(true);
    }
    for (auto i = _data["role"].asValueMap()["level"].asInt() + 1; i < 6 ; i++)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "star"+Value(i).asString())->setVisible(false);
    }
    seekChildByNameWithRetType<ui::ImageView*>(this, "nameBack")->setTouchEnabled(true);
    seekChildByNameWithRetType<ui::ImageView*>(this, "nameBack")->addTouchEventListener(CC_CALLBACK_2(ListItem::nameBtnCall, this));
    
    seekChildByNameWithRetType<ui::Layout*>(this, "infoButtonArea")->addTouchEventListener(CC_CALLBACK_2(Cemetery::ListItem::infoBtnCall, this));
    
    return true;
}

void Cemetery::ListItem::nameBtnCall(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType touchType)
{
    if (touchType == ui::Layout::TouchEventType::ENDED)
    {
        _delegate->revival(_data["role"].asValueMap());
    }
}

void Cemetery::ListItem::infoBtnCall(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType touchType)
{
    if (touchType == ui::Layout::TouchEventType::BEGAN)
    {
        auto light = seekChildByNameWithRetType<ui::ImageView*>(this, "infolight");
        light->setVisible(true);light->setOpacity(255);
    }
    if (touchType == ui::Layout::TouchEventType::MOVED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "infolight")->setVisible(false);
    }
    if (touchType == ui::Layout::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::ImageView*>(this, "infolight")->runAction(Sequence::create(FadeOut::create(0.3),NULL) );
        
        ListItem::showInfo();
    }
}

void Cemetery::ListItem::showInfo()
{
    auto light = seekChildByNameWithRetType<ui::ImageView*>(this, "infolight");
    if (light)
    {
        light->setVisible(false);
    }
    auto rodata = _data["role"].asValueMap();
    
    std::string summary =  rodata["summary"].asString();
    std::string info =  summary+ "\n";
    if (rodata["effect"].isNull()|| rodata["effect"].asValueMap()["name"].getType() == Value::Type::NONE) {
        info = info + data::Lang::getInstance()->getWord(110205) + ": "+"无 \n";
    }
    else{
        info = info + data::Lang::getInstance()->getWord(110205) + ": " + rodata["effect"].asValueMap()["name"].asString() + "\n";
    }
    info = info +
		data::Lang::getInstance()->getWord(110202) + ": " + rodata["skill"].asString() + "\n" +
		data::Lang::getInstance()->getWord(110201) + ": " + rodata["life"].asString() + data::Lang::getInstance()->getWord(110203) + ": " + rodata["power"].asString() + data::Lang::getInstance()->getWord(110204) + ": " + rodata["speed"].asString(); +"\n";
    auto area = seekChildByNameWithRetType<ui::ImageView*>(this, "nameBack");
    _delegate->showInfo(info,area);
}

RevivalChoice * RevivalChoice::create(cocos2d::ValueMap data)
{
    auto ret = new RevivalChoice();
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

bool RevivalChoice::init(cocos2d::ValueMap data)
{
    if (!ui::Layout::init())
    {
        return false;
    }
    _data = data;
    
    
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("revival.csb");
    view->setContentSize(win);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    seekChildByNameWithRetType<ui::Text*>(view, "heroname")->setString(_data["name"].asString());
    
    //适配按钮状态..
    ui::Button* revivalBtn  = view->getChildByName("Panel")->getChildByName<ui::Button*>("revivalBtn");
    ui::Text*   revivalText = view->getChildByName("Panel")->getChildByName<ui::Text*>("Text_3");
    ui::Text*   revivalText2 = view->getChildByName("Panel")->getChildByName<ui::Text*>("Text_1_0");
    ui::Text*   revivalconsume = view->getChildByName("Panel")->getChildByName<ui::Text*>("revivalconsume");
    ui::Layout* revivalBtnArea = view->getChildByName("Panel")->getChildByName<ui::Layout*>("revivalBtnArea");
    
    ui::Button* buryBtn     = view->getChildByName("Panel")->getChildByName<ui::Button*>("buryBtn");
    ui::Text*   buryText1   = view->getChildByName("Panel")->getChildByName<ui::Text*>("Text_3_0");
    ui::Text*   buryText2   = view->getChildByName("Panel")->getChildByName<ui::Text*>("Text_1_0_0");
    ui::Text*   buryget     = view->getChildByName("Panel")->getChildByName<ui::Text*>("buryget");
    ui::Layout* buryBtnArea = view->getChildByName("Panel")->getChildByName<ui::Layout*>("buryBtnArea");
    
    if(_data["rid"].asInt()== Cemetery::BUYHERO_ID1||_data["rid"].asInt()== Cemetery::BUYHERO_ID2||_data["rid"].asInt()== Cemetery::BUYHERO_ID3
       ||_data["rid"].asInt()== Cemetery::BUYHERO_ID4||_data["rid"].asInt()== Cemetery::BUYHERO_ID5)
    {
        revivalBtn->setPosition(buryBtn->getPosition());
        revivalText->setPosition(buryText1->getPosition());
        revivalText2->setPosition(buryText2->getPosition());
        revivalconsume->setPosition(buryget->getPosition());
        revivalBtnArea->setPosition(buryBtnArea->getPosition());
        
        buryBtn->setVisible(false);
        buryBtn->setEnabled(false);
        buryText1->setVisible(false);
        buryText2->setVisible(false);
        buryget->setVisible(false);
        buryBtnArea->setTouchEnabled(false);
    }
    //ui::Button* quitBtn
    
    
    revivalBtnArea->addTouchEventListener(CC_CALLBACK_2(RevivalChoice::revivalBtnCall, this));
    buryBtnArea->addTouchEventListener(CC_CALLBACK_2(RevivalChoice::buryBtnCall, this));
    seekChildByNameWithRetType<ui::Layout*>(view, "quitBtnArea")->addTouchEventListener(CC_CALLBACK_2(RevivalChoice::quitBtnCall, this));
    
    return true;
}

void RevivalChoice::revivalBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "revivalBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        revivalReal();
    }
}

void RevivalChoice::revivalReal()
{
    bool ret = data::Role::getInstance()->revive(_data["rid"].asInt());
    if (ret)
    {
		Notice::getInstance()->postBoard(data::Lang::getInstance()->getWord(210104) + _data["name"].asString() + data::Lang::getInstance()->getWord(210101));
        _delegate->updateItems();
    }
    else
    {
		Notice::getInstance()->postTips(data::Lang::getInstance()->getWord(210102));
    }
    this->removeFromParent();
}

void RevivalChoice::buryBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "buryBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        buryReal();
    }
}

void RevivalChoice::buryReal()
{
    auto ret = data::Role::getInstance()->bury(_data["rid"].asInt());
	if (ret) Notice::getInstance()->postBoard(data::Lang::getInstance()->getWord(210103) + data::Lang::getInstance()->getWord(210104) + _data["name"].asString());
    else CCLOG("bury fail");
    _delegate->updateItems();
    this->removeFromParent();
}

void RevivalChoice::quitBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        seekChildByNameWithRetType<ui::Layout*>(this, "quitBtnArea")->setTouchEnabled(false);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        quitReal();
    }
}

void RevivalChoice::quitReal()
{
    this->removeFromParent();
}
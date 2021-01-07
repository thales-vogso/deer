/****************************************************************************
 
 *	@desc	随机事件
 *	@date	2015-5-20
 *	@author	110101
     
 *	@file	manor/Sudden.h
 *  @modify	null
 ******************************************************************************/

#include "Sudden.h"
#include "utils/DeerUtils.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "popUp/BlackMask.h"
#include "data/Task.h"
#include "manor/Manor.h"
#include "notice/Notice.h"
#include "data/Lang.h"

USING_NS_CC;

Sudden *Sudden::create(cocos2d::ValueMap data)
{
    auto ret = new Sudden();
    if (ret && ret->init(data))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return ret;
    }
}
void Sudden::onEnter()
{
    Layout::onEnter();
    addChild(Notice::getInstance()->getTips());
}
Sudden::~Sudden()
{
    Manor::getInstance()->showSuddenAfferInterval();
}

bool Sudden::init(cocos2d::ValueMap data)
{
    if (!ui::Layout::init())
    {
        return false;
    }
    
    _data = data;
    
    addChild(BlackMask::create());
    
	_view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("sudden.csb");
	_view->setContentSize(Director::getInstance()->getWinSize());
	this->addChild(_view);
	ui::Helper::doLayout(_view);
    
	_view->getChildByName("panel")->getChildByName<ui::Text*>("cueText")->setString(_data["sudden"].asValueMap()["cue"].asString());
	_view->getChildByName("panel")->getChildByName<ui::Text*>("messageText")->setString(_data["sudden"].asValueMap()["message"].asString());

	_view->getChildByName("panel")->getChildByName("confirm")->getChildByName<ui::Button*>("btn")->addTouchEventListener(CC_CALLBACK_2(Sudden::onConfirmClick, this));
	std::string confirmBtnText("");
	if (_data["sudden"].asValueMap()["iid"].asInt() == 0){
		confirmBtnText = data::Lang::getInstance()->getWord(120201);
	}
	else{
		confirmBtnText.append(_data["sudden"].asValueMap()["num"].asString());
		confirmBtnText.append(_data["sudden"].asValueMap()["item"].asValueMap()["name"].asString());
		
	}
	_view->getChildByName("panel")->getChildByName("confirm")->getChildByName<ui::Text*>("text")->setString(confirmBtnText);
    
	int rmb = _data["sudden"].asValueMap()["rmb"].asInt();
	if (rmb == 0){
		_view->getChildByName("panel")->getChildByName("confirm")->setPositionX(_view->getChildByName("panel")->getChildByName("cancel")->getPositionX());
		_view->getChildByName("panel")->getChildByName("rmb")->setVisible(false);
	}
	else{
		_view->getChildByName("panel")->getChildByName("confirm")->setPositionX(confirmBtnInitX);
		_view->getChildByName("panel")->getChildByName("rmb")->setVisible(true);
		std::string rmbStr("");
	//	rmbStr.append(data::Lang::getInstance()->getWord(110101));
		rmbStr.append(_data["sudden"].asValueMap()["rmb"].asString());
		rmbStr.append(data::Lang::getInstance()->getWord(110104));
		_view->getChildByName("panel")->getChildByName("rmb")->getChildByName<ui::Text*>("text")->setString(rmbStr);
	}
	_view->getChildByName("panel")->getChildByName("rmb")->getChildByName<ui::Button*>("btn")->addTouchEventListener(CC_CALLBACK_2(Sudden::onRMBClick, this));
	_view->getChildByName("panel")->getChildByName("cancel")->getChildByName<ui::Button*>("btn")->addTouchEventListener(CC_CALLBACK_2(Sudden::onCancelClick, this));
	_view->getChildByName("panel")->getChildByName<ui::Text*>("tipText")->setVisible(false);
    return true;
}

void Sudden::onConfirmClick(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if(t == ui::Button::TouchEventType::ENDED)
    {
        confirmExe();
    }
    //BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Sudden::confirmExe, this));
}

void Sudden::confirmExe()
{
	bool res = data::Task::getInstance()->suddenConfirm(_data["tid"].asInt());
	if (res){
		this->removeFromParent();
	}else{
		if (_data["sudden"].asValueMap()["rmb"].asInt() > 0 )
        {
			_view->getChildByName("panel")->getChildByName<ui::Text*>("tipText")->setVisible(true);
			auto act = Sequence::create(FadeIn::create(1), DelayTime::create(2), FadeOut::create(1), CallFunc::create([=](){_view->getChildByName("panel")->getChildByName<ui::Text*>("tipText")->setVisible(false); }), NULL);
			_view->getChildByName("panel")->getChildByName<ui::Text*>("tipText")->runAction(act);
		}else{
			std::string str = _data["sudden"].asValueMap()["item"].asValueMap()["name"].asString();
			str.append(data::Lang::getInstance()->getWord(120202));
			Notice::getInstance()->postTips(str);
			this->removeFromParent();
		}
	}
}

void Sudden::onRMBClick(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if(t == ui::Button::TouchEventType::ENDED)
    {
        RMBExe();
    }
	//BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Sudden::RMBExe, this));
}

void Sudden::RMBExe()
{
    int crystal = data::Item::getInstance()->getUserItemAmountByIID(data::Item::ItemID::CRYSTAL);
    int costcrystal = _data["sudden"].asValueMap()["rmb"].asInt();
    if (crystal >= costcrystal)
    {
        data::Item::getInstance()->userItemReduce(data::Item::ItemID::CRYSTAL, costcrystal);
        data::Task::getInstance()->suddenRMB(_data["tid"].asInt());
        this->removeFromParent();
    }
    else
    {
        std::string str = "钻石数目不足";
        Notice::getInstance()->postTips(str);
    }
}

void Sudden::onCancelClick(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if(t == ui::Button::TouchEventType::ENDED)
    {
        cancelExe();
    }
	//BTN_CALL_ACT(t, sender, sender, CC_CALLBACK_0(Sudden::cancelExe, this));
}

void Sudden::cancelExe()
{
    data::Task::getInstance()->suddenCancel(_data["tid"].asInt());
    this->removeFromParent();
}
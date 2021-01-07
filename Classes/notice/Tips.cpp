/************************************************************
   
*	@desc	提示信息
*	@date	2015-6-12
*	@author	110101
   
*	@file	notice/Tips.cpp
*	@modify	null
*****************************************************/

#include "Tips.h"
#include "core/custom/LibDate.h"
#include "core/custom/LibString.h"
#include "utils/DeerUtils.h"

USING_NS_CC;

Tips::~Tips()
{
	clear();
}

bool Tips::init()
{
    if (!Layout::init())
    {
        return false;
    }
	retain();
	_msgQueue = new MsgQueue();
    return true;
}
void Tips::show(std::string msg, std::string color){
	webdevlib::String str(msg);
	str.trim();
	std::vector<webdevlib::String> vec = str.split("\n");
	for (webdevlib::String s : vec)
	{
		webdevlib::trim(s);
		ValueMap map = ValueMapNull;
		map["msg"] = s;
		map["color"] = color;
		_msgQueue->add(Value(map));
	}
	showExe();
}

void Tips::clear(){
	_msgQueue->clear();
	removeAllChildren();
	stopAllActions();
}
void Tips::showExe(){
    
	if (_msgQueue->size() == 0) return;
    clock_t nowTime = clock();

    float durTime = (float)(((float)nowTime-(float)_lastStartTime)/(float)CLOCKS_PER_SEC);
    
    if (durTime < SHOW_DURATION){
        runAction(Sequence::create(DelayTime::create((float)SHOW_DURATION-durTime), CallFunc::create([=](){showExe(); }), NULL, NULL));
        return;
    }
    
	ValueMap map = _msgQueue->del().asValueMap();
	_lastStartTime = nowTime;
	textMotion(map["msg"].asString(), map["color"].asString());
}
void Tips::textMotion(std::string msg, std::string color){
	webdevlib::trim(msg);
	Size win = Director::getInstance()->getWinSize();
	setContentSize(win);
    
    Label* text = Label::create();
    text->setString(msg);
    text->setSystemFontSize(32);
    text->setColor(DeerUtils::hexToColor3B(color));
    
    ui::ImageView* tipsBg = ui::ImageView::create("xinxilanbeijing.png");
    tipsBg->setScale9Enabled(true);
    tipsBg->setCapInsets(Rect(20,20,20,20));
	
    
    tipsBg->setContentSize(text->getContentSize()+Size(20,20));
    text->setPosition(Vec2(tipsBg->getContentSize().width/2.0f,tipsBg->getContentSize().height/2.0f));
    tipsBg->addChild(text);
    tipsBg->setScale(FONT_SIZE/32.0f);
    tipsBg->setPosition(Vec2(win.width / 2, win.height / 2));
    tipsBg->setOpacity(204);
	addChild(tipsBg);
    Spawn* act1 = Spawn::create(MoveBy::create(3, Vec2(0, -300)), FadeOut::create(3),NULL, NULL);
    Sequence* act2 = Sequence::create(DelayTime::create(3),RemoveSelf::create(), NULL);
	tipsBg->runAction(act1);
    tipsBg->runAction(act2);
}
/****************************************************************************
 
 *	@desc	页面缓冲标识
 *	@date	2015-5-26
 *	@author	110101
     
 *	@file	manor/TranslateScene.cpp
 *  @modify	null
 ******************************************************************************/
#include "Translate.h"
#include "crusade/CrusadeMap.h"
#include "popUp/BlackMask.h"
USING_NS_CC;

Scene* Translate::getTransLateScene()
{
    Scene *ret = Scene::create();
    auto tr = Translate::create();
    tr->setRunAsScene(true);
    ret->addChild(tr);
    return ret;
}

bool Translate::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
    _runAsScene = false;
    
    auto win = Director::getInstance()->getWinSize();
    setContentSize(win);
    _mask = BlackMask::create();
    _mask->setOpacity(0);
    addChild(_mask);
//    setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    
    _in = Sprite::create("loading-nei.png");
    _out = Sprite::create("loading-wai.png");
    
    _in->setPosition(win/2);
    _out->setPosition(win/2);
    
    addChild(_in);
    addChild(_out);
    
    return true;
}

void Translate::onEnter()
{
    ui::Layout::onEnter();
    
    _mask->runAction(FadeIn::create(0.7));
    
    _out->runAction(RepeatForever::create(RotateBy::create(1, 360)));
    
    if (_runAsScene)
    {
        runAction(Sequence::create(DelayTime::create(1), CallFunc::create([](){
            CrusadeMap::goToCrusadeMapScene(0,true);
        }),NULL));
    }
}
RankTranslate* RankTranslate::create(cocos2d::Size size)
{
    RankTranslate* translate = new RankTranslate();
    if(translate&& translate->init(size))
    {
        translate->autorelease();
        return translate;
    }
    else
    {
        delete translate;
        translate = NULL;
        return NULL;
    }
}
bool RankTranslate::init(cocos2d::Size size)
{
    if (!Layout::init())
    {
        return false;
    }
    
    auto win = Director::getInstance()->getWinSize();
    setContentSize(size);
    
    _mask = LayerColor::create(Color4B(0, 0, 0, 0),size.width,size.height);
    this->addChild(_mask);
    _in = Sprite::create("loading-nei.png");
    _out = Sprite::create("loading-wai.png");
    
    _in->setPosition(size/2);
    _out->setPosition(size/2);
    
    addChild(_in);
    addChild(_out);
    return true;
}
void RankTranslate::onEnter()
{
    ui::Layout::onEnter();
    
     _mask->runAction(FadeIn::create(0.7));
     _out->runAction(RepeatForever::create(RotateBy::create(1, 360)));
}
RmbBuyTranslate* RmbBuyTranslate::create(cocos2d::Size size, std::string str)
{
    RmbBuyTranslate* translate = new RmbBuyTranslate();
    if(translate&& translate->init(size, str))
    {
        translate->autorelease();
        return translate;
    }
    else
    {
        delete translate;
        translate = NULL;
        return NULL;
    }
}
bool RmbBuyTranslate::init(cocos2d::Size size, std::string str)
{
    if (!Layout::init())
    {
        return false;
    }
    
    auto win = Director::getInstance()->getWinSize();
    setContentSize(size);
    
  //  _mask = LayerColor::create(Color4B(0, 0, 0, 0),size.width,size.height);
  //  this->addChild(_mask);
    _in = Sprite::create("loading-nei.png");
    _out = Sprite::create("loading-wai.png");
    
    _in->setPosition(size/2);
    _out->setPosition(size/2);
    
   // _tips = ui::Text::create("[请等待，正在发送购买请求...]","",38);
    _tips = ui::Text::create(str,"",38);
    _tips->setAnchorPoint(Vec2(0.5,0.5));
    _tips->setPosition(Vec2(win.width/2,win.height/4));
    addChild(_tips);
    
    addChild(_in);
    addChild(_out);
    return true;
}
void RmbBuyTranslate::onEnter()
{
    ui::Layout::onEnter();
    
  //  _mask->runAction(FadeIn::create(0.7));
    _out->runAction(RepeatForever::create(RotateBy::create(1, 360)));
}

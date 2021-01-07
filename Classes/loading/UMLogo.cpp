/****************************************************************************
 
 *	@desc	logo
 *	@date	2014-12-12
 *	@author	110101
 
 *	@file	Loading/UMLogo.h
 ******************************************************************************/

#include "UMLogo.h"
#include "gateWay/Gateway.h"

USING_NS_CC;
Scene* UMLogo::createScene()
{
    auto scene = Scene::create();
    auto layer = UMLogo::create();
    scene->addChild(layer);
    return scene;
}
bool UMLogo::init()
{
    if (!Layout::init())
    {
        return false;
    }
    auto win = Director::getInstance()->getWinSize();
    Sprite* logo = Sprite::create("Default.png");
	logo->setAnchorPoint(Vec2(0.5, 0.5));
    logo->setScale(Director::getInstance()->getWinSize().width/logo->getContentSize().width*0.6*0.5);
    logo->setPosition(Vec2(win.width/2,win.height/2 * 1.15));
    addChild(logo);
    
    auto spashlogo = CallFuncN::create([=](Ref* ref){
        logo->runAction(Sequence::create(FadeIn::create(0.5f),DelayTime::create(1.5f),FadeOut::create(0.3f), NULL));
    });
    auto act = CallFunc::create([=](){
        auto scene = cocos2d::Scene::create();
        auto layer = Gateway::create(false);
        scene->addChild(layer);
        
        // run
        Director::getInstance()->replaceScene(scene);
    });
    
    runAction(Sequence::create(spashlogo, DelayTime::create(1.5f),act, NULL));
    
    return true;
}
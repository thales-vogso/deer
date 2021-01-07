/****************************************************************************
 *	@desc   触摸效果
 *	@date	2015-4-22
 *	@author	110101
     
 *	@file	popUp/TouchEffect.h
 *   @modify	null
 ******************************************************************************/
#include "TouchEffect.h"
USING_NS_CC;

bool TouchEffect::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchlis = EventListenerTouchOneByOne::create();
    _touchlis->setEnabled(true);
    _touchlis->setSwallowTouches(false);
    _touchlis->onTouchBegan = CC_CALLBACK_2(TouchEffect::onTouchBegan, this);
    _touchlis->onTouchMoved = CC_CALLBACK_2(TouchEffect::onTouchMoved, this);
    _touchlis->onTouchEnded = CC_CALLBACK_2(TouchEffect::onTouchEnded, this);
    _touchlis->onTouchCancelled = CC_CALLBACK_2(TouchEffect::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchlis, this);
    
    return true;
}

bool TouchEffect::onTouchBegan(cocos2d::Touch* t, cocos2d::Event *e)
{
//    auto par = ParticleSystemQuad::create("deertouch.plist");
//    par->setScale(2);
//    par->setAutoRemoveOnFinish(true);
//    par->setPosition(t->getLocation());
//    this->addChild(par);
    auto iter = std::find_if(_pars.begin(), _pars.end(), [](ParticleSystemQuad* p)->bool{
        return !p->isActive();
    });
    
    if (iter != _pars.end())
    {
        (*iter)->setPosition(t->getLocation());
        (*iter)->resetSystem();
    }
    else
    {
        auto par = ParticleSystemQuad::create("deertouch.plist");
        par->setScale(2);
        par->setPosition(t->getLocation());
        this->addChild(par);
        _pars.push_back(par);
    }
    
    return true;
}

void TouchEffect::onTouchMoved(cocos2d::Touch *t, cocos2d::Event *e)
{
//    if (t->getLocation().distance(t->getPreviousLocation()) > 10)
//    {
//        auto par = ParticleSystemQuad::create("deertouch.plist");
//        par->setScale(2);
//        par->setAutoRemoveOnFinish(true);
//        par->setPosition(t->getLocation());
//        this->addChild(par);
//    }
    
    ParticleSystemQuad *p = _pars.front();
    float delta = p->getPosition().distance(t->getLocation());
    for (int i(1); i < _pars.size(); ++i)
    {
        if (_pars.at(i)->isActive())
        {
            float distance = _pars.at(i)->getPosition().distance(t->getLocation());
            if (distance < delta)
            {
                delta = distance;
                p = _pars.at(i);
            }
        }
    }
    
    p->setPosition(t->getLocation());
//    _par->setPosition(t->getLocation());
}

void TouchEffect::onTouchEnded(cocos2d::Touch *t, cocos2d::Event *e)
{
//    if (_par)
//    {
//        _par->stopSystem();
//        _par->removeFromParent();
//        _par = nullptr;
//    }
    for (auto &i : _pars)
    {
        i->stopSystem();
    }
}

void TouchEffect::onTouchCancelled(cocos2d::Touch *t, cocos2d::Event *e)
{

}

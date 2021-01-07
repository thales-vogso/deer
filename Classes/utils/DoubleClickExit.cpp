/****************************************************************************
 *	@desc   双击退出
 *	@date	2015-4-25
 *	@author	110101
     
 *	@file	utils/DoubleClickExit.cpp
 *   @modify	null
 ******************************************************************************/

#include "DoubleClickExit.h"
#include "notice/Notice.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "MobClickCpp.h"
#endif
USING_NS_CC;
bool DoubleClickExit::init()
{
    _prequit = false;
    
    auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
        {
            if (_prequit)
            {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                umeng::MobClickCpp::end();
#endif
                Director::getInstance()->end();
            }
            else
            {
                _prequit = true;
				Notice::getInstance()->postTips("再按一次退出游戏  ");
            }
            if (_prequit)
            {
                runAction(Sequence::create(DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(DoubleClickExit::purgePrequit, this)), NULL));
            }
		}
	};
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void DoubleClickExit::purgePrequit()
{
    _prequit = false;
}
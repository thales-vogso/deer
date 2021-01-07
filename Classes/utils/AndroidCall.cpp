/****************************************************************************
 *	@desc   android回调
 *	@date	2015-5-5
 *	@author	110101
     
 *	@file	utils/AndroidCall.cpp
 *  @modify null
 ******************************************************************************/

#include "AndroidCall.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif
USING_NS_CC;
extern "C"
{
void phoneVibrate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "vibrate", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
#endif
}
}



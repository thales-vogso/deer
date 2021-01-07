/****************************************************************************
  
 *	@desc	鹿纪元公用功能
 *	@date	2015-1-8
 *	@author	110101
     
 *	@file	DeerUtils/DeerUtils.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_DEERUTILS__
#define __DEER_DEERUTILS__
#include "cocos2d.h"
#include "utils/ButtonAction.h"
#include "core/web/Client.h"
#include "cocos-ext.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "platform/android/jni/JniHelper.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "Ios_GlobleMethod.h"
#endif

#define USING_NS_DU                     using namespace DeerUtils
#define DEERUTILS_BEGIN                 namespace DeerUtils{
#define DEERUTILS_END                   }

#define UTF8_ASCII(byte) (((unsigned char)(byte)>=0x00)&&((unsigned char)(byte)<=0x7F))
#define UTF8_FIRST(byte) (((unsigned char)(byte)>=0xC0)&&((unsigned char)(byte)<=0xFD))
#define UTF8_OTHER(byte) (((unsigned char)(byte)>=0x80)&&((unsigned char)(byte)<=0xBF))

DEERUTILS_BEGIN

#define TIME_NULL "0000-00-00 00:00:00"

#define MAP_CANT_FIND(_MAP_, _KEY_) assert(!_MAP_[_KEY_].isNull())

/**
 *  在节点树中按名字来寻找节点
 *
 *  @param root 根节点
 *  @param name 节点名称
 *
 *  @return 找到返回对应节点，找不到返回null
 */
cocos2d::Node* seekChildByName(cocos2d::Node* root, const std::string& name);

template <class RET>
RET seekChildByNameWithRetType(cocos2d::Node* root, const std::string& name)
{
    auto ret = dynamic_cast<RET>(seekChildByName(root, name));
    
    CCASSERT(ret, "Can't get Object with the name or the type");
    
    return ret;
}

/**
 *  根据星级获取对应的颜色值
 *
 *  @param nLevel 星级
 *
 *  @return 对应的颜色值，默认是白色
 */
cocos2d::Color3B hexToColor3BByLevel(int nLevel);

/**
 *  根据16进制颜色值获取颜色值color3b
 *
 *  @param 16进制颜色值
 *  
 *  @return 对应的color3b颜色值
 */
cocos2d::Color3B hexToColor3B(std::string str);

/**
 *  网络请求返回结果比较
 *
 *  @param  rapidjson::Document 网络返回结果
 *  @param  int 返回结果标签
 *
 *  @return bool  是否相同
 */
bool reqResultCompare(rapidjson::Document &data,int resultCode);

/**
 *  根据字体大小，文本框宽度，来计算文本行数
 *
 *  @param char*    str         文本内容
 *  @param long     length      文本占位符长度
 *  @param int      fontSize    字体大小
 *  @param int      textWidth   文本框宽度
 *  
 *  @return int     文本行数
 */
int getTextRowNum(char* str,long length,int fontSize,int textWidth);
/**
 *  根据换行符来计算文本行数
 *
 *  @param char*    str         文本内容
 *
 *  @return int     文本行数
 */
int getTextRowNum(char* str,long length);

/**
 *  根据两点，计算以第一个点为原点的角度（0-90）
 *
 *  @param Vec2 point1  
 *  @param Vec2 point2
 *  
 *  @return float angle
 */
float getAngleTwoPoint(cocos2d::Vec2 point1,cocos2d::Vec2 point2);

/**
 *  在文本某个字节处插入换行符
 *
 *  @param std::string    str         文本内容
 *  @param int      point       插入文本位置
 *
 *  @return char*   结果文本
 */
std::string insertEnterInChineseStr(std::string str,int point);

std::string autoKeepToString(float num);

static const cocos2d::Size deerResouseSize(960, 1280);
static const float BoardBoxHeight(314);
static const float BoardBoxMoveDestance(390);
static const float BoardBoxMoveDuration(0.3);

static const cocos2d::Color4B ShotageColor(112, 114, 127, 255);
static const cocos2d::Color4B EnoughColor(233, 234, 208, 255);

DEERUTILS_END
#endif /* defined(__deer__DeerUtils__) */

/****************************************************************************
        
 *	@desc	鹿纪元全局宏
 *	@date	2015-4-23
 *	@author	110101
     
 *	@file	BlobalMacros.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_GLOBALMACROS__
#define __DEER_GLOBALMACROS__
#include "SimpleAudioEngine.h"
#include "core/custom/LibDate.h"

/**
 *  如果为 1 游戏启动会显示从数据库中读取的信息的行号
 */
#define DEBUG_BOARD_READ_NUM 0

/**
 *  1:当没有虫洞时免费回城
 */
#define DEBUG_GOMANOR_FREE 0


/**
 *  这个宏为1则我方不会miss
 */
#define DEBUG_ACCURANCE 0

/**
 *  0 正常
 *  1 显示触及范围
 */
#define DG_SHOW_BTN_AREA 0

/**
 *  新手引导开关
 *  
 *  true 开启新手引导
 *  false 关闭新手引导
 *  开启新手引导的时候注意清理数据库，或者重新安装
 */
#define DEBUG_SHOW_GUID true

#define SET_LAYOUT_SHOW(_ly_)\
_ly_->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);\
_ly_->setBackGroundColor(Color3B::BLUE);\
_ly_->setBackGroundColorOpacity(100);

/**
 *  屏幕相关常量
 */
#define SCREEN_HEIGHT   1280.0f
#define SCREEN_WIDTH    960.0f

#define LONG_PRESSED_TIME 0.7

#define SCALEX          Director::getInstance()->getWinSize().width/SCREEN_WIDTH
#define SCALEY          Director::getInstance()->getWinSize().height/SCREEN_HEIGHT

/**
*  技能冷却，单位毫秒
*/
enum CooldownMicroSec
{
	kMINT = 300,
	kGATHER = 10000,
	kGAIN = 20000,
	kBread = 1500
};


/**
*   返回类按钮的触及效果中向箭头方向的移动常数
*/
#define BACKBTN_ACT_DISTANCE 20


#define MANOR_BUTTON_OPACITY 140

/**
*  背景音乐的放缩因子
*/
//#define BACKGROUN_MUSIC_FACTOR 0.3

/**
 *  这个宏会在运行时输出所在类型的名字 再gcc或者clang条件下，会输出P(表示this为一个指针)＋名字长度＋类名称
 */
#define LOGTYPENAME CCLOG("%s",typeid(this).name());

#endif

/********************************************
 *	@desc   战斗界面
 *	@date	2015-5-18
 *	@author	110101
     
 *	@file	crusade/CrusadeMapPopupManager.h
 *  @modify	null
 ********************************************/


/**
 *  弹层管理器
 这个界面可能出现的弹窗：
 1、建筑进入选择界面
 
 2、战斗选择界面
 
 3、墓园界面
 
 4、野外商店界面
 
 5、英雄酒馆界面

 6、庇护所界面
 
 7、传送门界面
 */

#ifndef __DEER_CRUSADEMPAPOPUPMANAGER__
#define __DEER_CRUSADEMPAPOPUPMANAGER__

#include "cocos2d.h"

class CrusadeMap;

class CrusadeMapPopupManager : public cocos2d::Node
{
public:
    /**
     *  create
     *
     *  @param _target 管理对象
     *
     *  @return
     */
    static CrusadeMapPopupManager* create(CrusadeMap* target);
    
    /**
     *  初始化
     *
     *  @return
     */
    bool init(CrusadeMap* target);
    
    /**
     *  展示进入选择界面
     *
     *  @param data 数据
     */
    void showEnterChoice(cocos2d::ValueMap data);
    
    /**
     *  展示战斗选择弹窗
     */
    void showBattleChoice();
    
    /**
     *  展示墓园
     */
    void showCemetery();
    
    /**
     *  展示野外商店
     *
     *  @param data 数据
     */
    void showWildShop(cocos2d::ValueMap data);
    
    /**
     *  野外英雄界面的展示
     */
    void showWildHero();
    
    /**
     *  庇护所界面
     */
    void showShelter();
    
    /**
     *  显示传送门
     */
    void showRecall();
    
    /**
     *  控制对象
     */
    CrusadeMap * _target;
};


#endif /* defined(__deer__CrusadeMapPopupManager__) */

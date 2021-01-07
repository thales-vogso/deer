/****************************************************************************
 *	@desc	战斗逻辑的代理
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleControllerDelegate.h
 ******************************************************************************/

#ifndef __DEER_BATTLECONTROLLERDELEGATE_H__
#define __DEER_BATTLECONTROLLERDELEGATE_H__
/**
 *  这个类中的，虚函数需要被战斗控制类实现
 *  显示层通过调用这个接口中的函数来实现控制逻辑的反馈
 */
class BattleControllerDelegate
{
public:
    virtual void onSureInBuildClearup() = 0;
    
    virtual void onExitFromSettlement() = 0;
    
    virtual void onSureInEnterChoice() = 0;
    
    virtual void onQuitInEnterChoice() = 0;
    
    virtual void onTouchBreadBtn() = 0;
    
    virtual void onPressedAutoBattleBtn() = 0;
};


#endif

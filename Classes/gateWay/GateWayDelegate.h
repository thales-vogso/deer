/****************************************************************************
        
 *	@desc	入口
 *	@date	2015-5-28
 *	@author	110101
     
 *	@file	gateWay/GatewayDelegate.h
 ******************************************************************************/

#ifndef __DEER__GATEWAYDELEGATE__
#define __DEER__GATEWAYDELEGATE__

class GateWayDelegate
{
public:
    /**
     *  游戏初始化，这个函数再loading的onEnter()函数中调用
     */
    virtual void gameInit() = 0;
    
    virtual void gotoManor() = 0;
};

#endif /* defined(__deer__GateWayDelegate__) */

/****************************************************************************
 *	@desc   触摸停止
 *	@date	2015-5-5
 *	@author	110101
     
 *	@file	manor/LayerNoTouchTrans.cpp
 *  @modify null
 ******************************************************************************/

#include "LayerNoTouchTrans.h"
LayerNoTouchTrans::LayerNoTouchTrans()
{
    Manor::getInstance()->setTouchChangeEnable(false);
}

LayerNoTouchTrans::~LayerNoTouchTrans()
{
    Manor::getInstance()->setTouchChangeEnable(true);
}
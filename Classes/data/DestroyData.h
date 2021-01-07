/****************************************************************************
        
 *	@desc	数据销毁
 *	@date	2015-3
 *	@author	110101
     
 *	@file	data/DestroyData.cpp
 ******************************************************************************/
#ifndef __DATA_DESTORYDATA_H__
#define __DATA_DESTORYDATA_H__
#include "cocos2d.h"
#include "data/Task.h"
#include "data/Achievement.h"
#include "data/Building.h"
#include "data/Crusade.h"
#include "data/Game.h"
#include "data/Item.h"
#include "data/Monster.h"
#include "data/Role.h"
#include "data/Stage.h"
#include "data/Talent.h"
#include "data/User.h"
#include "core/db/DBConnector.h"
#include "data/Board.h"

void destroyData();

void recoveryRun();

#endif

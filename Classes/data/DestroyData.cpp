/****************************************************************************
        
*	@desc	数据销毁
*	@date	2015-3
*	@author	110101
    
*	@file	data/DestroyData.cpp
******************************************************************************/
#ifndef __DATA_DESTORYDATA_H__
#define __DATA_DESTORYDATA_H__
#include "DestroyData.h"
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
#include "gateWay/GateWay.h"
#include "data/Board.h"
#include "data/Mall.h"
#include "data/Guid.h"
#include "notice/Notice.h"
#include "manor/Manor.h"
#include "data/Lang.h"
#include "data/Ranking.h"


USING_NS_CC;


void destroyData()
{
    data::Task::destroyInstance();
    data::Achievement::destroyInstance();
    data::Building::destroyInstance();
    data::Crusade::destroyInstance();
    data::Game::destroyInstance();
    data::Item::destroyInstance();
    data::Monster::destroyInstance();
    data::Role::destoyInstance();
    data::Stage::destroyInstance();
    data::Talent::destroyInstace();
    data::User::destroyInstance();
    data::Board::destoryInstance();
    data::Guid::destroyInstance();
    data::Lang::destroyInstance();
    data::Mall::destroyInstance();
    data::Ranking::destroyInstance();
    DBConnector::destroyInstance();
    auto file = FileUtils::getInstance()->getWritablePath();
    FileUtils::getInstance()->removeFile(file + "/save.db");
}

void recoveryRun()
{
    //释放数据之前，先释放界面..
    Manor::destory();
	Notice::destoryInstance();

    destroyData();
    auto scene = Gateway::createScene(true);
    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    
}


#endif // __DATA_USER_H__

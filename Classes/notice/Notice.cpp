/****************************************************************************
       
*	@desc   信息管理器
*	@date	2015-7-7
*	@author	110101
   
*	@file	manor/Notice.cpp
*  @modify	null
 ******************************************************************************/


#include "Notice.h"
#include "data/Board.h"
#include "data/Task.h"
#include "data/Item.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"
#include "core/custom/LibString.h"

USING_NS_CC;

Notice *Notice::_instance = nullptr;

const int Notice::BOARD_Z_ORDER = 50;
const int Notice::TIPS_Z_ORDER = 52;

const std::string Notice::FOLD = "foldBoard";
const std::string Notice::UNFOLD = "unfoldBoard";


Notice::Notice(){
	init();
}

Notice::~Notice()
{
	for (auto pair : _eventPool){
        if(pair.second && pair.first)
        {
            pair.second->removeListener(pair.first);
        }
	}
	_board->release();
    _tips->release();
}

Notice* Notice::getInstance()
{
    if (_instance)
    {
        return _instance;
    }else{
        _instance = new Notice();
        return _instance;
    }
}

void Notice::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
    }
}

bool Notice::init()
{
    //广播初始化，不自动销毁..
	_board = Board::create();
	//侦听fold点击..
	EventListenerCustom* foldListener = _board->addListener(Board::BOARD_FOLD, CC_CALLBACK_1(Notice::onBoardFold, this));
	_eventPool[foldListener] = _board;
	//事件board
	data::Task* task = data::Task::getInstance();
	EventListenerCustom* taskListener = task->addListener(data::Task::TASK_COMPLETE, CC_CALLBACK_1(Notice::onTaskComplete, this));
	_eventPool[taskListener] = task;
	//随机事件sudden
	EventListenerCustom* suddenListener = task->addListener(data::Task::SUDDEN_CONFIRM, CC_CALLBACK_1(Notice::onTaskComplete, this));
	_eventPool[suddenListener] = task;
	//事件board
	data::Item* item = data::Item::getInstance();
	EventListenerCustom* itemListener = item->addListener(data::Item::USER_ITEM_CHANGE, CC_CALLBACK_1(Notice::onItemChange, this));
	_eventPool[itemListener] = item;
	//读取历史记录..
	//Value all = data::Board::getInstance()->getHistory();
	//_board->showHistory(all);
    //提示初始化，不自动销毁..
    _tips = Tips::create();
    return true;
}

Tips* Notice::getTips(){
    if (_tips->getParent()){
        _tips->removeFromParent();
    }
    _tips->removeAllChildren();
	return _tips;
}

Board* Notice::getBoard(bool isMain){
    if (_board->getParent()){
        _board->removeFromParent();
    }
	if (isMain)
		_board->mainMode();
	else
		_board->subMode();
    return _board;
}

void Notice::postBoard(std::string msg){
    /**
     *  数据库记录
     */
	webdevlib::trim(msg);
	if (msg == "") return;
	data::Board::getInstance()->record(msg);
    if(msg.length()>0)
    {
        _board->show(msg);
    }
}

float Notice::getBoardHeight(){
	return _board->getHeight();
}

void Notice::postTips(std::string msg, int type)
{
	webdevlib::trim(msg);
	if (msg == "") return;
	webdevlib::String str(msg);
	str.trim();
	std::vector<webdevlib::String> vec = str.split("\n");
	for (webdevlib::String s : vec)
	{
		webdevlib::trim(s);
		switch (type)
		{
		case kNormal:
			_tips->show(s);
			break;
		case kWarning:
			_tips->warning(s);
			break;
		case kMint:
			_tips->mint(s);
			break;
		default:
			_tips->show(s);
			break;
		}
		
	}
	//_board->show(msg);
}


void Notice::foldNoticeBoard()
{
	ValueMap param = ValueMapNull;
	float height = getBoardHeight();
	param["height"] = height;
	param["fold"] = true;
	dispatch(Notice::FOLD, &param);
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBoard);
}

void Notice::unfoldNoticeBoard()
{
	ValueMap param = ValueMapNull;
	float height = getBoardHeight();
	param["height"] = height;
	param["fold"] = false;
	dispatch(Notice::FOLD, &param);
    SoundManager::getInstance()->playEffect(SoundManager::EffectType::kBoard);
}
void Notice::onBoardFold(EventCustom* e){
	bool isFold = *(bool*)e->getUserData();
	if (isFold){
		CCLOG("fold");
		foldNoticeBoard();
	}else{
		CCLOG("unfold");
		unfoldNoticeBoard();
	}
}
void Notice::onTaskComplete(EventCustom* e){
	Value task = *(Value*)e->getUserData();
    std::string  boardStr = task.asValueMap()["board"].asString().c_str();
    std::string  tipsStr  = task.asValueMap()["tips"].asString().c_str();
	postBoard(boardStr);
	postTips(tipsStr);
}
void Notice::onSuddenConfirm(EventCustom* e){
	Value task = *(Value*)e->getUserData();
	postBoard(task.asValueMap()["board"].asString());
	postTips(task.asValueMap()["tips"].asString());
}
void Notice::onItemChange(EventCustom* e){
	Value item = *(Value*)e->getUserData();
	if (item.asValueMap()["iid"].asInt() == data::Item::COIN && item.asValueMap()["amount"].asInt() > 0)
	{
		std::string str = "得到了:金币 x " + item.asValueMap()["amount"].asString() + "," + data::Lang::getInstance()->getWord(120105) + "  " + item.asValueMap()["total"].asString();
		postTips(str, Notice::kMint);
	}
}
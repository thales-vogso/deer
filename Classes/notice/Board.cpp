/************************************************************
       
*	@desc   board信息板
*	@date	2015-7-6
*	@author	110101
   
*	@file	notice/Board.cpp
*  @modify	null
*****************************************************/

#include "Board.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "data/Board.h"
#include "core/custom/LibString.h"
#include "utils/DeerUtils.h"

USING_NS_CC;

const std::string Board::BOARD_FOLD = "onBoardFold";

Board::~Board()
{
	_msgQueue->clear();
	stopAllActions();
}

bool Board::init()
{
	if (!Widget::init())
	{
		return false;
	}
	retain();
	_view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("board.csb");
	this->addChild(_view);

	_msgQueue = new MsgQueue();

	_list = _view->getChildByName<ui::ListView*>("list");
	_pattern = dynamic_cast<ui::Layout*>(_list->getChildByName("itemNode"));
	_list->setItemModel(_pattern);
	_list->removeItem(1);
	//_list->insertDefaultItem(1);
	_list->setItemsMargin(20);
	getFoldBtn()->setSwallowTouches(false);
	getFoldBtn()->addTouchEventListener(CC_CALLBACK_2(Board::onSwitchBtnClick, this));
	return true;
}
void Board::show(std::string msg){
	webdevlib::trim(msg);
	_msgQueue->add(Value(msg));
	showExe();
}
void Board::showHistory(Value all){
	for (Value row : all.asValueVector()){
		_list->insertDefaultItem(1);
		std::string msg = row.asValueMap()["content"].asString();
        dynamic_cast<ui::Text*>(_list->getItem(1))->setFontName("text.fnt");
		dynamic_cast<ui::Text*>(_list->getItem(1))->setString(msg);
	}
}
void Board::mainMode(){
	_isSubFold = _isFold;
	unFold();
	getFoldBtn()->setVisible(false);
}
void Board::subMode(){
	if (_isSubFold)
		fold();
	else
		unFold();
	getFoldBtn()->setVisible(true);
}
ui::Layout* Board::getFoldBtn(){
	ui::Layout* btn = _view->getChildByName<ui::Layout*>("foldBtn");
	return btn;
}
float Board::getHeight(){
	return _isFold ? 0 : HEIGHT;
}
void Board::fold(){
	MoveTo* move = MoveTo::create(BoardMoveDuration, Vec2(0,  -HEIGHT));
	CallFunc* done = CallFunc::create([&]{
		getFoldBtn()->runAction(RotateTo::create(BoardBtnDuration, 180));
	});
	Sequence* seq = Sequence::create(move, done, NULL, NULL);
	runAction(seq);
}
void Board::unFold(){
	MoveTo* move = MoveTo::create(BoardMoveDuration, Vec2(0, 0));
	CallFunc* done = CallFunc::create([&]{
		getFoldBtn()->runAction(RotateTo::create(BoardBtnDuration, 0));
	});
	Sequence* seq = Sequence::create(move, done, NULL, NULL);
	runAction(seq);
}
void Board::showExe(){
	if (_msgQueue->size() == 0) return;
	if (_msgQueue->isLock()){
		runAction(Sequence::create(DelayTime::create((float)SHOW_DURATION / 1000.0f), CallFunc::create(CC_CALLBACK_0(Board::showExe, this)), NULL, NULL));
		return;
	}
	_msgQueue->lock();
	clock_t nowTime = clock();
	if (nowTime - _lastStartTime < SHOW_DURATION){
		runAction(Sequence::create(DelayTime::create((float)SHOW_DURATION / 1000.0f), CallFunc::create(CC_CALLBACK_0(Board::showExe, this)), NULL, NULL));
		return;
	}
	std::string msg = _msgQueue->del().asString();
	_lastStartTime = nowTime;
	_list->insertDefaultItem(1);
    //adapte item Size
    auto itemNode = _list->getItem(1);
    auto itemText = itemNode->getChildByName("item");
    auto itemImage = itemNode->getChildByName("itemImage");
	dynamic_cast<ui::Text*>(itemText)->setString(msg);
    if(itemText->getContentSize().width >= MAX_BOARD_WIDTH)
    {
        dynamic_cast<ui::Text*>(itemText)->setTextAreaSize(Size(MAX_BOARD_WIDTH,0));
        dynamic_cast<ui::Text*>(itemText)->ignoreContentAdaptWithSize(false);
    }
    itemNode->setContentSize(Size(itemText->getContentSize().width+50,itemText->getContentSize().height+31));
    itemImage->setContentSize(Size(itemText->getContentSize().width+50,itemText->getContentSize().height+31));
    itemImage->setPosition(Vec2(itemNode->getContentSize().width/2,itemNode->getContentSize().height/2));
    itemText->setPosition(Vec2(itemNode->getContentSize().width/2,itemNode->getContentSize().height/2));
    
	float percet = (1.0 - _list->getItem(1)->getPositionPercent().y) * 10.0;
	_list->jumpToPercentVertical(percet);
	_list->scrollToTop(1, true);
	_msgQueue->unlock();
	showExe();
}

void Board::onSwitchBtnClick(Ref *sender, ui::Widget::TouchEventType type){
	ui::ImageView* light = _view->getChildByName("foldBtn")->getChildByName<ui::ImageView*>("light");
	if (type == ui::Widget::TouchEventType::BEGAN){
		light->stopAllActions();
		light->setVisible(true);
		light->setOpacity(255);
	}else if (type == ui::Widget::TouchEventType::MOVED) {
		light->setVisible(false);
	}else if (type == ui::Widget::TouchEventType::ENDED) {
		light->runAction(FadeOut::create(0.3));
		bool isFold;
		if (_isFold){
			unFold();
			_isFold = false;
		}else{
			fold();
			_isFold = true;
		}
		dispatch(BOARD_FOLD, &_isFold);
	}
}

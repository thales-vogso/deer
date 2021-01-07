//
//  MapSummery.cpp
//  deer
//
//  Created by xinhua on 15/6/18.
//
//

#include "MapSummary.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "GlobalMacros.h"

USING_NS_CC;
USING_NS_DU;

MapSummary* MapSummary::create(std::string mapName,std::string txt)
{
    MapSummary* summery = new MapSummary();
    if(summery&& summery->init(mapName, txt))
    {
        summery->autorelease();
        return summery;
    }
    else{
        delete summery;
        summery = NULL;
        return NULL;
    }
}

bool MapSummary::init(std::string mapName, std::string txt)
{
    if(!Layer::init())
        return false;
    Size win = Director::getInstance()->getWinSize();
    
    _view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("xinxiyeqian.csb");
    
    ui::ImageView* Image_1 = _view->getChildByName<ui::ImageView*>("Image_1");
    Image_1->setContentSize(Size(Image_1->getContentSize().width,Image_1->getContentSize().height*win.height/SCREEN_HEIGHT));
    ui::Text* mapText = _view->getChildByName<ui::Text*>("mapName");
    ui::Text* text1   = _view->getChildByName<ui::Text*>("Text_1");
    ui::Text* text2   = _view->getChildByName<ui::Text*>("Text_2");
    ui::Text* text3   = _view->getChildByName<ui::Text*>("Text_3");
    ui::Text* text4   = _view->getChildByName<ui::Text*>("Text_4");
    ui::Text* text5   = _view->getChildByName<ui::Text*>("Text_5");
    ui::Text* text6   = _view->getChildByName<ui::Text*>("Text_6");
    ui::Text* text7   = _view->getChildByName<ui::Text*>("Text_7");
    ui::Text* text8   = _view->getChildByName<ui::Text*>("Text_8");
    text1->setOpacity(0);
    text2->setOpacity(0);
    text3->setOpacity(0);
    text4->setOpacity(0);
    text5->setOpacity(0);
    text6->setOpacity(0);
    text7->setOpacity(0);
    text8->setOpacity(0);

    _view->setContentSize(win);
    this->addChild(_view);

    ui::Helper::doLayout(this);
    
    _text = txt;
    
    
    mapText->setString(mapName);
    
    
    
    _textVector.push_back(text1);
    _textVector.push_back(text2);
    _textVector.push_back(text3);
    _textVector.push_back(text4);
    _textVector.push_back(text5);
    _textVector.push_back(text6);
    _textVector.push_back(text7);
    _textVector.push_back(text8);
    
    adaptContensize();
    
    _isRun  =   false;
    _actionIndex    =   0;
    return true;
}
void MapSummary::onEnter()
{
    scheduleUpdate();
    Layer::onEnter();
}
void MapSummary::onExit()
{
    unscheduleUpdate();
    Layer::onExit();
}
void MapSummary::update(float delta)
{

    if(!_isRun)
    {
        _isRun = true;
        this->runAction(Sequence::create(DelayTime::create(delayTime), CallFunc::create(std::bind(&MapSummary::showAction, this)),NULL));
    }
}
void MapSummary::remove()
{
    _view->runAction(FadeOut::create(removeTime));
    this->runAction(Sequence::create(DelayTime::create(removeTime),RemoveSelf::create(), NULL));
}

float MapSummary::getPlayTime()
{
    return delayTime+removeTime+_rowNum*perTextDT+(_rowNum-1)*removeTime;
}

void MapSummary::adaptContensize()
{
    const int len = _text.length();
    char* str = new char[len+1];
    strcpy(str,_text.c_str());
    
    const char* split = "\n";
    char* p;
    p = strtok(str, split);
    
    while(p!=NULL)
    {
        if(_rowNum < MAXROWNUM)
        {
            _textVector.at(_rowNum)->setString(p);
        }
        _rowNum++;
        p = strtok(NULL,split);
    }
}
void MapSummary::showAction()
{
    if(_actionIndex <  _rowNum)
    {
        if(_actionIndex == _rowNum -1)
        {
            CallFunc *calFun = CallFunc::create(std::bind(&MapSummary::remove, this));
            _textVector[_actionIndex]->runAction(Sequence::create(FadeIn::create(perTextDT), DelayTime::create(removeTime),calFun,NULL));
            _actionIndex ++;
        }
        else
        {
            CallFunc *calFun = CallFunc::create(std::bind(&MapSummary::showAction, this));
            _textVector[_actionIndex]->runAction(Sequence::create(FadeIn::create(perTextDT), calFun,NULL));
            _actionIndex ++;
        }
        
    }
   
}

/****************************************************************************
 *	@desc   探险界面
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	manor/VentureLayer.cpp
  
 ******************************************************************************/

#include "Ranking.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "popUp/BlackMask.h"
#include "manor/Manor.h"
#include "GlobalMacros.h"
#include "data/Ranking.h"
#include "data/Lang.h"
#include "sound/SoundManager.h"
USING_NS_CC;
USING_NS_DU;

bool Ranking::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->addChild(BlackMask::create());
    
    auto win = Director::getInstance()->getWinSize();
    
    _view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("rank.csb");
    _view->setContentSize(win);
    this->addChild(_view);
    ui::Helper::doLayout(_view);
    
    auto shutBtn = seekChildByNameWithRetType<ui::Layout*>(_view, "backArea");
    shutBtn->addTouchEventListener(CC_CALLBACK_2(Ranking::shutDownCallback, this));
    _rankTypeBtn = seekChildByNameWithRetType<ui::Button*>(_view, "comrankBtn");
    _rankTypeBtn->addTouchEventListener(CC_CALLBACK_2(Ranking::rankTypeCallback, this));
    _rankTypeBtnText = seekChildByNameWithRetType<ui::Text*>(_view,"comrankText");
    
    _panle       = _view->getChildByName<ui::Layout*>("Panel_1");
    _listView    = _view->getChildByName("Panel_1")->getChildByName<ui::ListView*>("ListView_1");
    auto image   = _view->getChildByName("Panel_1")->getChildByName("Image_10_0");
    _rankText    = _view->getChildByName("Panel_1")->getChildByName<ui::Text*>("scanDegreeTitle");
    
    
    image    ->setPositionY(image->getPositionY() + _listView->getContentSize().height*(1-win.height/SCREEN_HEIGHT));
    
    _listView    ->setContentSize(Size(_listView->getContentSize().width,_listView->getContentSize().height*win.height/SCREEN_HEIGHT));
    
   
    
    _itemPattern = _view->getChildByName("Panel_1")->getChildByName("ListView_1")->getChildByName<ui::Layout*>("Panel_2");
    _itemPattern ->removeFromParent();
    _itemPattern ->retain();
    
    _itemEmpty   = _view->getChildByName("Panel_1")->getChildByName("ListView_1")->getChildByName<ui::Layout*>("Panel_7");
    _itemEmpty   ->removeFromParent();
    _itemEmpty   ->retain();
    
    _reqListener = data::Ranking::getInstance()->addListener(data::Ranking::REQ_RANKINFO, CC_CALLBACK_1(Ranking::reqRankListInfoSeccess, this));
    addEventPool(_reqListener,data::Ranking::getInstance());
    _reRegListener = data::Ranking::getInstance()->addListener(data::Ranking::REQ_REGISTER,CC_CALLBACK_1(Ranking::reqRegister,this));
    addEventPool(_reRegListener,data::Ranking::getInstance());
    
    return true;
}
Ranking::Ranking()
{

}
Ranking::~Ranking()
{
    //data::Ranking::getInstance()->removeListener(_reqListener);
    //data::Ranking::getInstance()->removeListener(_reRegListener);
}
void Ranking::onEnter()
{
    Layer::onEnter();
//    web::Client::sendAndReceive(<#std::string address#>, <#std::string param#>, <#const std::function<void (std::string)> &callback#>)
}
void Ranking::removeSelf()
{
    runAction(Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL));
}

void Ranking::updateProperty()
{

}

void Ranking::showBoard()
{
    _listView->removeAllItems();
    if(data::Ranking::getInstance()->getRankType() == data::Ranking::EXPLORE_RANK)
    {
        //loading
        _translate = RankTranslate::create(_listView->getContentSize());
        _translate->setAnchorPoint(Vec2(0,1));
        _translate->setPosition(_listView->getPosition());
        _panle->addChild(_translate);
        _rankTypeBtnText->setString(data::Lang::getInstance()->getWord(130303));
        _rankText   ->setString(data::Lang::getInstance()->getWord(130305));
        data::Ranking::getInstance()->reqExpRankInfo();
    }
    else if(data::Ranking::getInstance()->getRankType() == data::Ranking::BABIETA_RANK)
    {
        _rankTypeBtnText->setString(data::Lang::getInstance()->getWord(130304));
        _rankText   ->setString(data::Lang::getInstance()->getWord(130306));
    }
    
}
void Ranking::reqRankListInfoSeccess(cocos2d::EventCustom *e)
{
    ValueVector data = *(ValueVector*)e->getUserData();
    upList(data);
}
void Ranking::reqRegister(cocos2d::EventCustom *e)
{
    _register = RankRegister::create();
    _register->setRegisterCallback(CC_CALLBACK_1(Ranking::reqRegCallback, this));
    this->addChild(_register);
}
void Ranking::upList(cocos2d::ValueVector rank)
{
    _listView->pushBackCustomItem(_itemEmpty->clone());
    for (int i = 0; i < rank.size(); i++){
        const ValueMap item = rank[i].asValueMap();
        
        //初始显示..
        ui::Widget* view = _itemPattern->clone();
        //view->setPosition(Point::ZERO);
        ui::Text* rankIndex = view->getChildByName<ui::Text*>("r1");
        rankIndex->setString(item.at("rank").asString());
        
        ui::Text* name      = view->getChildByName<ui::Text*>("n1");
        name->setString(item.at("name").asString());
        
        ui::Text* value     = view->getChildByName<ui::Text*>("s1");
        value->setString(item.at("value").asString());
        
        //用户自身改变颜色..
        if(item.at("uid").asString() == data::Ranking::getInstance()->getUserRankInfo().asValueMap().at("userUid").asString())
        {
            rankIndex->setColor(DeerUtils::hexToColor3B(SELF_COLOR));
            name->setColor(DeerUtils::hexToColor3B(SELF_COLOR));
            value->setColor(DeerUtils::hexToColor3B(SELF_COLOR));
        }
        
        _listView->pushBackCustomItem(view);
    }
    _listView->scrollToTop(0.1,false);
    _translate->removeFromParent();
}
void Ranking::reqRegCallback(std::string str)
{
    CCLOG("callback: %s",str.c_str());
    rapidjson::Document d;
    d.Parse<rapidjson::kNullType>(str.c_str());
    if(d.HasParseError())
    {
        CCLOG("GetParseError %s\n",d.GetParseError());
    }
    if(d.HasMember("code"))
    {
        if(reqResultCompare(d,data::Ranking::REQ_SUCCESS))
        {
            //request rank info again
            CCLOG("register success");
            data::Ranking::getInstance()->reqExpRankInfo();
        }
        else if(reqResultCompare(d,data::Ranking::REGISTER_FAILED))
        {
            //request login
            CCLOG("%s",d["msg"].GetString());
        }
    }
    
}

void Ranking::shutDownCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        Manor::getInstance()->showLayer(Manor::kVenture);
    }
}
void Ranking::rankTypeCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        if(data::Ranking::getInstance()->getRankType() == data::Ranking::EXPLORE_RANK)
        {
            data::Ranking::getInstance()->setRankType(data::Ranking::BABIETA_RANK);
            showBoard();
        }
        else if(data::Ranking::getInstance()->getRankType() == data::Ranking::BABIETA_RANK)
        {
            data::Ranking::getInstance()->setRankType(data::Ranking::EXPLORE_RANK);
            showBoard();
        }
    }
}
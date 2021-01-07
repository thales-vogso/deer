 /****************************************************************************
        
 *	@desc	引导
 *	@date	2015-4-15
 *	@author	110101
     
 *	@file	guide/Guide.cpp
 ******************************************************************************/

#include "Guide.h"
#include "GlobalMacros.h"
#include "data/Task.h"
#include "data/Item.h"
#include "battle/BattleLayer.h"
#include "battle/Monster.h"

USING_NS_CC;

Guide* Guide::create(int LayerType)
{
    Guide* guide = new Guide();
    if(guide && guide->init(LayerType))
    {
        guide->autorelease();
        return guide;
    }
    else
    {
        delete guide;
        guide = NULL;
        return NULL;
    }
}

bool Guide::init(int LayerType)
{
    if (!Layout::init())
    {
        return false;
    }
    
    _layerType = LayerType;
    
    _listener = NULL;
    
    //新手引导监听..
    EventListenerCustom * updateGuid = data::Guid::getInstance()->addListener(data::Guid::UPDATE_GUID, CC_CALLBACK_1(Guide::fingerCheck, this));
	addEventPool(updateGuid, data::Guid::getInstance());
    
    EventListenerCustom * deleteGuid = data::Guid::getInstance()->addListener(data::Guid::DELETE_GUID, CC_CALLBACK_1(Guide::deleteGuid, this));
    addEventPool(deleteGuid, data::Guid::getInstance());
    
    return true;
}
void Guide::addFinishGuid()
{
    /*if(!this->getChildByName("guidFinish"))
    {
        Size winsize = Director::getInstance()->getWinSize();
        this->setContentSize(winsize);
        this->setAnchorPoint(Vec2(0,0));
        this->setPosition(Vec2(0,0));
        cocos2d::ui::Button* _guidFinish = ui::Button::create("ui_button2.png","ui_button2.png","ui_button2.png");
        _guidFinish->setScale9Enabled(true);
        _guidFinish->setCapInsets(Rect(69,69,24,24));
        _guidFinish->setContentSize(Size(190,69));
        _guidFinish->setTitleText("结束引导");
        _guidFinish->setTitleFontSize(38);
        _guidFinish->setScale(SCALEX);
        _guidFinish->addTouchEventListener(CC_CALLBACK_2(Guide::deleteFinger, this));
        _guidFinish->setPosition(Vec2(winsize.width/2.0f,winsize.height/2.0f));
        if(data::Guid::getInstance()->isGuidFinish()||!DEBUG_SHOW_GUID)
        {
            _guidFinish->setVisible(false);
            _guidFinish->setEnabled(false);
        }
        _guidFinish->setName("guidFinish");
        this->addChild(_guidFinish);
    }*/
}
void Guide::insertButton(int key, cocos2d::ui::Layout* value)
{
    if(_btnsAreaMapInMain.at(key)&&value)
    {
        ereaseButton(key);
        _btnsAreaMapInMain.insert(key,value);
         buttonsVector.pushBack(value);
    }
    else if(value)
    {
        _btnsAreaMapInMain.insert(key,value);
        buttonsVector.pushBack(value);
    }
}
void Guide::ereaseButton(int key)
{
    if(_btnsAreaMapInMain.at(key))
    {
        for(int i = 0; i < buttonsVector.size();i++)
        {
            if(buttonsVector.at(i) == _btnsAreaMapInMain.at(key))
            {
                buttonsVector.erase(i);
            }
        }
        _btnsAreaMapInMain.erase(key);
    }
    
}
void Guide::clearButton()
{
    _btnsAreaMapInMain.clear();
}
void Guide::setTouchListener(cocos2d::EventListenerTouchOneByOne* listener)
{
    _listener =listener;
}
void Guide::setMapTouchListener(cocos2d::EventListenerTouchAllAtOnce* listener)
{
    _mapListener = listener;
}
void Guide::setPlayer(cocos2d::Sprite* player)
{
    _player = player;
}
void Guide::setBreadText(cocos2d::ui::Text* bread)
{
    _bread = bread;
}
void Guide::setCity(cocos2d::Sprite *city)
{
    _city = city;
}
void Guide::setMapScene(cocos2d::Scene* scene)
{
    _mapScene = scene;
}
void Guide::setCangkuBack(cocos2d::ui::ImageView* back)
{
    _back = back;
}
void Guide::setCangkuCanSaleIndex(int index)
{
    _canSaleItemIndex = index;
}
void Guide::setAutoBattle(cocos2d::ui::CheckBox* autoBattle)
{
    _autoBattle = autoBattle;
}
void Guide::setBreadBtn(cocos2d::ui::Button* btn)
{
    _breadBtn = btn;
}
void Guide::setSkillBtnVector(cocos2d::Vector<BattleLayer::SkillButton*> skillButtonVec)
{
    _skillButtonVec = skillButtonVec;
}
void Guide::setMonster(Monster* node)
{
    _monster = node;
}
void Guide::setHero(Player* node)
{
    _hero = node;
}
void Guide::removeSelf()
{
    
}
void Guide::updateProperty()
{

}
void Guide::setBattleBtnEnable(bool bRet)
{
    for(auto &i:_skillButtonVec)
    {
        i->setTouchResponse(bRet);
    }
    _breadBtn->setTouchEnabled(bRet);
    _autoBattle->setTouchEnabled(bRet);
}
void Guide::setBattleButtonEnableByBtnId(int btnId)
{
    for(auto &i:_skillButtonVec)
    {
        if(i->getBtnId() == btnId)
        {
            i->setTouchResponse(true);
        }
    }
    if(btnId == data::Guid::BATTLE_AUTO_ATTACK)
    {
        _autoBattle->setTouchEnabled(true);
    }
    if(btnId== data::Guid::BATTLE_RECOVERY)
    {
        _breadBtn->setTouchEnabled(true);
    }
}
void Guide::setButtonEnable(bool bRet)
{
    for(auto &i :_btnsAreaMapInMain)
    {
        ((ui::Layout*)i.second)->setTouchEnabled(bRet);
    }
    if(_listener)
    {
        _listener->setEnabled(bRet);
    }
    if(_mapListener)
    {
        _mapListener->setEnabled(bRet);
    }
}

void Guide::setButtonEnableByBtnId(int btnId)
{
    if(btnId == data::Guid::BUTTON_LAST)//取最后一个按钮可用..
    {
        buttonsVector.at(buttonsVector.size()-1)->setTouchEnabled(true);
    }
    else
    {
        if( _btnsAreaMapInMain.at(btnId))
        {
            _btnsAreaMapInMain.at(btnId)->setTouchEnabled(true);
        }
    }
    
}
void Guide::setPackageBtn(cocos2d::ui::Layout* packageBtn)
{
    _packageBtN = packageBtn;
}
void Guide::setHomeBtn(cocos2d::ui::Layout* homeBtn)
{
    _homeBtn = homeBtn;
}
void Guide::addHomeGuid()
{
    if(_homeBtn&&_bread&&_mapScene)
    {
        if(_finger_Tips)
        {
            
            _finger_Tips->removeFromParent();
            _finger_Tips = nullptr;
            _finger_Tips = Finger_Tips::create(_homeBtn->getParent()->getPosition(),_homeBtn->getParent(), _bread,_mapScene);
            this->addChild(_finger_Tips);
        }
        else
        {
            _finger_Tips = Finger_Tips::create(_homeBtn->getParent()->getPosition(),_homeBtn->getParent(), _bread,_mapScene);
            this->addChild(_finger_Tips);
        }
    }
    
}
void Guide::addGuideTips(int tipsID)
{
    _mapListener->setEnabled(false);
    _packageBtN->setEnabled(false);
    _homeBtn->setEnabled(false);
    if(_mapScene)
    {
        if(_tips)
        {
            _tips->removeFromParent();
            _tips = nullptr;
            if (tipsID == data::Guid::TIPS_HOME && _city) {
                _tips = GuideTips::create(_city->getPosition(),_city, _mapScene,data::Guid::TIPS_HOME);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_BREAD&&_bread)
            {
                _tips = GuideTips::create(_bread->getPosition(),_bread,_mapScene,data::Guid::TIPS_BREAD);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_EXPLORE)
            {
                _tips = GuideTips::create(Vec2(0,0),this, _mapScene,data::Guid::TIPS_EXPLORE);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_SHELTER&&_player)
            {
                _tips = GuideTips::create(_player->getPosition(),_player,_mapScene, data::Guid::TIPS_SHELTER);
                this->addChild(_tips);
            }
            
        }
        else
        {
            if (tipsID == data::Guid::TIPS_HOME&&_city) {
                _tips = GuideTips::create(_city->getPosition(),_city,_mapScene, data::Guid::TIPS_HOME);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_BREAD&&_bread)
            {
                _tips = GuideTips::create(_bread->getPosition(),_bread,_mapScene, data::Guid::TIPS_BREAD);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_EXPLORE)
            {
                _tips = GuideTips::create(Vec2(0,0), this,_mapScene,data::Guid::TIPS_EXPLORE);
                this->addChild(_tips);
            }
            else if(tipsID == data::Guid::TIPS_SHELTER&&_city)
            {
                _tips = GuideTips::create(_city->getPosition(),_city, _mapScene,data::Guid::TIPS_SHELTER);
                this->addChild(_tips);
            }
        }
    }
    
}
void Guide::removeGuideTips()
{
    if(_tips)
    {
        _tips->removeFromParent();
        _tips = nullptr;
    }
}
void Guide::addFinger()
{
    if (_finger)
    {
        _finger->removeFromParent();
        _finger = nullptr;
        
        if (_player) {
            _finger = Finger::create(_player->getPosition(),_player,data::Guid::FINGER_MOVE);
            this->addChild(_finger);
        }
    }
    else{
        if (_player) {
            _finger = Finger::create(_player->getPosition(),_player,data::Guid::FINGER_MOVE);
            this->addChild(_finger);
        }
    }

}

void Guide::addFinger(int btnId,int FingerType)
{
    setButtonEnable(false);
    setButtonEnableByBtnId(btnId);

	if (_finger)
    {
        _finger->removeFromParent();
		_finger = nullptr;
        
        if(btnId == data::Guid::BUTTON_LAST)
        {
            _finger = Finger::create(buttonsVector.at(buttonsVector.size()-1)->getPosition(),buttonsVector.at(buttonsVector.size()-1)->getParent(),FingerType);
            this->addChild(_finger);
        }
        else
        {
            if(btnId == data::Guid::RESOURCE_BUTTON_FIRST)//仓库界面适应..
            {
                Vec2 pos = Vec2(_btnsAreaMapInMain.at(btnId)->getPositionX()+_btnsAreaMapInMain.at(btnId)->getContentSize().width/2.0,_btnsAreaMapInMain.at(btnId)->getPositionY()+_btnsAreaMapInMain.at(btnId)->getContentSize().height/2.0);
                _finger = Finger::create(pos,_btnsAreaMapInMain.at(btnId)->getParent(),FingerType);
                this->addChild(_finger);
            }
            else
            {
                _finger = Finger::create(_btnsAreaMapInMain.at(btnId)->getPosition(),_btnsAreaMapInMain.at(btnId)->getParent(),FingerType);
                this->addChild(_finger);
            }
        }
    }
	else{
		if (btnId == data::Guid::BUTTON_LAST)
		{
			_finger = Finger::create(buttonsVector.at(buttonsVector.size() - 1)->getPosition(), buttonsVector.at(buttonsVector.size() - 1)->getParent(), FingerType);
			this->addChild(_finger);
		}
		else
		{
			if (btnId == data::Guid::RESOURCE_BUTTON_FIRST)//仓库界面适应..
			{
				Vec2 pos = Vec2(_btnsAreaMapInMain.at(btnId)->getPositionX() + _btnsAreaMapInMain.at(btnId)->getContentSize().width / 2.0, _btnsAreaMapInMain.at(btnId)->getPositionY() + _btnsAreaMapInMain.at(btnId)->getContentSize().height / 2.0);
				_finger = Finger::create(pos, _btnsAreaMapInMain.at(btnId)->getParent(), FingerType);
				this->addChild(_finger);
			}
			else
			{
				_finger = Finger::create(_btnsAreaMapInMain.at(btnId)->getPosition(), _btnsAreaMapInMain.at(btnId)->getParent(), FingerType);
				this->addChild(_finger);
			}

		}
	}
}
void Guide::addBattleFinger(int btnId, int FingerType)
{
    setBattleBtnEnable(false);
    setBattleButtonEnableByBtnId(btnId);
    if (_finger)
    {
        _finger->removeFromParent();
        _finger = nullptr;
        if(btnId == data::Guid::BATTLE_AUTO_ATTACK)
        {
            _finger = Finger::create(_autoBattle->getPosition(),_autoBattle->getParent(),FingerType);
            this->addChild(_finger);
        }
        else if(btnId == data::Guid::BATTLE_RECOVERY)
        {
            _finger = Finger::create(_breadBtn->getPosition(),_breadBtn->getParent(),FingerType);
            this->addChild(_finger);
        }
        else
        {
            for(auto &i :_skillButtonVec)
            {
                if(i->getBtnId() == btnId)
                {
                    _finger = Finger::create(i->_view->getPosition(),i->_view->getParent(),FingerType);
                    this->addChild(_finger);
                }
            }
        }
        
 
    }
    else{
      
        if(btnId == data::Guid::BATTLE_AUTO_ATTACK)
        {
            _finger = Finger::create(_autoBattle->getPosition(),_autoBattle->getParent(),FingerType);
            this->addChild(_finger);
        }
        else if(btnId == data::Guid::BATTLE_RECOVERY)
        {
            _finger = Finger::create(_breadBtn->getPosition(),_breadBtn->getParent(),FingerType);
            this->addChild(_finger);
        }
        else
        {
            for(auto &i :_skillButtonVec)
            {
                if(i->getBtnId() == btnId)
                {
                    _finger = Finger::create(i->_view->getPosition(),i->_view->getParent(),FingerType);
                    this->addChild(_finger);
                }
            }
        }
    }
}

void Guide::removeFinger()
{
    //setButtonEnable(true);
    if(_finger)
    {
        _finger->removeFromParent();
        _finger = NULL;
    }
}
void Guide::removeBattleFinger()
{
    if(_finger)
    {
        _finger->removeFromParent();
        _finger = NULL;
    }
}
void Guide::deleteGuid(cocos2d::EventCustom* e)
{
    if(this->getChildByName("guidFinish"))
    {
       this->removeChildByName("guidFinish");
    }
    removeFinger();
    setButtonEnable(true);
}
void Guide::deleteFinger(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType)
{
    if(touchType == ui::Button::TouchEventType::ENDED)
    {
        data::Guid::getInstance()->clearAllGuid();
        removeFinger();
        setButtonEnable(true);
    }
    
}
void Guide::fingerCheck(cocos2d::EventCustom* e)
{
    ValueMap  result = *(ValueMap*)e->getUserData();
    if(_layerType == data::Guid::MAIN_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::GOLD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Task::kAlchemyBtn,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAREHOUSE_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Task::kBuildBtn,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::COLLECTION)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kCollectionBtn,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAREHOUSE_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kWarehouseBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP8)
            {
                addFinger(data::Task::kAlchemyBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP9)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP10)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::SHELTER_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Task::kAlchemyBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USER1)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kCollectionBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::STORE_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                //setButtonEnable(true);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Task::kAlchemyBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Task::kCollectionBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                removeFinger();
                //setButtonEnable(true);
            }
            else if(result["step"].asInt() == data::Guid::STEP8)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP9)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kCollectionBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE2)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kCollectionBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE3)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kCollectionBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::SOLDIERS_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kVentureBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                //setButtonEnable(true);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Task::kBuildBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::TRAINING)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Task::kHeroBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAR)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                //新手引导中免费送15个面包..
                data::Item::getInstance()->userItemPlus(data::Item::FOOD,15);
                addFinger(data::Task::kVentureBtn, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::BUID_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::WAREHOUSE_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Guid::BUILD_BUTTON_WAREHOUSE,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
                //setButtonEnable(true);
                
            }
        }
        else if(result["guidId"].asInt() == data::Guid::COLLECTION)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAREHOUSE_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP6)
            {
                addFinger(data::Guid::BUILD_BUTTON_FARM, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP7)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP8)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP10)
            {
                addFinger(data::Guid::BUILD_BUTTON_FARM, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP11)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::SHELTER_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                if(_btnsAreaMapInMain.at(data::Guid::BUILD_BUTTON_SHELTER) != NULL)
                {
                    addFinger(data::Guid::BUILD_BUTTON_SHELTER, data::Guid::FINGER_CLICK);
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Guid::BUILD_BUTTON_SHELTER, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USER1)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else  if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::STORE_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP2)
            {
                addFinger(data::Guid::BUILD_BUTTON_STORE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP9)
            {
                addFinger(data::Guid::BUILD_BUTTON_STORE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP10)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE2)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE3)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::SOLDIERS_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                addFinger(data::Guid::BUILD_BUTTON_SOLGIER, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP7)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::TRAINING)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BUILD_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::COLLECTION_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::COLLECTION)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_GATHER,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAREHOUSE_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_BACK,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USER1)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_PLANT_ROBOT_ADD, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::STORE_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP6)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_GATHER, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP7)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP8)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_PLANT_ROBOT_DECREASE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_FOOD_ROBOT_ADD, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_FOOD_ROBOT_NAME, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                addFinger(data::Guid::COLLECTION_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP7)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE2)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
               
                addFinger(data::Guid::BUTTON_LAST, data::Guid::FINGER_CLICK);
                
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WORKERS_USE3)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::BUTTON_LAST, data::Guid::FINGER_CLICK);
                
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::RESOUCE_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::WAREHOUSE_USE1)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::RESOURCE_BUTTON_FIRST,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                if(_listener  && _back )
                {
                    setButtonEnable(false);
                    _listener->setEnabled(true);
                    if(_finger == NULL)
                    {
                        _finger = Finger::create(Vec2(_back->getContentSize().width+20,_back->getContentSize().height+50),_back,data::Guid::FINGER_CLICK);
                        this->addChild(_finger);
                    }
                    else
                    {
                        _finger->removeFromParent();
                        _finger = NULL;
                        
                        _finger = Finger::create(Vec2(_back->getContentSize().width+20,_back->getContentSize().height+50),_back,data::Guid::FINGER_CLICK);
                        this->addChild(_finger);
                    }
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAREHOUSE_USE2)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                if(_canSaleItemIndex != -1)
                {
                    setButtonEnable(false);
                    setButtonEnableByBtnId(_canSaleItemIndex);
                    Vec2 pos = Vec2(_btnsAreaMapInMain.at(_canSaleItemIndex)->getPositionX() + _btnsAreaMapInMain.at(_canSaleItemIndex)->getContentSize().width / 2.0, _btnsAreaMapInMain.at(_canSaleItemIndex)->getPositionY() + _btnsAreaMapInMain.at(_canSaleItemIndex)->getContentSize().height / 2.0);
                    if(_finger)
                    {
                        _finger->removeFromParent();
                        _finger = NULL;
                        
                        _finger = Finger::create(pos,_btnsAreaMapInMain.at(_canSaleItemIndex)->getParent(),data::Guid::FINGER_PRESS_ON);
                        this->addChild(_finger);
                    }
                    else
                    {
                        _finger = Finger::create(pos,_btnsAreaMapInMain.at(_canSaleItemIndex)->getParent(),data::Guid::FINGER_PRESS_ON);
                        this->addChild(_finger);
                    }
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::HERO_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::TRAINING)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::HERO_LAYER_BUTTON_GETHERO, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAR)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::HERO_LAYER_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::HERO_POP_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::TRAINING)
        {
            if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Guid::HERO_POP_LAYER_BUTTON_SURE,data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::VENTURE_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::SOLDIERS_BUILD)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::VENTURE_BUTTON_VENTURE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Guid::VENTURE_BUTTON_BACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::WAR)
        {
            if(result["step"].asInt() == data::Guid::STEP3)
            {
                addFinger(data::Guid::VENTURE_BUTTON_TAIKONGFOOD, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP4)
            {
                addFinger(data::Guid::VENTURE_BUTTON_SOLGIER, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP5)
            {
                addFinger(data::Guid::VENTURE_BUTTON_VENTURE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP6)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::MAP_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::MOVE)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger();
                if(_mapListener)
                {
                    _mapListener->setEnabled(true);
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                if(_mapListener)
                {
                    _mapListener->setEnabled(false);
                }
                removeFinger();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::CITY)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                //add tips
                addGuideTips(data::Guid::TIPS_HOME);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                //remove tips
                removeGuideTips();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::FOOD)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                //add tips
                addGuideTips(data::Guid::TIPS_BREAD);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                //remove tips
                removeGuideTips();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::EXPLORE)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                //add tips
                addGuideTips(data::Guid::TIPS_EXPLORE);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                //remove tips
                removeGuideTips();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::MOVE1)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger();
                if(_mapListener)
                {
                    _mapListener->setEnabled(true);
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                if(_mapListener)
                {
                    _mapListener->setEnabled(false);
                }
                removeFinger();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::MOVE2)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger();
                if(_mapListener)
                {
                    _mapListener->setEnabled(true);
                }
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                if(_mapListener)
                {
                    _mapListener->setEnabled(false);
                }
                removeFinger();
                /*_packageBtN->setEnabled(true);
                _homeBtn->setEnabled(true);*/
            }
        }
        else if(result["guidId"].asInt() == data::Guid::SHELTER_GUID)
        {
            if(result["step"].asInt() ==  data::Guid::STEP1)
            {
                //add tips
                addGuideTips(data::Guid::TIPS_SHELTER);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                //remove tips
                removeGuideTips();
                _mapListener->setEnabled(true);
                _packageBtN->setEnabled(true);
                _homeBtn->setEnabled(true);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::HOME_GUID)
        {
            if(result["step"].asInt() ==  data::Guid::STEP1)
            {
                addHomeGuid();
            }
        }
    }
    else if(_layerType == data::Guid::BATTLE_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::ATTACK1)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addBattleFinger(data::Guid::BATTLE_SKILL_1, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeBattleFinger();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::ATTACK2)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addBattleFinger(data::Guid::BATTLE_SKILL_1, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeBattleFinger();
            }
        }
        else if(result["guidId"].asInt() == data::Guid::AUTO_ATTACK)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addBattleFinger(data::Guid::BATTLE_AUTO_ATTACK, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeBattleFinger();
                setBattleBtnEnable(false);
            }
        }
        else if(result["guidId"].asInt() == data::Guid::FOOD_BACK)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addBattleFinger(data::Guid::BATTLE_RECOVERY, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeBattleFinger();
                setBattleBtnEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::DROP_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::DROP_GET)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::DROP_LAYER_GETAAL, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                setButtonEnable(true);
            }
        }
    }
    else if(_layerType == data::Guid::BATTLE_SELECT_LAYER)
    {
        if(result["guidId"].asInt() == data::Guid::BATTLE_SELECT)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BATTLE_SELECT_SURE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }

    }
    else if(_layerType == data::Guid::BATTLE_SELECT_LAYER2)
    {
        if(result["guidId"].asInt() == data::Guid::BATTLE_CHOICE)
        {
            if(result["step"].asInt() == data::Guid::STEP1)
            {
                addFinger(data::Guid::BATTLE_CHOICE_SURE, data::Guid::FINGER_CLICK);
            }
            else if(result["step"].asInt() == data::Guid::STEP2)
            {
                removeFinger();
                //setButtonEnable(true);
            }
        }
    }
    
}
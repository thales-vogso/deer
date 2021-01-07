/****************************************************************************
 *	@desc	战斗控制器
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleController.cpp
 ******************************************************************************/

#include "BattleController.h"
#include "data/Crusade.h"
#include "notice/Notice.h"
#include "data/Guid.h"
USING_NS_CC;

BattleController::~BattleController()
{
    
}

BattleController *BattleController::create(cocos2d::ValueMap data, BattleViewController * viewCon)
{
    BattleController * ret = new(std::nothrow) BattleController();
    if (ret && ret->init(data, viewCon))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool BattleController::init(cocos2d::ValueMap data, BattleViewController * viewCon)
{
    if (!Layer::init())
    {
        return false;
    }
    _viewCon = viewCon;
    _data = data;
    
    /**
     *  初始化模型层..
     */
    st_BaseData mdata;
    parseData(mdata);
    
    _monster = Monster::create(mdata);
    addChild(_monster);
    _player = Player::create();
    addChild(_player);
    
    /**
     *  建立模型层敌对关系
     */
    _monster->setEnemy(_player);
    _player->setEnemy(_monster);
    
    /**
     *  建立模型层和显示层关联
     */
    _monster->setAttackCDEnd(CC_CALLBACK_1(BattleController::onMonsterCDEnd, this));
    _monster->setAttackCDBegan(CC_CALLBACK_1(BattleController::onMonsterCDBegan, this));
    _monster->setAttackEnd(CC_CALLBACK_1(BattleController::onMonsterAttackEnd, this));
    _monster->setMissFun(CC_CALLBACK_0(BattleController::onMonsterMiss, this));
    _monster->setHurtFun(CC_CALLBACK_2(BattleController::onMonsterHurt, this));
    _monster->setStunFunc(CC_CALLBACK_2(BattleController::onMonsterStun, this));
    _monster->setDiedFunc(CC_CALLBACK_1(BattleController::onMonsterDied, this));
    
    _player->setHurtFun(CC_CALLBACK_2(BattleController::onPlayerHurt, this));
    _player->setWarriorsCDBeganFunc(CC_CALLBACK_1(BattleController::onWarriosSkillCDBegan, this));
    _player->setAttackCDEnd(CC_CALLBACK_1(BattleController::onPlayerCDEnd, this));
    _player->setAttackEnd(CC_CALLBACK_1(BattleController::onPlayerAttackEnd, this));
    _player->setDiedFunc(CC_CALLBACK_1(BattleController::onPlayerDied, this));
    _player->setHealFun(CC_CALLBACK_2(BattleController::onPlayerHeal, this));
    _player->setMissFunc(CC_CALLBACK_0(BattleController::onPlayerMiss, this));
    
    if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::AUTO_ATTACK)
       && !data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::FOOD_BACK)&&
       !data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK1)&&
       !data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK2))
    {
        _player->setAttackPause(true);
        _monster->setAttackPause(true);
        data::User::getInstance()->setAutoBattle(false);
    }
    
    /**
     *  触控逻辑
     */
    _viewCon->setSkillButtnFunc(CC_CALLBACK_1(BattleController::onSkillBtnFunc, this));
    
    _aceListener = data::Crusade::getInstance()->addListener(data::Crusade::ACE, CC_CALLBACK_1(BattleController::onAce, this));
    addEventPool(_aceListener,data::Crusade::getInstance());
    
    return true;
}
void BattleController::removeSelf()
{

}
void BattleController::updateProperty()
{

}
Monster* BattleController::getMonster()
{
    return _monster;
}
Player* BattleController::getPlayer()
{
    return _player;
}

void BattleController::onEnter()
{
    Layer::onEnter();
    
    /**
     *  初始化生命值显示
     */
    auto playerData = _player->getBaseData();
    _viewCon->setPlayerHP(playerData._HP, playerData._MaxHp);
    auto monData = _monster->getBaseData();
    _viewCon->setMonsterHP(monData._HP, monData._MaxHp);
    
    /**
     *  进入后延迟1s怪物开始攻击，一面一开始就攻击导致玩家觉得突兀
     */
    runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(BattleController::battleStart, this)), nullptr));
}

void BattleController::battleStart()
{
    _monster->attackCD();
    
    auto buffs = data::Talent::getInstance()->getUserBuff();
	if (!buffs.asValueMap()[Value(data::Talent::kAttackFirst).asString()].isNull() && buffs.asValueMap()[Value(data::Talent::kAttackFirst).asString()].asValueMap()["value"].asInt() == 1)
    {
        _viewCon->setSkillBtnTouch(true);
        if (data::User::getInstance()->isAutoBattle())
        {
            _viewCon->playPlayerAttack();
            //auto act  = CallFunc::create([=](){
                //_player->attack();
            //});
            //this->runAction(Sequence::create(DelayTime::create(0.5),act, NULL));
            
        }
    }
    else
    {
       _player->attackCDAll();
    }
}

/*******   怪物逻辑   ********/
void BattleController::onMonsterCDBegan(CombatBase*)
{
    _viewCon->playMonsterCD(_monster->getBaseData()._interval);
}

void BattleController::onMonsterCDEnd(CombatBase*)
{
    //_monster->attack();
    _viewCon->playMonsterAttack();
}

void BattleController::onMonsterAttackEnd(CombatBase*)
{
    _monster->attackCD();
}

void BattleController::onMonsterMiss()
{
    _viewCon->playMonsterMiss();
}

void BattleController::onMonsterHurt(CombatBase *monster, float hp)
{
    auto mondata = monster->getBaseData();
    _viewCon->playMonsterHurt(hp);
    _viewCon->setMonsterHP(mondata._HP, mondata._MaxHp);
}

void BattleController::onMonsterStun(CombatBase *, float second)
{
    _viewCon->playMonsterStun(second);
}

void BattleController::onMonsterDied(CombatBase *monster)
{
    CCLOG("ATTACK STOP");
    _player->setAttackPause(true);
    _monster->setAttackPause(true);
    
    _viewCon->playMonsterLost();
    _viewCon->playPlayerWin();
    //敌人死亡，战斗引导设为完成..
    data::Guid::getInstance()->FinishBattleGuide();
    
    data::Crusade::getInstance()->killMonster(_data["mid"].asInt());
    auto drops = data::Crusade::getInstance()->getDrops().asValueVector();
    
    if (drops.size() > 0)
    {
        
        runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=](){
            _viewCon->showSettlement(drops);
        }), NULL));
    }
    else
    {
        fightGoOn();
    }
}

/******   玩家逻辑  ******/
void BattleController::onPlayerHurt(CombatBase* player,float hp)
{
    //show guide
    if(data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::AUTO_ATTACK)
       && !data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::FOOD_BACK))
    {
        _player->setAttackPause(true);
        _monster->setAttackPause(true);
        data::User::getInstance()->setAutoBattle(false);
    }
    
    data::Guid::getInstance()->showGuid(data::Guid::FOOD_BACK);

    auto playerdata = player->getBaseData();
    _viewCon->playPlayerHurt(hp);
    _viewCon->setPlayerHP(playerdata._HP, playerdata._MaxHp);
}

void BattleController::onPlayerHeal(CombatBase *player, float hp)
{
    auto playerdata = player->getBaseData();
    _viewCon->playPlayerHeal(hp);
    _viewCon->setPlayerHP(playerdata._HP, playerdata._MaxHp);
}

void BattleController::onWarriosSkillCDBegan(CombatBase* warrior)
{
    //引导中暂停攻击..
    if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK1)
       ||!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK2)
       ||!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::AUTO_ATTACK))
    {
        _player->setAttackPause(true);
        _monster->setAttackPause(true);
    }
    
    auto war = dynamic_cast<Warrior*>(warrior);
    _viewCon->playSkillCD(war->getWid(), war->getBaseData()._interval);
}

void BattleController::onPlayerCDEnd(CombatBase *warrior)
{
    //show guide
    if(!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK1)
       ||!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::ATTACK2)
       ||!data::Guid::getInstance()->getGuideTaskFinishState(data::Guid::AUTO_ATTACK))
    {
        _player->setAttackPause(true);
        _monster->setAttackPause(true);
    }
    data::Guid::getInstance()->showGuid(data::Guid::ATTACK1);
    
    if (data::User::getInstance()->isAutoBattle())
    {
        Warrior* war = dynamic_cast<Warrior*>(warrior);
        _viewCon->playPlayerAttack(war->getWid());
        //auto act  = CallFunc::create([=](){
            //_player->attack(war->getWid());
        //});
        //this->runAction(Sequence::create(DelayTime::create(0.5),act, NULL));
    }
}

void BattleController::onPlayerAttackEnd(CombatBase *warrior)
{
    Warrior* war = dynamic_cast<Warrior*>(warrior);
    _player->attackCD(war->getWid());
}

void BattleController::playerWarriorAttack(int wid)
{
    _viewCon->playPlayerAttack(wid);
    //auto act  = CallFunc::create([=](){
        //_player->attack(wid);
    //});
    //this->runAction(Sequence::create(DelayTime::create(0.5),act, NULL));
}

void BattleController::onPlayerDied(CombatBase *player)
{
    _viewCon->playPlayerLost();
    _viewCon->playMonsterWin();
    
    data::Crusade::getInstance()->ace(data::Crusade::ACE_CAUSE::KILLED);
    Manor::goToManorScene(Manor::kNone);
}

void BattleController::onPlayerMiss()
{
    _viewCon->playPlayerMiss();
}


/***********玩家操作反馈逻辑 *********/
void BattleController::onSureInBuildClearup()
{
    auto cdata = data::Crusade::getInstance()->getUserCrusade().asValueMap();
    CrusadeMap::goToCrusadeMapScene(cdata["sid"].asInt());
};

void BattleController::onExitFromSettlement()
{
    /**
     *  需要跟进玩家的位置是在建筑中还是野外来处理
     */
    if (_data["field"].isNull())
    {
        auto sid = data::Crusade::getInstance()->getUserCrusade().asValueMap()["sid"].asInt();
        CrusadeMap::goToCrusadeMapSceneNoFade(sid);
    }
    else
    {
        fightGoOn();
    }
}

void BattleController::onSureInEnterChoice()
{
    
}

void BattleController::onQuitInEnterChoice()
{
    
}

void BattleController::onPressedAutoBattleBtn()
{
    _viewCon->playPlayerAttack();
    //auto act  = CallFunc::create([=](){
       //_player->attack();
    //});
    //this->runAction(Sequence::create(DelayTime::create(0.5),act, NULL));
}

void BattleController::onTouchBreadBtn()
{
    int drug = data::Item::getInstance()->getDrug();
    _player->heal(drug);
    data::Crusade::getInstance()->consumeByEnhanceLife();
}


/**********其他逻辑***********/

void BattleController::onSkillBtnFunc(int btnid)
{
    //更新手引导..
    _player->setAttackPause(false);
    _monster->setAttackPause(false);
    data::Guid::getInstance()->updateGuidInfo(data::Guid::ATTACK1);
    
    _viewCon->playPlayerAttack(btnid);

}

void BattleController::fightGoOn()
{
    if (!_data["block"].isNull())
    {
        auto next = !data::Crusade::getInstance()->fieldSiege(_data["block"]);
        if (next)
        {
            //这里可能需要强制讲"key"设为true
            _viewCon->showEnterChoice();
        }
        else
        {
            _viewCon->showBuildClearup();
        }
    }
    else
    {
        auto sid = data::Crusade::getInstance()->getUserCrusade().asValueMap()["sid"].asInt();
        CrusadeMap::goToCrusadeMapScene(sid);
    }
}

void BattleController::parseData(st_BaseData &data)
{
    data = st_BaseData(_data["power"].asFloat(), 0, _data["blood"].asFloat(), _data["blood"].asFloat(), 10.0/_data["speed"].asFloat(), _data["speed"].asFloat(), _data["accuracy"].asFloat());
}

void BattleController::onAce(cocos2d::EventCustom *e)
{
    ValueMap data = *(ValueMap*)e->getUserData();
    if (data.at("cause").asInt() == data::Crusade::ACE_CAUSE::KILLED)
    {
        Notice::getInstance()->postBoard("你被击败了，探险队伍迷失在了茫茫宇宙中......");
        std::stringstream txt;
        txt<<"探险死亡: ";
        for (auto  &i : data.at("roles").asValueVector())
        {
            if (i.asValueMap()["amount"].asInt() > 0)
            {
                txt<<i.asValueMap()["name"].asString()<<"X"<<i.asValueMap()["amount"].asInt()<<" ";
            }
        }
        bool addItemlost = false;
        for (auto &i : data.at("items").asValueVector())
        {
            if (i.asValueMap()["amount"].asInt() > 0)
            {
                if (!addItemlost)
                {
                    txt<<"\n探险损失: ";
                    addItemlost = true;
                }
                txt<<i.asValueMap()["name"].asString()<<"X"<<i.asValueMap()["amount"].asInt()<<" ";
            }
        }
		Notice::getInstance()->postBoard(txt.str());
        Manor::goToManorScene(Manor::GoManorType::kNone);
    }
}
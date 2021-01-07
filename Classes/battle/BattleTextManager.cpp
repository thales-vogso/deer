/****************************************************************************
 *	@desc	文字效果管理
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleTextmanager.h
 ******************************************************************************/
#include "BattleTextManager.h"
#include "utils/DeerUtils.h"
#include "data/Lang.h"
USING_NS_CC;
USING_NS_DU;
bool BattleTextManager::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _normalPStep = 2;
    _normalMStep = 2;
    _currentPStep = 0;
    _currentMStep = 0;
    
    auto win = Director::getInstance()->getWinSize();
    
    _topY = win.height - 360;

    _bottomY = 560;
    
    return true;
}

void BattleTextManager::setTargetAndModel(ui::TextBMFont* hurt, ui::TextBMFont* miss, ui::TextBMFont* heal,
                                          ui::TextBMFont* monsterhurt, ui::TextBMFont* monstermiss, ui::TextBMFont* monsterheal)
{
    _models.insert(kHurtModel, hurt);
    _models.insert(kMissModel, miss);
    _models.insert(kHealModel, heal);
    _models.insert(kMonsterHurtModel, monsterhurt);
    _models.insert(kMonsterMissModel, monstermiss);
    _models.insert(kMonsterHurtModel, monsterheal);
    
}
void BattleTextManager::setParticles(cocos2d::ParticleSystemQuad*  monsterParticle,
                  cocos2d::ParticleSystemQuad*  playerParticle)
{
    _monsterParticle = monsterParticle;
    _monsterPos = _monsterParticle ->getPosition();
    _playerParticle  = playerParticle;
    _playerPos  = _playerParticle  ->getPosition();
}

void BattleTextManager::onEnter()
{
    Node::onEnter();
    scheduleUpdate();
}

void BattleTextManager::update(float delta)
{
    Node::update(delta);
    /**
     *  怪物部分
     */
    if (_monsterTxt.empty() ||
        _monsterTxt.back()->getTopBoundary() < _topY - 20)
    {
        if (!_monsterWait.empty())
        {
            addChild(_monsterWait.front());
            _monsterTxt.pushBack(_monsterWait.front());
            _monsterWait.erase(0);
        }
    }

    _currentMStep = _normalMStep * (_monsterWait.size() + 1);
    for (auto &i : _monsterTxt)
    {
        float py = i->getPositionY();
        i->setPositionY(py - _currentMStep);
        py = i->getPositionY();
        //设置透明度
        i->setOpacity(255 - (_topY - py));
        //检测Y值过小就 删除
        if (py < _topY - 255)
        {
            i->removeFromParent();
            _monsterTxt.eraseObject(i);
        }
    }

    
    /**
     *  玩家部分
     */
    _currentPStep = _normalPStep * (_playerWait.size() + 1);

    if (_playerTxt.empty() ||
        _playerTxt.back()->getBottomBoundary() > _bottomY + 20)
    {
        if (!_playerWait.empty())
        {
            addChild(_playerWait.front());
            _playerTxt.pushBack(_playerWait.front());
            _playerWait.erase(0);
        }
    }
    for (auto &i : _playerTxt)
    {
        float py = i->getPositionY();
        i->setPositionY(py + _currentMStep);
        py = i->getPositionY();
        
        //设置透明度
        i->setOpacity(255 - (py - _bottomY));
        
        //检测Y值过大就 删除
        if (py > _bottomY + 255)
        {
            i->removeFromParent();
            _playerTxt.eraseObject(i);
        }
    }

}

void BattleTextManager::addMonsterHurt(float num)
{
    auto txt = _models.at(kMonsterHurtModel)->clone();
    char s[100];
    sprintf(s, data::Lang::getInstance()->getWord(130319).c_str(), num);
    dynamic_cast<ui::TextBMFont*>(txt)->setString(std::string(s));
    txt->setVisible(true);
    _monsterWait.pushBack(txt);
    txt->setPositionY(_topY);
}

void BattleTextManager::addMonsterMiss()
{
    auto txt = _models.at(kMonsterMissModel)->clone();
    txt->setVisible(true);
    _monsterWait.pushBack(txt);
    txt->setPositionY(_topY);
}

void BattleTextManager::addMonsterHeal(float num)
{
    auto txt = _models.at(kMonsterHealModel)->clone();
    char s[100];
    sprintf(s, data::Lang::getInstance()->getWord(130318).c_str(), num);
    dynamic_cast<ui::TextBMFont*>(txt)->setString(std::string(s));
    txt->setVisible(true);
    _monsterWait.pushBack(txt);
    txt->setPositionY(_topY);
    
    _monsterParticle->setPosition(_monsterPos);
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=](){
        _monsterParticle->setPosition(Vec2(-SCREEN_HEIGHT,-SCREEN_HEIGHT));
    }), NULL));
}

void BattleTextManager::addPlayerHurt(float num)
{
    auto txt = _models.at(kHurtModel)->clone();
    char s[100];
    sprintf(s, data::Lang::getInstance()->getWord(130319).c_str(), num);
    dynamic_cast<ui::TextBMFont*>(txt)->setString(std::string(s));
    txt->setVisible(true);
    _playerWait.pushBack(txt);
    txt->setPositionY(_bottomY);
}

void BattleTextManager::addPlayerMiss()
{
    auto txt = _models.at(kMissModel)->clone();
    txt->setVisible(true);
    _playerWait.pushBack(txt);
    txt->setPositionY(_bottomY);
}

void BattleTextManager::addPlayerHeal(float num)
{
    auto txt = _models.at(kHealModel)->clone();
    char s[100];
    sprintf(s, data::Lang::getInstance()->getWord(130318).c_str(), num);
    dynamic_cast<ui::TextBMFont*>(txt)->setString(std::string(s));
    txt->setVisible(true);
    _playerWait.pushBack(txt);
    txt->setPositionY(_bottomY);
    
    _playerParticle->setPosition(_playerPos);
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=](){
        _playerParticle->setPosition(Vec2(-SCREEN_HEIGHT,-SCREEN_HEIGHT));
    }), NULL));
}

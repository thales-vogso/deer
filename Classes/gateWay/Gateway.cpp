/****************************************************************************
 
*	@desc	入口
*	@date	2014-12-12
*	@author	110101
   
*	@file	gateWay/Gateway.cpp
******************************************************************************/

#include "Gateway.h"
#include "data/Crusade.h"
#include "crusade/CrusadeMap.h"
#include "data/Game.h"
#include "data/Task.h"
#include "data/Achievement.h"
#include "data/Building.h"
#include "data/Crusade.h"
#include "data/Item.h"
#include "data/Monster.h"
#include "data/Role.h"
#include "data/Stage.h"
#include "data/Talent.h"
#include "data/User.h"
#include "data/Board.h"
#include "data/Guid.h"
#include "GlobalMacros.h"
#include "base/CCAsyncTaskPool.h"
#include "data/Mall.h"
#include "sound/SoundManager.h"
#include "DebugCommand.h"

USING_NS_CC;

Scene* Gateway::createScene(bool recovery)
{
    auto scene = Scene::create();
    auto layer = Gateway::create(recovery);
    scene->addChild(layer);
    return scene;
}

Gateway::~Gateway()
{
    
}

Gateway* Gateway::create(bool recover)
{
    auto ret = new Gateway();
    if (ret && ret->init(recover))
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

bool Gateway::init(bool recovery)
{
    if (!Layer::init())
    {
        return false;
    }

    _recovery = recovery;
    uiloadCount = 0;
    _totalpicCount = 0;
    _loadModuleCount = 0;
    
    _nextScene = nullptr;
    _userCruInfo = ValueMapNull;
    auto path = FileUtils::getInstance()->getWritablePath();
    
    
    CCLOG("%s",path.c_str());
        
    _load = Loading::create();
    _load->setDelegate(this);
        
    _load->setOnEnterDo(CC_CALLBACK_0(Gateway::gameInit, this));
    this->addChild(_load);
    return true;
}

void Gateway::gameInit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::thread minit(CC_CALLBACK_0(Gateway::initDataModules, this));
    minit.detach();
#else
    initDataModules();
#endif
    initUIModules();
	//命令调试启动..
	DebugCommand::launch();
}

void Gateway::gotoManor()
{
    /**
     *  这里需要初始化音量
     */
//    int bgm = data::User::getInstance()->getBGM();
//    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(bgm) /100 * BACKGROUN_MUSIC_FACTOR);
    
//    int se = data::User::getInstance()->getSE();
//    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(float(se) / 100);
    
    
    if (_nextScene)
    {
        if (_userCruInfo["depart"].asInt() == 1)
        {
            SoundManager::getInstance()->stopMusic();
            SoundManager::getInstance()->playMusic(SoundManager::MusicType::kMap);
//            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BackGroundMusic_Map, true);
        }
        else
        {
            if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
            {
//                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BackGroundMusic_Manor, true);
                SoundManager::getInstance()->playMusic(SoundManager::MusicType::kMap);
            }
        }
        Director::getInstance()->replaceScene(TransitionFade::create(2, _nextScene));
        _nextScene->release();
    }
    else
    {
        runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(Gateway::gotoManor, this)), nullptr));
    }
}

void Gateway::initDataModules()
{
    data::User::getInstance();
    _load->setprogressAdd(2);
    data::Game::getInstance();
    _load->setprogressAdd(2);
    data::Task::getInstance();
    _load->setprogressAdd(2);
    data::Achievement::getInstance();
    _load->setprogressAdd(2);
    data::Building::getInstance();
    _load->setprogressAdd(2);
    data::Crusade::getInstance();
    _load->setprogressAdd(2);
    data::Item::getInstance();
    _load->setprogressAdd(2);
    data::Monster::getInstance();
    _load->setprogressAdd(2);
    data::Role::getInstance();
    _load->setprogressAdd(2);
    data::Stage::getInstance();
    _load->setprogressAdd(2);
    data::Talent::getInstance();
    _load->setprogressAdd(2);
    data::Board::getInstance();
    _load->setprogressAdd(2);
    data::Mall::getInstance();
    _load->setprogressAdd(2);
    //data::Guid::getInstance();
    //_load->setprogressAdd(2);
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(Gateway::loadAlready, this));
}

void Gateway::initUIModules()
{
    ValueVector pic = FileUtils::getInstance()->getValueVectorFromFile("pic.plist");
    _totalpicCount = (int)pic.size();
 
    for (auto &i : pic)
    {
        //if(!Sprite::create(i.asString()))
        //{
           // CCLOG("未找到  %s", i.asString().c_str());
        //}
        Director::getInstance()->getTextureCache()->addImageAsync(i.asString(), CC_CALLBACK_1(Gateway::textureLoadCall, this));
    }
}

void Gateway::textureLoadCall(cocos2d::Texture2D * tex)
{
    ++uiloadCount;
    if (uiloadCount%2 == 0)
    {
        _load->setprogressAdd(1);
    }
    if (_totalpicCount <= uiloadCount + 1)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(Gateway::loadAlready, this));
    }
}

void Gateway::loadAlready()
{
    ++_loadModuleCount;
    if (_loadModuleCount == 2/*初始化的模块总数目为2*/)
    {
        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_OTHER, CC_CALLBACK_0(Gateway::initNextScene, this), nullptr, [&]()
        {
            _userCruInfo = data::Crusade::getInstance()->getUserCrusade().asValueMap();
            
        });
        _load->runRest();
    }
}

void Gateway::initNextScene()
{
    if (_userCruInfo["depart"].asInt() == 1)
    {
        _nextScene = Scene::create();
        CrusadeMap* map = CrusadeMap::create(_userCruInfo["sid"].asInt(),true);
        map->CrusadeRecover();
        _nextScene->addChild(map);
        map->setGuidScene(_nextScene);
    }
    else
    {
        _nextScene = Manor::createScene(Manor::GoManorType::kNone);
    }
    _nextScene->retain();
}
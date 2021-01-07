/****************************************************************************
 *	@desc   声音控制
 *	@date	2015-6-18
 *	@author	110101
     
 *	@file	SoundManager.h
 *  @modify null
 ******************************************************************************/

#include "SoundManager.h"
#include "GlobalMacros.h"
#include "base/CCAsyncTaskPool.h"
#include "data/User.h"
USING_NS_CC;
using namespace CocosDenshion;

#define BACKGROUN_MUSIC_FACTOR 0.3

SoundManager * SoundManager::_instance = nullptr;

SoundManager *SoundManager::getInstance()
{
    if (!_instance)
    {
        _instance = new SoundManager();
        _instance->init();
    }
    return _instance;
}

void SoundManager::destoryInstance()
{
    CC_SAFE_DELETE(_instance);
}

bool SoundManager::init()
{
    
    _sae = SimpleAudioEngine::getInstance();
    _soundMap[kPlayerDie] = "playerdie.MP3";
    _soundMap[kPlayerHurt] = "playerhurt.MP3";
    _soundMap[kPlayerAtk] = "playeratk.MP3";
    _soundMap[kEnemyDie] = "enemydie.MP3";
    _soundMap[kEnemyHurt] = "enemyhurt.MP3";
    _soundMap[kEnemyAtk] = "enemyatk.MP3";
    
    _soundMap[kAlchemy] = "alchemy.MP3";
    _soundMap[kCollection] = "collection.MP3";
    _soundMap[kBoard] = "openboard.MP3";
    _soundMap[kPopUp] = "popup.MP3";
    _soundMap[kUndefine] = "undefine.wav";
    _soundMap[kRecall] = "recall.MP3";
    _soundMap[kNewView] = "newview.wav";
    _soundMap[kOpenwarehouse] = "openwarehouse.MP3";
    _soundMap[kMoveStep1] = "step1.MP3";
    _soundMap[kMoveStep2] = "step2.MP3";
    _soundMap[kFastAdd] = "fastAdd.wav";
    _soundMap[kBuildFinish] = "buidFinish.MP3";
    _soundMap[kLACK] = "lack.wav";
    _soundMap[kHit] = "hit.MP3";
    
    
    _soundMap[kManor] = "bgmap.MP3";//bgmanor.MP3修改为bgmap.MP3
    _soundMap[kBattle] = "bgbattle.MP3";
    _soundMap[kMap] = "bgmap.MP3";
    
    
    for (auto &i : _soundMap)
    {
        if (i.first > 0)
        {
            _sae->preloadEffect(i.second.asString().c_str());
        }
        else if (i.first < 0)
        {
            _sae->preloadBackgroundMusic(i.second.asString().c_str());
        }
    }
    
    _musicVolume = data::User::getInstance()->getBGM();
    _effectVolume = data::User::getInstance()->getSE();
    
    _sae->setBackgroundMusicVolume(volumeFactorHelp(_musicVolume) * BACKGROUN_MUSIC_FACTOR);
    _sae->setEffectsVolume(volumeFactorHelp(_effectVolume));
    
    return true;
}

void SoundManager::setBackgroundMusicVolume(int volume)
{
    _musicVolume = volume;
    _sae->setBackgroundMusicVolume(volumeFactorHelp(volume) * BACKGROUN_MUSIC_FACTOR);
}

void SoundManager::setEffectsVolume(int volume)
{
    _effectVolume = volume;
    _sae->setEffectsVolume(volumeFactorHelp(volume));
}

void SoundManager::saveVolume()
{
    data::User::getInstance()->setBGM(_musicVolume);
    data::User::getInstance()->setSE(_effectVolume);
}

void SoundManager::playMusic(int musicType)
{
    _sae->setBackgroundMusicVolume(volumeFactorHelp(_musicVolume) * BACKGROUN_MUSIC_FACTOR);
    _sae->playBackgroundMusic(_soundMap.at(musicType).asString().c_str(), true);
}
void SoundManager::playMusicFadeIn(int musicType)
{
    static int FadeIndex = 0;
    static float preTime = 0;
    float volumeAdd = (float)data::User::getInstance()->getBGM()/(float)_fadeMaxNum;
    if(FadeIndex == 0)
    {
        _sae->playBackgroundMusic(_soundMap.at(musicType).asString().c_str(), true);
        _sae->setBackgroundMusicVolume(0);
        FadeIndex++;
        playMusicFadeIn(musicType);
    }
    else
    {
        _sae->setBackgroundMusicVolume(volumeFactorHelp(volumeAdd*FadeIndex) * BACKGROUN_MUSIC_FACTOR);
        if(FadeIndex == _fadeMaxNum)
        {
            FadeIndex = 0;
        }
        else
        {
            float time = clock();
            while(time- preTime < 0.01*CLOCKS_PER_SEC)
            {
                time = clock();
            }
            
            preTime = time;
            FadeIndex++;
            playMusicFadeIn(musicType);
        }
    }
    
    
}

void SoundManager::stopMusic()
{
    _sae->stopBackgroundMusic();
}

void SoundManager::stopMusicFadeOut()
{
    static int FadeIndex = 0;
    static float preTime = 0;
    float volumeReduce = (float)data::User::getInstance()->getBGM()/(float)_fadeMaxNum;
    if(FadeIndex == 0)
    {
        _sae->setBackgroundMusicVolume(volumeFactorHelp(_musicVolume) * BACKGROUN_MUSIC_FACTOR);
        FadeIndex++;
        stopMusicFadeOut();
    }
    else
    {
        _sae->setBackgroundMusicVolume(volumeFactorHelp(_musicVolume-volumeReduce*FadeIndex) * BACKGROUN_MUSIC_FACTOR);
        if(FadeIndex == _fadeMaxNum)
        {
            FadeIndex = 0;
        }
        else
        {
            float time = clock();
            while(time- preTime < 0.01*CLOCKS_PER_SEC)
            {
                time = clock();
            }
            
            preTime = time;
            FadeIndex++;
            stopMusicFadeOut();
        }
    }
}

void SoundManager::playEffect(int effectType)
{
    _sae->playEffect(_soundMap.at(effectType).asString().c_str());
}
void SoundManager::playEffect(int effectType,bool loop)
{
    _sae->playEffect(_soundMap.at(effectType).asString().c_str(),loop);
}
void SoundManager::stopAllEffects()
{
    _sae->stopAllEffects();
}

float SoundManager::volumeFactorHelp(int percent)
{
    float vFactor = 0;
    if (percent < 0)
    {
        return vFactor;
    }
    else if (percent > 100)
    {
        vFactor = 100;
        return vFactor;
    }
    
    if (percent <= 50)
    {
        vFactor = 0.004 * percent;
    }
    else if (percent <= 70)
    {
        vFactor = 0.01 * percent - 0.3;
    }
    else if (percent <= 100)
    {
        vFactor = 0.02 * percent - 1.0;
    }
    
    return vFactor;
}
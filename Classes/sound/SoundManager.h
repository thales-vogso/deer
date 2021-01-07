/****************************************************************************
 *	@desc   声音控制
 *	@date	2015-6-18
 *	@author	110101
     
 *	@file	SoundManager.h
 *  @modify null
 ******************************************************************************/

#ifndef __DEER_SOUNDMANAGER__
#define __DEER_SOUNDMANAGER__

#include <iostream>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

/*
 音量部分的需求
 10为音量最大，1为音量无声。
 
 10至9，音量设置为100%---80%
 
 9至8，音量设置为80%--60%
 
 8至7，音量设置为60%--40%
 
 7至6，音量设置为40%--30%
 
 6至5，音量设置为30%--20%
 
 5至1，音量设置为20%--0%
 
 音量部分的应对方案
 设置部分添加一个函数来计算音量问题，计算规则如下：
 
 进度条的百分比x      音量放缩系数     映射关系
 [0,50]              [0-0.2]        y=0.004x
 (50,70]             (0.2-0.4]      y=0.01(x-50) + 0.2=0.01x-0.3
 (70,100]            (0.4-1.0]      y=0.02(x-70) + 0.4=0.02x-1.0
 */
class SoundManager
{
public:
    
    enum MusicType
    {
        kManor = -1,
        kBattle = -2,
        kMap = -3
    };
    
    enum EffectType
    {
        kPlayerDie = 1,
        kPlayerHurt = 2,
        kPlayerAtk = 3,
        
        kEnemyDie = 4,
        kEnemyHurt = 5,
        kEnemyAtk = 6,
        
        kAlchemy = 7,
        kCollection = 8,
        kBoard = 9,
        kPopUp = 10,
        kRecall = 11,
        kNewView = 12,
        kOpenwarehouse = 13,
        kUndefine = 14,
        kMoveStep1 = 15,
        kMoveStep2 = 16,
        kFastAdd   = 17,
        kBuildFinish = 18,
        kLACK = 19, 
        kHit = 20
    };
    
    static SoundManager* getInstance();
    
    static void destoryInstance();
    
    /**
     *  设置背景音乐的大小
     *
     *  @param volume
     */
    void setBackgroundMusicVolume(int volume);
    
    /**
     *  设置音效的大小
     *
     *  @param volume
     */
    void setEffectsVolume(int volume);
    
    /**
     *  保存声音,这个方法的存在是因为保存声音是需要读写数据库，对显示效率影响较大，使用是慎重考虑
     */
    void saveVolume();
    
    int getMusicVolume()
    {
        return _musicVolume;
    };
    
    int getEffectsVolume()
    {
        return _effectVolume;
    }
    
    /**
     *  播放背景音乐
     *
     *  @param musicType 音乐类型
     */
    void playMusic(int musicType);
    
    void playMusicFadeIn(int musicType);
    
    /**
     *  停止音乐
     */
    void stopMusic();
    
    void stopMusicFadeOut();
    
    /**
     *  播放音效
     *
     *  @param effectType 音效类型
     */
    void playEffect(int effectType);
    
    /**
     *  播放循环音效
     *  @param effectType 音效类型
     */
    void playEffect(int effectType,bool loop);
    
    /**
     *  停止所有音效
     */
    void stopAllEffects();
    
protected:
    
    bool init();
    
    /**
     *  声音放缩因子转换函数
     *
     *  @param percent
     *
     *  @return
     */
    float volumeFactorHelp(int percent);
    
    /**
     *  单例
     */
    static SoundManager * _instance;
    
    /**
     *  所使用的引擎
     */
    CocosDenshion::SimpleAudioEngine *_sae;
    
    /**
     *  背景音乐音量
     */
    int _musicVolume;
    
    /**
     *  音量淡入淡出步数
     */
    int _fadeMaxNum = 100;
    /**
     *  音效音量
     */
    int _effectVolume;
    
    /**
     *  声音名称字典
     */
    cocos2d::ValueMapIntKey _soundMap;
};

#endif /* defined(__deer__SoundManager__) */

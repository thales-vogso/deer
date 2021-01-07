/****************************************************************************
    
 *	@desc   地图信息简介
 *	@date	2015-6-18
 *	@author	120103
     
 *	@file	popUp/MapSummery.h
 *   @modify	null
 ******************************************************************************/

#ifndef __deer__MapSummery__
#define __deer__MapSummery__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"

class MapSummary   :   public cocos2d::Layer
{
public:
    /**
     *  生成信息界面
     *
     *  @param mapName 地图名字
     *  @param txt  显示内容
     */
    static MapSummary* create(std::string mapName,std::string txt);
    
    void onEnter() override;
    
    void onExit() override;
    
    /**
     *  帧检测回调
     */
    void update(float delta) override;
    
    /**
     *  获取播放时间
     */
    float getPlayTime();
    
protected:
    /**
     *  初始化信息界面
     *
     *  @param mapName 地图名字
     *  @param txt  显示内容
     */
    bool init(std::string mapName, std::string txt);
    
    void remove();
    
    /**
     *   内容描述文字分行
     *
     */
    void adaptContensize();
    
    /**
     *  show Action
     *
     */
    void showAction();
    
    /**
     *  要显示的文字
     */
    std::string _text;
    
    /**
     *  存储所有可用文本框
     */
    vector<cocos2d::ui::Text*>   _textVector;
    
    /**
     *  界面显示内容
     */
    cocos2d::Node*    _view;
    
    /**
     *  最大行数
     */
    const int   MAXROWNUM   =   8;
    
    /**
     *  文本显示动画执行延时
     */
    const float delayTime   =   0.7f;
    
    /**
     *  文本每行显示动画时间
     */
    const float perTextDT   =   1.5f;
    const float removeTime  =   1.0f;
    
    /**
     *  isRun  动画是否被播放过
     *  _actionIndex 动画播放到第几个文本了
     */
    bool _isRun;
    int  _actionIndex;
    
    /**
     *  简介行数
     */
    int _rowNum = 0;
};

#endif /* defined(__deer__MapSummery__) */

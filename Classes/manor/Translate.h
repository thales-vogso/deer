/****************************************************************************
 
 *	@desc	随机事件
 *	@date	2015-5-26
 *	@author	110101
     
 *	@file	manor/TranslateScene.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_TRANSLATESCENE__
#define __DEER_TRANSLATESCENE__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class Translate : public cocos2d::ui::Layout
{
public:
    
    static cocos2d::Scene* getTransLateScene();
    
    CREATE_FUNC(Translate);
protected:
    bool init();
    
    void onEnter();
    
protected:
    
    inline void setRunAsScene(bool run)
    {
        _runAsScene = run;
    }
    /**
     *  图标的内精灵
     */
    cocos2d::Sprite* _in;
    /**
     *  图标的外精灵
     */
    cocos2d::Sprite* _out;
    
    cocos2d::Node* _mask;
    
    bool _runAsScene;
};

class RankTranslate : public cocos2d::ui::Layout
{
public:
    static RankTranslate* create(cocos2d::Size size);
protected:
    bool init(cocos2d::Size size);
    
    void onEnter();
    
    /**
     *  图标的内精灵
     */
    cocos2d::Sprite* _in;
    /**
     *  图标的外精灵
     */
    cocos2d::Sprite* _out;
    
    cocos2d::LayerColor* _mask;
};
class RmbBuyTranslate:public cocos2d::ui::Layout
{
public:
    static RmbBuyTranslate* create(cocos2d::Size size, std::string str);
protected:
    bool init(cocos2d::Size size, std::string str);
    
    void onEnter();
    
    /**
     *  图表的内精灵
     */
    cocos2d::Sprite* _in;
    /**
     *  图标的外精灵
     */
    cocos2d::Sprite* _out;
    
    /**
     *购买等候提示
     */
    cocos2d::ui::Text*  _tips;
    
    cocos2d::LayerColor* _mask;
};

#endif /* defined(__deer__TranslateScene__) */

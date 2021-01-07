/****************************************************************************
 *	@desc   成就界面
 *	@date	2015-1-20
 *	@author	110101
     
 *	@file	manor/HeroChoicePopup.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_HEROCHOICEPOPUP__
#define __DEER_HEROCHOICEPOPUP__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "manor/LayerNoTouchTrans.h"
#include "LayerBase.h"
#include "guide/Guide.h"

class HeroChoicePopup : public cocos2d::ui::Layout, public LayerNoTouchTrans
{
public:
    /**
     *  实例创建函数
     *
     *  @param rid 角色id
     *
     *  @return HeroChoicePopup*
     */
    static HeroChoicePopup *create(int rid);
    
protected:
    /**
     *  初始化
     *
     *  @param rid 角色ID
     *
     *  @return 初始化是否成功
     */
    bool init(int rid);
    
private:
    /**
     *  返回按钮的回调
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void backButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType touchType);
    void backReal();
    
    /**
     *  训练英雄按钮
     *
     *  @param sender    按钮
     *  @param touchType 触摸类型
     */
    void trainToHero(cocos2d::Ref* sender, cocos2d::ui::Text::TouchEventType touchType);
    void trainToHeroAct();
    
    cocos2d::Node *_touchBtn;
    cocos2d::Vector<cocos2d::ui::Widget*> _model;
    
private:
    /**
     *  按钮向量，不包含返回按钮
     */
    cocos2d::Map<int, Ref*> _buttonMap;
    
};

/**
 *  招募询问界面..
 */
class RecruitAsk : public cocos2d::ui::Layout, public LayerNoTouchTrans, public LayerBase
{
public:
    RecruitAsk();
    
    void removeSelf() override;
    void updateProperty() override;
    
    static RecruitAsk* create(cocos2d::ValueMap data);
    
    inline void setDelegate(HeroChoicePopup *delegate)
    {
        _delegate = delegate;
    };
    
    /**
     * 新手引导手指
     */
    Guide* _guide = NULL;
    
protected:
    bool init(cocos2d::ValueMap data);
    
    void sureBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t);
    void sureAct();
    
    void quitBtnCall(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType t);
    void quitAct();
    
    cocos2d::ValueMap _data;
    
    HeroChoicePopup* _delegate;
};
#endif /* defined(__deer__HeroChoicePopup__) */

/****************************************************************************
*	@Copyright(c)	2014,青岛UMGame
*	@desc	购买钻石界面
*	@date	2015-
*	@author	120102
*	@file	
*	@modify	null
******************************************************************************/
#ifndef __MONOR_BUYRMB__
#define __MONOR_BUYRMB__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "manor/Translate.h"
#include "LayerBase.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSPurchase/IOSiAPBridge.h"
#endif

class RmbBuyLayer : public cocos2d::ui::Layout, public LayerBase
{
public:
	CREATE_FUNC(RmbBuyLayer);
	//static RmbBuyLayer * create();
protected:
	bool init();
    
    virtual void onEnter();
	virtual ~RmbBuyLayer();
    /**
     * 返回按钮
     */
	void gobackCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t);

    /**
     * 购买按钮
     */
    void BuyBtnCall(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t);
    
    void SetIsTouchEnable(bool enable);

	/**
	*  道具刷新
	*/
	void itemChange(cocos2d::EventCustom* e);
    
    void removeSelf() override;
    
    void updateProperty() override;
	
private:
    
    std::vector<cocos2d::ui::Layout*> _LayoutVec;
    
    RmbBuyTranslate* translate = nullptr;
    
    cocos2d::ui::Text* _rmbTxt;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSiAPBridge* newiosbridge;
#endif
	cocos2d::ui::ImageView* _rmbimage;
  //  bool failisSev = false;
  //  bool successisSev = false;
  //  bool recoveryIsSev = false;
    
    

};
#endif
/****************************************************************************
 *	@desc   选择弹窗
 *	@date	2015-1-28
 *	@author	110101
     
 *	@file	popUp/ChoicePopup.h
 *   @modify	null
 ******************************************************************************/

#ifndef __DEER_CHOICEPOPUP__
#define __DEER_CHOICEPOPUP__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class ChoicePopup : public cocos2d::ui::Widget
{
public:
    CREATE_FUNC(ChoicePopup);
    
    /**
     *  设置标题
     *
     *  @param title 标题
     */
    void setTitle(std::string title);
    
    /**
     *  设置内容
     *
     *  @param 文字内容
     */
    void setContent(std::string text);
    
    /**
     *  设置确定按钮的回调
     */
    void setSureCallback(std::function<void (int)> callback);
    
    /**
     *  设置否决按钮的回调
     *
     *  @param callback 函数
     */
    void setQuitCallback(std::function<void (int)> callback);
    
CC_CONSTRUCTOR_ACCESS:
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init();
    
    /**
     *  确定按钮的回调
     *
     *  @param cocos2d::Ref* unused sender
     *  @param type          触摸类型
     */
    void sureBtnCall(cocos2d::Ref*, cocos2d::ui::Text::TouchEventType type);
    
    /**
     *  否定按钮的回调
     *
     *  @param cocos2d::Ref* unused sender
     *  @param type          触摸类型
     */
    void quitBtnCall(cocos2d::Ref*, cocos2d::ui::Text::TouchEventType type);
    
    /**
     *  确定回调
     */
    std::function<void (int)> _sureCallback;
    
    /**
     *  否决回调
     */
    std::function<void (int)> _quitCallback;
    
    cocos2d::ui::Text* _sureBtn;
    cocos2d::ui::Text* _quitBtn;
    
};

#endif /* defined(__deer__ChoicePopup__) */

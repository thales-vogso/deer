/****************************************************************************
  
 *	@desc	入口
 *	@date	2014-12-12
 *	@author	110101
     
 *	@file	Loading/Loading.h
 ******************************************************************************/
#ifndef __DEER_LOADING__
#define __DEER_LOADING__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "gateWay/GateWayDelegate.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "core/custom/LibDate.h"
/**
 *  加载界面
 */
class Loading : public cocos2d::ui::Layout, public custom::LibDate
{
public:
    static const int LOADINGTIME;
    
    CREATE_FUNC(Loading);
    
    inline void setDelegate(GateWayDelegate * delegate)
    {
        _delegate = delegate;
    };
    
    void setprogressAdd(int add = 5);
    
    void runRest();
    
    inline void setOnEnterDo(std::function<void ()> fun)
    {
        _onEnterDo = fun;
    };
    
protected:
    
    void storyOver();
    
    std::function<void ()> _onEnterDo;
    
    void guideCheck();
    
    void onEnter();
    
    bool init();
    
    void loadingEnd();
    
    cocos2d::ui::LoadingBar *_bar;
    
    int _progress;
    
    GateWayDelegate * _delegate;
    
    cocostudio::timeline::ActionTimeline *_guideact;
};



#endif /* defined(__deer__Loading__) */

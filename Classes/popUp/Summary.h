/****************************************************************************
    
 *	@desc   信息简介
 *	@date	2015-6-16
 *	@author	120103
     
 *	@file	popUp/Summery.h
 *   @modify	null
 ******************************************************************************/

#ifndef __deer__Summery__
#define __deer__Summery__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "manor/LayerBase.h"

class Summary   :   public cocos2d::ui::Layout
{
public:
    /**
     *  生成信息界面
     *
     *  @param txt  显示内容
     *  @param node 显示内容的结点
     *  @param nodeScale   是否有缩放
     */
    static Summary* create(std::string txt, cocos2d::Node* node = NULL,float nodeScale = 1.0f);
    void setDelegate(LayerBase* node);
    
protected:
    /**
     *  初始化信息界面
     *
     *  @param txt  显示内容
     *  @param node 显示内容的结点
     */
    bool init(std::string txt, cocos2d::Node* node = NULL,float nodeScale = 1.0f);
    
    void remove(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType t);
    
    /**
     *  适配文本框大小
     *
     */
    void adaptContensize();
    
    /**
     *  适配文本框朝向
     */
    void adaptDirection(cocos2d::Node* node,float nodeScale);
    
    /**
     *  文本框Y轴位移偏移量
     */
    float _displaceY;
    float   _DISPLACE_Y     = 8;
    
    /**
     *  文本框大小行距
     */
    int _textSpaceing   =   8;
    int _textDisplaceX  =   17;
    
    enum ARROW_POSX
    {
        LEFT_POSX = -310,
        MID_POSX  = 0,
        RIGHT_POSX = 310
    };
    LayerBase* _delegate = nullptr;
    
    enum OUTLINE_TYPE
    {
        LITTLE = 600,
        WIDTH = 800
    };
    
    float txtWidth;

};


#endif /* defined(__deer__Summery__) */

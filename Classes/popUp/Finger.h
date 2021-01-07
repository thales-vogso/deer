/****************************************************************************
    
 *	@desc   新手引导
 *	@date	2015-7-2
 *	@author	120103
     
 *	@file	popUp/Finger.h
 *	@modify	null
 ******************************************************************************/

#ifndef __deer__Finger__
#define __deer__Finger__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GlobalMacros.h"
#include "data/Guid.h"
class Finger   :   public cocos2d::Node
{
public:
    /**
     *  生成信息界面
     *
     *  @param pos  手指起始位置
     *  @param node  父节点
     *  @param type  手指引导类型
     */
    static Finger* create(cocos2d::Vec2 pos, cocos2d::Node* node,int type);
    
    
    ~Finger();
    
protected:
    /**
     *  初始化信息界面
     *
     *  @param pos  手指起始位置
     *  @param node  父节点
     *  @param type  手指引导类型
     */
    bool init(cocos2d::Vec2 pos, cocos2d::Node* node,int type);
    
    cocos2d::Sprite*  _finger = nullptr;
    cocos2d::Sprite*  _arrow4 = nullptr;
    cocos2d::Sprite*  _circle = nullptr;
    
    cocos2d::Vec2  _startPos;
    int               _type;
    
    cocos2d::ui::Text*  _guideText = nullptr;
    
    void fingerAction(cocos2d::Node* node);
    
    void removeFinger();
};

class GuideTips    :   public cocos2d::Node
{
public:
    /**
     *  生成信息界面
     *
     *  @param  pos tips显示位置
     *  @param  tipsId tips显示内容的ID
     */
    static GuideTips* create(cocos2d::Vec2 pos,cocos2d::Node* node,cocos2d::Scene* scene,int tipsId);

    ~GuideTips();
private:
    /**
     *  初始化信息界面
     *
     *  @param  pos tips显示位置
     *  @param  tipsId tips显示内容的ID
     */
    bool init(cocos2d::Vec2 pos,cocos2d::Node* node,cocos2d::Scene* scene,int tipsId);
    
    void removeTips();
    
    cocos2d::ClippingNode* pClip = nullptr;
    cocos2d::ui::Text* _lable = nullptr;
    cocos2d::ui::Text* _lable1 = nullptr;
    cocos2d::ui::Text* _continue = nullptr;
    cocos2d::Scene* _scene = nullptr;
    
    cocos2d::Vec2 _playerPos =  cocos2d::Vec2(512,820);
    
    /**
     *  创建遮罩
     */
    void createMask(cocos2d::Node* node,int tipsId);
};
class Finger_Tips    :   public cocos2d::Node
{
public:
    /**
     *  生成信息界面
     *
     *  @param  pos tips显示位置
     *  @param  tipsId tips显示内容的ID
     */
    static Finger_Tips* create(cocos2d::Vec2 fingerPos,cocos2d::Node* fingerNode,cocos2d::Node* lightNode,cocos2d::Scene* scene);
    
    ~Finger_Tips();
private:
    /**
     *  初始化信息界面
     *
     *  @param  pos tips显示位置
     *  @param  tipsId tips显示内容的ID
     */
    bool init(cocos2d::Vec2 fingerPos,cocos2d::Node* fingerNode,cocos2d::Node* lightNode,cocos2d::Scene* scene);
    
    void removeTips();
    
    cocos2d::ClippingNode* pClip = nullptr;
    cocos2d::ui::Text* _lable = nullptr;
    cocos2d::ui::Text* _lable1 = nullptr;
    cocos2d::ui::Text* _lable2 = nullptr;
    cocos2d::ui::Text* _continue = nullptr;
    cocos2d::Scene* _scene = nullptr;
    cocos2d::Sprite*  _finger = nullptr;
    
    cocos2d::Vec2 _playerPos =  cocos2d::Vec2(512,820);
    
    /**
     *  创建遮罩
     */
    void createMask(cocos2d::Node* node);
    
    //小手动画..
    void fingerAction();
};
#endif /* defined(__deer__Finger__) */

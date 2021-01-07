//
//  Guid.cpp
//  deer
//
//  Created by xinhua on 15/7/2.
//
//

#include "Finger.h"
#include "data/Lang.h"
#include "data/Guid.h"

USING_NS_CC;

Finger* Finger::create(cocos2d::Vec2 pos, cocos2d::Node* node,int type)
{
    Finger* finger = new Finger();
    if(finger&&finger->init(pos, node, type))
    {
        finger->autorelease();
        return finger;
    }
    else
    {
        delete finger;
        finger = NULL;
        return NULL;
    }
}
Finger::~Finger()
{
    removeFinger();
}
bool Finger::init(cocos2d::Vec2 pos, cocos2d::Node* node,int type)
{
    if(!Node::init())
        return false;
    
    _startPos = pos;
    _type   = type;
    
    _finger = Sprite::create("finger.png");
    _finger->setAnchorPoint(Vec2(0.5,1));
    _finger ->setPosition(pos);
    
    if(type == data::Guid::FINGER_MOVE)
    {
        _arrow4 = Sprite::create("ditujiantou.png");
        _arrow4->setRotation(180);
        _arrow4->setPosition(Vec2(pos.x,pos.y-100));
        node->getParent()->addChild(_arrow4);
        _finger ->setPosition(Vec2(node->getContentSize().width/2,node->getContentSize().height/2-100));
    }
    else if(type == data::Guid::FINGER_PRESS_ON)
    {
        _circle = Sprite::create("04.png");
        _circle ->setScale(0.3);
        _circle ->setOpacity(0);
        _circle ->setPosition(pos);
        node->addChild(_circle);
        _guideText = ui::Text::create();
        _guideText->setString(data::Lang::getInstance()->getWord(130322));
        _guideText->setFontSize(34);
        _guideText->setAnchorPoint(Vec2(0.5,0.5));
        if(node->getPositionX()>=400)
        {
            _guideText->setPosition(Vec2(_startPos.x,_startPos.y-_finger->getContentSize().height-30));//-40*4
        }
        else
        {
            _guideText->setPosition(Vec2(_startPos.x,_startPos.y-_finger->getContentSize().height-30));//+40*4
        }
        
        node->addChild(_guideText);
    }
    node->addChild(_finger);
    fingerAction(node);
    return true;
}

void Finger::fingerAction(cocos2d::Node* node)
{
    if(_type == data::Guid::FINGER_CLICK)
    {
        _finger->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, -30)),
                                                                  MoveBy::create(1, Vec2(0, 30)),
                                                                  NULL)));
    }
    else if(_type == data::Guid::FINGER_MOVE)
    {
        
        _arrow4->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5),FadeIn::create(0.5), NULL)));
       
        
        Vec2 pos3 = Vec2(node->getContentSize().width/2,node->getContentSize().height/2-100);

        auto action = Sequence::create(MoveBy::create(0.5, Vec2(0, -30)),
                                       MoveBy::create(1, Vec2(0, 30)),
                                       MoveBy::create(0.5, Vec2(0, -30)),
                                       MoveBy::create(1, Vec2(0, 30)),
                         NULL);
        
        _finger->runAction(RepeatForever::create(Sequence::create(action,NULL)));
        
        
    }
    else if(_type == data::Guid::FINGER_SLIDING)
    {
    
    }
    else if(_type == data::Guid::FINGER_PRESS_ON)
    {
        _finger->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, -30)),
                                                                  MoveBy::create(1, Vec2(0, 30)),
                                                                  DelayTime::create(2.4),
                                                                NULL)));
        _circle->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.5),FadeTo::create(0.1, 153),ScaleTo::create(0.5, 0.5),
                                                                  FadeTo::create(0.1, 0),ScaleTo::create(0.1, 0.3),
                                                                  FadeTo::create(0.1, 153),ScaleTo::create(0.5, 0.5),
                                                                  FadeTo::create(0.1, 0),ScaleTo::create(0.1, 0.3),
                                                                  FadeTo::create(0.1, 153),ScaleTo::create(0.5, 0.5),
                                                                  FadeTo::create(0.1, 0),ScaleTo::create(0.1, 0.3),NULL)));
    }
}
void Finger::removeFinger()
{
    if(_finger&& _finger->getParent())
    {
        _finger->removeFromParent();
        _finger = nullptr;
    }
    
    if(_type == data::Guid::FINGER_MOVE)
    {
        if(_arrow4&& _arrow4->getParent())
        {
            _arrow4->removeFromParent();
            _arrow4 = nullptr;
        }
        
    }
    else if(_type == data::Guid::FINGER_PRESS_ON)
    {
        _circle->removeFromParent();
        _circle = nullptr;
        _guideText->removeFromParent();
        _guideText = nullptr;
    }
}
GuideTips::~GuideTips()
{
    removeTips();
}
GuideTips* GuideTips::create(cocos2d::Vec2 pos,cocos2d::Node* node,cocos2d::Scene* scene,int tipsId)
{
    GuideTips* tips = new GuideTips();
    if(tips && tips->init(pos, node,scene,tipsId))
    {
        tips->autorelease();
        return tips;
    }
    else
    {
        delete tips;
        tips = NULL;
        return NULL;
    }
}
bool GuideTips::init(cocos2d::Vec2 pos, cocos2d::Node* node,cocos2d::Scene* scene,int tipsId)
{
    if(!Node::init())
        return false;
    /**
     *   添加遮罩
     */
    _scene = scene;
    createMask(node,tipsId);
   
    _lable = ui::Text::create();
    _lable->setString(data::Lang::getInstance()->getWord(tipsId));
    _lable->setFontSize(34*SCALEX);
    _lable->setAnchorPoint(Vec2(0.5,0.5));
    Vec2 lablePos = node->convertToWorldSpace(Vec2(0,0));
   
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(_lable);
    }
    else
    {
        _scene->addChild(_lable);
    }
    
    
    _continue = ui::Text::create();
    CCLOG("continue =  %s",data::Lang::getInstance()->getWord(data::Guid::TIPS_CONTINUE).c_str());
    _continue->setString(data::Lang::getInstance()->getWord(data::Guid::TIPS_CONTINUE));
    _continue->setFontSize(34*SCALEX);
    _continue->setAnchorPoint(Vec2(0.5,0.5));
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(_continue);
    }
    else
    {
        _scene->addChild(_continue);
    }
    _continue->setVisible(false);
    _continue->runAction(RepeatForever::create(Sequence::create(FadeTo::create(1, 150),FadeTo::create(1, 255), NULL)));
    
    if(tipsId == data::Guid::TIPS_SHELTER)
    {
        _lable1 = ui::Text::create();
        _lable1->setString(data::Lang::getInstance()->getWord(tipsId+1));
        _lable1->setFontSize(34*SCALEX);
        _lable1->setAnchorPoint(Vec2(0.5,0.5));
        _lable1->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,lablePos.y - 100*SCALEY));
        _scene->addChild(_lable1);
        
        _lable->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,lablePos.y- 50*SCALEY));
        _continue->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,lablePos.y-150*SCALEY));
    }
    else
    {
        _lable->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,lablePos.y - 50*SCALEY));
        _continue->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,lablePos.y-100*SCALEY));
    }
    
    if(tipsId == data::Guid::TIPS_EXPLORE)
    {
        _lable->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,Director::getInstance()->getWinSize().height/2));
        _continue->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,Director::getInstance()->getWinSize().height/2-100*SCALEY));
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t, Event* e)->bool
    {
        if (isVisible())
        {
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [=](Touch * t, Event *e)
    {
        //更新新手引导状态..
        CCLOG("update guide 217");
        data::Guid::getInstance()->updateGuidInfo(tipsId);
    };
    listener->setEnabled(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(true);
    
    listener->setEnabled(false);
    auto delay = DelayTime::create(3);
    auto callFunc = CallFunc::create([=](){
        //更新新手引导状态..
        _continue->setVisible(true);
        listener->setEnabled(true);
    });
    _lable->runAction(Sequence::create(delay,callFunc, NULL));
    
    return true;
}
void GuideTips::createMask(cocos2d::Node* node,int tipsId)
{
    //创建遮罩层..
    LayerColor* player = LayerColor::create(Color4B(0,0,0,150));
    
    //创建剪裁结点..
    pClip = ClippingNode::create();
    pClip->setInverted(true);//设置是否反响，将决定画出来的节点十透明的还是黑色的..
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(pClip);
    }
    else
    {
        _scene->addChild(pClip);
    }
    
    //注意将LayerColor层添加到裁切节点上..
    pClip->addChild(player);
    
    if(tipsId == data::Guid::TIPS_HOME ||tipsId == data::Guid::TIPS_SHELTER)
    {
        //绘制圆形..
        //设置参数..
        Color4F red = {1,0,0,1};//顶点颜色设置为红色，参数是R,G,B,A..
        float radius;
        if(SCALEY > SCALEX)
        {
           radius  = 30.0f*SCALEY; //设置圆的半径..
        }
        else
        {
            radius = 30.0f*SCALEX; //设置圆的半径..
        }
        
        const int nCount = 200;//设置定点数，此处我们将园看做多边形..
        const float angel = 2.0f*(float)M_PI/nCount;//两个顶点与中心的夹角（弧度）..
        Vec2 circle[nCount];//保存顶点的数据..
        for(int i = 0; i < nCount;i++)
        {
            float radian = i*angel;//弧度..
            circle[i].x = radius*cosf(radian);//顶点x坐标..
            circle[i].y = radius*sinf(radian);//顶点y坐标..
        }
        //绘制裁切区域..
        //注意不要将pStencil addChild..
        DrawNode* pStencil = DrawNode::create();
        pStencil->drawPolygon(circle, nCount, red, 0, red);
        Vec2 pos =  node->convertToWorldSpace(Vec2(0,0));
        if(tipsId == data::Guid::TIPS_SHELTER)
        {
            pStencil->setPosition(Vec2(pos.x+node->getContentSize().width/2.0f,pos.y+node->getContentSize().height/2.0f));
        }
        else
        {
            pStencil->setPosition(Vec2(pos.x+node->getContentSize().width/2.0f,pos.y+node->getContentSize().height/2.0f));
        }
        
        pStencil->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.2),ScaleTo::create(1, 1), NULL)));
        
        //将这个圆形从剪裁节点上面抠出来，Stencil是模板的意思..
        pClip->setStencil(pStencil);
        
    }
    else if(tipsId == data::Guid::TIPS_BREAD)
    {
        //绘制长方形..
        //设置参数..
        Color4F red = {1,0,0,1};//顶点颜色设置为红色，参数是R,G,B,A..
        float radius;
        if(SCALEY > SCALEX)
        {
            radius  = 30.0f*SCALEY; //设置圆的半径..
        }
        else
        {
            radius = 30.0f*SCALEX; //设置圆的半径..
        }
        const int nCount = 200;//设置定点数，此处我们将园看做多边形..
        const float angel = 2.0f*(float)M_PI/nCount;//两个顶点与中心的夹角（弧度）..
        Vec2 circle[nCount];//保存顶点的数据..
        for(int i = 0; i < nCount;i++)
        {
            float radian = i*angel;//弧度..
            circle[i].x = radius*cosf(radian);//顶点x坐标..
            circle[i].y = radius*sinf(radian);//顶点y坐标..
        }
        
        //绘制裁切区域..
        //注意不要将pStencil addChild..
        DrawNode* pStencil = DrawNode::create();
        pStencil->drawPolygon(circle, nCount, red, 0, red);
        Vec2 pos =node->convertToWorldSpace(Vec2(0,0));
        pStencil->setPosition(Vec2(pos.x+node->getContentSize().width/2.0f,pos.y+node->getContentSize().height/2.0f));
        pStencil->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.2),ScaleTo::create(1, 1), NULL)));
        
        //将这个圆形从剪裁节点上面抠出来，Stencil是模板的意思..
        pClip->setStencil(pStencil);

    }
    else if(tipsId == data::Guid::TIPS_EXPLORE)
    {
        //绘制点..
        //设置参数..
        Color4F red = {1,0,0,1};//顶点颜色设置为红色，参数是R,G,B,A..
        float radius = 5.0f*SCALEY; //设置圆的半径..
        const int nCount = 4;//设置定点数，此处我们将园看做多边形..
        Vec2 circle[nCount];//保存顶点的数据..
        circle[0].x = radius*cosf(2.0f*(float)M_PI*20.0f/360.0f);
        circle[0].y = radius*sinf(2.0f*(float)M_PI*20.0f/360.0f);
        circle[1].x = radius*cosf(2.0f*(float)M_PI*160.0f/360.0f);
        circle[1].y = radius*sinf(2.0f*(float)M_PI*160.0f/360.0f);
        circle[2].x = radius*cosf(2.0f*(float)M_PI*200.0f/360.0f);
        circle[2].y = radius*sinf(2.0f*(float)M_PI*200.0f/360.0f);
        circle[3].x = radius*cosf(2.0f*(float)M_PI*340.0f/360.0f);
        circle[3].y = radius*sinf(2.0f*(float)M_PI*340.0f/360.0f);
        
        //绘制裁切区域..
        //注意不要将pStencil addChild..
        DrawNode* pStencil = DrawNode::create();
        pStencil->drawPolygon(circle, nCount, red, 0, red);
        Vec2 pos =node->convertToWorldSpace(Vec2(0,0));
        pStencil->setPosition(Vec2(pos.x+node->getContentSize().width/2.0f,pos.y+node->getContentSize().height/2.0f));
        pStencil->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.2),ScaleTo::create(1, 1), NULL)));
        
        //将这个圆形从剪裁节点上面抠出来，Stencil是模板的意思..
        pClip->setStencil(pStencil);
    }
    
    
    
}
void GuideTips::removeTips()
{
	if (pClip && pClip->getParent())
	{
		pClip->removeFromParent();
	}
	if (_lable && _lable->getParent())
	{
		_lable->removeFromParent();
	}
	if (_continue && _continue->getParent())
	{
		_continue->removeFromParent();
	}
    if(_lable1 && _lable1->getParent())
    {
        _lable1->removeFromParent();
    }
}
Finger_Tips* Finger_Tips::create(cocos2d::Vec2 fingerPos, cocos2d::Node *fingerNode, cocos2d::Node* lightNode, cocos2d::Scene *scene)
{
    Finger_Tips* tips = new Finger_Tips();
    if(tips && tips->init(fingerPos, fingerNode,lightNode,scene))
    {
        tips->autorelease();
        return tips;
    }
    else
    {
        delete tips;
        tips = NULL;
        return NULL;
    }
}
bool Finger_Tips::init(cocos2d::Vec2 fingerPos, cocos2d::Node *fingerNode, cocos2d::Node* lightNode, cocos2d::Scene *scene)
{
    if(!Node::init())
        return false;
    /**
     *   添加遮罩
     */
    _scene = scene;
    createMask(lightNode);
    //添加小手..
    _finger = Sprite::create("finger.png");
    _finger->setAnchorPoint(Vec2(0.5,1));
    _finger ->setPosition(Vec2(fingerNode->getContentSize().width/2.0f,0));
    
    fingerNode->addChild(_finger);
    
    fingerAction();
    
    //添加文本..
    _lable = ui::Text::create();
    _lable->setString(data::Lang::getInstance()->getWord(130327));
    _lable->setFontSize(34*SCALEX);
    _lable->setAnchorPoint(Vec2(0.5,0.5));
    Vec2 lablePos = fingerNode->convertToWorldSpace(Vec2(0,0));
    
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(_lable);
    }
    else
    {
        _scene->addChild(_lable);
    }
    _continue = ui::Text::create();
    _continue->setString(data::Lang::getInstance()->getWord(data::Guid::TIPS_CONTINUE));
    _continue->setFontSize(34*SCALEX);
    _continue->setAnchorPoint(Vec2(0.5,0.5));
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(_continue);
    }
    else
    {
        _scene->addChild(_continue);
    }
    _continue->setVisible(false);
    _continue->runAction(RepeatForever::create(Sequence::create(FadeTo::create(1, 150),FadeTo::create(1, 255), NULL)));
    
   
    _lable1 = ui::Text::create();
    _lable1->setString(data::Lang::getInstance()->getWord(130328));
    _lable1->setFontSize(34*SCALEX);
    _lable1->setAnchorPoint(Vec2(0.5,0.5));
    _lable1->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,Director::getInstance()->getWinSize().height/2.0f - 100*SCALEY));
    
    _lable2 = ui::Text::create();
    _lable2->setString(data::Lang::getInstance()->getWord(130329));
    _lable2->setFontSize(34*SCALEX);
    _lable2->setAnchorPoint(Vec2(0.5,0.5));
    _lable2->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,Director::getInstance()->getWinSize().height/2.0f - 150*SCALEY));
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(_lable1);
        Director::getInstance()->getRunningScene()->addChild(_lable2);
    }
    else
    {
        _scene->addChild(_lable1);
        _scene->addChild(_lable2);
    }
        
    _lable->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,Director::getInstance()->getWinSize().height/2.0f- 50*SCALEY));
    _continue->setPosition(Vec2(Director::getInstance()->getWinSize().width/2.0f,Director::getInstance()->getWinSize().height/2.0f-200*SCALEY));
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t, Event* e)->bool
    {
        if (isVisible())
        {
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [=](Touch * t, Event *e)
    {
        //更新回城引导状态..
        data::Guid::getInstance()->setHomeGuid(true);
        this->removeFromParent();
    };
    listener->setEnabled(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(true);
    
    listener->setEnabled(true);
    auto delay = DelayTime::create(3);
    auto callFunc = CallFunc::create([=](){
        //更新新手引导状态..
        _continue->setVisible(true);
    });
    _lable->runAction(Sequence::create(delay,callFunc, NULL));
    
    return true;
}
Finger_Tips::~Finger_Tips()
{
    removeTips();
}
void Finger_Tips::createMask(cocos2d::Node *node)
{
    //创建遮罩层..
    LayerColor* player = LayerColor::create(Color4B(0,0,0,150));
    
    //创建剪裁结点..
    pClip = ClippingNode::create();
    pClip->setInverted(true);//设置是否反响，将决定画出来的节点十透明的还是黑色的..
    if(!_scene)
    {
        Director::getInstance()->getRunningScene()->addChild(pClip);
    }
    else
    {
        _scene->addChild(pClip);
    }
    
    //注意将LayerColor层添加到裁切节点上..
    pClip->addChild(player);
    
    
    Color4F red = {1,0,0,1};//顶点颜色设置为红色，参数是R,G,B,A..
    float radius;
    if(SCALEY > SCALEX)
    {
        radius  = 30.0f*SCALEY; //设置圆的半径..
    }
    else
    {
        radius = 30.0f*SCALEX; //设置圆的半径..
    }
    const int nCount = 200;//设置定点数，此处我们将园看做多边形..
    const float angel = 2.0f*(float)M_PI/nCount;//两个顶点与中心的夹角（弧度）..
    Vec2 circle[nCount];//保存顶点的数据..
    for(int i = 0; i < nCount;i++)
    {
        float radian = i*angel;//弧度..
        circle[i].x = radius*cosf(radian);//顶点x坐标..
        circle[i].y = radius*sinf(radian);//顶点y坐标..
    }
    
    
    //绘制裁切区域..
    //注意不要将pStencil addChild..
    DrawNode* pStencil = DrawNode::create();
    pStencil->drawPolygon(circle, nCount, red, 0, red);
    Vec2 pos =node->convertToWorldSpace(Vec2(0,0));
    pStencil->setPosition(Vec2(pos.x+node->getContentSize().width/2.0f,pos.y+node->getContentSize().height/2.0f));
    pStencil->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.2),ScaleTo::create(1, 1), NULL)));
        
    //将这个圆形从剪裁节点上面抠出来，Stencil是模板的意思..
    pClip->setStencil(pStencil);
}
void Finger_Tips::fingerAction()
{
    _finger->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, -30)),
                                                              MoveBy::create(1, Vec2(0, 30)),
                                                              NULL)));
}
void Finger_Tips::removeTips()
{
    if (pClip && pClip->getParent())
    {
        pClip->removeFromParent();
    }
    if (_lable && _lable->getParent())
    {
        _lable->removeFromParent();
    }
    if (_continue && _continue->getParent())
    {
        _continue->removeFromParent();
    }
    if(_lable1 && _lable1->getParent())
    {
        _lable1->removeFromParent();
    }
    if(_lable2 && _lable2->getParent())
    {
        _lable2->removeFromParent();
    }
    if(_finger && _finger->getParent())
    {
        _finger->removeFromParent();
    }
}
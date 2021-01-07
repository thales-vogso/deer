/****************************************************************************
    
 *	@desc   信息简介
 *	@date	2015-6-16
 *	@author	120103
     
 *	@file	popUp/Summery.cpp
 *   @modify	null
 ******************************************************************************/

#include "Summary.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"

USING_NS_CC;
USING_NS_DU;

Summary* Summary::create(std::string txt, cocos2d::Node* node,float nodeScale)
{
    Summary* summery = new Summary();
    if(summery&&summery->init(txt,node,nodeScale))
    {
        summery->autorelease();
        return summery;
    }
    else{
        delete summery;
        summery = NULL;
        return NULL;
    }
}
void Summary::setDelegate(LayerBase *node)
{
    _delegate = node;
}
bool Summary::init(std::string txt, cocos2d::Node* node,float nodeScale)
{
    if (!Layout::init())
    {
        return false;
    }
    Size win = Director::getInstance()->getWinSize();
    _displaceY = _DISPLACE_Y*win.height/SCREEN_HEIGHT;
    
    int size = (int)txt.length();
    if(size > 300)
    {
        txtWidth = WIDTH;
        auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("summaryWidth.csb");
        view->setContentSize(win);
        view->setPosition(Vec2(SCREEN_WIDTH/2,0));
        this->addChild(view);
        ui::Helper::doLayout(this);
        
        auto text = seekChildByNameWithRetType<ui::Text*>(this, "text");
        text->setString(txt);
        
        //适配界面大小..
        adaptContensize();
        
        auto panel = seekChildByNameWithRetType<ui::Layout*>(this, "Panel");
        panel->setTouchEnabled(false);
        auto outLine = seekChildByNameWithRetType<ui::ImageView*>(this, "outLine");
        auto closebtn = seekChildByNameWithRetType<ui::Layout*>(this, "closeBtn");
        closebtn->addTouchEventListener(CC_CALLBACK_2(Summary::remove, this));
        closebtn->setPositionY(outLine->getContentSize().height-31);
    }
    else
    {
        txtWidth = LITTLE;
        auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("summary.csb");
        view->setContentSize(win);
        if(node != NULL)
        {
            view->setPosition(Vec2(SCREEN_WIDTH/2,SCREEN_HEIGHT/2));
        }
        
        this->addChild(view);
        ui::Helper::doLayout(this);
        
        auto text = seekChildByNameWithRetType<ui::Text*>(this, "text");
        text->setString(txt);
        
        auto panel = seekChildByNameWithRetType<ui::Layout*>(this, "Panel");
        panel->setTouchEnabled(false);
        auto posPanle = seekChildByNameWithRetType<ui::Layout*>(this, "posPanle");
        //适配界面大小..
        adaptContensize();
        
        if(node!= NULL)
        {
            auto arrow = seekChildByNameWithRetType<ui::ImageView*>(this, "arrow");
            Vec2 nodeWps = node->convertToWorldSpace(Vec2(0,0));  //node's world pos
            Vec2 nodeArchPoint = node->getAnchorPoint();           //node 's archPoint
            float nodeWidth =   node->getContentSize().width;         //node 's width
            
            float arrowX = nodeWps.x + nodeWidth*(0.5)*nodeScale; // arrow 's world posx
            
            Vec2 wroldPos = Vec2(arrowX,arrow->getPositionY()); //arrow' s world pos
            
            Vec2 arrowRealPos = posPanle->convertToNodeSpace(wroldPos); //arrow's pos in posPanle
            
            if(arrowRealPos.x < LEFT_POSX)
            {
                arrowRealPos.x = LEFT_POSX;
            }
            else if(arrowRealPos.x > RIGHT_POSX)
            {
                arrowRealPos.x = RIGHT_POSX;
            }
            arrow->setPositionX(arrowRealPos.x);
            float pY = panel->convertToNodeSpace(nodeWps).y;
            posPanle->setPositionY(pY-_displaceY);
            
            //适配界面方向
            adaptDirection(node,nodeScale);
        }
    }
    //点击移除..
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch* t, Event* e)->bool
    {
        if (isVisible())
        {
            auto outLine  = seekChildByNameWithRetType<ui::ImageView*>(this, "outLine");
            Vec2 outLinePos = outLine->convertTouchToNodeSpace(t);
            Rect box = outLine->boundingBox();
            Rect containRect = Rect(0,0,box.size.width,box.size.height);
            if(containRect.containsPoint(outLinePos))
            {
                return false;
            }
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [=](Touch * t, Event *e)
    {
        
        auto outLine  = seekChildByNameWithRetType<ui::ImageView*>(this, "outLine");
        Vec2 outLinePos = outLine->convertTouchToNodeSpace(t);
        Rect box = outLine->boundingBox();
        Rect containRect = Rect(0,0,box.size.width,box.size.height);
        if(containRect.containsPoint(outLinePos))
        {
               
        }
        else
        {
            this->removeFromParent();
        }
    };

    listener->setEnabled(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void Summary::remove(cocos2d::Ref *sender, cocos2d::ui::Layout::TouchEventType t)
{
    if (t == ui::Layout::TouchEventType::BEGAN)
    {
        
        this->removeFromParent();
    }
    else if(t == ui::Layout::TouchEventType::ENDED)
    {
        this->removeFromParent();
    }
}
void Summary::adaptContensize()
{
    auto text = seekChildByNameWithRetType<ui::Text*>(this, "text");
    std::string txt =   text->getString();
    
    text->setTextAreaSize(Size(txtWidth,0));
    text->ignoreContentAdaptWithSize(false);
    
    auto outLine = seekChildByNameWithRetType<ui::ImageView*>(this, "outLine");
    outLine->setContentSize(Size(outLine->getContentSize().width,text->getContentSize().height + 64));
    
}
void Summary::adaptDirection(cocos2d::Node* node,float nodeScale)
{
    auto Panle    = seekChildByNameWithRetType<ui::Layout*>(this, "Panel");
    auto posPanle = seekChildByNameWithRetType<ui::Layout*>(this, "posPanle");
    auto outLine  = seekChildByNameWithRetType<ui::ImageView*>(this, "outLine");
    auto text     = seekChildByNameWithRetType<ui::Text*>(this, "text");
    Vec2 panleWps = posPanle->convertToWorldSpace(Vec2(0,0));
    
    Vec2 nodeWps     =   node->convertToWorldSpace(Vec2(0,0));
    float nodeHeight =   node->getContentSize().height;
    
    float panelPosy = nodeWps.y + nodeHeight*nodeScale;
    Vec2 wroldPos = Vec2(posPanle->getPositionX(),panelPosy);
    
    Vec2 RealPos = Panle->convertToNodeSpace(wroldPos); //posPanle's pos in Panle
    if(panleWps.y- outLine->getContentSize().height <= 0)
    {
        posPanle->setScaleY(-1);
        text    ->setScaleY(-1);
        posPanle->setPositionY(RealPos.y+_displaceY);
        text    ->setPositionY(text->getPositionY()-text->getContentSize().height);//
        
    }
    
}






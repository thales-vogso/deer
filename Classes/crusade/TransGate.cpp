/********************************************
 *	@desc   战斗界面
 *	@date	2015-5-18
 *	@author	110101
     
 *	@file	crusade/TransGate.cpp
 *  @modify	null
 ********************************************/

#include "TransGate.h"
#include "popUp/BlackMask.h"
#include "utils/DeerUtils.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "data/Crusade.h"
#include "data/Stage.h"
#include "GlobalMacros.h"
#include "crusade/CrusadeMap.h"
#include "sound/SoundManager.h"
USING_NS_CC;
USING_NS_DU;

bool TransGate::init()
{
    if (!Layout::init())
    {
        return false;
    }
    
//    data::Stage::getInstance()->addUserStage(110201);
    //添加触摸遮罩..
    this->addChild(BlackMask::create());
    //初始化显示部分..
    auto win = Director::getInstance()->getWinSize();
    auto view = CSLoader::getInstance()->createNodeWithFlatBuffersFile("changjing.csb");
    view->setContentSize(win);
    this->addChild(view);
    ui::Helper::doLayout(view);
    
    /**
     *  listView的长度自适应
     */
    auto list = seekChildByNameWithRetType<ui::ListView*>(view, "list");
    Size conSize = list->getContentSize();
    float listOldHeight = conSize.height;
    conSize.height = Director::getInstance()->getWinSize().height - (deerResouseSize.height - list->getPositionY());
    list->setContentSize(conSize);
    
    auto bottomMask = seekChildByNameWithRetType<ui::ImageView*>(view, "bottomMask");
    bottomMask->setPositionY(bottomMask->getPositionY()-(conSize.height-listOldHeight));
    
    /**
     *  获取玩家现有的地图数据
     */
    auto ustage = data::Stage::getInstance()->getUserStages().asValueVector();
    
    auto sid = data::Crusade::getInstance()->getUserCrusade().asValueMap()["sid"].asInt();
    
    
    
    /**
     *  保存显示的模版，这个模版用来克隆
     */
    auto model = seekChildByNameWithRetType<ui::Layout*>(view, "map1");
    auto model_empty = seekChildByNameWithRetType<ui::Layout*>(view, "map1_0");
    _itemModel.pushBack(model);
    _itemModel.pushBack(model_empty);
    model->removeFromParent();
    model_empty->removeFromParent();
    
    list->addChild(_itemModel.at(1));
    /**
     *  玩家现有的地图不应显示
     */
    for (int i(0); i < ustage.size() ; ++i)
    {
        if (ustage.at(i).asValueMap()["sid"].asInt() == sid) continue;
        
        auto item = ListItem::createWithModel(_itemModel.at(0));
        item->upView(ustage.at(i).asValueMap());
        list->addChild(item);
    }
    
    /**
     *  添加玩所有的按钮我们来添加取消按钮
     */
    auto quit = QuitListItem::createWithModel(_itemModel.at(0));
    quit->setController(this);
    list->addChild(quit);
    
    return true;
}


/****************ListItems*************/

TransGate::ListItem *TransGate::ListItem::createWithModel(cocos2d::ui::Layout *model)
{
    auto ret = new TransGate::ListItem();
    if (ret && ret->init(model))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool TransGate::ListItem::init(cocos2d::ui::Layout *model)
{
    if (!Layout::init())
    {
        return false;
    }
    
    auto view = model->clone();
    setContentSize(view->getContentSize());
    view->setPosition(Point::ZERO);
    addChild(view);
    
    _name = seekChildByNameWithRetType<ui::Text*>(view, "name");
    
    auto btn = seekChildByNameWithRetType<ui::ImageView*>(view, "btnback");
    btn->setTouchEnabled(true);
    btn->addTouchEventListener(CC_CALLBACK_2(ListItem::choiceButtonCallback, this));
    btn->setOpacity(140);

    return true;
}

void TransGate::ListItem::upView(cocos2d::ValueMap data)
{
    _sid = data["sid"].asInt();
    _name->setString(data["name"].asString());
}

void TransGate::ListItem::choiceButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type)
{
    if (type == ui::Text::TouchEventType::ENDED)
    {
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(Effect_Undefine);
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        choiceReal();
    }
    //BTN_CALL_ACT(type, sender, sender, CC_CALLBACK_0(ListItem::choiceReal, this));
}

void TransGate::ListItem::choiceReal()
{
    //当经过传送门时，我们清空玩家出城的信息..
//    data::Crusade::getInstance()->townPortal();
    data::Crusade::getInstance()->depart(_sid);
    //运行下一个新的地图场景..
    CrusadeMap::goToCrusadeMapScene(_sid,true);
}

TransGate::QuitListItem* TransGate::QuitListItem::createWithModel(cocos2d::ui::Layout *model)
{
    auto ret = new TransGate::QuitListItem();
    if (ret && ret->init(model))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool TransGate::QuitListItem::init(cocos2d::ui::Layout *model)
{
    if(!ListItem::init(model))
    {
        return false;
    }
    _con = nullptr;
    _name->setString("  取消  ");
    
    return true;
}

void TransGate::QuitListItem::choiceReal()
{
    if(_con)
    {
        _con->removeFromParent();
    }
}


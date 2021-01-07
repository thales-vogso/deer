/**************************************************************
 *	@desc   战斗界面
 *	@date	2015-1-22
 *	@author	110101
     
 *	@file	crusade/CrusadeMap.cpp
 *   @modify	null
 ****************************************************/

#include "CrusadeMap.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "utils/DeerUtils.h"
#include "battle/BattleLayer.h"
#include "data/Crusade.h"
#include "popUp/ChoicePopup.h"
#include "battle/BattleLayer.h"
#include "manor/Manor.h"
#include "data/Crusade.h"
#include "manor/Shop.h"
#include "crusade/WildTavern.h"
#include "crusade/EnterChoicepop.h"
#include "data/Achievement.h"
#include "crusade/Package.h"
#include "crusade/Cemetery.h"
#include "crusade/SettlementLayer.h"
#include "crusade/GobackAsk.h"
#include "popUp/TouchEffect.h"
#include "GlobalMacros.h"
#include "utils/DoubleClickExit.h"
#include "crusade/CrusadeMapEventManager.h"
#include "notice/Notice.h"
#include "popUp/MapSummary.h"
#include "sound/SoundManager.h"

#include "core/custom/LibDate.h"
#include "data/Guid.h"

USING_NS_CC;
USING_NS_DU;

#define TO_S(_var_) #_var_
#define popUpOrder 50
#define ASKPOPNAME "askpopname"

TMXLayer* CrusadeMap::_fogLayer = nullptr;

CrusadeMap::CrusadeMap()
:_ismove(false)
,_prequit(false)
,_mode(TouchMode::kNone)
{
    _mapbackFileName.insert(std::make_pair(110101, "map_fqbj.csb"));
    _mapbackFileName.insert(std::make_pair(110201, "map_ttyhmxx.csb"));
    _mapbackFileName.insert(std::make_pair(110301, "map_hsbxxx.csb"));
    _mapbackFileName.insert(std::make_pair(110401, "map_crhs.csb"));
    _mapbackFileName.insert(std::make_pair(110501, "map_arsl.csb"));
    
    _defaultScale.insert(std::make_pair(110101, 0.5));
    _defaultScale.insert(std::make_pair(110201, 0.5));
    _defaultScale.insert(std::make_pair(110301, 0.5));
    _defaultScale.insert(std::make_pair(110401, 0.25));
    _defaultScale.insert(std::make_pair(110501, 0.25));
    _defaultScale.insert(std::make_pair(110601, 0.25));
    _defaultScale.insert(std::make_pair(110701, 0.25));
    _defaultScale.insert(std::make_pair(110801, 0.25));
    _defaultScale.insert(std::make_pair(110901, 0.25));
    _defaultScale.insert(std::make_pair(111001, 0.25));
    _defaultScale.insert(std::make_pair(111101, 0.25));
    _defaultScale.insert(std::make_pair(111201, 1.0));
    
    _mapBackTile.insert(std::make_pair(110101, std::make_pair(0, 1)));
    _mapBackTile.insert(std::make_pair(110201, std::make_pair(8, 9)));
    _mapBackTile.insert(std::make_pair(110301, std::make_pair(6, 7)));
    _mapBackTile.insert(std::make_pair(110401, std::make_pair(4, 5)));
    _mapBackTile.insert(std::make_pair(110501, std::make_pair(2, 3)));
    _mapBackTile.insert(std::make_pair(110601, std::make_pair(10, 11)));
    _mapBackTile.insert(std::make_pair(110701, std::make_pair(16, 17)));
    _mapBackTile.insert(std::make_pair(110801, std::make_pair(12, 13)));
    _mapBackTile.insert(std::make_pair(110901, std::make_pair(20, 21)));
    _mapBackTile.insert(std::make_pair(111001, std::make_pair(14, 15)));
    _mapBackTile.insert(std::make_pair(111101, std::make_pair(18, 19)));
    _mapBackTile.insert(std::make_pair(111201, std::make_pair(22, 23)));
}

CrusadeMap::~CrusadeMap()
{
    _mapbackFileName.clear();
    _defaultScale.clear();
    _mapBackTile.clear();
    _eventDispatcher->removeEventListener(_touchListener);
    Director::getInstance()->purgeCachedData();
}

CrusadeMap *CrusadeMap::create(int sid,bool newScene)
{
    CrusadeMap *ret = new CrusadeMap();
    if (ret && ret->init(sid,newScene))
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

void CrusadeMap::goToCrusadeMapScene(int sid,bool newScene)
{
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BackGroundMusic_Map, true);
    SoundManager::getInstance()->stopMusic();
    SoundManager::getInstance()->playMusic(SoundManager::MusicType::kMap);
    
    Scene * sce = Scene::create();
    CrusadeMap* map = CrusadeMap::create(sid,newScene);
    sce->addChild(map);
    map->setGuidScene(sce);
    Director::getInstance()->replaceScene(TransitionFade::create(0.7, sce));
}

void CrusadeMap::goToCrusadeMapSceneNoFade(int sid)
{
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BackGroundMusic_Map, true);
    SoundManager::getInstance()->stopMusic();
    SoundManager::getInstance()->playMusic(SoundManager::MusicType::kMap);
    Scene * sce = Scene::create();
    CrusadeMap* map = CrusadeMap::create(sid);
    sce->addChild(map);
    map->setGuidScene(sce);
    Director::getInstance()->replaceScene(sce);
}
void CrusadeMap::setGuidScene(cocos2d::Scene* scene)
{
    if(_guide&&scene)
    {
        _guide->setMapScene(scene);
    }
    /**
     *  添加地图简介
     */
    if(data::Stage::getInstance()->isUserStageFirstEnter(_sid))
    {
        _touchListener->setEnabled(false);
        _homeBtn->setEnabled(false);
        _packageBtn->setEnabled(false);
        
        MapSummary*  summery =  MapSummary::create(data::Stage::getInstance()->getStageBySID(_sid).asValueMap()["name"].asString(), data::Stage::getInstance()->getStageBySID(_sid).asValueMap()["summary"].asString());
        this->addChild(summery,154,"MapTitle");
        //延时手动显示地图新手引导..
        auto callFunc1 = CallFunc::create([=](){
            if(!data::Guid::getInstance()->showGuid())
            {
                _touchListener->setEnabled(true);
                _homeBtn->setEnabled(true);
                _packageBtn->setEnabled(true);
            }
        });
        this->runAction(Sequence::create(DelayTime::create(summery->getPlayTime()),callFunc1, NULL));
    }
    else
    {
        if(data::Guid::getInstance()->isCanShowShelterGuid())
        {
            _touchListener->setEnabled(false);
            _homeBtn->setEnabled(false);
            _packageBtn->setEnabled(false);
            //手动显示庇护所新手引导..
            data::Guid::getInstance()->showGuid(data::Guid::SHELTER_GUID);
        }
        else if(data::Guid::getInstance()->isCanShowMapGuid())
        {
            _touchListener->setEnabled(false);
            _homeBtn->setEnabled(false);
            _packageBtn->setEnabled(false);
            //手动显示地图新手引导..
            data::Guid::getInstance()->showGuid();
        }
    }
    
}
void CrusadeMap::toggleFog(bool fog){
	if (_fogLayer)_fogLayer->setVisible(fog);
}
bool CrusadeMap::init(int sid,bool newScene)
{
    
    if (!Layer::init())
    {
        return false;
    }
    _newScene = newScene;

    //数据初始化..
    auto userCruInfo = data::Crusade::getInstance()->getUserCrusade().asValueMap();

    
    _sid = userCruInfo["sid"].asInt();


    auto mapData = data::Stage::getInstance()->getStageMap(_sid);
    int mapHeight =  mapData.size();
    int mapWidth = mapData.at(0).size();
    
    _crusadeMap = TMXTiledMap::create("map/map.tmx");
    this->addChild(_crusadeMap);
    
    _tileSize = _crusadeMap->getTileSize();
    _mapSize = Size(mapWidth, mapHeight);
    _crusadeMap->setMapSize(Size(mapWidth, mapHeight));
    _crusadeMap->setContentSize(Size(mapWidth * _tileSize.width, mapHeight * _tileSize.height));
    _crusadeMap->setScale(1);
    
    auto mapLayer = _crusadeMap->getLayer("backLayer");
    mapLayer->setLayerSize(Size(mapWidth, mapHeight));
    auto buildingLayer = _crusadeMap->getLayer("buildLayer");
    buildingLayer->setLayerSize(Size(mapWidth, mapHeight));
    auto fogLayer = _crusadeMap->getLayer("fogLayer");
    fogLayer->setLayerSize(Size(mapWidth, mapHeight));
	_fogLayer = fogLayer;
    
    auto flagLayer = _crusadeMap->getLayer("flagLayer");
    flagLayer->setLayerSize(Size(mapWidth, mapHeight));
    
    ValueMap representation = FileUtils::getInstance()->getValueMapFromFile("mapFidReinterpret.plist");
    
    for (auto row(0); row < mapHeight; row++)
    {
        for (auto unit(0); unit < mapWidth; unit++)
        {
            //初始化地图的背景..
            if ((row + unit) % 2 == 0)
            {
                mapLayer->setTileGID(mapLayer->getTileSet()->_firstGid + _mapBackTile.at(_sid).first, Vec2(unit, mapHeight-1-row));
            }
            else
            {
                mapLayer->setTileGID(mapLayer->getTileSet()->_firstGid + _mapBackTile.at(_sid).second, Vec2(unit, mapHeight-1-row));
            }
            
            //初始化建筑..
            auto tileData = mapData[row][unit].asValueMap();
            
            int fid = tileData["fid"].asInt();
            int firstGid = buildingLayer->getTileSet()->_firstGid;
            
            if(fid != 0)
            {
                bool addflag = false;
                int imageid = tileData["field"].asValueMap()["img"].asInt();
                if (imageid % 2 == 0)
                {
                    --imageid;
                    addflag = true;
                }
                if (/*tileData["type"].asInt() == data::Stage::FieldType::SHELTER*/ tileData["field"].asValueMap()["type"].asInt() == data::Stage::FieldType::SHELTER)
                {
                    int visit = tileData.at("visit").asInt();
                    if (visit == data::Stage::ALREADY_VISIT)
                    {
                        imageid = 5;
                    }
                }
                
                buildingLayer->setTileGID((imageid+1)/2 -1 + firstGid, Vec2(unit, mapHeight-1-row));
                auto build = buildingLayer->getTileAt(Vec2(unit, mapHeight-1-row));
                //给建筑添加偏移量..
                build->setPosition(build->getPosition()+Vec2(2,2));
                //第三张地图和第十一张地图上庇护所要变色..
                if(_sid == 110301)
                {
                    if(fid == 110201||fid == 110202)
                    {
                        build->setColor(DeerUtils::hexToColor3B("#43b138"));
                    }
                }
                else if(_sid == 111101)
                {
                    if(fid == 110201||fid == 110202)
                    {
                        build->setColor(DeerUtils::hexToColor3B("#232323"));
                    }
                }
                
                if (addflag)
                {
                    flagLayer->setTileGID(54, Vec2(unit, mapHeight-1-row));
                }
            }
            if (tileData["fog"].asInt() == 1)
            {
                int fogfirst = fogLayer->getTileSet()->_firstGid;
                fogLayer->setTileGID(fogfirst , Vec2(unit, mapHeight-1-row));
                auto picefog = fogLayer->getTileAt(Vec2(unit, mapHeight-1-row));
                
                //给迷雾添加偏移量..
                picefog->setPosition(picefog->getPosition() + Vec2(-58, -58));
            }
        }
    }
    
    buildingLayer->getTexture()->setAntiAliasTexParameters();
    
    _crusadeMap->setAnchorPoint(Vec2(0.5, 0.5));
    _crusadeMap->setPosition(Director::getInstance()->getWinSize()/2);
    
    //布置玩家的位置..
    _player = Sprite::create("player.png");
    auto flash = Sprite::create("shanguang.png");
    flash->setPosition(Vec2(_player->getContentSize().width/2,_player->getContentSize().height/2));
    auto actFlash = RepeatForever::create(Sequence::create(FadeIn::create(0.5),FadeOut::create(0.5),NULL));
    flash->runAction(actFlash);
    _player->addChild(flash);
    auto wow = Sprite::create("WOW.png");
    wow->setNormalizedPosition(Vec2(0.5, 0.5));
    wow->setName("wow");
    wow->setVisible(false);
    _player->addChild(wow);
    
    auto moveHalo = Sprite::create("dituyuandian.png");
    moveHalo->setNormalizedPosition(Vec2(0.5, 0.5));
    moveHalo->setName("halo");
    moveHalo->setVisible(false);
    _player->addChild(moveHalo);
    
    auto mArrow = Sprite::create("ditujiantou.png");
    mArrow->setNormalizedPosition(Vec2(0.5, 1.2));
    mArrow->setName("arrow");
    moveHalo->addChild(mArrow);
    
    if (userCruInfo["sid"].isNull())
    {
        auto portal = data::Stage::getInstance()->getPortal(sid).asValueMap();
        _player->setPosition(mapLayer->getTileAt(Vec2(portal["x"].asInt(),_mapSize.height-1 - portal["y"].asInt()))->getPosition() + _tileSize/2);
    }
    else
    {
        Vec2 pos = Vec2(userCruInfo["x"].asInt(), _mapSize.height-1 - userCruInfo["y"].asInt());
        _player->setPosition(mapLayer->getTileAt(pos)->getPosition() + _tileSize/2);
    }
    _crusadeMap->addChild(_player, 5);
    
//    auto bchildren = buildingLayer->get;
//    for (auto &i: bchildren)
//    {
//        auto child = dynamic_cast<SpriteBatchNode*>(i);
//        if (child)
//        {
//            child->getTexture()->setAntiAliasTexParameters();
//        }
//    }
    
    
    //载入地图信息部分..
    auto mapTitle = CSLoader::getInstance()->createNodeWithFlatBuffersFile("mapTitle.csb");
    mapTitle->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(mapTitle);
    seekChildByNameWithRetType<ui::Layout*>(mapTitle, "Panel_1")->setSwallowTouches(false);
    ui::Helper::doLayout(mapTitle);
    
    auto stageInfo = data::Stage::getInstance()->getStageBySID(_sid);
    seekChildByNameWithRetType<ui::Text*>(mapTitle, "mapname")->setString(stageInfo.asValueMap()["name"].asString());
    int breadNum = data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD);
    _preBreadNum = breadNum;
    if(breadNum <= 5 )
    {
        seekChildByNameWithRetType<ui::Text*>(mapTitle, "breadnum")->setColor(Color3B::RED);
    }
    seekChildByNameWithRetType<ui::Text*>(mapTitle, "breadnum")->setString(Value(breadNum).asString());
    int achiNum = data::Achievement::getInstance()->getCounterAmountByCid(data::Achievement::EXPLORE);
    seekChildByNameWithRetType<ui::Text*>(mapTitle, "scandegree")->setString("探索度:" + Value(achiNum).asString());
    auto packCur = data::Crusade::getInstance()->getPackageSpace();
    auto packTotal = data::Crusade::getInstance()->getPackageTotal();
    _prePackageNum = packCur;
    seekChildByNameWithRetType<ui::Text*>(mapTitle, "packageCapacity")->setString(Value(packCur).asString() + "/" + Value(packTotal).asString());
    seekChildByNameWithRetType<ui::Text*>(mapTitle, "gohomenum")->setString(Value(data::Item::getInstance()->getUserItemAmountByIID(data::Item::WORMHOLE)).asString());
    
    //背包按钮..
    _packageBtn = seekChildByNameWithRetType<ui::Layout*>(mapTitle, "packageBtnArea");
    _packageBtn->addTouchEventListener(CC_CALLBACK_2(CrusadeMap::packageBtnCallback, this));
    
    _homeBtn = seekChildByNameWithRetType<ui::Layout*>(mapTitle, "gohomeBtnArea");
    _homeBtn->addTouchEventListener(CC_CALLBACK_2(CrusadeMap::backManor, this));
    
    /**
     *  添加事件管理器
     */
    this->addChild(CrusadeMapEventManager::createWithTarget(this));
    
    //多点触摸..
    _touchListener = EventListenerTouchAllAtOnce::create();
    _touchListener->onTouchesBegan =CC_CALLBACK_2(CrusadeMap::touchesBeganCall, this);
    _touchListener->onTouchesMoved = CC_CALLBACK_2(CrusadeMap::touchesMovedCall, this);
    _touchListener->onTouchesEnded = CC_CALLBACK_2(CrusadeMap::touchesEndedCall, this);
    _touchListener->onTouchesCancelled = CC_CALLBACK_2(CrusadeMap::touchesCancelledCall, this);
    _touchListener->setEnabled(true);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    
    /**
     *  添加tips管理器
     */
    Tips* tips = Notice::getInstance()->getTips();
    tips->clear();
	addChild(tips, popUpOrder - 1, "tips");
    
    _popUpManager = CrusadeMapPopupManager::create(this);
    this->addChild(_popUpManager, popUpOrder);
    

    //安卓双击返回退出..
    this->addChild(DoubleClickExit::create());
    
    /**
     *  点击效果
     */
    this->addChild(TouchEffect::create(), 155);
    
    /**
     *  玩家放在中心
     */
    movePointToCenter(_player->getPosition());
    
    /**
     *  新手引导
     */
    _guide = Guide::create(data::Guid::MAP_LAYER);
    _guide->setPlayer(_player);
    _guide->setCity(buildingLayer->getTileAt(Vec2(mapWidth/2,mapHeight/2)));
    _guide->setBreadText( seekChildByNameWithRetType<ui::Text*>(mapTitle, "breadnum"));
    _guide->setMapTouchListener(_touchListener);
    _guide->setHomeBtn(_homeBtn);
    _guide->setPackageBtn(_packageBtn);
    
    this->addChild(_guide,10000);

    //地图进入检测..
    data::Task::getInstance()->stageEnterCheck(_sid);
    
    //重置回城引导状态..
    data::Guid::getInstance()->setHomeGuid(false);
    
    return true;
}
void CrusadeMap::CrusadeRecover()
{
    /**
     *  战斗恢复
     */
    data::Crusade::getInstance()->moveOrigin();
}
void CrusadeMap::onEnter()
{
    Layer::onEnter();
//    getChildByName<TipsView*>("tips")->reset();
}

CrusadeMapPopupManager *CrusadeMap::getPopupManager()
{
    return _popUpManager;
};

void CrusadeMap::backManor(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType t)
{
    if (t == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        backManorReal();
    }
}

void CrusadeMap::backManorReal()
{
    addChild(GobackAsk::create(), popUpOrder);
}

void CrusadeMap::touchesBeganCall(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *e)
{
    if(!this->getChildByName("MapTitle"))
    {
        _mode = TouchMode::kPreMoveOrHit;
    }
}

void CrusadeMap::touchesMovedCall(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *e)
{
    if(!this->getChildByName("MapTitle"))
    {
        if (touches.size() >=2 && _mode == TouchMode::kPreMoveOrHit)
        {
            _mode = TouchMode::kZoom;
            _startScale = _crusadeMap->getScale();
            
            Vec2 anchor = _crusadeMap->getAnchorPoint();
            auto startpos1 = touches.at(0)->getStartLocation();
            auto startpos2 = touches.at(1)->getStartLocation();
            Vec2 mid = (startpos1 + startpos2)/2;
            mid = _crusadeMap->convertToNodeSpace(mid);
            
            Size mapS = _crusadeMap->getContentSize();
            Vec2 newAnchor = Vec2(mid.x / mapS.width, mid.y / mapS.height);
            _crusadeMap->setAnchorPoint(newAnchor);
            
            auto realWidth = _crusadeMap->getContentSize().width * _crusadeMap->getScale();
            auto newpos = _crusadeMap->getPosition() - anchor * realWidth + newAnchor * realWidth;
            _crusadeMap->setPosition(newpos);
        }
        else if (touches.size() >= 2 && _mode == TouchMode::kZoom)
        {
            auto touch1 = touches.front();
            auto touch2 = touches.at(1);
            float changelenth = std::abs(touch1->getLocation().x - touch2->getLocation().x) - std::abs(touch1->getStartLocation().x - touch2->getStartLocation().x);
            
            float scale = changelenth * 3 /1400;
            scale = _startScale + scale;
            if(scale > 3) scale = 3;
            if(scale < _defaultScale.at(_sid)) scale = _defaultScale.at(_sid);
            _crusadeMap->setScale(scale);
        }
        else if (touches.size() == 1 && (_mode == TouchMode::kPreMoveOrHit || _mode == TouchMode::kMove))
        {
            if (touches.at(0)->getDelta().length() > 15)
            {
                if (_mode != TouchMode::kMove) _mode = TouchMode::kMove;
                
                _crusadeMap->setPosition(_crusadeMap->getPosition() + touches.at(0)->getDelta());
                
                auto center = Director::getInstance()->getWinSize() / 2;
                auto crupos = _crusadeMap->getPosition();
                auto anchor = _crusadeMap->getAnchorPoint();
                auto scale = _crusadeMap->getScale();
                auto cruSiz = _crusadeMap->getContentSize() * scale;
                
                float XMax = center.width + cruSiz.width * anchor.x;
                float XMin = center.width - cruSiz.width * (1-anchor.x);
                
                if (crupos.x >= XMax)
                {
                    _crusadeMap->setPositionX(XMax);
                }
                else if (crupos.x <= XMin)
                {
                    _crusadeMap->setPositionX(XMin);
                }
                
                float YMax = center.height + cruSiz.height * anchor.y;
                float YMin = center.height - cruSiz.height * (1- anchor.y);
                
                if (crupos.y >= YMax)
                {
                    _crusadeMap->setPositionY(YMax);
                }
                else if (crupos.y <= YMin)
                {
                    _crusadeMap->setPositionY(YMin);
                }
            }
        }
    }
    
}

void CrusadeMap::touchesEndedCall(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *e)
{
    if(!this->getChildByName("MapTitle"))
    {
        if (_mode == TouchMode::kMove || _mode == TouchMode::kZoom)
        {
            _mode = TouchMode::kNone;
        }
        else if(touches.size() == 1 && _mode == TouchMode::kPreMoveOrHit)
        {
            //将触摸设为不可用..
            _touchListener->setEnabled(false);
            auto touchPoint = touches.at(0)->getLocation();
            Vec2 vec(_crusadeMap->convertToNodeSpace(touchPoint) - _player->getPosition());
            float angle = CC_RADIANS_TO_DEGREES(vec.getAngle());
            Vec2 destination(-1, -1);
            
            auto halo = _player->getChildByName("halo");
            if(data::Guid::getInstance()->isCanMoveOtherDirection())
            {
                if (-45<angle && angle<45)
                {
                    destination = data::Crusade::getInstance()->moveEast();
                    halo->setRotation(90);
                }
                else if(45 < angle && angle < 135)
                {
                    destination = data::Crusade::getInstance()->moveNorth();
                    halo->setRotation(0);
                }
                else if (135 < angle || angle < -135)
                {
                    destination = data::Crusade::getInstance()->moveWest();
                    halo->setRotation(-90);
                }
                else if (-135 < angle && angle < -45)
                {
                    destination = data::Crusade::getInstance()->moveSouth();
                    halo->setRotation(180);
                }
            }
            else
            {
                if (-135 < angle && angle < -45)
                {
                    destination = data::Crusade::getInstance()->moveSouth();
                    halo->setRotation(180);
                }
            }
            
            //迈腿音效哦..
            if(_footStep == 1)
            {
                _footStep++;
                SoundManager::getInstance()->playEffect(SoundManager::EffectType::kMoveStep1);
            }
            else
            {
                _footStep = 1;
                SoundManager::getInstance()->playEffect(SoundManager::EffectType::kMoveStep2);
            }
            if (!destination.equals(Vec2(-1, -1)))
            {
                auto iiii = _crusadeMap->convertToWorldSpace(_player->getPosition());
                auto win = Director::getInstance()->getWinSize();
                if (_crusadeMap->convertToWorldSpace(_player->getPosition()).getDistance(Director::getInstance()->getWinSize()/2) > 100)
                {
                    this->movePointToCenter(_player->getPosition());
                    
                    CallFunc* act2 = CallFunc::create([=](){this->movePointToCenter(Vec2(destination.x * _tileSize.width, destination.y * _tileSize.height) + _tileSize/2);});
                    this->runAction(Sequence::create(DelayTime::create(0.6), act2, NULL));
                    
                    MoveTo* move = MoveTo::create(0.5, Vec2(destination.x * _tileSize.width, destination.y * _tileSize.height) + _tileSize/2);
                    CallFunc* touchen = CallFunc::create([=]()
                                                         {
                                                             //引导过程中禁止自动恢复点击状态,转由引导层控制..
                                                             if(data::Guid::getInstance()->isCanMoveOtherDirection())
                                                             {
                                                                 _touchListener->setEnabled(true);
                                                             }
                                                         });
                    auto halo = _player->getChildByName("halo");
                    halo->setOpacity(255);
                    halo->setScale(1);
                    halo->runAction(Sequence::create(DelayTime::create(0.6), Show::create(), Spawn::create(FadeOut::create(0.5),NULL), Hide::create(),NULL));
                    _player->runAction(Sequence::create(DelayTime::create(0.6), move, touchen, NULL));
                    auto callFunc = CallFunc::create([=](){
                        //更新新手引导..
                        data::Guid::getInstance()->updateGuidInfo();
                    });
                    this->runAction(Sequence::create(DelayTime::create(1.1),callFunc, NULL));
                }
                else
                {
                    movePointToCenter(Vec2(destination.x * _tileSize.width, destination.y * _tileSize.height) + _tileSize/2);
                    MoveTo* goR = MoveTo::create(0.5, Vec2(destination.x * _tileSize.width, destination.y * _tileSize.height) + _tileSize/2);
                    CallFunc* touchen = CallFunc::create([=]()
                                                         {
                                                             //引导过程中禁止自动恢复点击状态,转由引导层控制..
                                                             if(data::Guid::getInstance()->isCanMoveOtherDirection())
                                                             {
                                                                 _touchListener->setEnabled(true);
                                                             }
                                                             
                                                         });
                    auto halo = _player->getChildByName("halo");
                    halo->setOpacity(255);
                    halo->setScale(1);
                    halo->runAction(Sequence::create(Show::create(), Spawn::create(FadeOut::create(0.5),NULL), Hide::create(),NULL));
                    _player->runAction(Sequence::create(goR,DelayTime::create(0.1), touchen, NULL));
                    auto callFunc = CallFunc::create([=](){
                        //更新新手引导..
                        data::Guid::getInstance()->updateGuidInfo();
                    });
                    this->runAction(Sequence::create(DelayTime::create(0.5),callFunc, NULL));
                }
            }
            _mode = TouchMode::kNone;
        }
    }
}

void CrusadeMap::touchesCancelledCall(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *e)
{
    _mode = TouchMode::kNone;
}

void CrusadeMap::enterShelter()
{
    
}

void CrusadeMap::onClearField(EventCustom *e)
{
   
}
void CrusadeMap::onItemChange(cocos2d::EventCustom* e)
{
    auto item = *(Value*)e->getUserData();
    if(item.asValueMap()["iid"].asInt() ==  data::Item::ItemID::WORMHOLE)
    {
        auto gohomenum = seekChildByNameWithRetType<ui::Text*>(this, "gohomenum");
        gohomenum->setString(item.asValueMap()["total"].asString());
    }
}
void CrusadeMap::onPackageChange(cocos2d::EventCustom *e)
{
    auto capacity = data::Crusade::getInstance()->getPackageSpace();
    auto totalCapacity = data::Crusade::getInstance()->getPackageTotal();
    auto packageTxt = seekChildByNameWithRetType<ui::Text*>(this, "packageCapacity");
    if(_prePackageNum != capacity)
    {
        packageTxt->setString(Value(capacity).asString() + "/" + Value(totalCapacity).asString());
        packageTxt->stopAllActions();
        packageTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.3),ScaleTo::create(0.1, 1),NULL));
    }
    
    int breadNum = data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD);
    auto breadTxt = seekChildByNameWithRetType<ui::Text*>(this, "breadnum");
    
    if(_preBreadNum != breadNum)
    {
        breadTxt->setString(Value(breadNum).asString());
        breadTxt->stopAllActions();
        if(breadNum <= 5 )
        {
            breadTxt->setColor(Color3B::RED);
            breadTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.6),ScaleTo::create(0.1, 1),NULL));
        }
        else
        {
            breadTxt->setColor(DeerUtils::hexToColor3B("#C1C1CA") );
            breadTxt->runAction(Sequence::create(ScaleTo::create(0.1, 1.3),ScaleTo::create(0.1, 1),NULL));
        }
    }
    
}

void CrusadeMap::exitFromSettlement(cocos2d::EventCustom *e)
{
    this->removeChildByName("settlement");
}

void CrusadeMap::packageBtnCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        SoundManager::getInstance()->playEffect(SoundManager::EffectType::kUndefine);
        packageReal();
    }
}

void CrusadeMap::packageReal()
{
    this->addChild(Package::create(), popUpOrder);
}

void CrusadeMap::movePointToCenter(cocos2d::Vec2 point)
{
    auto pWpos = _crusadeMap->convertToWorldSpace(point);
    Vec2 de = Vec2(Director::getInstance()->getWinSize()/2) - pWpos;
    
    _crusadeMap->runAction(MoveTo::create(0.5, _crusadeMap->getPosition() + de));
}

void CrusadeMap::setTouchEnabelInDelay(bool able)
{
    auto delay = DelayTime::create(1);
    CallFunc *call = CallFunc::create([=](){_touchListener->setEnabled(able);});
    this->runAction(Sequence::create(delay, call, NULL));
}

void CrusadeMap::enterShelter(const cocos2d::ValueMap &data)
{
    
}

void CrusadeMap::enterWildShop(const cocos2d::ValueMap &data)
{
    
}

void CrusadeMap::enterCemetery(const cocos2d::ValueMap &data)
{
    
}
void CrusadeMap::moveNothingHappen(cocos2d::EventCustom* e)
{
    if(data::Guid::getInstance()->isCanShowHomeGuid())
    {
        data::Guid::getInstance()->showHomeGuid();
    }
}
void CrusadeMap::playerAce(cocos2d::EventCustom *e)
{
    ValueMap data = *(ValueMap*)e->getUserData();
    if (data.at("cause").asInt() == data::Crusade::ACE_CAUSE::STARVATION)
    {
//        NotificationManager::getInstance()->postDeathNotice(cause);
		Notice::getInstance()->postBoard("食物耗尽了，探险队伍迷失在了茫茫宇宙中......");
        std::stringstream txt;
        txt<<"探险死亡: ";
        for (auto  &i : data.at("roles").asValueVector())
        {
            if (i.asValueMap()["amount"].asInt() > 0)
            {
                txt<<i.asValueMap()["name"].asString()<<"X"<<i.asValueMap()["amount"].asInt()<<" ";
            }
        }
        bool addItemlost = false;
        for (auto &i : data.at("items").asValueVector())
        {
            if (i.asValueMap()["amount"].asInt() > 0)
            {
                if (!addItemlost)
                {
                    txt<<"\n探险损失: ";
                    addItemlost = true;
                }
                txt<<i.asValueMap()["name"].asString()<<"X"<<i.asValueMap()["amount"].asInt()<<" ";
            }
        }
		Notice::getInstance()->postBoard(txt.str());
        Manor::goToManorScene(Manor::GoManorType::kNone);
    }
}

void CrusadeMap::playWow()
{
    auto wow = _player->getChildByName("wow");
    wow->setScale(2);
    wow->setVisible(true);
    wow->runAction(ScaleTo::create(0.4, 1));
}
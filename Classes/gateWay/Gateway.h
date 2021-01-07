/****************************************************************************
 
*	@desc	入口
*	@date	2014-12-12
*	@author	110101
   
*	@file	gateWay/Gateway.h
******************************************************************************/
#ifndef __GATEWAY_SCENE_H__
#define __GATEWAY_SCENE_H__

#include "cocos2d.h"
#include "manor/Manor.h"
#include "data/User.h"
#include "loading/Loading.h"
#include "gateWay/GateWayDelegate.h"
#include "SimpleAudioEngine.h"

class Gateway : public cocos2d::Layer, public GateWayDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(bool recovery = false);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(bool recovery);
    
    virtual ~Gateway();

    // implement the "static create()" method manually
//    CREATE_FUNC(Gateway);
    static Gateway *create(bool recovery);

private:
    /**
	 *	去主场景
	 */
    void gotoManor();
    
    /**
     *  游戏初始化
     */
    void gameInit();
    
    /**
     *  初始化数据模块
     */
    void initDataModules();
    
    /**
     *  显示模块初始化
     */
    void initUIModules();
    
    /**
     *  预加载下一个场景
     */
    void initNextScene();
    
    /**
     *  每张贴图加载后会调用这个函数使loading前进
     *
     *  @param cocos2d::Texture2D* <#cocos2d::Texture2D* description#>
     */
    void textureLoadCall(cocos2d::Texture2D*);
    
    /**
     *  数据模块和显示模块load完成后分别会调用一次这个函数，这个函数会检测当两个模块都初始完成时，会继续向下运行
     */
    void loadAlready();
    
    /**
     *  loading
     */
    Loading *_load;
    
    /**
     *  已经加载的贴图数量计数器
     */
    int uiloadCount;
    
    /**
     *  需要加载的贴图总数
     */
    int _totalpicCount;
    
    /**
     *  加载的模块计数器
     */
    int _loadModuleCount;
    
    /**
     *  crusade信息
     */
    cocos2d::ValueMap _userCruInfo;
    
    /**
     *  下一场景
     */
    cocos2d::Scene* _nextScene;
    
    cocos2d::Label *_per;
    
    /**
     *  是否需要清理数据
     */
    bool _recovery = false;
};

#endif // #define __GATEWAY_SCENE_H__
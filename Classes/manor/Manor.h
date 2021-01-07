/****************************************************************************
 *	@desc	主界面
 *	@date	2015-1-15
 *	@author	110101
     
 *	@file	manor/Manor.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_MANOR__
#define __DEER_MANOR__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "data/Role.h"
#include "data/Building.h"
#include "data/Crusade.h"
#include "manor/ManorMainLayer.h"

class Manor : public cocos2d::Layer
{
public:
    enum LayoutName
    {
        kBuildings =1,
        kWarehouse =2,
        kSettings =3,
        kStore =4,
        kMake = 5,
        kHero = 6,
        kCrystal = 7,
        kAchivement = 8,
        kTalent = 9,
        kRank = 10,
        kCollection = 11,
        kVenture = 12,
        kMainLayer = 13
    };
    
    enum GoManorType
    {
        kLoadingAndGuid = 0,
        kLoading = 1,
        kNone = 2
    };
    /**
     *  主城
     *
     *  @return
     */
    static Manor* getInstance(Manor::GoManorType type = Manor::GoManorType::kLoading);
    /**
     *   摧毁单例；
     */
    static void destory();
    
    /**
     *  创建场景
     *
     *  @return 主城场景
     */
    static cocos2d::Scene* createScene(Manor::GoManorType type = Manor::GoManorType::kLoading);
    
    /**
     *  以场景方式运行manor
     */
    static void goToManorScene(Manor::GoManorType type = Manor::GoManorType::kNone);
    
    /**
     *  初始化函数
     *
     *  @return 初始是否成功
     */
    virtual bool init(Manor::GoManorType type);
    
    /**
     *  构造
     */
    Manor();
    
    /**
     *  析构
     */
    virtual ~Manor();
    
    /**
     *  展示界面
     */
    void showLayer(int name);
    
    /**
     *   关闭界面
     */
    void shutLayer(int name);
    
    /**
     *  关闭当前界面之外的界面
     */
    void shutCurrentLayer();
    
    
    /**
     *  移除手指
     */
    void removeFinger();
    
    /**
     *  仓库是否打开
     *
     *  @return true 打开 false 关闭
     */
    bool isWarehouseOpen();
    
    void setTouchChangeEnable(bool enble);
    
    void showSuddenAfferInterval();
    
    void exitManorToMap();
    
    void setGuidScene(cocos2d::Scene* scene);

private:
    
    /**
     *  create函数
     */
    static Manor *create(Manor::GoManorType type = Manor::GoManorType::kLoading);
    
    /**
     *  界面的预备初始化检查
     */
    void preInitCheck(int name);
    
    /**
     *  反转到收集界面
     *
     *  @param unusingEvent 事件信息
     */
    void turnToCollection(cocos2d::EventCustom* unusedEvent);
    
    /**
     *  反转到探险界面
     *
     *  @param unusingEvent 事件
     */
    void turnToVenture(cocos2d::EventCustom* unusedEvent);
    
    /**
     *  转到左边的左边（这个位置显示的是成就等界面）
     */
    void turnToLeftLeft();
    
    /**
     *  向左转
     */
    void turnToLeft();
    /**
     *  向右转
     */
    void turnToRight();
    
    /**
     *  转到中间
     */
    void turnToCenter();
    
    /**
     *  收获
     *
     *  @param dt 时间
     */
    void gain(float dt);
	/**
	*	安卓键位
	*/
	void androidKeyinit();
    
    void purgePrequit();
    
    /**
     *  数据测试
     */
    void dataTest();
    
    /**
     *  播放开始动画
     */
    void playStartAnimation();
    
    /**
     *  播放按钮展示动画
     */
    void runButtonShowAct();
    
    /**
     *  展示随机事件
     */
    void showSudden(cocos2d::EventCustom *e);
    
    /**
     * 继承的onEnter()
     */
    void onEnter() override;
    
    /**
     *  我们重新实现这个方法的目的是为了在转换的时候就开始释放内存
     */
//    void onExitTransitionDidStart() override;
    
    /**
     *  新手引导界面恢复
     */
    void recoverGuideLayer();
    
private:
    
    /**
     *  调用收获的间隔
     */
    static const int callGainIntval;
    
    /**
     *  三个主界面
     */
    cocos2d::Node * _viewsNode;
    
    
    cocos2d::Map<int, cocos2d::Ref*> _layoutMap;
    
    /**
     *  触摸侦听
     */
    cocos2d::EventListenerTouchOneByOne * _touchLi;
    
    /**
     *  随机事件侦听
     */
    cocos2d::EventListenerCustom *_suddenLi;
    
    /**
     *  场景
     */
    static Manor* _instance;
    
	/**
	 *	随机事件标签
	 */
	const int SUDDEN_ACTION_TAG = 100;
    
    //预备退出标识位..
    bool _prequite;
    
    LayoutName currentlay;
    
    /**
     *  这个是用来记录子界面的析构进度的。
     */
    int _releaseRefCount;
};
#endif /* defined(__deer__Manor__) */

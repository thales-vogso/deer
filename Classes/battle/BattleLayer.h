/****************************************************************************
 *	@desc	战斗界面
 *	@date	15/8/17
 *	@author	110103
 *	@file	battle/BattleLayer.h
 ******************************************************************************/

#ifndef __DEER_BATTLELAYER__
#define __DEER_BATTLELAYER__

#include <iostream>
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "core/custom/Event.h"
#include "manor/LayerBase.h"

/**
 *  这个类需要成为一个总控制器（需要包含各种原因的战斗停止接口）
 *  要添加
 战斗实体（超级类）debuff控制接口
 怪物类（super node），怪物的战斗过程中的全部数据逻辑的实现，（需要循环调用攻击）
 玩家类（super node），负责玩家战斗中的数据逻辑的实现，（接口基本函数： 技能释放（带有参数指定技能），攻击减弱）
 效果显示类（控制负责控制攻击或回血等特效的显示，需要提供各个动效的接口，不含冷却）
 数值计算工具类
 *
 */

typedef std::function<void (int)> SkillBtnFunc;

class BattleViewController;

class BattleLayer : public cocos2d::Layer, public custom::Event,public LayerBase
{
public:
    /**
     *  创建参数
     *
     *  @return 一个战斗层对象
     */
    static BattleLayer *create(cocos2d::ValueMap battleData = cocos2d::ValueMapNull, bool isInBuilding = false);
    
    /**
     *  去战斗场景
     *
     *  @param sceneData 场景数据
     */
    static void gotoBattleScene(cocos2d::ValueMap battleData = cocos2d::ValueMapNull, bool isInBuilding = false);
    
    /**
     *   析构
     */
    virtual ~BattleLayer();
    
    void removeSelf() override;
    
    void updateProperty() override;
    /**
     *  离开战斗界面
     *
     *  @param event 事件
     */
    void exitFightingLayer(cocos2d::EventCustom *event);
    
    /**
     *  回血反馈
     */
    std::function<void ()> recovery;
    
    /**
     *  自动战斗反馈
     */
    std::function<void ()> onPressedAutoBattle;
    
public://共有变量
    
    //事件名称：玩家死亡..
    static std::string const PLAYER_DIE;
    
    //事件名称：怪兽死亡..
    static std::string const MONSTER_DIE;
    
    class SkillButton : public cocos2d::Node , public custom::Event
    {
    public:
        static SkillButton *create(cocos2d::ValueMap roleData, BattleLayer* delegate);
        
        /**
         *  设置自动攻击
         *
         *  @param autoBattle 是否自动攻击
         */
        void setAutoBattle(bool autoBattle);
        
        /**
         *  获取是够自动攻击
         *
         *  @return 是否自动攻击
         */
        bool isAutoBattle();
        
        /**
         *  战斗结束
         *
         *  @param event 事件
         */
        void endBattleCallback(cocos2d::EventCustom *event);
        
        /**
         *  设置可以响应触摸
         */
        void setTouchResponse(bool res);
        
        inline void setBtnId(int btnid){_btnid = btnid;};
        inline int getBtnId(){return _btnid;};
        
        void runCDAction(float interval);
        
        void CDEnd();
        
        SkillBtnFunc _btnFunc;
        
        /**
         *  视图
         */
        cocos2d::ui::Widget * _view;
        
    public:
        static std::string const PLAYER_ATTACK;
        
    protected:
        bool init(cocos2d::ValueMap roleData);
        
        /**
         *  技能按钮的回调
         *
         *  @param sender    按钮
         *  @param eventType 触摸类型
         */
        void skillButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType eventType);
        
    private:
        /**
         *  是否自动战斗
         */
        bool _isAutoBattle;
        
        /**
         *  角色数据
         */
        cocos2d::ValueMap _roleData;
        
        /**
         *  代理
         */
        BattleLayer* _delegate;
        
        /**
         *  技能按钮的序号
         */
        int _btnid;
        
        cocos2d::ProgressTimer* skillLoad;
        
        bool isAttackEnd= true;
    };
protected:
    /**
     *  初始化函数
     *
     *  @return 初始化是否成功
     */
    bool init(cocos2d::ValueMap battleData, bool isInBuilding);
    
//    /**
//     *  怪物攻击
//     */
//    void enemyAttack();
    
    /**
     *  玩家攻击..
     *
     *  @param roleAttackData 展示数据..
     */
    void playerAttack(cocos2d::EventCustom* event);

    /**
     *  自动战斗切换
     *
     *  @param sender    按钮
     *  @param eventType 事件类型
     */
    void autoBattleButtonCallback(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType eventType);
    
    /**
     *  回血回调
     *
     *  @param sender 按钮
     *  @param type   类型
     */
    void recoveryButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Button::TouchEventType type);
    
    /**
     *  遇怪事件的侦听，主要是为了进入建筑的下一层
     *
     *  @param event
     */
    void encountCallback(cocos2d::EventCustom *event);
    
    /**
     *  建筑肃清事件回调
     *
     *  @param event 事件
     */
    void fieldClearCallback(cocos2d::EventCustom *event);
    
    /**
     *
     *
     *  @param dt
     */
    void playerAttackAnimation(float dt);
    
    /**
     *  震动
     *
     *  @param target 震动对象
     */
    void shakeScreen(cocos2d::Node *target);
    
    void setAutoBattle(bool autbat);
    
    void stopAutoBattleAction();
    
private:
    
    cocos2d::MotionStreak* _streak1;
    /**
     *  怪兽的攻击动画
     */
    cocostudio::timeline::ActionTimeline *_attackAction;
    
    /**
     *  战斗数据
     */
    cocos2d::ValueMap _battleData;
    
    /**
     *  按钮的Vector
     */
    cocos2d::Vector<SkillButton*> _skillButtonVec;
    
    /**
     *  是否在建筑中
     */
//    bool _inBuilding;
    
    /**
     *  生命值字典
     */
    cocos2d::ValueMap _lifeMap;
    
    /**
     *  cd按钮的模板
     */
    cocos2d::Vector<cocos2d::ui::Button*> _cdBtnModel;
    
    /**
     *  遇怪事件侦听器
     */
    cocos2d::EventListenerCustom *_encountListener;
    
    /**
     *  场景清楚事件侦听器
     */
    cocos2d::EventListenerCustom *_clearListener;
    
    friend class BattleViewController;

    /**
     *  自动攻击按钮
     */
    cocos2d::ui::CheckBox* autoBattle;
    
    /**
     *  面包按钮
     */
    cocos2d::ui::Button*    breadBtn;
    
    /**
     *  batllecontroller_layer
     */
    cocos2d::Layer* batcon;
    
    /**
     *  battleviewcontroller_layer
     */
    cocos2d::Node* _viewCon;
};


#endif /* defined(__deer__BattleLayer__) */

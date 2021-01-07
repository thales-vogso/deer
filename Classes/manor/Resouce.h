/****************************************************************************
  
 *	@desc	资源界面
 *	@date	2015-1-9
 *	@author	110101
     
 *	@file	manor/ResouceLayer.h
 *  @modify	null
 ******************************************************************************/

#ifndef __DEER_RESOUCE__
#define __DEER_RESOUCE__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "manor/LayerNoTouchTrans.h"
#include "popUp/DoBatch.h"
#include "manor/LayerBase.h"
#include "guide/Guide.h"
#include "core/custom/Caches.h"

class Resouce : public cocos2d::ui::Layout, public LayerBase, public custom::Caches
{
protected:
    
    class ResouceItem;
    
public:
    Resouce();
    
    virtual ~Resouce();
    
    CREATE_FUNC(Resouce);
    
    /**
     *  打开
     *  @param type 打开的类型，该参数决定仓库顶端的箭头的指向位置
     */
    void open(int type = 0);
    
    /**
     *  关闭
     */
    void shut();
    
    inline bool isOpened()
    {
        return _isOpened;
    }
    
    /**
     *  移除自身，这个方法有Manor控制器调用，包含一个延时之后在移除自己
     */
    void removeSelf() override;
    void updateProperty() override;
    
private:
    
    bool init();
    
    /**
     *  item模版
     */
    cocos2d::Vector<cocos2d::ui::Layout*> _itemModel;
    
    /**
     *  模版位置
     */
    std::vector<cocos2d::Vec2> _modelPos;
    
    /**
     *  用来实现自动排列的中转变量
     */
    std::vector<cocos2d::Vector<cocos2d::ui::Layout*>> vov;
    
    ResouceItem* getItemByID(int resourceId);
    /**
     *  刷新
     */
    void updateitems();
    
    /**
     *  删除没用的items
     */
    void removeUnusedItems();
    
    /**
     *  是否可以被删除
     */
    bool isCanBeRemove(int resourceId);
    
    /**
     *  添加引导卖出文本
     */
    void addGuideText();
    
    /**
     *  提示文本关闭回调
     *
     */
    void textCloseCallback(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType t);
    
    /**
     *  侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _limap;
    
    /**
     *  刷新事件的响应回调
     */
    void onChange(cocos2d::EventCustom* unusede);
    
    /**
     *  弹出卖出界面
     *
     *  @param idata 数据
     */
    void showSell(cocos2d::ValueMap idata);
    
    /**
     *  弹出不能卖出界面
     *
     *  @param idata 数据
     */
    void showSellDeny(cocos2d::ValueMap idata);
    
    /**
     *  展示商品信息
     *
     *  @param txt 商品描述的内容
     */
    void showInfo(std::string txt , cocos2d::Node* node,float nodeScale);
    
    /**
     *  打开和关闭动画
     */
    cocostudio::timeline::ActionTimeline * _animation;
    
    /**
     *  打开状态标识位
     */
    bool _isOpened;
    
    /**
     * 新手引导手指
     */
    Guide* _guide = NULL;
    
    /**
     *  第一个可以卖出的商品
     */
    int _canSaleFirstIndex = -1;
    
    /**
     *  引导卖出文本显示状态
     *
     */
    bool _textShow = true;
    
    /**
     *  点击事件
     */
    cocos2d::EventListenerTouchOneByOne* listener;
    
    /**
     *  是否正在刷新
     */
    bool _isUpadteList = false;
    
    cocos2d::Value preReouceData = cocos2d::Value();
};

class Resouce::ResouceItem : public cocos2d::ui::Layout, public LayerBase
{
public:
    
    /**
     *  crate函数
     *
     *  @param resouceID 资源ID
     *
     *  @return 资源界面的item
     */
    static ResouceItem* create(Resouce *delegate);
    
    /**
     *  析构
     */
    virtual ~ResouceItem();
    
    void removeSelf() override;
    
    void updateProperty() override;
    /**
     *  设置透明度 我们重新写这个函数是为了在使fade动画可以递归调用
     *
     *  @param opacity 透明度值
     */
    virtual void setOpacity(GLubyte opacity) override;
    
    /**
     *  商品数据
     */
    cocos2d::ValueMap _data;
    
    /**
     *  商品ID
     */
    int getResourceId();
    
    void setData(cocos2d::Value data);
    
private:
    /**
     *  初始化函数
     *  @return 资源界面的item
     */
    bool init();
    
    /**
     *  点击回调
     *
     *  @param sender
     *  @param t
     */
    void touchCallback(cocos2d::Ref* sender, cocos2d::ui::Layout::TouchEventType t);
    void touchReal();
    
    /**
     *  长按检测
     */
    void longTouchCheck();
    
    /**
     *  update
     *
     */
    
    
    /**
     *  资源改变
     *
     *  @param event 事件
     */
    void onResouseChange(cocos2d::EventCustom *event);
    
    
private://私有变量
    
    /**
     *  侦听器字典
     */
    cocos2d::Map<std::string, cocos2d::EventListenerCustom*> _listenerMap;
    
    /**
     *  代理
     */
    Resouce *_delegate;
    
    /**
     *   点击表示
     */
    bool _longtouched;
    
    /**
     *  资源ID
     */
    int _resouceIID = -1;
    
    /**
     *  长按进度条
     */
    cocos2d::ui::LoadingBar* _loadingBar;
    
    cocos2d::ui::Layout* _view = nullptr;
    
    std::function<void()> actCall = nullptr;
};


class SellDeny : public cocos2d::ui::Layout , public LayerNoTouchTrans
{
public:
    static SellDeny *create(cocos2d::ValueMap idata);
    
protected:
    bool init(cocos2d::ValueMap idata);
    
    virtual void sureButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Text::TouchEventType type);
    void sureBtnCall();
    
    cocos2d::ValueMap _data;
};


#endif /* defined(__deer__ResouceLayer__) */

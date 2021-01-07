/****************************************************************************
 *	@desc   探险界面
 *	@date	2015-3-13
 *	@author	110101
     
 *	@file	manor/VentureLayer.cpp
  
 ******************************************************************************/

#ifndef __deer__Ranking__
#define __deer__Ranking__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "manor/LayerBase.h"
#include "cocos-ext.h"
#include "Translate.h"
#include "popUp/RankRegister.h"


class Ranking : public cocos2d::Layer, public LayerBase
{
public:
    CREATE_FUNC(Ranking);
    
    void removeSelf() override;
    void updateProperty() override;
    
    void showBoard();
    Ranking();
    ~Ranking();
    
private:

    bool init();
    
    void onEnter() override;
    
    /**
     *  关闭
     *
     *  @param sender 按钮
     *  @param type   类型
     */
    void shutDownCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    
    /**
     *  排行榜切换
     *
     *  @param sender 按钮
     *  @param type   类型
     */
    void rankTypeCallback(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType type);
    
    /**
     *  请求注册返回
     */
    void reqRegCallback(std::string str);
    
    /**
     *  请求排行榜信息成功
     */
    void reqRankListInfoSeccess(cocos2d::EventCustom *e);
    
    /**
     *  请求注册监听事件
     */
    void reqRegister(cocos2d::EventCustom *e);
    
    /**
     *  排行信息显示
     *
     */
    void  upList(cocos2d::ValueVector rank);
    
    const std::string   SELF_COLOR    = "#EF4A54";
    
    /**
     *  原件模型
     */
    cocos2d::ui::Layout* _itemPattern;
    
    /**
     *  list适配原件模型
     */
    cocos2d::ui::Layout* _itemEmpty;
    
    /**
     *  排行榜按钮
     */
    cocos2d::ui::Button* _rankTypeBtn;
    
    /**
     *  排行榜按钮文字
     */
    cocos2d::ui::Text*  _rankTypeBtnText;
    
    /**
     *  关闭按钮文字
     */
    cocos2d::ui::Text*  _closeBtnText;
    
    /**
     *  排行榜排行文本框
     */
    cocos2d::ui::Text*  _rankText;
    
    
    /**
     *  排行列表
     */
    cocos2d::ui::ListView* _listView;
    
    
    /**
     *  loading
     */
    RankTranslate* _translate;
    
    /**
     *  注册
     */
    RankRegister* _register;
    
    /**
     *  请求排行榜信息监听事件
     *
     */
    cocos2d::EventListenerCustom *_reqListener;
    
    /**
     *  请求注册监听事件
     *
     */
    cocos2d::EventListenerCustom* _reRegListener;
    
    /**
     *  界面显示主体
     */
    cocos2d::Node* _view;
    
    cocos2d::ui::Layout* _panle;
};

#endif /* defined(__deer__Ranking__) */

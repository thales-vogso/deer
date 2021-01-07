/****************************************************************************
        
 *	@desc	引导
 *	@date	2015-4-15
 *	@author	110101
     
 *	@file	guide/Guide.h
 ******************************************************************************/

#ifndef __DEER_GUIDE__
#define __DEER_GUIDE__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "popUp/Finger.h"
#include "manor/LayerBase.h"
#include "data/Guid.h"
#include "core/custom/Event.h"
#include "battle/BattleLayer.h"
#include "battle/Monster.h"
#include "battle/Player.h"

class Guide : public cocos2d::ui::Layout,  public custom::Event,public LayerBase
{
public:
    
    static Guide* create(int LayerType);
    
    void insertButton(int key, cocos2d::ui::Layout* value);
    void ereaseButton(int key);
    
    void setTouchListener(cocos2d::EventListenerTouchOneByOne* listener);
    
    void setCangkuBack(cocos2d::ui::ImageView* back);
    void setCangkuCanSaleIndex(int index);
    
    void clearButton();
    
    //地图界面相关..
    void setMapTouchListener(cocos2d::EventListenerTouchAllAtOnce* listener);
    void setPlayer(cocos2d::Sprite* player);
    void setBreadText(cocos2d::ui::Text* bread);
    void setCity(cocos2d::Sprite* city);
    void setMapScene(cocos2d::Scene* scene);
    void setPackageBtn(cocos2d::ui::Layout* packageBtn);
    void setHomeBtn(cocos2d::ui::Layout* homeBtn);
    
    //战斗界面相关..
    void setSkillBtnVector(cocos2d::Vector<BattleLayer::SkillButton*> skillButtonVec);
    void setMonster(Monster* node);
    void setHero(Player* node);
    void setAutoBattle(cocos2d::ui::CheckBox* autoBattle);
    void setBreadBtn(cocos2d::ui::Button* btn);
    
    /**
     *  战斗界面设置可点击区域的可用状态
     */
    void setBattleBtnEnable(bool bRet);
    
    void addFinishGuid();
    
private:
    
    bool init(int LayerType);
    
    void removeSelf() override;
    
    void updateProperty() override;
    
    /**
     *  标识当前引导是在那个界面的
     */
    int _layerType;
    
    /**
     *  新手引导小手
     */
    Finger* _finger = nullptr;
    
    /**
     *  回城引导界面
     */
    Finger_Tips* _finger_Tips = nullptr;
    
    /**
     *  新手引导tips
     */
    GuideTips* _tips = nullptr;
    
    /**
     *  地图界面主角
     */
    cocos2d::Sprite* _player = nullptr;
    
    /**
     *  地图界面相关节点
     */
    cocos2d::ui::Text* _bread = nullptr;
    cocos2d::Sprite* _city = nullptr;
    cocos2d::Scene* _mapScene = nullptr;
    cocos2d::ui::Layout* _homeBtn = nullptr;
    cocos2d::ui::Layout* _packageBtN = nullptr;
    
    /**
     *  战斗界面相关节点
     */
    cocos2d::Vector<BattleLayer::SkillButton*> _skillButtonVec;
    Monster*  _monster = nullptr;
    Player*   _hero = nullptr;
    cocos2d::ui::CheckBox* _autoBattle = nullptr;
    cocos2d::ui::Button* _breadBtn = nullptr;
    
    /**
     *  新手引导监听
     */
    void fingerCheck(cocos2d::EventCustom* e = nullptr);
    
    /**
     * 删除新手引导
     */
    void deleteGuid(cocos2d::EventCustom* e);
    void deleteFinger(cocos2d::Ref *sender, cocos2d::ui::Button::TouchEventType touchType);
    
    /**
     *  战斗界面根据按钮标签设置按钮可用状态
     */
    void setBattleButtonEnableByBtnId(int btnId);
    
    /**
     *  设置所有可点击区域的可用状态
     */
    void setButtonEnable(bool bRet);
    
    /**
     *  根据按钮标签设置该按钮可用
     */
    void setButtonEnableByBtnId(int btnId);
    
    /**
     *  添加新手引导手指
     */
    void addFinger(int btnId,int FingerType);
    
    /**
     *  地图界面添加新手引导手指
     */
    void addFinger();
    
    /**
     *  战斗界面添加新手引导手指
     */
    void addBattleFinger(int btnId,int FingerType);
    
    /**
     *  地图界面添加tips引导
     */
    void addGuideTips(int tipsID);
    
    /**
     *  地图界面添加回城指引
     */
    void addHomeGuid();
    
    /**
     *  删除地图界面tips引导
     */
    void removeGuideTips();
    
    /**
     *  删除新手手指
     */
    void removeFinger();
    
    /**
     * 战斗界面删除手指
     */
    void removeBattleFinger();
    
    /**
     *  主页上的所有可点击的按钮区域
     */
    cocos2d::Map<int, cocos2d::ui::Layout*> _btnsAreaMapInMain;
    
    /**
     *  顺序存储key值
     */
    cocos2d::Vector<cocos2d::ui::Layout*> buttonsVector;
    
    /**
     *  仓库点击事件
     */
    cocos2d::EventListenerTouchOneByOne* _listener = nullptr;
    
    /**
     *  地图点击事件
     */
    cocos2d::EventListenerTouchAllAtOnce* _mapListener = nullptr;
    
    /**
     *  仓库返回
     */
    cocos2d::ui::ImageView* _back = NULL;
    
    /**
     *  仓库可卖出商品的序列号
     */
    int _canSaleItemIndex = -1;
 };


#endif /* defined(__deer__Guide__) */

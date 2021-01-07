/****************************************************************************
 
*	@desc	用户信息
*	@date	2014-12-12
*	@author	110101
   
*	@file	data/User.h
******************************************************************************/
#ifndef __DATA_USER_H__
#define __DATA_USER_H__

#include "cocos2d.h"
#include "core/db/DBConnector.h"
#include "core/db/Table.h"
#include "core/custom/Event.h"
#include "core/custom/Memories.h"
#include "core/custom/LibDate.h"

namespace data {
/**
	 *	用户信息
	 */
typedef struct userTag {
    int uid;
    std::string name;
    int gold;
} UserInfo;
/**
	*	设置信息
	*/
typedef struct settingTag {
    int uid;
    int bgm;
    int se;
    int guide;
    int cguide;
    int first;
    bool guidTxt;
    bool rmbshow;
} SettingInfo;

class User : public custom::Event {
public:
    /**
		 *	初始化
		 */
    User();
    /**
		 *	析构函数，要清除有指针的内容
		 */
    virtual ~User();
    /**
		 *	获取实体
		 */
    static User* getInstance();
    static void destroyInstance();
    /**
		 *	初始化
		 */
    void init();
    /**
		*	初始化用户信息
		*/
    void initUserData();
    /**
	*	初始化设置信息--
	*/
    void initSettingData();

    /**
	*	从表中读取数据**
	*/
    cocos2d::Value getUserData();
    /**
	 *	设置金币**
	 *	@param	coin	金币数量**
	 */
    void setGold(int gold);
    /**
		 *	获取金币
		 *	@return	金币数量
		 */
    int getGold();
    /**
		 *	金币增加
		 *	@param	num	 增加的数量，可为负数
		 */
    bool goldPlus(int num);
    /**
		 *	金币减少
		 *	@param	num	减少的数量，不能为负数
		 *	@return	结果
		 */
    bool goldReduce(unsigned num);
    /**
		 *	设置名字
		 *	@param	姓名
		 */
    void setName(std::string name);
    /**
		 *	获取姓名
		 *	@return	姓名
		 */
    std::string getName();
    /**
		 *	获取用户id
		 */
    int getUID();
    
    /**
     *  获取仓库引导卖出文本的显示状态
     */
    bool isGuidTextClose();
    /**
     *  设置仓库引导卖出文本的显示状态
     */
    void setGuidTextClose(bool bRet);
    
    
    /**
		 *	设置日期
		 *	@param	date	日期，可以用时间戳，可以用datetime，没参数为当前日期
		 */
    void setDate(time_t timestamp);
    void setDate(std::string dateStr);
    void setDate();
    /**
		 *	获取日期
		 *	@return	日期
		 */
    std::string getDate();
    /**
		 *	用户默认信息
		 */
    static const UserInfo DEFAULT_USER_DATA;
    /**
		 *	数据表-用户
		 */
    static const std::string TABLE_USER;
    /**
		 *	金币改变事件
		 */
    static const std::string GOLD_CHANGE;

    /**
		*	设置背景音乐开关
		*	@param	bgm	背景音乐开关
		*/
    void setBGM(int bgm);
    /**
		*	获取背景音乐开关
		*	@return	背景音乐开关
		*/
    int getBGM();
    /**
		*	设置音效开关
		*	@param	effect	音效开关
		*/
    void setSE(int se);
    void setSoundEffect(bool valid);
    /**
		*	获取音效开关
		*	@return	音效开关
		*/
    int getSE();
    bool getSoundEffect();
    /**
		*	设置是否第一次进入游戏
		*	@param	first	第一次进入游戏
		*/
    void setFirstPlay(bool first);
    /**
		*	获取是否第一次进入游戏
		*	@return		是否第一次进入游戏
		*/
    int getFirstPlay();
    
    /**
     *  设置新手引导开关
     *
     *  @param guide 新手引导开关
     */
    void setGuide(bool guide);
    
    /**
     *  获取新手引导开关
     *
     *  @return guide 新手引导开关
     */
    bool getGuide();
    
    /**
     *  是否显示rmb购买
     *
     *  @return rmbShow rmb购买开关
     */
    bool getRMBShow();
    
    /**
     *  设置RMB购买开关
     *
     */
    void setRMBShow(bool rmbshow);
    
    /**
     *  设置当前新手引导任务ID
     *  @param  guidID 当前新手引导任务ID
     */
    void setGuideID(int guidID);
    
    /**
     *  获取当前新手引导任务ID
     *  @return 当前新手引导任务ID
     */
    int getGuideID();
    
    
    
    bool isAutoBattle();
    void setAutoBattle(bool autbat);

    void getUserInit();
    /**
		*	获取时间字符串
		*/
    std::string getTimeDate(time_t timestamp = NULL);
    /**
		*	设置场景默认信息
		*/
    static const SettingInfo DEFAULT_SETTING_DATA;
    /**
		*	数据表-设置场景
		*/
    static const std::string TABLE_SETTING;
    /**
		*	数据表-炸弹鱼的日志表
		*/
    static const std::string USER_BOMBFISH_LOG_TABLE;
    /**
		*	清空炸弹鱼的数据表
		*/
    void clearBombFishLog();

private:
    /**
		 *	自身
		 */
    static User* _instance;
    /**
		 *	数据库
		 */
    DBSqlite* _db;
    /**
		 *	金币数量
		 */
    int _gold;
    /**
		 *	用户uid
		 */
    int _uid;
    /**
		 *	姓名
		 */
    std::string _name;
    /**
		 *	音乐
	   	 */
    int _bgm;
    /**
		 *	音效
		 */
    int _se;
    /**
		 *	新手引导
		 */
    bool _guide = false;
    
    /**
         *  是不是第一次进入游戏
         */
    bool _first;
    
    /**
     *  新手引导任务ID
     */
    int _guideId;
    
    /**
     *  仓库引导卖出文字显示状态
     */
    bool _guideText;
    
    /**
     *  rmb购买开关
     */
    bool _rmbShow;
};
}
#endif // __DATA_USER_H__

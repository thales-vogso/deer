/****************************************************************************
  
 *	@desc	排行榜信息
 *	@date	2015-6-12
 *	@author	120103
     
 *	@file	data/Ranking.h
 ******************************************************************************/

#ifndef __deer__RankData__
#define __deer__RankData__

#include "cocos2d.h"
#include "core/custom/Event.h"
#include "popUp/RankRegister.h"

namespace data {
    
class Ranking : public  custom::Event
{
public:
    enum RANK_TYPE
    {
        EXPLORE_RANK = 0,
        BABIETA_RANK = 1
    };
    
    enum RANK_CALLBACK_CODE
    {
        REQ_SUCCESS  = 0,
        NOT_LOGIN    = 110101,
        NOT_REGISTER = 110107,
        NOT_REPORT_RANK = 112001,
        LOGIN_FAILED    = 110109,
        REGISTER_FAILED = 110202,
    };
    /**
     *	初始化
     */
    Ranking();
    /**
     *	析构函数，要清除有指针的内容
     */
    ~Ranking();
    
    /**
     *	获取实体
     */
    static Ranking* getInstance();
    static void destroyInstance();
    
    
    /**
     *  请求探索度排行榜
     */
    void reqExpRankInfo();
    
    /**
     *  请求探索度排行榜返回
     */
    void reqExpRankCallback(std::string str);
    
    /**
     *  请求提交探索度
     */
    void reqReportExpValue();
    
    /**
     *  请求提交探索度返回
     */
    void reqReportExpValueCallback(std::string str);
    
    
    /**
     *  请求登录
     */
    void reqLogin();
    
    /**
     *  请求登录返回
     */
    void reqLoginCallback(std::string str);
    
    /**
     *  返回用户自身排行榜信息
     *
     *  @return value
     */
    cocos2d::Value getUserRankInfo();
    
    /**
     *  返回排行榜列表
     *  
     *  @return cocos2d::Value
     */
    cocos2d::Value getRankListInfo();
    
    /**
     *  获取当前排行榜类别
     *
     *  @return int RANK_TYPE
     */
    int getRankType();
    
    /**
     *  设置当前排行榜类别
     *
     *  @param int RANK_TYPE
     */
    void setRankType(int rankType);
    
    /**
     *  请求排行榜信息监听标签
     */
    static const std::string REQ_RANKINFO;
    static const std::string REQ_REGISTER;
    
private:
    
    
    /**
     *  用户自身探索度排行榜信息
     */
    cocos2d::ValueMap  _userRankInfo;
    
    /**
     *  排行榜信息列表
     */
    cocos2d::ValueVector _rankList;

    /**
     *  当前排行榜类别
     *
     *  @value   RANK_TYPE
     */
    int   _rank_Type;
    
};
    
}

#endif /* defined(__deer__RankData__) */

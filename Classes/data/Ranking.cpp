/****************************************************************************
  
 *	@desc	排行榜信息
 *	@date	2015-6-12
 *	@author	120103
     
 *	@file	data/RankData.cpp
 ******************************************************************************/

#include "Ranking.h"
#include "GlobalMacros.h"
#include "core/web/Client.h"
#include "data/Achievement.h"
#include "utils/DeerUtils.h"

USING_NS_CC;
USING_NS_DU;

namespace data {
    
const  std::string  Ranking::REQ_RANKINFO = "reqRankInfo";
const  std::string  Ranking::REQ_REGISTER = "reqRegister";
static Ranking* _instance = nullptr;

Ranking::Ranking()
{
    _rank_Type = EXPLORE_RANK;
}

Ranking::~Ranking()
{

}


Ranking* Ranking::getInstance()
{
    if (!_instance) {
        _instance = new Ranking();
    }
    return _instance;
}
void Ranking::destroyInstance()
{
    
    if (_instance) {
        CC_SAFE_DELETE(_instance);
    }
}
Value Ranking::getUserRankInfo()
{
    return Value(_userRankInfo);
}
void Ranking::reqExpRankInfo()
{
    ValueMap param;
    param["type"] = "explore";
    web::Client::sendAndReceive("http://www.deer.vogso.com/rank.php?ac=getRankList",param,CC_CALLBACK_1(Ranking::reqExpRankCallback, this));
    CCLOG("requset expRank start");
}
void Ranking::reqExpRankCallback(std::string str)
{
    CCLOG("callback: %s",str.c_str());
    rapidjson::Document d;
    d.Parse<rapidjson::kNullType>(str.c_str());
    if(d.HasParseError()){
        CCLOG("GetParseError %s\n",d.GetParseError());
    }
    if(d.HasMember("code")){
        if(reqResultCompare(d,REQ_SUCCESS))
        {
            CCLOG("callback success!");
            //_expRankInfo;
            const rapidjson::Value& rank = d["data"]["rankList"];
            _rankList.clear();
            for (int i = 0; i < rank.Size(); i++){
                const rapidjson::Value& item = rank[i];
                ValueMap rankV;
                rankV["rank"] = Value(item["rank"].GetInt()).asString();
                rankV["name"]= item["name"].GetString();
                rankV["value"]= item["value"].GetString();
                rankV["uid"]  = item["uid"].GetString();
                _rankList.push_back(Value(rankV));
            }
                
            _userRankInfo["userExpRank"] = d["data"]["userRank"].GetInt();
            _userRankInfo["userUid"]     = d["data"]["userUid"].GetString();
            
            dispatch(REQ_RANKINFO,&_rankList);
        }
        else if(reqResultCompare(d,NOT_LOGIN))
        {
            CCLOG("%s",d["msg"].GetString());
            reqLogin();
        }
        else if(reqResultCompare(d,NOT_REGISTER))
        {
            CCLOG("%s",d["msg"].GetString());
            dispatch(REQ_REGISTER,NULL);
        }
        else if(reqResultCompare(d,NOT_REPORT_RANK))
        {
            CCLOG("%s",d["msg"].GetString());
            reqReportExpValue();
        }
    }
}
Value Ranking::getRankListInfo()
{
    return Value(_rankList);
}
int Ranking::getRankType()
{
    return _rank_Type;
}
void Ranking::setRankType(int rankType)
{
    _rank_Type =    rankType;
}
void Ranking::reqReportExpValue()
{
    ValueMap param;
    param["value"] =  data::Achievement::getInstance()->getCounterAmountByCid(data::Achievement::EXPLORE);
        
    web::Client::sendAndReceive("http://www.deer.vogso.com/rank.php?ac=addExplore",param, CC_CALLBACK_1(Ranking::reqReportExpValueCallback, this));
    CCLOG("request report expValue start");
}
void Ranking::reqReportExpValueCallback(std::string str)
{
    CCLOG("callback: %s",str.c_str());
    rapidjson::Document d;
    d.Parse<rapidjson::kNullType>(str.c_str());
    if(d.HasParseError()){
        CCLOG("GetParseError %s\n",d.GetParseError());
    }
    if(d.HasMember("code"))
    {
        if(reqResultCompare(d,REQ_SUCCESS))
        {
            //requset expRank info again
            CCLOG("callback success");
            reqExpRankInfo();
                
        }
        else if(reqResultCompare(d,NOT_LOGIN))
        {
            CCLOG("%s",d["msg"].GetString());
            reqLogin();
        }
        else if(reqResultCompare(d,NOT_REGISTER))
        {
            CCLOG("%s",d["msg"].GetString());
            dispatch(REQ_REGISTER,NULL);
        }
    }
}
void Ranking::reqLogin()
{
    cocos2d::ValueMap param;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    param["device"] = getDeviceID_IOS();
#else
    param["device"] = "123456";
#endif
    web::Client::sendAndReceive("http://www.deer.vogso.com/user.php?ac=login", param, CC_CALLBACK_1(Ranking::reqLoginCallback, this));
}
    
void Ranking::reqLoginCallback(std::string str)
{
    CCLOG("callback: %s",str.c_str());
    rapidjson::Document d;
    d.Parse<rapidjson::kNullType>(str.c_str());
    if(d.HasParseError())
    {
        CCLOG("GetParseError %s\n",d.GetParseError());
    }
    if(d.HasMember("code"))
    {
        if(reqResultCompare(d,REQ_SUCCESS))
        {
            //request rank info again
            CCLOG("register success");
                
            reqExpRankInfo();
        }
        else if(reqResultCompare(d,NOT_REGISTER))
        {
            //requset register user
            CCLOG("%s",d["msg"].GetString());
            dispatch(REQ_REGISTER,NULL);
        }
            
    }
}
    
    
}
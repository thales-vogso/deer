//
//  HttpManager.cpp
//  jelly3.5
//
//  Created by mac on 15/9/29.
//
//

#include "HttpManager/HttpManager.h"
//#include "MD5/CMD5Checksum.h"
using namespace std;
using namespace cocos2d;

using namespace network;
HttpManager* HttpManager::s_sharedHelper = NULL;
HttpManager* HttpManager::getInstance()
{
    if (!s_sharedHelper) {
        s_sharedHelper = new HttpManager();
    }
    return s_sharedHelper;
}

void HttpManager::request(string pURL, HttpCallblock block)
{
    //    return;
    CCLOG("url = %s",pURL.c_str());
    
    __String *pRequestUrl = __String::create(pURL);
    pRequestUrl->retain();
    
    HttpRequest* pRequest = new HttpRequest();
    pRequest->setUrl(pRequestUrl->getCString());
    pRequest->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    pRequest->setResponseCallback(this, httpresponse_selector(HttpManager::response));
    if (block) {
        HttpCallObject* callObject = new HttpCallObject();
        callObject->block = block;
        pRequest->setUserData((void *)callObject);
//        callObject->release();
    }
    
    HttpClient::getInstance()->send(pRequest);
    pRequest->release();
    CC_SAFE_RELEASE(pRequestUrl);
    
}
void HttpManager::requestpost(std::string ac, std::string device_id, std::string receipt_data,std::string good_id, std::string num, HttpCallblock block)
{
    std::string url = "http://deer.mgame.vogso.com/ios_verify.php?";
    std::string pURL =  url + "ac=" + ac +
    "&"+"device_id="+ device_id +
    "&" + "receipt_data=" + receipt_data +
    "&" +"good_id="+ good_id +
    "&" +"num=" + num;
    CCLOG("pURL = %s",pURL.c_str());
    this->request(pURL, block);
}
void HttpManager::response(cocos2d::network::HttpClient *sender,
              cocos2d::network::HttpResponse *response)
{
    vector<char> *buffer = response->getResponseData();
    CCLOG("error buffer: %s",response->getErrorBuffer());
    string strBuffer(buffer->begin(), buffer->end());
    
    HttpRequest* pRequest = response->getHttpRequest();
    HttpCallObject* callObject = (HttpCallObject *)(pRequest->getUserData());
//    callObject->retain();
    if (strBuffer == "") {
        if (callObject) {

            callObject->block(StringMake(""));
            callObject = NULL;
        }
        return;
    }
    
    if (callObject) {
        callObject->block(StringMake(strBuffer));
        callObject->release();
    }

}

void HttpManager::getProductOrder(HttpCallblock block)
{
    
    string pURL = "http://candy.mgame.vogso.com/ios_verify.php?";
    CCLOG("pURL = %s",pURL.c_str());
    this->request(pURL, block);
}

void HttpManager::getSecurityCode(std::string orderid,std::string mobile,std::string price,std::string Subject,HttpCallblock block)
{
    std::string url = "http://wo.unidian.com/VerificationCode_V1.aspx?";
    int times = (int)(time(0));
    time_t now = time(0);

    std::string time = Value(times).asString();
    std::string signStr = orderid + "#" + mobile + "#" + price + "#" + "lEocJ6VIKXEUIXvJYxP9rE7S";
 //   std::string signStrMD5 = CMD5Checksum::GetMD5OfString(signStr);
    std::string pURL =  url +
    "orderid=" + orderid +
    "&channelid=" + "27" +
    "&mobile=" + mobile +
    "&price=" + price +
    "&time=" + time +
    "&productid=" + "613" +
    "&sign=";
    CCLOG("pURL = %s",pURL.c_str());
    this->request(pURL, block);


}

void HttpManager::verificationSecurityCode(std::string orderid,std::string mobile,std::string price,std::string codesms,HttpCallblock block)
{
    std::string url = "http://wo.unidian.com/ConfirmPayment_V1.aspx?";
    int times = (int)(time(0));
    time_t now = time(0);
    std::string time = Value(times).asString();
    
    std::string signStr = orderid + "#" + mobile + "#" + price + "#" + time + "#" + codesms + "#" + "lEocJ6VIKXEUIXvJYxP9rE7S";
  //  std::string signStrMD5 = CMD5Checksum::GetMD5OfString(signStr);
    std::string pURL =  url +
    "orderid=" + orderid +
    "&codesms=" + codesms +
    "&time=" + time +
    "&sign=" +
    "&channelid=" + "27";
    
    CCLOG("pURL = %s",pURL.c_str());
    this->request(pURL, block);
}


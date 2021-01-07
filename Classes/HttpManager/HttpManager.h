//
//  HttpManager.h
//  jelly3.5
//
//  Created by mac on 15/9/29.
//
//

#ifndef __HttpManager__
#define __HttpManager__

#include <stdio.h>
#include "cocos2d.h"
#include "network/HttpClient.h"
USING_NS_CC;
typedef std::function<void (String *)> HttpCallblock;



class HttpCallObject : public Ref
{
public:
    HttpCallblock block;
};

class HttpManager :
public Ref
{
public:
    static HttpManager* getInstance();
    /** 通用请求方法 */
    void request(std::string pURL, HttpCallblock block);
    void requestpost(std::string ac, std::string device_id, std::string receipt_data,std::string good_id, std::string num, HttpCallblock block);

    
    /** 通用返回方法 */
    void response(cocos2d::network::HttpClient *sender,
                  cocos2d::network::HttpResponse *response);
    void getProductOrder(HttpCallblock block);
    void getSecurityCode(std::string orderid,std::string mobile,std::string price,std::string Subject,HttpCallblock block);
    void verificationSecurityCode(std::string orderid,std::string mobile,std::string price,std::string codesms,HttpCallblock block);


private:
    static HttpManager* s_sharedHelper;

    
};
#endif

#ifndef  __IOS_IAP_BRIDGE__
#define  __IOS_IAP_BRIDGE__

#include "cocos2d.h"
#include "IOSiAP.h"
#include "data/User.h"
#include "HttpManager/HttpManager.h"

class IOSiAPBridge : public IOSiAPDelegate
{
public:
	IOSiAPBridge();
	~IOSiAPBridge();
	IOSiAP *iap;
	void requestProducts(int gold);

	virtual void onRequestProductsFinish(void);
	virtual void onRequestProductsError(int code);
	virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity);
    
    void request_server(std::string verify, std::string imei, std::string receipt_data, std::string goods_id, std::string num);
    
    /*
    *   付款成功事件
    */
    static  std::string PURCHASESUCCESS;
    /*
    *   付款取消事件
    */
    static  std::string PURCHASEFAIL;
    /*
     *   恢复购买成功
     */
    static std::string RECOVERSUCCESS;
    /*
     *   完毕事件
     */
    static  std::string PURCHASEOVER;
    /*
     *   正在验证
     */
    static  std::string PURCHASEVERIFY;
private:
	std::vector<std::string> identifiers;
	std::string identifier;
    int _gold;
    //是否是广告的恢复购买。。。
 //   bool _adBlock;
    /*
     *  交易次数
     */
 //   int _transitionNum;
    
    int verify_num;
    
};

#endif // __IOS_IAP_BRIDGE__
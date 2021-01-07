#include "IOSiAPBridge.h"
#include "data/Item.h"
#include "notice/Tips.h"
#include "notice/Notice.h"
std::string IOSiAPBridge::PURCHASESUCCESS="purchasesuccess";
std::string IOSiAPBridge::PURCHASEFAIL="Purchasefails";
std::string IOSiAPBridge::RECOVERSUCCESS="recoversuccess";
std::string IOSiAPBridge::PURCHASEOVER="purchaseover";
std::string IOSiAPBridge::PURCHASEVERIFY = "PURCHASEVERIFY";

IOSiAPBridge::IOSiAPBridge()
{
	iap = new IOSiAP();
  //  _transitionNum=0;
	iap->delegate = this;
    identifiers.push_back("com.vogso.deer.diamonds_1");
    identifiers.push_back("com.vogso.deer.diamonds_2");
    identifiers.push_back("com.vogso.deer.diamonds_3");
    identifiers.push_back("com.vogso.deer.diamonds_4");
    identifiers.push_back("com.vogso.deer.diamonds_5");
    identifiers.push_back("com.vogso.deer.diamonds_6");
}

IOSiAPBridge::~IOSiAPBridge()
{
	delete iap;
}

void IOSiAPBridge::requestProducts(int gold)
{
    _gold = gold;
    verify_num = 0;
  //  _adBlock=ad;
    switch (gold) {
        case 50:
            identifier = "com.vogso.deer.diamonds_1";
            break;
        case 100:
            identifier = "com.vogso.deer.diamonds_2";
            break;
        case 500:
            identifier = "com.vogso.deer.diamonds_3";
            break;
        case 1000:
            identifier = "com.vogso.deer.diamonds_4";
            break;
        case 5000:
            identifier = "com.vogso.deer.diamonds_5";
            break;
        case 10000:
            identifier = "com.vogso.deer.diamonds_6";
            break;
        default:
            _gold = 0;
          //  identifier = "vogso.xiaolongnv.goodsone";
            break;
    }
    iap->requestProducts(identifiers);
    CCLOG("requestProducts");
}

void IOSiAPBridge::onRequestProductsFinish(void)
{
	//必须在onRequestProductsFinish后才能去请求iAP产品数据。
	IOSProduct *product = iap->iOSProductByIdentifier(identifier);
	// 然后可以发起付款请求。
    
//    if (_adBlock) {
//        iap->paymentWithADProduct(product);
//    }
//    else
//    {
         iap->paymentWithProduct(product);
  //  }
	//iap->paymentWithProduct(product, quantity);
    CCLOG("onRequestProductsFinish");
}

void IOSiAPBridge::onRequestProductsError(int code)
{
	//这里requestProducts出错了，不能进行后面的所有操作。
    CCLOG("onRequestProductsError");
}

void IOSiAPBridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
   // _transitionNum++;
   
	if (event == IOSIAP_PAYMENT_PURCHAED)
    {
        std::string imei = iap->require_imei();
        std::string receipt_data = iap->require_receiptdata();
        int times = (int)(time(NULL));
        std::string goods_id = Value(times).asString();
        std::string num = Value(1).asString();
        auto size = Director::getInstance()->getWinSize();
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASEVERIFY, nullptr);
        request_server("ios_verify", imei, receipt_data, goods_id, num);
 
	}
    else if(event==IOSIAP_PAYMENT_FAILED)
    {
        CCLOG("onPaymentEvent cancel");
    //    if (_transitionNum==1)
        {
            //付款失败或者取消付款。。
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASEFAIL,nullptr);
        }
    }
    else if(event==IOSIAP_PAYMENT_RESTORED)
    {
        //恢复购买成功。。。
//        if (_transitionNum==1)
//        {
//            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(RECOVERSUCCESS,nullptr);
//            data::Item::getInstance()->userItemPlus(data::Item::ItemID::CRYSTAL, _gold);
//        }

    }
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASEOVER,nullptr);
	//其他状态依情况处理掉。
    CCLOG("onPaymentEvent event:%d", event);
}
void IOSiAPBridge::request_server(std::string verify, std::string imei, std::string receipt_data, std::string goods_id, std::string num)
{
    HttpCallblock block =[=](cocos2d::__String *response)
    {
        std::string responseStr = response->getCString();
        CCLOG("responseStr = %s",responseStr.c_str());
        rapidjson::Document m_doc;
        m_doc.Parse<rapidjson::kNullType>(responseStr.c_str());
        if (m_doc.HasParseError())
        {}
        const rapidjson::Value &resultCode = m_doc["code"];
        int code = resultCode.GetInt();
        //   CCLOG("code : %d", code);
        if (code == 0)
        {
            data::Item::getInstance()->userItemPlus(data::Item::ItemID::CRYSTAL, _gold);
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASESUCCESS, nullptr);
        }
        else if(code == -1)
        {
            CCLOG("code = -1");
            if (verify_num < 2)
            {
                verify_num++;
                request_server(verify, imei, receipt_data, goods_id, num);
            }
            else
            {
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASEFAIL, nullptr);
            }
        }
        else
        {
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(PURCHASEFAIL, nullptr);
        }
        
    };
    HttpManager::getInstance()->requestpost(verify, imei, receipt_data, goods_id, num, block);
}
/****************************************************************************
 
*	@desc	web客户端
*	@date	2014-11-24
*	@author	110101
   
*	@file	game/Client.cpp
*	@modify	null
******************************************************************************/
#include "Client.h"
USING_NS_CC;
using namespace network;

namespace web{
	const std::string Client::STATUS = "webClientStatus";
	const std::string Client::COMPLETE = "webClientComplete";
	const std::string Client::CLOSE = "nodejsClientClose";
	const std::string Client::ERR = "webClientError";

	Client::Client(){
		network::HttpClient::getInstance()->enableCookies(NULL);
		_req = new HttpRequest();
		_evt = new EventDispatcher();
		_evt->setEnabled(true);
	}
	Client::~Client(){
		if (_req) _req->release();
	}
	Client* Client::sendAndReceive(std::string address, std::string param, const std::function<void(std::string)>& callback){
		Client* client = new Client();
		client->open(address);
		client->send(param);
		client->receive(callback);
		return client;
	}
	Client* Client::sendAndReceive(std::string address, ValueMap param, const std::function<void(std::string)>& callback){
		Client* client = new Client();
		client->open(address);
		client->send(param);
		client->receive(callback);
		return client;
	}
	Client* Client::sendAndReceive(std::string address, ValueMap param){
		Client* client = new Client();
		client->open(address);
		client->send(param);
		client->close();
		return client;
	}
	Client* Client::sendAndReceive(std::string address){
		Client* client = new Client();
		client->open(address);
		client->close();
		return client;
	}
	Client* Client::sendAndReceive(std::string address, const std::function<void(std::string)>& callback){
		Client* client = new Client();
		client->open(address);
		client->receive(callback);
		return client;
	}
	void Client::open(std::string address){
		_req->setUrl(address.c_str());
	}
	void Client::close(){
		_req->release();
	}
	void Client::send(std::string param){
		_req->setRequestType(HttpRequest::Type::POST);
		const char* postData = param.c_str();
		_req->setRequestData(postData, strlen(postData));
	}
	void Client::send(ValueMap param){
		std::stringstream str;
		for (std::pair<std::string, Value> v : param){
			str << v.first.c_str() << "=" << v.second.asString() << "&";
		}
		send(str.str());
	}
	void Client::receive(const std::function<void(std::string)>& callback){
		_callback = callback;
		addListener(Client::COMPLETE, CC_CALLBACK_1(Client::onComplete, this));
		/**/
		_req->setResponseCallback(
			[=](HttpClient *sender, HttpResponse *response){
				if (!response)return;
				int statusCode = response->getResponseCode();
				_evt->dispatchCustomEvent(Client::STATUS, &statusCode);
				if (!response->isSucceed()){
					std::string err(response->getErrorBuffer());
					CCLOG("err: %s", err.c_str());
					_evt->dispatchCustomEvent(Client::ERR, &err);
					return;
				}
				std::vector<char> *buffer = response->getResponseData();
				std::string str;
				for (char s : *buffer){
					str += s;
				}
				CCLOG("receive: %s", str.c_str());
				_evt->dispatchCustomEvent(Client::COMPLETE, &str);
				close();
			}
		);
		network::HttpClient::getInstance()->send(_req);
	}
	void Client::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback){
		_evt->addCustomEventListener(eventName, callback);
	}
	void Client::onComplete(EventCustom* e){
		std::string str = *(std::string*)e->getUserData();
		_callback(str);
	}
}
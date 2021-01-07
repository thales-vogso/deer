/****************************************************************************
*	@desc	socket客户端
*	@date	2015-2-27
*	@author	110101
*	@file	core/socket/Client.cpp
*	@modify	null
******************************************************************************/
#include "Client.h"
USING_NS_CC;
namespace tcp{
	const std::string Client::OPEN = "socketClientOpen";
	const std::string Client::MESSGAE = "socketClientMessage";
	const std::string Client::CLOSE = "socketClientClose";
	const std::string Client::ERR = "socketClientError";

	Client::Client(){
		_evt = new EventDispatcher();
		_evt->setEnabled(true);
		//_client = new network::SIOClient();
	}
	Client::~Client(){

	}
	void Client::open(std::string address){
		_client = network::SocketIO::connect(address, *this);
	}
	void Client::close(){
		_client->disconnect();
	}
	void Client::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback){
		_evt->addCustomEventListener(eventName, callback);
	}
	void Client::send(std::string msg){
		_client->send(msg);
	}
	void Client::onConnect(cocos2d::network::SIOClient* client)
	{
		_evt->dispatchCustomEvent(Client::OPEN, nullptr);
	}
	void Client::onMessage(cocos2d::network::SIOClient* client, const std::string& data)
	{
		std::string str = data;
		_evt->dispatchCustomEvent(Client::MESSGAE, &str);
	}
	void Client::onClose(cocos2d::network::SIOClient* client)
	{
		if (client == _client)
		{
			_client = NULL;
		}
		CC_SAFE_DELETE(client);
		_evt->dispatchCustomEvent(Client::CLOSE, nullptr);
	}
	void Client::onError(cocos2d::network::SIOClient* client, const std::string& data)
	{
		if (client == _client)
		{
			std::string str = data;
			_evt->dispatchCustomEvent(Client::ERR, &str);
		}
	}
}
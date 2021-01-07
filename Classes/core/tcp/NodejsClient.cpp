/****************************************************************************
*	@desc	node.js客户端
*	@date	2014-11-20
*	@author	110101
*	@file	game/NodejsClient.cpp
*	@modify	null
******************************************************************************/
#include "NodejsClient.h"
USING_NS_CC;

const std::string NodejsClient::OPEN = "nodejsClientOpen";
const std::string NodejsClient::MESSGAE = "nodejsClientMessage";
const std::string NodejsClient::CLOSE = "nodejsClientClose";
const std::string NodejsClient::ERR = "nodejsClientError";

NodejsClient::NodejsClient(){
	_evt = new EventDispatcher();
	_evt->setEnabled(true);
	_client = new network::WebSocket();
}
NodejsClient::~NodejsClient(){
	
}
void NodejsClient::open(std::string address){
	_client->init(*this, address);
}
void NodejsClient::close(){
	_client->close();
}
void NodejsClient::addListener(const std::string &eventName, const std::function<void(EventCustom*)>& callback){
	_evt->addCustomEventListener(eventName, callback);
}
void NodejsClient::send(std::string msg){
	_client->send(msg);
}
void NodejsClient::onOpen(cocos2d::network::WebSocket* ws)
{
	_evt->dispatchCustomEvent(NodejsClient::OPEN, nullptr);
}
void NodejsClient::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)
{
	std::string str = data.bytes;
	_evt->dispatchCustomEvent(NodejsClient::MESSGAE, &str);
}
void NodejsClient::onClose(cocos2d::network::WebSocket* ws)
{
	if (ws == _client)
	{
		_client = NULL;
	}
	CC_SAFE_DELETE(ws);
	_evt->dispatchCustomEvent(NodejsClient::CLOSE, nullptr);
}
void NodejsClient::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)
{
	if (ws == _client)
	{
		int code = (int)error;
		_evt->dispatchCustomEvent(NodejsClient::ERR, &code);
	}
}
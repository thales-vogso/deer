/****************************************************************************
*	@desc	node.js客户端
*	@date	2014-11-20
*	@author	110101
*	@file	game/NodejsClient.h
*	@modify	null
******************************************************************************/
/** example
client = new NodejsClient();
client->open("ws://feixin2014.mgametest.vogso.com:8100");
client->addListener(NodejsClient::OPEN, CC_CALLBACK_1(Game::onOpen, this));//侦听开始
client->addListener(NodejsClient::CLOSE, CC_CALLBACK_1(Game::onClose, this));//侦听关闭
client->addListener(NodejsClient::MESSGAE, CC_CALLBACK_1(Game::onMessage, this));//侦听数据传输
client->addListener(NodejsClient::ERR, CC_CALLBACK_1(Game::onError, this));//侦听错误
client->send("msg:100");
client->close();
void Game::onMessage(cocos2d::EventCustom* e){
std::string str = *(std::string*)e->getUserData();
CCLOG("messge from server, msg:%s", str.c_str());
}
*/
#ifndef __NODE_JS_CLIENT_H__
#define  __NODE_JS_CLIENT_H__
#include "cocos2d.h"
#include "network/WebSocket.h"

class NodejsClient : cocos2d::network::WebSocket::Delegate
{
public: 

	NodejsClient();
	virtual ~NodejsClient();
	/**
	 *	打开连接
	 */
	void open(std::string address);
	/**
	 *	关闭连接
	 */
	void close();
	/**
	 *	 发送信息
	 */
	void send(std::string msg);
	/**
	 *	接收原本的数据
	 */
	virtual void onOpen(cocos2d::network::WebSocket* ws);
	virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data);
	virtual void onClose(cocos2d::network::WebSocket* ws);
	virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error);
	/**
	 *	侦听
	 */
	void addListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback);
	/**
	 *	数据连接成功
	 */
	static const std::string OPEN;
	/**
	 *	接到服务器数据
	 *	@data	std::string	str	服务器传输的字符串
	 */
	static const std::string MESSGAE;
	/**
	 *	连接关闭
	 */
	static const std::string CLOSE;
	/**
	 *	错误
	 *	@data	int code	错误代码
	 */
	static const std::string ERR;
private:
	/*
	 *	客户端
	 */
	cocos2d::network::WebSocket* _client;
	/**
	 *	事件
	 */
	cocos2d::EventDispatcher* _evt;
};
#endif
/****************************************************************************
 
*	@desc	web客户端
*	@date	2014-11-24
*	@author	110101
   
*	@file	core/web/WebClient.h
*	@modify	null
******************************************************************************/
#ifndef __WEB_CLIENT_H__
#define __WEB_CLIENT_H__
#include "cocos2d.h"
#include "network/HttpClient.h"

namespace web{
	class Client
	{
	public:

		Client();
		virtual ~Client();
		/**
		 *	 发送信息,，并且回调
		 */
		static Client* sendAndReceive(std::string address, std::string param, const std::function<void(std::string)>& callback);
		static Client* sendAndReceive(std::string address, cocos2d::ValueMap param, const std::function<void(std::string)>& callback);
		static Client* sendAndReceive(std::string address, std::string param);
		static Client* sendAndReceive(std::string address, cocos2d::ValueMap param);
		static Client* sendAndReceive(std::string address);
		static Client* sendAndReceive(std::string address, const std::function<void(std::string)>& callback);
		/**
		 *	打开
		 *	@param	address	地址
		 */
		void open(std::string address);
		/**
		 *	关闭
		 */
		void close();
		/**
		 *	发送
		 *	@param	param	参数
		 */
		void send(std::string param);
		void send(cocos2d::ValueMap param);
		/**
		 *	接收
		 *	@param	callback	回调
		 */
		void receive(const std::function<void(std::string)>& callback);
		/**
		 *	侦听
		 */
		void addListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback);
		/**
		*	连接状态
		*	@data	int	code	连接状态代码
		*/
		static const std::string STATUS;
		/**
		*	完成
		*	@data	std::string	str	服务器返回的字符串
		*/
		static const std::string COMPLETE;
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
		*	请求
		*/
		cocos2d::network::HttpRequest* _req;
		/**
		*	事件
		*/
		cocos2d::EventDispatcher* _evt;
		/**
		 *	回调
		 */
		std::function<void(std::string)> _callback;
		/**
		 *	完成
		 */
		void onComplete(cocos2d::EventCustom* e);
	};
}
#endif //__WEB_CLIENT_H__
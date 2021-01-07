#include "DebugCommand.h"
#include "core/custom/LibString.h"
#include "data/Item.h"
#include "crusade/CrusadeMap.h"
#include "data/Task.h"
#include "utils/DeerUtils.h"
USING_NS_CC;

DebugCommand* DebugCommand::_instance = nullptr;

DebugCommand::DebugCommand()
{
	Director::getInstance()->getConsole()->listenOnTCP(TELNET_PORT);
}

DebugCommand::~DebugCommand()
{
    
}
void DebugCommand::launch(){
	if (!_instance)
		_instance = new DebugCommand();
	_instance->itemInit();
	_instance->statusInit();
	_instance->crusadeInit();
	_instance->talentInit();
	_instance->taskInit();
    _instance->roleInit();
}
void DebugCommand::itemInit(){
	Console::Command command;
	command.name = "item";
	command.help = "道具操作，plus添加道具，shop添加商品，produce添加制作";
	command.callback = [=](int n, const std::string& s){
		webdevlib::String str(s);
		str.trim();
		std::vector<webdevlib::String> vec = str.split(" ");
		if (vec[0] == "plus"){
			if (vec.size() == 3){
				data::Item::getInstance()->userItemPlus(Value(vec[1]).asInt(), Value(vec[2]).asInt());
			} else{
				data::Item::getInstance()->userItemPlus(Value(vec[1]).asInt());
			}
		}
		else if (vec[0] == "shop"){
			data::Item::getInstance()->shopItemShow(Value(vec[1]).asInt());
		}
		else if (vec[0] == "produce"){
			data::Item::getInstance()->addUserProduct(Value(vec[1]).asInt());
		}
	};
	Director::getInstance()->getConsole()->addCommand(command);
}
void DebugCommand::statusInit(){
	Console::Command command;
	command.name = "status";
	command.help = "on开启，off关闭";
	command.callback = [=](int n, const std::string& s){
		if (s == "on"){
			Director::getInstance()->setDisplayStats(true);
		}
		else if(s == "off"){
			Director::getInstance()->setDisplayStats(false);
		}
	};
	Director::getInstance()->getConsole()->addCommand(command);
}
void DebugCommand::crusadeInit(){
	Console::Command command;
	command.name = "crusade";
	command.help = "出征。move移动位置，x，y；encounter设置遇敌；fog战争迷雾on打开，off关闭；consume消耗move移动battle战斗";
	command.callback = [=](int n, const std::string& s){
		webdevlib::String str(s);
		str.trim();
		std::vector<webdevlib::String> vec = str.split(" ");
		if (vec[0] == "move"){
			if (vec.size() == 3){
				int x = atoi(vec[1].c_str());
				int y = atoi(vec[2].c_str());
				data::Crusade::getInstance()->move(x, y, true);
			}
		}
		else if (vec[0] == "encounter"){
			data::Crusade::getInstance()->setEncounter(Value(vec[1]).asBool());
		}
		else if (vec[0] == "fog"){
			if (vec[1] == "on"){
				CrusadeMap::toggleFog(true);
			}
			else if (vec[1] == "off"){
				CrusadeMap::toggleFog(false);
			}
		}
		else if (vec[0] == "consume"){
			if (vec[1] == "move"){
				data::Crusade::getInstance()->setConsumeForMove(Value(vec[2]).asBool());
			}
			else if (vec[1] == "battle"){
				data::Crusade::getInstance()->setConsumeForBattle(Value(vec[2]).asBool());
			}
		}
	};
	Director::getInstance()->getConsole()->addCommand(command);
}
void DebugCommand::talentInit(){
	Console::Command command;
	command.name = "talent";
	command.help = "天赋操作，add添加天赋";
	command.callback = [=](int n, const std::string& s){
		webdevlib::String str(s);
		str.trim();
		std::vector<webdevlib::String> vec = str.split(" ");
		if (vec[0] == "add"){
			data::Talent::getInstance()->addTalent(Value(vec[1]).asInt());
		}
	};
	Director::getInstance()->getConsole()->addCommand(command);
}
void DebugCommand::taskInit(){
	Console::Command command;
	command.name = "task";
	command.help = "sudden随机事件..";
	command.callback = [=](int n, const std::string& s){
		if (s == "sudden"){
			data::Task::getInstance()->suddenCheck();
		}
		else{
			data::Task::getInstance()->achieve(Value(s).asInt());
		}
	};
	Director::getInstance()->getConsole()->addCommand(command);
}
void DebugCommand::roleInit(){
    Console::Command command;
    command.name = "role";
    command.help = "角色操作，add添加角色";
    command.callback = [=](int n,const std::string& s)
    {
        webdevlib::String str(s);
        str.trim();
        std::vector<webdevlib::String> vec = str.split(" ");
        if(vec[0] == "add"){
            data::Role::getInstance()->addRole(Value(vec[1]).asInt());
        }
    };
    Director::getInstance()->getConsole()->addCommand(command);
}
/*void DebugCommand::appTimeInit()
{
    Console::Command command;
    command.name = "time";
    command.help = "应用时效操作，修改应用时效";
    command.callback = [=](int n,const std::string& s)
    {
        webdevlib::String str(s);
        str.trim();
        std::vector<webdevlib::String> vec = str.split(" ");
        if(vec[0] == "change"){
            //data::Role::getInstance()->addRole(Value(vec[1]).asInt());
            DeerUtils::_appTime = Value(vec[1]).asDouble();
        }
    };
    Director::getInstance()->getConsole()->addCommand(command);
}*/
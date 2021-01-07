/****************************************************************************
      
*	@desc	事件
*	@date	2015-3-18
*	@author	110101
   
*	@file	data/Task.cpp
******************************************************************************/

#include "Task.h"
#include "GlobalMacros.h"
#include "utils/DeerUtils.h"
#include "base/CCAsyncTaskPool.h"

USING_NS_CC;

namespace data {
	const std::string Task::TASK_COMPLETE = "taskComplete";
	const std::string Task::SUDDEN_OCCUR = "suddenOccur";
	const std::string Task::SUDDEN_CONFIRM = "suddenConfirm";
	const std::string Task::SUDDEN_CANCEL = "suddenCancel";

	Task* Task::_instance = nullptr;

	Task::Task()
	{
		this->init();
	}
	Task::~Task()
	{
	}
	Task* Task::getInstance()
	{
		if (!_instance) {
			_instance = new Task();
		}
		return _instance;
	}
    void Task::destroyInstance()
    {
        if (_instance)
        {
            CC_SAFE_DELETE(_instance);
        }
    }
	void Task::init()
	{
		_db = DBConnector::getInstance()->open();
		std::stringstream sql;
		//更新表--
        auto test = Table::getInstance()->getCSVByName(TABLE_TASK);
		_db->upgrade(TABLE_TASK, Table::getInstance()->getTableByName(TABLE_TASK), Table::getInstance()->getCSVByName(TABLE_TASK));//事件表..
		_db->upgrade(TABLE_TASK_TYPE, Table::getInstance()->getTableByName(TABLE_TASK_TYPE), Table::getInstance()->getCSVByName(TABLE_TASK_TYPE));//事件类型表..
		_db->upgrade(TABLE_TASK_BUTTON, Table::getInstance()->getTableByName(TABLE_TASK_BUTTON), Table::getInstance()->getCSVByName(TABLE_TASK_BUTTON));//事件按钮表..
		_db->upgrade(TABLE_TASK_CAUSE, Table::getInstance()->getTableByName(TABLE_TASK_CAUSE), Table::getInstance()->getCSVByName(TABLE_TASK_CAUSE));//事件前置表..
		_db->upgrade(TABLE_TASK_CONDITION, Table::getInstance()->getTableByName(TABLE_TASK_CONDITION), Table::getInstance()->getCSVByName(TABLE_TASK_CONDITION));//事件条件表..
		_db->upgrade(TABLE_TASK_RESULT, Table::getInstance()->getTableByName(TABLE_TASK_RESULT), Table::getInstance()->getCSVByName(TABLE_TASK_RESULT));//事件结果表..
		_db->upgrade(TABLE_TASK_SUDDEN, Table::getInstance()->getTableByName(TABLE_TASK_SUDDEN), Table::getInstance()->getCSVByName(TABLE_TASK_SUDDEN));//随机事件表..
		_db->upgrade(TABLE_USER_TASK, Table::getInstance()->getTableByName(TABLE_USER_TASK));//用户事件表..
        
        _db->upgrade(TABLE_USER_BUTTON, Table::getInstance()->getTableByName(TABLE_USER_BUTTON));
	}

	Value Task::getCause(int tid)
	{
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK_CAUSE << "` WHERE `tid` = '" << tid << "' ";
		Value all = _db->getAll(sql.str());
		return all;
	}
	Value Task::getCondition(int tid)
	{
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK_CONDITION << "` WHERE `tid` = '" << tid << "' ";
		Value all = _db->getAll(sql.str());
		return all;
	}
	Value Task::getResult(int tid)
	{
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK_RESULT << "` WHERE `tid` = '" << tid << "' ";
		Value all = _db->getAll(sql.str());
		return all;
	}
	bool Task::buildCheck(int building){
		/*
		Value* taskPool = new Value(ValueVectorNull);
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=]()
			{
				std::stringstream sql;
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::BUILDING << "' AND `type_id` = '" << building << "' ORDER BY `tid` ASC ";
				Value all = _db->getAll(sql.str());
				for (Value row : all.asValueVector()){
					int tid = row.asValueMap()["tid"].asInt();
					bool res = achieve(tid);
					if (!res) continue;
					taskPool->asValueVector().push_back(Value(tid));
				}
			}
		);
		*/
		std::stringstream sql;
		sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::BUILDING << "' AND `type_id` = '" << building << "' ORDER BY `tid` ASC /*task->buildCheck*/";
		Value all = _db->getAll(sql.str());
		for (Value row : all.asValueVector()){
			int tid = row.asValueMap()["tid"].asInt();
			bool res = achieve(tid);
			if (!res) continue;
            runResult(tid);
			Value task = getTaskByTID(tid);
			dispatch(TASK_COMPLETE, &task);
		}
		return true;
	}
	bool Task::baseCheck(){
		std::stringstream sql;
		sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::NONE << "' ORDER BY `tid` ASC LIMIT 1 /*task->fieldCheck*/";
		int tid = _db->getOne(sql.str()).asInt();
		bool res = achieve(tid);
		if (!res) return false;
        runResult(tid);
		Value task = getTaskByTID(tid);
		this->dispatch(TASK_COMPLETE, &task);
		return true;
	}
	bool Task::wormholeCheck(){
		bool res = achieve(CustomTaskID::wormhole);
		if (!res) return false;
        runResult(CustomTaskID::wormhole);
		Value task = getTaskByTID(CustomTaskID::wormhole);
		this->dispatch(TASK_COMPLETE, &task);
		return true;
	}
	bool Task::buttonCheck(int btn)
    {
        //将按钮点击记录下来..
        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [](void*){}, nullptr,
        [=]()
        {
            std::stringstream sql;
            sql << "SELECT COUNT(*) FROM "<<TABLE_USER_BUTTON<<" WHERE btid = "<<btn;
            bool recode = _db->getOne(sql.str()).asBool();
            if(!recode)
            {
                std::stringstream sql;
                sql.str("");
                sql<<"INSERT INTO "<<TABLE_USER_BUTTON<<" (btid, mdate) VALUES ("<<btn<<", '"<<data::User::getInstance()->getTimeDate()<<"')";
                _db->query(sql.str());
                
            }
            else
            {
                bool touched = BtnIsTouched(btn);
                if (!touched)
                {
                    std::stringstream sql;
                    sql.str("");
                    sql<<"UPDATE "<<TABLE_USER_BUTTON<<" SET mdate = '"<<data::User::getInstance()->getTimeDate()<<"' WHERE btid = "<< btn;
                    _db->query(sql.str());
                }
            }
        });
        
		for (int btnID : _btns){
			if (btnID == btn){
				break;
			}
		}
		
        
		Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=]()
			{
				std::stringstream sql;
				sql.str("");
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << Task::BUTTON << "' AND `type_id` ="<<btn;
                
                Value all = _db->getAll(sql.str());
                for (Value row : all.asValueVector()){
                    int tid = row.asValueMap()["tid"].asInt();
                    bool res = achieve(tid);
                    if (res)taskPool->asValueVector().push_back(Value(tid));
                }
			}
		);
		return true;
	}
    
	std::vector<int> Task::getButtons(){
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK_RESULT << "` `tr`,`" << TABLE_USER_TASK << "` `ut`  WHERE `tr`.`type`= '" << Task::BUTTON << "' AND `tr`.`tid` = `ut`.`tid`  /*task->getButtons*/";
		Value all = _db->getAll(sql.str());
		_btns.clear();
		for (Value row : all.asValueVector())
		{
			_btns.push_back(row.asValueMap()["type_id"].asInt());
		}
		return _btns;
	}
	void Task::stageEnterCheck(int sid){
		Value* taskPool = new Value(ValueVectorNull);
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=]()
			{
				std::stringstream sql;
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::STAGE_ENTER << "' AND `type_id` = '" << sid << "' ORDER BY `tid` ASC /*task->stageEnterCheck*/";
				Value all = _db->getAll(sql.str());
				for (Value row : all.asValueVector()){
					int tid = row.asValueMap()["tid"].asInt();
					bool res = achieve(tid);
					if (!res) continue;
					taskPool->asValueVector().push_back(Value(tid));
				}
			}
		);
	}
	void Task::stageClearCheck(int sid){
		Value* taskPool = new Value(ValueVectorNull);
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=]()
			{
				std::stringstream sql;
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::STAGE_CLEAR << "' AND `type_id` = '" << sid << "' ORDER BY `tid` ASC /*task->stageEnterCheck*/";
				Value all = _db->getAll(sql.str());
				for (Value row : all.asValueVector()){
					int tid = row.asValueMap()["tid"].asInt();
					bool res = achieve(tid);
					if (!res) continue;
					taskPool->asValueVector().push_back(Value(tid));
				}
			}
		);
	}
	bool Task::fieldCheck(int fid, int type){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << type << "' AND `type_id` IN (0, '" << fid << "') " << comment;
		Value all = _db->getAll(sql.str());
		bool ret = false;
		for (Value row : all.asValueVector()){
			int tid = row.asValueMap()["tid"].asInt();
			bool res = achieve(tid);
			if (res){
				ret = true;
                runResult(tid);
				Value task = getTaskByTID(tid);
				this->dispatch(TASK_COMPLETE, &task);
			}
		}
		return ret;
	}
	bool Task::fieldEnterCheck(int fid){
		return fieldCheck(fid, Task::FIELD_ENTER);
	}
	bool Task::fieldClearCheck(int fid){
		return fieldCheck(fid, Task::FIELD_CLEAR);
	}
	bool Task::fieldQuitCheck(int fid){
		return fieldCheck(fid, Task::FIELD_QUIT);
	}
	bool Task::fieldFailedCheck(int fid){
		return fieldCheck(fid, Task::FIELD_FAILED);
	}
    void Task::scanCheck(int sid)
    {
        char comment[200];
        sprintf(comment,"/* %s, %d */",__FILE__,__LINE__);
        std::stringstream sql;
        sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE<<"`WHERE `type` = '" << Task::SCAN<<"' AND `type_id` IN (0, '" << sid << "') "<<comment;
        Value all = _db->getAll(sql.str());
        for(Value row : all.asValueVector())
        {
            int tid = row.asValueMap()["tid"].asInt();
            bool res = achieve(tid);
            if(res)
            {
                runResult(tid);
                Value task = getTaskByTID(tid);
                this->dispatch(TASK_COMPLETE, &task);
            }
        }
    }
	void Task::itemCheck(int iid){
		Value* taskPool = new Value(ValueVectorNull);
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=](){
				char comment[200];
				sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
				std::stringstream sql;
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::ITEM << "' AND `type_id` = '" << iid << "' " << comment;
				Value all = _db->getAll(sql.str());
				for (Value row : all.asValueVector()){
					int tid = row.asValueMap()["tid"].asInt();
					bool res = achieve(tid);
					if (res){
						taskPool->asValueVector().push_back(Value(tid));
					}
				}
			}
		);
	}
	void Task::roleCheck(int rid){
		Value* taskPool = new Value(ValueVectorNull);
		AsyncTaskPool::getInstance()->enqueue(
			AsyncTaskPool::TaskType::TASK_IO,
			CC_CALLBACK_1(Task::taskThreadComplete, this),
			taskPool,
			[=](){
				char comment[200];
				sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
				std::stringstream sql;
				sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::ROLE << "' AND (`type_id` = '" << rid << "' OR `type_id` = 0) " << comment;
				Value all = _db->getAll(sql.str());
				for (Value row : all.asValueVector()){
					int tid = row.asValueMap()["tid"].asInt();
					bool res = achieve(tid);
					if (res){
						taskPool->asValueVector().push_back(Value(tid));
					}
				}
			}
		);
	}
    void Task::towerCheck(int round)
    {
        Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(
                                              AsyncTaskPool::TaskType::TASK_IO,
                                              CC_CALLBACK_1(Task::taskThreadComplete, this),
                                              taskPool,
                                              [=](){
                                                  char comment[200];
                                                  sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
                                                  std::stringstream sql;
                                                  sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::TOWER <<comment;
                                                  Value all = _db->getAll(sql.str());
                                                  for (Value row : all.asValueVector()){
                                                      int tid = row.asValueMap()["tid"].asInt();
                                                      if(checkTowerCondition(tid,round))
                                                      {
                                                          taskPool->asValueVector().push_back(Value(tid));
                                                      }
                                                  }
                                              }
                                              );
    }
    bool Task::checkTowerCondition(int tid,int towerRound)
    {
         Value task = getTaskByTID(tid);
         if(task.asValueMap()["multiple"].asInt() == 0&&isUserTaskExist(tid))
         {
             return false;
         }
         if(towerRound >= task.asValueMap()["condition"].asValueMap()["num"].asInt())
         {
             return true;
         }
         return false;
    }
    void Task::eatCheck()
    {
        Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(
                                              AsyncTaskPool::TaskType::TASK_IO,
                                              CC_CALLBACK_1(Task::taskThreadComplete, this),
                                              taskPool,
                                              [=](){
                                                  char comment[200];
                                                  sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
                                                  std::stringstream sql;
                                                  sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::EAT_BREAD<<"'" <<comment;
                                                  Value all = _db->getAll(sql.str());
                                                  for (Value row : all.asValueVector()){
                                                      int tid = row.asValueMap()["tid"].asInt();
                                                      bool res = achieve(tid);
                                                      if (res){
                                                          taskPool->asValueVector().push_back(Value(tid));
                                                      }
                                                  }
                                              }
                                              );
    }
    void Task::killEnemyCheck(int mid)
    {
        Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(
                                              AsyncTaskPool::TaskType::TASK_IO,
                                              CC_CALLBACK_1(Task::taskThreadComplete, this),
                                              taskPool,
                                              [=](){
                                                  char comment[200];
                                                  sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
                                                  std::stringstream sql;
                                                  sql << "SELECT * FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::KILL_ENEMY <<"'"<<comment;
                                                  Value all = _db->getAll(sql.str());
                                                  for (Value row : all.asValueVector()){
                                                      int typeID = row.asValueMap()["type_id"].asInt();
                                                      int tid    = row.asValueMap()["tid"].asInt();
                                                      if(typeID == mid) //杀死特定怪物成就..
                                                      {
                                                          Value task = getTaskByTID(tid);
                                                          if (task.asValueMap()["multiple"].asInt() == 0 && isUserTaskExist(tid)){
                                                              return;
                                                          }
                                                          sql.clear();
                                                          sql.str("");
                                                          sql << "REPLACE INTO `" << TABLE_USER_TASK << "` (`uid`, `tid`, `date`) VALUES ('" << User::getInstance()->getUID() << "','" << tid << "','" << User::getInstance()->getTimeDate() << "') " << comment;
                                                          bool res = _db->query(sql.str());
                                                          if (!res) return;
                                                          taskPool->asValueVector().push_back(Value(tid));
                                                      }
                                                      else if(typeID == 0) //杀死怪物数量成就..
                                                      {
                                                          int res = achieve(tid);
                                                          if(res)
                                                          {
                                                              taskPool->asValueVector().push_back(Value(tid));
                                                          }
                                                      }
                                                  }
                                              }
                                              );
    }
    void Task::buriedCheck(int rid)
    {
        Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(
                                              AsyncTaskPool::TaskType::TASK_IO,
                                              CC_CALLBACK_1(Task::taskThreadComplete, this),
                                              taskPool,
                                              [=](){
                                                  
                                                  Value role = data::Role::getInstance()->getRoleByRID(rid);
                                                  int level = role.asValueMap()["level"].asInt();
                                                  char comment[200];
                                                  sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
                                                  std::stringstream sql;
                                                  sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::BURIED <<"' AND (`type_id` = '" << level << "' OR `type_id` = 0) "  <<comment;
                                                  Value all = _db->getAll(sql.str());
                                                  for (Value row : all.asValueVector()){
                                                      int tid = row.asValueMap()["tid"].asInt();
                                                      bool res = achieve(tid);
                                                      if (res){
                                                          taskPool->asValueVector().push_back(Value(tid));
                                                      }
                                                  }
                                              }
                                              );
    }
    void Task::gamblingCheck()
    {
        Value* taskPool = new Value(ValueVectorNull);
        AsyncTaskPool::getInstance()->enqueue(
                                              AsyncTaskPool::TaskType::TASK_IO,
                                              CC_CALLBACK_1(Task::taskThreadComplete, this),
                                              taskPool,
                                              [=](){
                                                  char comment[200];
                                                  sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
                                                  std::stringstream sql;
                                                  sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::GAMEBLING <<comment;
                                                  Value all = _db->getAll(sql.str());
                                                  for (Value row : all.asValueVector()){
                                                      int tid = row.asValueMap()["tid"].asInt();
                                                      bool res = achieve(tid);
                                                      if (res){
                                                          taskPool->asValueVector().push_back(Value(tid));
                                                      }
                                                  }
                                              }
                                              );
    }
	int Task::suddenCheck(){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_USER_TASK << "` WHERE  `complete` != '0' ORDER BY `tid` DESC LIMIT 1 " << comment;
		Value row = _db->getRow(sql.str());
		int type;
		if (row.asValueMap()["tid"].isNull())
        {
			type = SUDDEN_TYPE_MAX;
		}
        else
        {
			time_t t = time(NULL);
			custom::LibDate d(row.asValueMap()["date"].asString());
			int last = d.secs() + SUDDEN_INTERVAL;
			if (t < last){ return 0; }
			Value task = getTaskByTID(row.asValueMap()["tid"].asInt());
			type = task.asValueMap()["sudden"].asValueMap()["type"].asInt();
		}
		int loop = 5;
		Value all;
		do
		{
			type = (type == TaskSuddenType::SUDDEN_TYPE_MAX) ? TaskSuddenType::SUDDEN_TYPE_MIN : type + 1;
			if (type == TaskSuddenType::SUDDEN_TYPE_MAX){
				if (random(0.0f, 1.0f) < 0.3){
					type = TaskSuddenType::SUDDEN_TYPE_MIN;

				}
			}
			sql.str(""); sql.clear();
			sql << "SELECT * FROM `" << TABLE_TASK_SUDDEN << "` WHERE `type` = '" << type << "' AND `tid` NOT IN (SELECT `tid` FROM `" << TABLE_USER_TASK << "` WHERE `complete` = 1)  ORDER BY `tid` ASC " << comment;
			all = _db->getAll(sql.str());
			loop--;
			if (loop == 0) return 0;
		} while (all.asValueVector().size() == 0);
		int weightTotal = 0;
		int tid = all.asValueVector()[0].asValueMap()["tid"].asInt();
		for (Value row : all.asValueVector())
		{
			weightTotal += row.asValueMap()["weight"].asInt();
		}
		int seed = random(0, weightTotal);
		int refer = 0;
		for (Value row : all.asValueVector())
		{
			refer += row.asValueMap()["weight"].asInt();
			if (seed <refer){
				tid = row.asValueMap()["tid"].asInt();
				break;
			}
		}
		bool res = suddenAchieve(tid);
		if (res)
        {
			return tid;
		}
		return 0;
	}
	bool Task::suddenConfirm(int tid){
		Value task = getTaskByTID(tid);
		int iid = task.asValueMap()["sudden"].asValueMap()["iid"].asInt();
		if (iid > 0){
			int amount = Item::getInstance()->getUserItemAmountByIID(iid);
			if (amount < task.asValueMap()["sudden"].asValueMap()["num"].asInt()) return false;
		}
		bool res = runResult(tid);
		if (!res) return false;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_TASK << "` SET `complete` = '1' WHERE `tid` = '" << tid << "' " << comment;
		res = _db->query(sql.str());
		if (!res) return false;
		std::string coinName = Item::getInstance()->getItemByIID(Item::COIN).asValueMap()["name"].asString();
		std::stringstream sstr;
		std::string itemName = Item::getInstance()->getItemByIID(iid).asValueMap()["name"].asString();
		float rate = task.asValueMap()["sudden"].asValueMap()["num"].asFloat() / 100.00f;
		int amount = floor((float)Item::getInstance()->getUserItemAmountByIID(iid) * rate);
		switch (tid)
		{
		case kSuddenMap :
			Crusade::getInstance()->scan(random(0, 25), random(0, 25));
			break;
		case kSuddenSell0:
		case kSuddenSell1:
			sstr.clear(); sstr.str("");
			sstr << "出售了  " << Item::getInstance()->getItemByIID(iid).asValueMap()["name"].asString() << " x " << amount << "，获得了  " << coinName << "  x " << Item::getInstance()->sell(iid, amount);
			break;
		case kSuddenLend0:
		case kSuddenLend1:
			sstr.clear(); sstr.str("");
			if (random(0.0f, 1.0f) < 0.5){
				Item::getInstance()->userItemPlus(iid, amount);
				sstr << "它们带着更多的  " << itemName << "  回来了。(" << itemName << " + " << amount << ")";
			}
			else{
				Item::getInstance()->userItemReduce(iid, amount);
				sstr << "它们带着你的  " << itemName << "  一去不复返。(" << itemName << " - " << amount << ")";
			}
			break;
		case kSuddenJeu:
			sstr.clear(); sstr.str("");
			if (random(0.0f, 1.0f) < 0.5f){
				Item::getInstance()->userItemPlus(iid, amount);
                data::Achievement::getInstance()->count(data::Achievement::CountID::GAMEBLING);
                data::Task::getInstance()->gamblingCheck();
				sstr << "你赢了！(" << coinName << " 增加了 " << amount << ")";
			}
			else{
				Item::getInstance()->userItemReduce(iid, amount);
				sstr << "你输了......(" << coinName << " 减少了 " << amount << ")";
			}
			break;
		default:
			break;
		}
		if (task.asValueMap()["board"].asString() == "") task.asValueMap()["board"] = sstr.str();
		this->dispatch(SUDDEN_CONFIRM, &task);
		return true;
	}
	void Task::suddenRMB(int tid){
		Value task = getTaskByTID(tid);
		if (task.asValueMap()["sudden"].asValueMap()["rmb"].asInt() > 0){

		}
		bool res = runResult(tid);
		if (!res) return;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_TASK << "` SET `complete` = '1' WHERE `tid` = '" << tid << "' " << comment;
		res = _db->query(sql.str());
		if (!res) return;
		this->dispatch(SUDDEN_CONFIRM, &task);
	}
	void Task::suddenCancel(int tid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "UPDATE `" << TABLE_USER_TASK << "` SET `complete` = '-1' WHERE `tid` = '" << tid << "' " << comment;
		_db->query(sql.str());
		Value task = getTaskByTID(tid);
		this->dispatch(SUDDEN_CANCEL, &task);
	}
    bool Task::acheivementCheck(int aid)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::ACHIEVEMENT << "' AND `type_id` IN (0, '" << aid << "')  ORDER BY `tid` ASC LIMIT 1 " << comment;
		int tid = _db->getOne(sql.str()).asInt();
		bool res = achieve(tid);
		if (!res) return false;
        runResult(tid);
		Value task = getTaskByTID(tid);
		this->dispatch(TASK_COMPLETE, &task);
		return true;
    }
    
    bool Task::talentCheck(int talentid)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `tid` FROM `" << TABLE_TASK_CAUSE << "` WHERE `type` = '" << TaskType::TALENT << "' AND `type_id` IN (0, '" << talentid << "')  ORDER BY `tid` ASC LIMIT 1 " << comment;
		int taskid = _db->getOne(sql.str()).asInt();
		bool res = achieve(taskid);
        runResult(taskid);
		if (!res) return false;
		Value task = getTaskByTID(taskid);
		this->dispatch(TASK_COMPLETE, &task);
        return true;
    }

	bool Task::achieve(int tid)
    {
		if (!checkCondition(tid)){
			return false;
		}
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		Value task = getTaskByTID(tid);
		if (task.asValueMap()["multiple"].asInt() == 0 && isUserTaskExist(tid)){
			return false;
		}
		sql.clear();
		sql.str("");
		sql << "REPLACE INTO `" << TABLE_USER_TASK << "` (`uid`, `tid`, `date`) VALUES ('" << User::getInstance()->getUID() << "','" << tid << "','" << User::getInstance()->getTimeDate() << "') " << comment;
		bool res = _db->query(sql.str());
		if (!res) return false;
		return true;
	}
	bool Task::suddenAchieve(int tid){
		if (!checkCondition(tid)){
			return false;
		}
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		Value task = getTaskByTID(tid);
		if (task.asValueMap()["multiple"].asInt() == 0){
			sql.clear(); sql.str("");
			sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_TASK << "` WHERE `tid` = '" << tid << "' AND `complete` = 1 LIMIT 1  /*task->isUserTaskExist*/";
			bool exist = _db->getOne(sql.str()).asBool();
			if (exist) return false;
		}
		if (tid == kSuddenLend0 || tid == kSuddenLend1 || tid == kSuddenJeu){
			float rate = task.asValueMap()["sudden"].asValueMap()["num"].asFloat() / 100.00f;
			task.asValueMap()["sudden"].asValueMap()["num"] = int(Item::getInstance()->getUserItemAmountByIID(task.asValueMap()["sudden"].asValueMap()["iid"].asInt()) * rate);
		}
		dispatch(SUDDEN_OCCUR, &task);
		sql.clear();sql.str("");
		sql << "REPLACE INTO `" << TABLE_USER_TASK << "` (`uid`, `tid`, `date`) VALUES ('" << User::getInstance()->getUID() << "','" << tid << "','" << User::getInstance()->getTimeDate() << "') " << comment;
		bool res = _db->query(sql.str());
		if (!res) return false;
		return true;
	}
	Value Task::getTaskByTID(int tid){
		Value val = getMem(tid);
		if (!val.isNull()) return val;
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK << "` WHERE `tid` = '" << tid << "' LIMIT 1 " << comment;
		val = _db->getRow(sql.str());
		val.asValueMap()["cause"] = getCause(tid);
		val.asValueMap()["condition"] = getCondition(tid);
		val.asValueMap()["result"] = getResult(tid);
		val.asValueMap()["sudden"] = getSudden(tid);
		setMem(tid, val);
		return val;
	}
	bool Task::runResult(int tid){
		Value task = getTaskByTID(tid);
		for (Value row : task.asValueMap()["result"].asValueVector())
		{
			int type = row.asValueMap()["type"].asInt();
			int id = row.asValueMap()["type_id"].asInt();
			int num = row.asValueMap()["num"].asInt();
			switch (type)
			{
			case TaskType::ITEM:
				if (num == 0) num = 1;
				Item::getInstance()->userItemPlus(id, num);
				break;
			case TaskType::ITEM_PRODUCE :
				Item::getInstance()->addUserProduct(id);
				break;
			case TaskType::ITEM_PRODUCE_CANNEL:
				Item::getInstance()->delUserProduct(id);
				break;
			case TaskType::BUILD:
				Building::getInstance()->addUserBuild(id);
				break;
			case TaskType::WORKER:
				Building::getInstance()->addUserWorker(id);
				break;
			case TaskType::ROLE:
				Role::getInstance()->train(id);
				break;
			case TaskType::STAGE_UNLOCK:
				Stage::getInstance()->addUserStage(id);
				break;
			case TaskType::SHOP_SHOW:
				Item::getInstance()->shopItemShow(id);
				break;
			case TaskType::TALENT:
				Talent::getInstance()->addTalent(id);
				break;
            case TaskType::ACHIEVEMENT:
                Achievement::getInstance()->addAchievement(id);
                break;
            case TaskType::STAGE_ENTER:
                
                break;
			default:
				break;
			}
		}
        for(Value row : task.asValueMap()["cause"].asValueVector())
        {
            int type = row.asValueMap()["type"].asInt();
            int id = row.asValueMap()["type_id"].asInt();
            switch (type)
            {
                case TaskType::STAGE_ENTER:
                    if(id == 110201)//进入第二张地图..
                    {
                        User::getInstance()->setRMBShow(true);
                    }
                    break;
                default:
                    break;
            }
        }
		return true;
	}
	bool Task::isUserTaskExist(int tid){
		std::stringstream sql;
		sql << "SELECT COUNT(*) AS `num` FROM `" << TABLE_USER_TASK << "` WHERE `tid` = '" << tid << "' LIMIT 1  /*task->isUserTaskExist*/";
		return _db->getOne(sql.str()).asBool();
	}
	bool Task::checkCondition(int tid)
    {
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT `complete` FROM `" << TABLE_USER_TASK << "` WHERE  `tid` = '" << tid << "' LIMIT 1 " << comment;
		int isComplete = _db->getOne(sql.str()).asInt();
		Value task = getTaskByTID(tid);
		if (task.asValueMap()["multiple"].asInt() == 0 && isComplete == 1){
			return false;
		}
		for (Value row : task.asValueMap()["condition"].asValueVector())
        {
			int type = row.asValueMap()["type"].asInt();
			int id = row.asValueMap()["type_id"].asInt();
			switch (type)
			{
                case TaskType::ITEM:
                    if (Item::getInstance()->getUserItemAmountByIID(id) < row.asValueMap()["num"].asInt()){
                        return false;
                    }
                    break;
                case TaskType::TASK:
                    if (!isUserTaskExist(id))
                    {
                        return false;
                    }
                    break;
				case TaskType::COUNTER:
					if (Achievement::getInstance()->getCounterAmountByCid(id) < row.asValueMap()["num"].asInt())
					{
						return false;
					}
					break;
                case BUTTON:
                    if (false /*  某按钮是否解锁  */)
                    {
                        return false;
                    }
                case BUILDING:
                    if (false/* 是否拥有某种建筑 */)
                    {
                        return false;
                    }
                    break;
                case ACHIEVEMENT:
                    if (false /*   是否完成某个成就  */ )
                    {
                        return false;
                    }
                    break;
                case SCAN:
                    if (Achievement::getInstance()->getCounterAmountByCid(Achievement::EXPLORE) < row.asValueMap()["num"].asInt()) {
                        return  false;
                    }
                    break;
                default:
                    break;
			}
		}
        return true;
	}
	cocos2d::Value Task::getSudden(int tid){
		char comment[200];
		sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
		std::stringstream sql;
		sql << "SELECT * FROM `" << TABLE_TASK_SUDDEN << "` WHERE  `tid` = '" << tid << "' ORDER BY `tid` DESC LIMIT 1 " << comment;
		Value row = _db->getRow(sql.str());
		row.asValueMap()["item"] = Item::getInstance()->getItemByIID(row.asValueMap()["iid"].asInt());
		return row;
	}
    bool Task::BtnIsTouched(int btid)
    {
        std::stringstream sql;
        sql<<"SELECT mdate FROM "<<TABLE_USER_BUTTON<<" WHERE btid ="<< btid;
        string mdate = _db->getOne(sql.str()).asString();
        if (mdate.empty()) return false;
        return mdate != TIME_NULL;
    }
    
    void Task::setBtnShowNew(int btid)
    {
        std::stringstream sql;
        sql<<"UPDATE "<<TABLE_USER_BUTTON<<" SET mdate = '"<<TIME_NULL<<"' WHERE btid = "<< btid;
        _db->query(sql.str());
    }
	void Task::taskThreadComplete(void* param){
		Value* taskPool = (Value*)param;
		for (Value id : taskPool->asValueVector()){
            runResult(id.asInt());
			Value task = getTaskByTID(id.asInt());
			this->dispatch(TASK_COMPLETE, &task);
		}
		taskPool->~Value();
		CC_SAFE_DELETE(param);
	}
}
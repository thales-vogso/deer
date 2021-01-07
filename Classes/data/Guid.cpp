/****************************************************************************
    
 *	@desc	新手引导信息
 *	@date	2015-06-30
 *	@author	120103
     
 *	@file	data/User.h
 ******************************************************************************/

#include "Guid.h"
#include "Item.h"
#include "Building.h"
#include "Crusade.h"
#include "GlobalMacros.h"
#include "DestroyData.h"
namespace data {
    
static    Guid* _instance = nullptr;
const std::string Guid::TABLE_GUID = "user_guid";
const std::string Guid::GUID_USER_OPERATION = "userOperation";
const std::string Guid::UPDATE_GUID = "updateGuid";
const std::string Guid::DELETE_GUID = "deleteGuid";
    
Guid::Guid()
{
    this->init();
}

Guid::~Guid()
{

}
Guid* Guid::getInstance()
{
    if (!_instance) {
        _instance = new Guid();
    }
    return _instance;
}
void Guid::destroyInstance()
{
    if (_instance) {
        CC_SAFE_DELETE(_instance);
    }
}
void Guid::init()
{
    _db = DBConnector::getInstance()->open();
    std::stringstream sql;
    sql << "SELECT COUNT(*) AS `num` FROM `" << Guid::TABLE_GUID.c_str() << "` LIMIT 1";
    /*int num = _db->getOne(sql.str()).asInt();
    if (num == 0) {
        
    }*/
    _db->upgrade(TABLE_GUID, Table::getInstance()->getTableByName(TABLE_GUID), Table::getInstance()->getCSVByName(TABLE_GUID));
    _maxGuidNum = 0;
    _guidFinishNum = 0;
    
    //如果战斗引导未完成重置并重新引导..
    isFinishBattleGuide();
    
    //计算剩余没完成引导任务数..
    sql.clear();
    sql.str("");
    sql << "SELECT * FROM `" << TABLE_GUID.c_str()<< "`WHERE `cstep`< `tstep`";
    cocos2d::Value all = _db->getAll(sql.str());
    for(int i = 0 ; i < all.asValueVector().size();i++)
    {
        cocos2d::ValueMap map =all.asValueVector().at(i).asValueMap();
        if(map["cstep"].asInt() < map["tstep"].asInt())
        {
            _maxGuidNum++;
        }
    }

    
}
bool Guid::isFinishBattleGuide()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        if(getGuideTaskFinishState(ATTACK1)&& getGuideTaskFinishState(ATTACK2)
           &&getGuideTaskFinishState(AUTO_ATTACK)&& getGuideTaskFinishState(FOOD_BACK))
        {
            return  true;
        }
        else
        {
            setStepByGuidId(ATTACK1,STEP0);
            setStepByGuidId(ATTACK2, STEP0);
            setStepByGuidId(AUTO_ATTACK, STEP0);
            setStepByGuidId(FOOD_BACK, STEP0);
            if(getGuideTaskFinishState(EXPLORE))
            {
                data::User::getInstance()->setGuideID(EXPLORE);
            }
            data::User::getInstance()->setAutoBattle(false);
            return false;
        }
    }
    return true;
}
bool Guid::FinishBattleGuide()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        setStepByGuidId(ATTACK1,STEP1);
        setStepByGuidId(ATTACK2, STEP1);
        setStepByGuidId(AUTO_ATTACK, STEP1);
        setStepByGuidId(FOOD_BACK, STEP1);
    }
    return false;
}
bool Guid::isNeedClearData()
{
    //如果玩家上次进行了新手引导，但是并没有完成，对不起请重新来过，就是这么任性..
    if(DEBUG_SHOW_GUID&&!isGuidFinish())
    {
        int currentID =getCurrentGuidId();
        if (currentID== GOLD) {
            if(getStepByGuidId(GOLD) < getMaxStepByGuidId(GOLD) && getStepByGuidId(GOLD)>0)
            {
                return true;
            }
        }
        else if(currentID > GOLD && currentID <WAR)
        {
            return true;
        }
        else if(currentID == WAR)
        {
            if(getStepByGuidId(WAR) < getMaxStepByGuidId(WAR))
            {
                return true;
            }
        }
    }
    return false;
}
int Guid::getCurrentGuidId()
{
    return User::getInstance()->getGuideID();
}
bool    Guid::isCanFastOperateWorkers()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        int currentID = getCurrentGuidId();
        if(currentID == WORKERS_USER1||currentID == WORKERS_USE1||
           currentID == WORKERS_USE2|| currentID == WORKERS_USE3)
        {
            if(getGuideTaskFinishState(currentID))
            {
                return true;
            }
            return  false;
        }
    }
    return true;
}
    
int Guid::getGuideLayer()
{
    if(!isGuidFinish()&& DEBUG_SHOW_GUID)
    {
        int guideID = getCurrentGuidId();
        int currentStep = getStepByGuidId(guideID);
        if(guideID == GOLD && currentStep == STEP0)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_BUILD && currentStep == STEP0)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_BUILD&& currentStep == STEP1)
        {
            return BUID_LAYER;
        }
        else if(guideID == COLLECTION&& currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == COLLECTION&& currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == COLLECTION&& currentStep == STEP2)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1&& currentStep == STEP0)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP2)
        {
            return RESOUCE_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP3)
        {
            return RESOUCE_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP4)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP5)
        {
            return BUID_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP6)
        {
            return BUID_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP7)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP8)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAREHOUSE_USE1 && currentStep == STEP9)
        {
            return BUID_LAYER;
        }
        else if(guideID == SHELTER_BUILD && currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == SHELTER_BUILD && currentStep == STEP1)
        {
            return BUID_LAYER;
        }
        else if(guideID == SHELTER_BUILD && currentStep == STEP2)
        {
            return MAIN_LAYER;
        }
        else if(guideID == SHELTER_BUILD && currentStep == STEP3)
        {
            return MAIN_LAYER;
        }
        else if(guideID == SHELTER_BUILD && currentStep == STEP4)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USER1 && currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USER1 && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WORKERS_USER1 && currentStep == STEP2)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USER1 && currentStep == STEP3)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP0)
        {
            return MAIN_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP1)
        {
            return BUID_LAYER;
        }
        else if(guideID == STORE_BUILD&& currentStep == STEP2)
        {
            return BUID_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP3)
        {
            return MAIN_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP4)
        {
            return MAIN_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP5)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP6)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP7)
        {
            return MAIN_LAYER;
        }
        else if(guideID == STORE_BUILD && currentStep == STEP8)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP2)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP3)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP4)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE1 && currentStep == STEP5)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE2 && currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USE2 && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WORKERS_USE2 && currentStep == STEP2)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE3 && currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == WORKERS_USE3 && currentStep == STEP1)
        {
            return COLLECTION_LAYER;
        }
        else if(guideID == WORKERS_USE3 && currentStep == STEP2)
        {
            return  COLLECTION_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD&&currentStep == STEP0)
        {
            return BUID_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD && currentStep == STEP2)
        {
            return VENTURE_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD && currentStep == STEP3)
        {
            return VENTURE_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD && currentStep == STEP4)
        {
            return MAIN_LAYER;
        }
        else if(guideID == SOLDIERS_BUILD && currentStep == STEP5)
        {
            return BUID_LAYER;
        }
        else if(guideID == TRAINING && currentStep == STEP0 )
        {
            return BUID_LAYER;
        }
        else if(guideID == TRAINING && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == TRAINING && currentStep == STEP2)
        {
            return HERO_LAYER;
        }
        else if(guideID == TRAINING && currentStep == STEP3)
        {
            char comment[200];
            sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
            std::stringstream sql;
            sql << "UPDATE `" << TABLE_GUID << "` SET `cstep`='" << STEP2 <<"' WHERE `gid` = '" << TRAINING << "'" << comment;
            _db->query(sql.str());
            return HERO_LAYER;
        }
        else if(guideID == WAR && currentStep == STEP0)
        {
            return HERO_LAYER;
        }
        else if(guideID == WAR && currentStep == STEP1)
        {
            return MAIN_LAYER;
        }
        else if(guideID == WAR && currentStep == STEP2)
        {
            return VENTURE_LAYER;
        }
        else if(guideID == WAR && currentStep == STEP3)
        {
            return VENTURE_LAYER;
        }
        else if(guideID == WAR && currentStep == STEP4)
        {
            return VENTURE_LAYER;
        }
        else if(guideID == WAREHOUSE_USE2&&currentStep == STEP0)
        {
            return RESOUCE_LAYER;
        }
        else if(guideID == MOVE && currentStep == STEP0)
        {
            return MAP_LAYER;
        }
        else if(guideID == CITY && currentStep == STEP0)
        {
            return MAP_LAYER;
        }
        else if(guideID == FOOD && currentStep == STEP0)
        {
            return MAP_LAYER;
        }
        else if(guideID == EXPLORE && currentStep == STEP0)
        {
            return MAP_LAYER;
        }
        else if(guideID == ATTACK1 && currentStep == STEP0)
        {
            return -1;
        }
        else if(guideID == ATTACK2 && currentStep == STEP0)
        {
            return -1;
        }
        else if(guideID == AUTO_ATTACK && currentStep == STEP0)
        {
            return -1;
        }
        else if(guideID == FOOD_BACK && currentStep == STEP0)
        {
            return -1;
        }
        else if(guideID == DROP_GET && currentStep == STEP0)
        {
            return -1;
        }
        else if(guideID == DROP_GET && currentStep == STEP1)
        {
            return -1;
        }
    }
    
    return -1;
}
    
int Guid::getStepByGuidId(int GuidID)
{
    std::stringstream sql;
    sql << "SELECT `cstep` FROM `" << TABLE_GUID.c_str() << "` WHERE `gid`=" << GuidID << " LIMIT 1";
    int date = _db->getOne(sql.str()).asInt();
    return date;
}
void Guid::setStepByGuidId(int GuidID,int nStep)
{
    char comment[200];
    sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
    std::stringstream sql;
    sql << "UPDATE `" << TABLE_GUID.c_str() << "` SET `cstep`='" << nStep <<"' WHERE `gid` = '" << GuidID << "'" << comment;
    _db->query(sql.str());
}
cocos2d::ValueMap Guid::getInfoByGuidId(int GuidID)
{
    std::stringstream sql;
    sql << "SELECT `cstep` FROM `" << TABLE_GUID.c_str() << "` WHERE `gid`=" << GuidID << " LIMIT 1";
    int date = _db->getOne(sql.str()).asInt();
    cocos2d::ValueMap info;
    info["guidId"] =GuidID;
    info["step"]   =date;
    return info;
}
int Guid::getMaxStepByGuidId(int GuidID)
{
    std::stringstream sql;
    sql << "SELECT `tstep` FROM `" << TABLE_GUID.c_str() << "` WHERE `gid`=" << GuidID << " LIMIT 1";
    int date = _db->getOne(sql.str()).asInt();
    
    return date;
}
cocos2d::ValueMap Guid::getGuidInfo()
{
    std::stringstream sql;
    sql << "SELECT `cstep` FROM `" << TABLE_GUID.c_str() << "` WHERE `gid`=" << getCurrentGuidId() << " LIMIT 1";
    int date = _db->getOne(sql.str()).asInt();
    cocos2d::ValueMap info;
    info["guidId"] =getCurrentGuidId();
    info["step"]   =date;
    return info;
}
bool Guid::showGuid(int GuidId)
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        if(GuidId == FOOD_BACK)
        {
            if(getGuideTaskFinishState(AUTO_ATTACK))
            {
                User::getInstance()->setGuideID(GuidId);
            }
        }
        else if(GuidId == ATTACK1)
        {
            if(!getGuideTaskFinishState(ATTACK1))
            {
                User::getInstance()->setGuideID(GuidId);
            }
            else
            {
                if(!getGuideTaskFinishState(ATTACK2))
                {
                    User::getInstance()->setGuideID(ATTACK2);
                }
                else
                {
                    if(!getGuideTaskFinishState(AUTO_ATTACK))
                    {
                         User::getInstance()->setGuideID(AUTO_ATTACK);
                    }
                }
            }
        }
        else if(GuidId == SHELTER_GUID)
        {
            cocos2d::Value row = data::Crusade::getInstance()->fieldView();
            if(row.asValueMap()["fid"].asInt() == 110202)
            {
                if(!getGuideTaskFinishState(SHELTER_GUID)&& getGuideTaskFinishState(EXPLORE))
                {
                    User::getInstance()->setGuideID(SHELTER_GUID);
                }
            }
            
        }
        else
        {
            if(GuidId != 0)
            {
                User::getInstance()->setGuideID(GuidId);
            }
            else
            {
                if(getCurrentGuidId() ==  WAREHOUSE_USE2)
                {
                    //不需要重复显示仓库卖出引导..
                    return false;
                }
            }
        }
        int currentID = getCurrentGuidId();
        if(getStepByGuidId(currentID) < getMaxStepByGuidId(currentID))
        {
            if(currentID == SHELTER_GUID)
            {
                cocos2d::Value row = data::Crusade::getInstance()->fieldView();
                if(row.asValueMap()["fid"].asInt() == 110202)
                {
                    cocos2d::ValueMap info;
                    info["step"] = getStepByGuidId(currentID)+1;
                    info["guidId"] = currentID;
                    dispatch(UPDATE_GUID,&info);
                    return true;
                }
            }
            else
            {
                cocos2d::ValueMap info;
                info["step"] = getStepByGuidId(currentID)+1;
                info["guidId"] = currentID;
                dispatch(UPDATE_GUID,&info);
                return true;
            }
            
        }
        
    }
    return false;
}
void Guid::showBattleSelectGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        cocos2d::ValueMap info;
        info["step"] = 1;
        info["guidId"] = BATTLE_SELECT;
        dispatch(UPDATE_GUID,&info);
    }
}
void Guid::showHomeGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        cocos2d::ValueMap info;
        info["step"] = 1;
        info["guidId"] = HOME_GUID;
        dispatch(UPDATE_GUID,&info);
    }
}
void Guid::showBattleChoiceGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        cocos2d::ValueMap info;
        info["step"] = 1;
        info["guidId"] = BATTLE_CHOICE;
        dispatch(UPDATE_GUID,&info);
    }
}
bool Guid::isCanShowShelterGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        cocos2d::Value row = data::Crusade::getInstance()->fieldView();
        if(row.asValueMap()["fid"].asInt() == 110202)
        {
            if(!getGuideTaskFinishState(SHELTER_GUID)&& getGuideTaskFinishState(EXPLORE))
            {
                return true;
            }
        }
        return false;
    }
    return false;
}
bool Guid::isCanShowMapGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        if(!getGuideTaskFinishState(MOVE))
        {
            data::User::getInstance()->setGuideID(MOVE);
            return true;
        }
        else if(!getGuideTaskFinishState(CITY))
        {
            data::User::getInstance()->setGuideID(CITY);
            return true;
        }
        else if(!getGuideTaskFinishState(FOOD))
        {
            data::User::getInstance()->setGuideID(FOOD);
            return true;
        }
        else if(!getGuideTaskFinishState(EXPLORE))
        {
            data::User::getInstance()->setGuideID(EXPLORE);
            return true;
        }
        else if(!getGuideTaskFinishState(MOVE1))
        {
            data::User::getInstance()->setGuideID(MOVE1);
            return true;
        }
        else if(!getGuideTaskFinishState(MOVE2))
        {
            data::User::getInstance()->setGuideID(MOVE2);
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
bool Guid::isCanShowHomeGuid()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        int breadnum = data::Crusade::getInstance()->getPackageCurrent(data::Item::FOOD);
        if(isCanShowShelterGuid()||isCanShowMapGuid())
        {
            return false;
        }
        else if(breadnum <= 2 && _homeGuid == false)
        {
            return true;
        }
    }
    return false;
}
bool Guid::isCanMoveOtherDirection()
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        if(getGuideTaskFinishState(MOVE2))
        {
            return true;
        }
        return false;
    }
    return true;
}
bool Guid::getGuideTaskFinishState(int GuidID)
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        if(getStepByGuidId(GuidID) >= getMaxStepByGuidId(GuidID))
        {
            return true;
        }
        return false;
    }
    return true;
}
void Guid::clearAllGuid()
{
    User::getInstance()->setGuide(true);
    dispatch(DELETE_GUID, NULL);
}
void Guid::updateGuidInfo(int operationType)
{
    if(!isGuidFinish()&&DEBUG_SHOW_GUID)
    {
        int currentID = getCurrentGuidId();
        int nStep = getStepByGuidId(currentID);
        //战斗选择界面引导不需要进行记录，只要条件合适就引导..
        if(operationType == data::Guid::BATTLE_SELECT_SURE)
        {
            battleSelectSure();
        }
        else if(operationType == data::Guid::BATTLE_CHOICE_SURE)
        {
            battleChoiceSure();
        }
        else if(getStepByGuidId(currentID) < getMaxStepByGuidId(currentID))
        {
            if(currentID == WAREHOUSE_USE1 && nStep == STEP7)//非一次点击就完成的新手引导..
            {
                if (Item::getInstance()->getUserItemAmountByIID(Item::ItemID::COIN) < 10) {
                    
                    restartTask();
                }
                else
                {
                    nextTask(operationType);
                }
            }
            else if(currentID == SHELTER_BUILD && nStep == STEP2)
            {
                if (Item::getInstance()->getUserItemAmountByIID(Item::ItemID::COIN) < 5) {
                    
                    restartTask();
                }
                else
                {
                    nextTask(operationType);
                }
            }
            else if(currentID == WORKERS_USER1 && nStep == STEP2)
            {
                if (Building::getInstance()->getUserWorkerAmountByWID(Building::PLANT_WORKERID) < 5) {
                    
                    restartTask();
                }
                else
                {
                    nextTask(operationType);
                }
            }
            else if(currentID == STORE_BUILD && nStep == STEP3)
            {
                if (Item::getInstance()->getUserItemAmountByIID(Item::ItemID::COIN) < 5) {
                    
                    restartTask();
                }
                else
                {
                    nextTask(operationType);
                }
            }
            else if(currentID == WAR && nStep == STEP2)
            {
                if (Crusade::getInstance()->getPackageCurrent(Item::ItemID::FOOD) < Item::getInstance()->getUserItemAmountByIID(Item::ItemID::FOOD)&&Crusade::getInstance()->getPackageCurrent(Item::ItemID::FOOD)<15
                    &&Crusade::getInstance()->getPackageSpace()<15) {
                    
                    restartTask();
                }
                else
                {
                    nextTask(operationType);
                }
            }
            else  //一次点击就完成的新手引导..
            {
                nextTask(operationType);
                
            }
            
            
        }
        
    }
}
void  Guid::restartTask()
{
    //未完成继续当前任务..
    cocos2d::ValueMap info;
    info["step"] =  getStepByGuidId(getCurrentGuidId())+1;
    info["guidId"] = getCurrentGuidId();
    dispatch(UPDATE_GUID,&info);
}
void  Guid::nextTask(int operationType)
{
    int currentID = getCurrentGuidId();
    //未进行同一项操作..
    if (_preOperationType != operationType||_preOperationType == ATTACK1||currentID == MOVE1||currentID == MOVE2) {
        _preOperationType = operationType;
        
        char comment[200];
        sprintf(comment, "/* %s, %d */", __FILE__, __LINE__);
        std::stringstream sql;
        
        sql << "UPDATE `" << TABLE_GUID << "` SET `cstep`='" << getStepByGuidId(currentID) +1 <<"' WHERE `gid` = '" << currentID << "'" << comment;
        _db->query(sql.str());
        
        int nstep = getStepByGuidId(currentID);
        //是否完成了该引导任务..
        if(nstep < getMaxStepByGuidId(currentID))
        {
            //未完成继续..
            cocos2d::ValueMap info;
            info["step"] =  nstep+1;
            info["guidId"] = currentID;
            dispatch(UPDATE_GUID,&info);
        }
        else
        {
            cocos2d::ValueMap info;
            info["step"] =  nstep+1;
            info["guidId"] = currentID;
            dispatch(UPDATE_GUID,&info);
            
            //完成了完成度加1..
            _guidFinishNum++;
            if(_guidFinishNum >= _maxGuidNum)
            {
                User::getInstance()->setGuide(true);
            }
            //更新当前执行的新手任务引导..
			if (currentID < STORE_BUILD || (currentID >= WAR&& currentID < MOVE2))
            {
				User::getInstance()->setGuideID(currentID + 1);
				currentID = getCurrentGuidId();
				if (currentID == STORE_BUILD)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
				else if (currentID == MOVE)
                {
                    //这一步需要手动显示下一步引导..
                    //data::Guid::getInstance()->showGuid();
                }
				else if (currentID == CITY)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
				else if (currentID == FOOD)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
				else if (currentID == EXPLORE)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
                else if(currentID == MOVE1)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
                else if(currentID == MOVE2)
                {
                    //这一步需要手动显示下一步引导..
                    data::Guid::getInstance()->showGuid();
                }
            }
            else if(currentID == WAREHOUSE_USE2)
            {
                User::getInstance()->setGuideID(currentID + 1);
            }
        }
    }
    
    
}
void Guid::battleSelectSure()
{
    //未完成继续..
    cocos2d::ValueMap info;
    info["step"] =  2;
    info["guidId"] = BATTLE_SELECT;
    dispatch(UPDATE_GUID,&info);
}
void Guid::battleChoiceSure()
{
    //未完成继续..
    cocos2d::ValueMap info;
    info["step"] =  2;
    info["guidId"] = BATTLE_CHOICE;
    dispatch(UPDATE_GUID,&info);
}
bool Guid::isGuidFinish()
{
    return User::getInstance()->getGuide();
}
    
}
/****************************************************************************
    
 *	@desc	新手引导信息
 *	@date	2015-06-30
 *	@author	120103
     
 *	@file	data/User.h
 ******************************************************************************/

#ifndef __deer__Guid__
#define __deer__Guid__

#include "cocos2d.h"
#include "User.h"
#include "core/custom/Event.h"

namespace data {
    
    class Guid: public  custom::Event
    {
    public:
        enum GUID_TASK_ID
        {
            BATTLE_SELECT = 0,
            BATTLE_CHOICE = 1,
            HOME_GUID = 2,
            GOLD = 110101,          //教玩家炼金..
            WAREHOUSE_BUILD = 110102,     //教玩家建造仓库..
            COLLECTION = 110103,    //教玩家采集..
            WAREHOUSE_USE1 = 110104, //教玩家仓库内的操作..
            SHELTER_BUILD = 110105, //教玩家建造庇护所..
            WORKERS_USER1   =110106,    //教玩家分配工人1..
            STORE_BUILD     =110107,           //教玩家建造后勤部..
            WORKERS_USE1    =110108,    //教玩家分配工人2..
            WORKERS_USE2    =110109,    //教玩家分配工人3..
            WORKERS_USE3    =110110,    //教玩家分配工人4..
            SOLDIERS_BUILD  =110111,    //教玩家建造兵营..
            TRAINING        =110112,    //教玩家训练兵种..
            WAR             =110113,    //教玩家出征..
            MOVE            =110114,    //行动指引..
            CITY            =110115,    //主城指引..
            FOOD            =110116,    //面包介绍..
            EXPLORE         =110117,    //探索指引..
            MOVE1           =110118,    //移动指引1..
            MOVE2           =110119,      //移动指引2..
            ATTACK1         =110120,    //第一次手动攻击..
            ATTACK2         =110121,    //第二次手动攻击..
            AUTO_ATTACK     =110122,    //自动攻击..
            FOOD_BACK       =110123,    //吃太空食品回血..
            DROP_GET        =110124,    //战斗结束拾取资源..
            SHELTER_GUID    =110125,    //庇护所引导..
            WAREHOUSE_USE2  =110126    //教玩家仓库内的操作..
            
        };
        enum LAYER_TYPE
        {
            MAIN_LAYER = 0,
            BUID_LAYER = 1,
            COLLECTION_LAYER = 2,
            RESOUCE_LAYER = 3,
            HERO_LAYER  = 4,
            HERO_POP_LAYER = 5,
            VENTURE_LAYER = 6,
            MAP_LAYER = 7,
            BATTLE_LAYER = 8,
            DROP_LAYER = 9,
            BATTLE_SELECT_LAYER = 10,
            BATTLE_SELECT_LAYER2 = 11
        };
        enum LAYER_BUTTON_ID
        {
            BUTTON_LAST = 0,
            BUILD_BUTTON_BACK = 1,
            BUILD_BUTTON_SHELTER = 2,
            BUILD_BUTTON_WAREHOUSE = 111801,
            BUILD_BUTTON_FARM   =   111101,
            BUILD_BUTTON_STORE = 111601,
            BUILD_BUTTON_FOOD = 111201,
            BUILD_BUTTON_SOLGIER = 111701,
            COLLECTION_BUTTON_BACK = 3,
            COLLECTION_BUTTON_GATHER = 4,
            COLLECTION_BUTTON_PLANT_ROBOT_ADD = 110101,
            COLLECTION_BUTTON_PLANT_ROBOT_DECREASE = 1101010,
            COLLECTION_BUTTON_FOOD_ROBOT_ADD = 110102,
            COLLECTION_BUTTON_FOOD_ROBOT_DECREASE = 1101020,
            COLLECTION_BUTTON_FOOD_ROBOT_NAME = 11010200,
            VENTURE_BUTTON_VENTURE  =   5,
            VENTURE_BUTTON_BACK = 6,
            VENTURE_BUTTON_RANK = 7,
            VENTURE_BUTTON_ACHIEVEMENT = 8,
            VENTURE_BUTTON_TALENT = 9,
            VENTURE_BUTTON_TAIKONGFOOD = 110104,
            VENTURE_BUTTON_SOLGIER = 110001,
            RESOURCE_BUTTON_FIRST = 10,
            HERO_LAYER_BUTTON_BACK = 11,
            HERO_LAYER_BUTTON_GETHERO = 12,
            HERO_POP_LAYER_BUTTON_SURE = 13,
            HERO_POP_LAYER_BUTTON_QUIT = 14,
            HERO_LAYER_HERO_NAME = 15,
            DROP_LAYER_BACK = 16,
            DROP_LAYER_GETAAL = 17,
            BATTLE_SELECT_SURE = 18,
            BATTLE_SELECT_CANCLE = 19,
            BATTLE_CHOICE_SURE = 20,
            BATTLE_CHOICE_CANCLE = 21

        };
        enum OPEREATION_TYPE
        {
            MAIN_GOLD = 100001,
            MAIN_BUID = 100002,
            BUID_BUILD = 100003,
            BUID_BACK      = 100004,
            MAIN_COLLECTION = 100005,
            COLLECTION_GATHER = 100006,
            COLLECTION_BACK = 100007,
            MAIN_WAREHOUSE = 100008,
            WAREHOUSE_SOURCE = 100009,
            WAREHOUSE_EMPTY =   100010,
            COLLECTION_ADD =100011,
            COLLECTION_REDUCE = 100012,
            COLLECTION_NAME_CLICK = 100013,
            MAIN_VENTURE = 100014,
            VENTURE_VENTURE = 100015,
            VENTURE_BACK = 100016,
            MAIN_TRAINING = 100017,
            TRAINING_GET  = 100018,
            TRAINING_SURE = 100019,
            TRAINING_BACK = 100020,
            VENTURE_ADD =   100021,
            VENTURE_SOLDIGER_ADD = 100022,
            BATTLE_AUTO_BTN     =   100023,
            BATTLE_FOOD_BTN     =   100024,
            BATTLE_SELECT_SURE_BTN = 100025
        };
        enum GUID_STEP
        {
            STEP0   =   0,
            STEP1   =   1,
            STEP2   =   2,
            STEP3   =   3,
            STEP4   =   4,
            STEP5   =   5,
            STEP6   =   6,
            STEP7   =   7,
            STEP8   =   8,
            STEP9   =   9,
            STEP10   =   10,
            STEP11   =   11,
        };
        enum FINGER_TYPE
        {
            FINGER_CLICK    =   0,
            FINGER_SLIDING  =   1,
            FINGER_PRESS_ON =   2,
            FINGER_MOVE     =   3
        };
        enum TIPS_ID
        {
            TIPS_HOME = 130312,
            TIPS_CONTINUE = 130313,
            TIPS_BREAD  =   130314,
            TIPS_EXPLORE = 130315,
            TIPS_SHELTER = 130325
        };
        enum BATTLE_BUTTON_ID
        {
            BATTLE_SKILL_1 = 1,
            BATTLE_SKILL_2 = 2,
            BATTLE_SKILL_3 = 3,
            BATTLE_RECOVERY = 100001,
            BATTLE_AUTO_ATTACK = 100002
        };

        /**
         *	新手引导用户操作
         */
        static  const std::string GUID_USER_OPERATION;
        
        /**
         *  更新新手引导
         */
        static  const std::string UPDATE_GUID;
        /**
         *  删除新手引导
         */
        static const std::string  DELETE_GUID;
        
        Guid();
        /**
         *	析构函数，要清除有指针的内容
         */
        ~Guid();
        
        /**
         *  初始化
         */
        void init();
        
        /**
         *	获取实体
         */
        static Guid* getInstance();
        static void destroyInstance();
        
        /**
         *	数据表-新手引导
         */
        static const std::string TABLE_GUID;
        
   
        /**
         *  获取当前进行到第几个引导任务
         *
         *  @return 当前进行到的GuidID
         */
        int getCurrentGuidId();
        
        /**
         *  根据引导id获取该引导是否完成了
         *
         *  @param guideId 引导任务ID
         *
         *  @return bool 该引导任务是否完成
         */
        bool getGuideTaskFinishState(int GuidID);
        
        bool isCanShowShelterGuid();
        bool isCanShowMapGuid();
        bool isCanShowHomeGuid();
        
        bool isCanMoveOtherDirection();
        
        
        /**
         *  根据引导id获取完成到第几步
         *
         *  @param  GuidID 引导任务ID
         *
         *  @return 该任务完成到第几步
         */
        int getStepByGuidId(int GuidID);
        
        /**
         *  根据引导id设置完成到第几步
         *
         *  @param  GuidID 引导任务ID
         *  @param  nStep  引导到第几步
         */
        void setStepByGuidId(int GuidID,int nStep);
        
        /**
         *  根据引导id获取总共多少步
         *
         *  @param GuidID 引导任务ID
         *  
         *  @return 该任务总共多少步
         */
        int getMaxStepByGuidId(int GuidID);
        
        /**
         *  根据引导id获取完成到信息
         *
         *  @param  GuidID 引导任务ID
         *
         *  @return 该任务完成信息
         */
        cocos2d::ValueMap getInfoByGuidId(int GuidID);
        
        /**
         *  获取引导完成到信息
         *
         *  @return 该任务完成信息
         */
        cocos2d::ValueMap getGuidInfo();
        /**
         *  进行新手引导
         *
         *  @param GuidId 下一步引导任务ID 只对不是连续进行的任务有效
         */
        bool showGuid(int GuidId = 0);
        
        void showHomeGuid();
        
        void showBattleSelectGuid();
        
        void showBattleChoiceGuid();
        
        /**
         *  更新新手引导信息
         *  顺序进行的新手引导任务自动更新当前执行的引导任务ID
         *
         *  @param   operationType 为界面中自定义操作类型，目的是为了防止同一操作重复进行
         *
         */
        void updateGuidInfo(int operationType = 0);
        
        /**
         *  删除引导手指
         */
        void clearAllGuid();
        
        /**
         *  所有新手引导是否完成
         *
         *  @return 新手引导是否完成
         */
        bool    isGuidFinish();
        
        /**
         *  是否可以快速增加和减少工人
         *  
         *  @return bool 是否可以快速增加和减少工人
         */
        bool    isCanFastOperateWorkers();
        
        /**
         *  不是执行一次的新手任务
         */
        void    restartTask();
        
        /**
         *  执行下一步任务
         *
         */
        void    nextTask(int operationType);
        
        /**
         *  战斗选择引导..
         */
        void    battleSelectSure();
        void    battleChoiceSure();
        
        /**
         *  是否需要清理数据重新引导
         */
        bool isNeedClearData();
        
        /**
         *  战斗引导是否需要完成，未完成重置重新进行
         */
        bool FinishBattleGuide();
        
        bool isFinishBattleGuide();
        
        /**
         *  获取新手引导到哪个界面
         */
        int getGuideLayer();
        
        /**
         *  获取是否进行过新手引导
         */
        bool getHomeGuid(){
            return _homeGuid;
        }
        /**
         *  设置新手引导状态
         */
        void setHomeGuid(bool bret)
        {
            _homeGuid = bret;
        }
        
    private:
        /**
         *	数据库
         */
        DBSqlite* _db;
        
        /**
         *  临时存储默认表数据
         */
        cocos2d::ValueVector defaultValue;
        
        /**
         *  这次游戏引导完成次数
         */
        int _guidFinishNum;
        
        /**
         *  剩余引导数量
         */
        int _maxGuidNum;
        
        /**
         *  上一个任务id
         */
        int _preOperationType = -1;
        
        /**
         *  上一个操作id，用于判断是否可以重复点击
         */
        int _preTaskId = 0;
        
        /**
         *  是否进行过回城引导
         */
        bool _homeGuid = false;
    };
    
}
#endif /* defined(__deer__Guid__) */

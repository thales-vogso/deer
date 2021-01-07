#include "AppDelegate.h"
#include "loading/UMLogo.h"
#include "utils/DeerUtils.h"
#include "SimpleAudioEngine.h"
#include "GlobalMacros.h"
#include "notice/Notice.h"
#include "sound/SoundManager.h"
#include "data/Guid.h"
#include "data/DestroyData.h"
#include "data/Lang.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MobClickCpp.h"
#endif


USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Deer");
        director->setOpenGLView(glview);
		glview->setFrameSize(500, 720);
    }
    
    /* 屏幕自适应 */
    
	glview->setDesignResolutionSize(DeerUtils::deerResouseSize.width, DeerUtils::deerResouseSize.height, ResolutionPolicy::FIXED_WIDTH);
    
    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    this->preSound();
    
    /* 初始化资源搜索路径 */
    this->initResoucePath();
    
    Director::getInstance()->setProjection(cocos2d::Director::Projection::_2D);
    
    

    // create a scene. it's an autorelease object
    /*auto scene = cocos2d::Scene::create();
    auto layer = Gateway::create(false);
    scene->addChild(layer);*/
        
    // run
    director->runWithScene(UMLogo::createScene());
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    MOBCLICKCPP_START_WITH_APPKEY("56cae12de0f55a260300054e");
    umeng::MobClickCpp::updateOnlineConfig();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	MOBCLICKCPP_START_WITH_APPKEY("56cbcc20e0f55ac787002bcd");
	umeng::MobClickCpp::updateOnlineConfig();
#endif
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    umeng::MobClickCpp::applicationDidEnterBackground();
#endif
    
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    umeng::MobClickCpp::applicationWillEnterForeground();
#endif
    
    /* 后台程序运行资源获取 */
    CallFunc *calFun = CallFunc::create(std::bind(&AppDelegate::backgroundGain, this));
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.0f),calFun, NULL));
}

void AppDelegate::backgroundGain()
{
    auto crudata = data::Crusade::getInstance()->getUserCrusade();
    if (crudata.asValueMap()["depart"].asBool())
    {
		return;
    }
    auto ret = data::Building::getInstance()->gain().asValueMap();
    
    std::stringstream sstr;
    for (auto &i : ret)
    {
        if (i.second.asValueMap()["amount"].asInt() > 0 )
        {
            sstr << data::Lang::getInstance()->getWord(120104) << " : " << i.second.asValueMap()["name"].asString() << "x" << i.second.asValueMap()["amount"].asInt() << ","<< data::Lang::getInstance()->getWord(120105) <<": " << i.second.asValueMap()["total"].asInt() << "\n";
        }
    }
    if (sstr.str().size() > 0)
    {
        std::string str = data::Lang::getInstance()->getWord(120106) + ":\n" + sstr.str();
        Notice::getInstance()->postTips(str);
        Notice::getInstance()->postBoard(str);
    }
}

void AppDelegate::initResoucePath()
{
    auto FUinstance = FileUtils::getInstance();
    FUinstance->addSearchPath("jiemian");
    FUinstance->addSearchPath("map");
    FUinstance->addSearchPath("monster");
    FUinstance->addSearchPath("loading");
    FUinstance->addSearchPath("manor");
    FUinstance->addSearchPath("guid");
    FUinstance->addSearchPath("paticles");
    FUinstance->addSearchPath("sound");
    FUinstance->addSearchPath("piecemeal");
}

void AppDelegate::preSound()
{
    SoundManager::getInstance();
}

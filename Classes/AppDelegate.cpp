#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "BattleMapScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "DataLoader.h"
#include "EventHandler.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	
    // turn on display FPS
   // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 /60);//fps 30
	//set  window size to the game
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(648,432,kResolutionShowAll);
	CCDirector::sharedDirector()->setContentScaleFactor((CCEGLView::sharedOpenGLView()->getVisibleSize().height)/432);
	
	//init game data
	CharacterDataLoader::instance()->Init();
	SkillDataLoader::instance()->Init();
	EventHandler::instance()->init();
	AbilityDataLoader::instance()->Init();
    // create a scene. it's an autorelease object
	CCScene *pScene = StartNow::scene();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

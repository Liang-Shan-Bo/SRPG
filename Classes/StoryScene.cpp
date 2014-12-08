#include "StoryScene.h"
#include "BattleResources.h"
#include "SimpleAudioEngine.h"
#include "GameUI.h"
#include "BattleMapScene.h"
#include "EventHandler.h"
using namespace CocosDenshion;
#define STORYSCENE_RESOURCE_COUNT 2

CCScene* StoryScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		StoryScene  *layer = StoryScene ::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer,1,LAYER_STORY_LAYER_ID);
	} while (0);
	// return the scene
	return scene;
}
bool StoryScene::init()
{
	bool bInit = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		bInit = true;
	} while (0);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	m_nLoadCount = 0;
	/////////
	m_nID = EventHandler::instance()->GetSceneID();;
	/////////
	InitResource();


	return bInit;
}

void StoryScene ::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}
bool StoryScene::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
	return true;
}

void StoryScene::ccTouchesMoved( CCTouch *pTouch,CCEvent *pEvent )
{
	CCPoint touchedPoint = pTouch->getPreviousLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
}


void StoryScene::ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
}

void StoryScene::onEnter()
{
	CCLayer::onEnter();
	
	
}
void StoryScene::InitResource()
{
	char chStr[50] = {0};
	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	sprintf(chStr,"%s.png",DIALOG_FILE_FULLPATH);
	pCache->addImageAsync(chStr,this,callfuncO_selector(StoryScene::LoadDialogCallBack));
	sprintf(chStr,"%s%d.png",STORY_BACKGROUND_FILE_FULLPATH,m_nID);
	pCache->addImageAsync(chStr,this,callfuncO_selector(StoryScene::LoadBackgroundCallBack));
}
void StoryScene::onExit()
{
	CCLayer::onExit();

}

void StoryScene::LoadBackgroundCallBack( CCObject* pSender )
{
	m_nLoadCount++;
	if(m_nLoadCount==STORYSCENE_RESOURCE_COUNT)
	{
		InitStory();
	}
}

void StoryScene::LoadDialogCallBack(CCObject* pTexture)
{
	m_nLoadCount++;
	char chStr[50] = {0};
	sprintf(chStr,"%s.plist",DIALOG_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chStr,(CCTexture2D*)pTexture);
	if(m_nLoadCount==STORYSCENE_RESOURCE_COUNT)
	{
		InitStory();
	}
}

void StoryScene::InitStory()
{
	char chStr[50] = {0};
	sprintf(chStr,"%s%d.png",STORY_BACKGROUND_FILE_FULLPATH,m_nID);
	m_pBackGround = CCSprite::create(chStr);
	m_pBackGround->setAnchorPoint(ccp(0,0));
	this->addChild(m_pBackGround);
	EventHandler* pHandler = EventHandler::instance();
	pHandler->GetDefaultBGMName(m_strBGMName);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(m_strBGMName.c_str());
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_strBGMName.c_str(),true);
	pHandler->OnStoryStart();
}

void StoryScene::GoNext()
{
	EventHandler::instance()->FomatNextScene();
}


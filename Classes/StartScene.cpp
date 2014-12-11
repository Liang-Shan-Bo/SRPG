#include "StartScene.h"
#include "BattleMapScene.h"
#include "SimpleAudioEngine.h"
#include "StoryScene.h"
#include "EventHandler.h"

using namespace CocosDenshion;

void StartNow::onEnter()
{
	CCLayer::onEnter();
	RunLoading();
	m_nLoadCount = 0;
	InitStartSceneResoures();
}
//void StartNow::updateDialog(float time){
//	static char count = '0';
//	static string s ="";
//	if(count>'9'){
//		count = '0';
//		s = count;
//	}else{
//	 s = s+count;
//	}
//	count++;
//	m_pPassStartLabel->setString(s.c_str());
//}

bool StartNow ::init()
{
	bool bInit = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		bInit = true;
	} while (0);
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	char chPathStr[50] = {0};
	sprintf(chPathStr,"%s.mp3",MUSIC_TITLE_FILE_FULLPATH);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(chPathStr);
	

	
	//m_bGetStart = false;
	//m_pPassStartLabel = CCLabelTTF::create("", "Arial",24,::CCSizeMake(200,150), kCCTextAlignmentLeft);
	//schedule(schedule_selector(StartNow::updateDialog),0.5f,kCCRepeatForever,0);

	//m_pNewGameLabel = CCLabelTTF::create("NewGame", "Arial", 24);
	//m_pExitLabel = CCLabelTTF::create("Exit", "Arial", 24);
	//if(!m_pPassStartLabel||!m_pNewGameLabel||!m_pExitLabel)
	//{
	//	return false;
	//}
	//addChild(m_pPassStartLabel,255);
	//addChild(m_pNewGameLabel,0);
	//addChild(m_pExitLabel,0);
	//m_pPassStartLabel->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y/4));
	//m_pNewGameLabel->setPosition(ccp(VisibleRect::center().x,VisibleRect::center().y + m_pNewGameLabel->getTextureRect().getMaxY()));
	//m_pExitLabel->setPosition(ccp(VisibleRect::center().x,VisibleRect::center().y - m_pExitLabel->getScaleY()));
	//m_pNewGameLabel->setVisible(false);
	//m_pExitLabel->setVisible(false);

	return bInit;	
}
void StartNow ::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}
bool StartNow::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	CCArray* pLabels =m_pBatchNode->getChildren();
	touchedPoint = convertToNodeSpace(touchedPoint);
	CCSprite* pOnSelect = NULL;
	if(m_pStarGame->boundingBox().containsPoint(touchedPoint))
	{
		m_pStarGame->setVisible(true);
		pOnSelect = m_pStarGame;
	}
	else if (m_pQuitGame->boundingBox().containsPoint(touchedPoint))
	{
		m_pQuitGame->setVisible(true);
		pOnSelect =m_pQuitGame;
	}
	else if(m_pContinue->boundingBox().containsPoint(touchedPoint))
	{
		m_pContinue->setVisible(true);
		pOnSelect = m_pContinue;
	}
	if(pOnSelect!=NULL)
	{
        CCObject* pChild = NULL; 
		CCARRAY_FOREACH(pLabels,pChild)
		{
			CCSprite* pLabel = (CCSprite*)pChild;
			if(pOnSelect!=pLabel)
			{
					pLabel->setVisible(false);
			}
		}
	}
	return true;
}

void StartNow::ccTouchesMoved( CCTouch *pTouch,CCEvent *pEvent )
{
	CCPoint touchedPoint = pTouch->getPreviousLocation();
	CCArray* pLabels =m_pBatchNode->getChildren();
	touchedPoint = convertToNodeSpace(touchedPoint);
	CCSprite* pOnSelect = NULL;
	if(m_pStarGame->boundingBox().containsPoint(touchedPoint))
	{
		m_pStarGame->setVisible(true);
		pOnSelect = m_pStarGame;
	}
	else if (m_pQuitGame->boundingBox().containsPoint(touchedPoint))
	{
		m_pQuitGame->setVisible(true);
		pOnSelect =m_pQuitGame;
	}
	else if(m_pContinue->boundingBox().containsPoint(touchedPoint))
	{
		m_pContinue->setVisible(true);
		pOnSelect = m_pContinue;
	}

	if(pOnSelect!=NULL)
	{
		CCObject* pChild = NULL; 
		CCARRAY_FOREACH(pLabels,pChild)
		{
			CCSprite* pLabel = (CCSprite*)pChild;
			if(pOnSelect!=pLabel)
			{
				pLabel->setVisible(false);
			}
		}
	}
}




void StartNow ::ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
	EventHandler* pHandler = EventHandler::instance();
	if(m_pStarGame->boundingBox().containsPoint(touchedPoint)&&m_pStarGame->isVisible())
	{
		pHandler->FomatNextScene();
	}
	else if(m_pLoadGame->boundingBox().containsPoint(touchedPoint))
	{
		char chStr[50];
		sprintf(chStr,"%s%d.tmx",STAGE_FILE_FULLPATH,1);
		CCTMXTiledMap* pMap = CCTMXTiledMap::create(chStr);
		addChild(pMap,MAX_LAYER_ZORDER, BATTLEMAP_TMX_TAG);
		pMap->setPosition(ccp(0,0));
	}
	else if (m_pQuitGame->boundingBox().containsPoint(touchedPoint)&&m_pQuitGame->isVisible())
	{
		CCDirector::sharedDirector()->end();
	}
	else if(m_pContinue->boundingBox().containsPoint(touchedPoint)&&m_pContinue->isVisible())
	{
		CCDirector::sharedDirector()->replaceScene(BattleMap::LoadBattleMapScene());	
	}
	CCArray* pLabels =m_pBatchNode->getChildren();
	CCObject* pChild = NULL;
	CCARRAY_FOREACH(pLabels,pChild)
	{
		CCSprite* pLabel = (CCSprite*)pChild;
		pLabel->setVisible(false);
	}
	 
}   


CCScene* StartNow::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
       StartNow  *layer = StartNow ::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer,1);
    } while (0);
    // return the scene
    return scene;
}

void StartNow::LoadingCallBack(CCObject* pSender)
{
	m_nLoadCount ++;
	if(!(m_nLoadCount<MAX_TITLE_BACKGROUND_FRAME))
	{
		m_pLoading->stopAllActions();
		m_pLoading->removeFromParentAndCleanup(false);

		AnimationManager* pManager = AnimationManager::instance();
		CCAnimation* pAnimation = pManager->CreateTitleBackGroundAnimation();
		CCArray* pFrames = pAnimation->getFrames();
		CCSpriteFrame* pFrame = dynamic_cast<CCAnimationFrame*>(pFrames->lastObject())->getSpriteFrame();
		CCSprite* pBackGround = CCSprite::createWithSpriteFrame(pFrame);
		addChild(pBackGround,-1);
		pBackGround->setAnchorPoint(ccp(0,0));
		pBackGround->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));

		char chPlistStr[50] = {0};
		char chImageStr[50] = {0};
		sprintf(chPlistStr,"%s.plist",TITLE_MENU_FILE_FULLPATH);
		sprintf(chImageStr,"%s.png",TITLE_MENU_FILE_FULLPATH);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr);
		m_pBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));

		sprintf(chImageStr,"%s.png",TTTLE_MENU_NEWGAME_LABEL_NAME);
		m_pStarGame = CCSprite::createWithSpriteFrameName(chImageStr);
	
		sprintf(chImageStr,"%s.png",TTTLE_MENU_LOADGAME_LABEL_NAME);
		m_pLoadGame = CCSprite::createWithSpriteFrameName(chImageStr);
		sprintf(chImageStr,"%s.png",TTTLE_MENU_CONTINUE_LABEL_NAME);
		m_pContinue = CCSprite::createWithSpriteFrameName(chImageStr);
		sprintf(chImageStr,"%s.png",TTTLE_MENU_QUITGAME_LABEL_NAME);
		m_pQuitGame = CCSprite::createWithSpriteFrameName(chImageStr);

		m_pBatchNode->addChild(m_pStarGame);
		m_pBatchNode->addChild(m_pContinue);
		m_pBatchNode->addChild(m_pLoadGame);
		m_pBatchNode->addChild(m_pQuitGame);
		;
		CCArray* pLabels =m_pBatchNode->getChildren();
		CCObject* pChild = NULL;
		CCARRAY_FOREACH(pLabels,pChild)
		{
			CCSprite* pLabel = (CCSprite*)pChild;
			pLabel->setVisible(false);
		}
	
		addChild(m_pBatchNode);
		m_pQuitGame->setPosition(ccp(320,42));
		m_pContinue->setPosition(ccp(320,70));
		m_pLoadGame->setPosition(ccp(320,98));
		m_pStarGame->setPosition(ccp(320,126));
		setTouchEnabled(true);
		//播放背景音乐
		char chPathStr[50] = {0};
		sprintf(chPathStr,"%s.mp3",MUSIC_TITLE_FILE_FULLPATH);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(chPathStr,true);
	}
}


void StartNow::onExit()
{
	CCLayer::onExit();
}

void StartNow::RunLoading()
{
	AnimationManager* pManager = AnimationManager::instance();
	CCAnimation* pAnimation = pManager->CreateSimpleLoadingAnimation();
	CCSpriteFrame* pFrame = dynamic_cast<CCAnimationFrame*>(pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame();
	m_pLoading = CCSprite::createWithSpriteFrame(pFrame);
	addChild(m_pLoading,MAX_LAYER_ZORDER);
	m_pLoading->setPosition(ccp(50,50));
	m_pLoading->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));
}
void StartNow::InitStartSceneResoures()
{
	char chPathStr[50] = {0};
	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	for(int i = 1;i<=MAX_TITLE_BACKGROUND_FRAME;i++)
	{
		sprintf(chPathStr,"%s%d.png",TITLE_BACKGROUND_FILE_FULLPATH,i);
		pCache->addImageAsync(chPathStr,this,callfuncO_selector(StartNow::LoadingCallBack));
	}	
}


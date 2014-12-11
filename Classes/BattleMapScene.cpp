#include "BattleMapScene.h"
#include "GameUI.h"
#include "AnimationManager.h"
#include "tinyxml2.h"
#include "EventHandler.h"
#include  "PlayerDataManager.h"
#define MAP_HEIGHT (12)
#define MAP_WIDTH  (15)

#define  MIN_MOVE_SIZE 3
#define  MAX_SAVE_DATA_SIZE 40000


using namespace CocosDenshion;
using namespace tinyxml2;


BattleMap::BattleMap()
{
	m_nID = EventHandler::instance()->GetSceneID();
	m_pTMXTiledMap = NULL;
	m_pShowRangeObjects =CCArray::createWithCapacity(5);
	m_pShowRangeObjects->retain();
	m_pUnits = CCArray::createWithCapacity(10);
	m_pUnits->retain();
	schedule(schedule_selector(BattleMap::update));
	//BattleMenu::instance()->SetParent(this);
	m_nTurns = 1;
	m_nForceTurnFlag = FORCE_PLAYER;
	m_bIsToucheMoved = false;
	m_AIModeIndex  = 0;
	m_nLoadedImages = 0;
	m_nLoadedFrames = 0;
	m_pNumsBatchNode =NULL;
	m_bLoadInit = false;

}

BattleMap::~BattleMap()
{
	m_pShowRangeObjects->release();
	m_pUnits->release();
	m_vEventMarks.clear();
	//m_pEventMarks->release();
	//m_pMap->release();
	//m_pEnermyUnits->release();
	//m_pPlayerUnits->release();
	//m_pNPCUnits->release();
}

void BattleMap::update(float dt)
{

}

CCDictionary* BattleMap::SearchTMXObject(const char* pStr)
{
	CCTMXObjectGroup* group = m_pTMXTiledMap->objectGroupNamed("object"); 
	return group->objectNamed(pStr);
}


CCArray* BattleMap::SearchArrayFromTMXLayer(const char* pStr)
{
	CCTMXObjectGroup* group = m_pTMXTiledMap->objectGroupNamed(pStr); 
	return group->getObjects();//获取当前层上对象集合 


}

CCScene* BattleMap::InitBattleMapScene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		BattleMap *layer = BattleMap::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer,1,LAYER_BATTLEMAP_LAYER_ID);
	} while (0);

	return scene;
}

CCScene* BattleMap::LoadBattleMapScene()
{
	CCScene* scene  =NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		BattleMap *pLayer = new BattleMap();
		CC_BREAK_IF(! pLayer);
		pLayer->m_bLoadInit = true;
		CC_BREAK_IF(!pLayer->init());
		scene->addChild(pLayer,1,LAYER_BATTLEMAP_LAYER_ID);
	} while (0);

	return scene;
}
void BattleMap::draw()
{
	ccGLEnable(CC_GL_ALL);
	//DrawSquareLines();
	DrawRanges();
	//ccGLEnable(CC_GL_BLEND);
	//DrawEventMarks();
}

void BattleMap::ViewLock(BattleCharacter *pChar)
{
	if(pChar!=NULL)
	{
		int viewX = pChar->GetMapPositionX();
		int viewY = pChar->GetMapPositionY();
		float winSizeX = CCDirector::sharedDirector()->getWinSize().width;
		float winSizeY = CCDirector::sharedDirector()->getWinSize().height;
		float lockX = winSizeX/2 - viewX;
		float lockY = winSizeY/2 - viewY;

		/*	float minX =  winSizeX- m_pTMXTiledMap->getContentSize().width;
		float minY =  winSizeY- m_pTMXTiledMap->getContentSize().height;*/
		lockX = lockX>m_fMaxPositionX?m_fMaxPositionX:lockX;
		lockX = lockX<m_fMinPositionX?m_fMinPositionX:lockX;
		lockY = lockY>m_fMaxPositionY?m_fMaxPositionY:lockY;
		lockY = lockY<m_fMinPositionY?m_fMinPositionY:lockY;

		setPosition(ccp(lockX,lockY));
	}
}
void BattleMap::ViewLockUpdate(float time)
{
	ViewLock(m_pViewLocker);
}
void BattleMap::LockCharacterView(BattleCharacter* pChar)
{
	m_pViewLocker = pChar;
	setTouchEnabled(false);
	schedule(schedule_selector(BattleMap::ViewLockUpdate));
}

void BattleMap::UnLockCharacterView()
{
	m_pViewLocker = NULL;
	unschedule(schedule_selector(BattleMap::ViewLockUpdate));
	setTouchEnabled(true);
}

void BattleMap::DrawSquareLines()
{
	float offsetX = m_pTMXTiledMap->getPositionX();
	float offsetY = m_pTMXTiledMap->getPositionY();
	CHECK_GL_ERROR_DEBUG();
	glLineWidth(1.0f );
	ccDrawColor4B(255,255,25,125);
	for(int i = 0;i<MAP_WIDTH+1;i++)
	{
		ccDrawLine(ccp(offsetX+i*SQUARE_LENTH,offsetY),ccp(offsetX+i*SQUARE_LENTH,offsetY+MAP_HEIGHT*SQUARE_LENTH));
	}
	for(int i = 0;i<MAP_HEIGHT+1;i++)
	{
		ccDrawLine(ccp(offsetX,offsetY+i*SQUARE_LENTH),ccp(offsetX+MAP_WIDTH*SQUARE_LENTH,offsetY+i*SQUARE_LENTH));
	}

	glLineWidth(1);
	ccDrawColor4B(255,255,255,255);
	ccPointSize(1);

	CHECK_GL_ERROR_DEBUG();

}



void BattleMap ::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);  
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

bool BattleMap::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent)
{
	m_bIsToucheMoved =false;
	return true;
}

void BattleMap::ccTouchMoved(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint diff = pTouch->getDelta();
	if(abs((int)diff.x) > MIN_MOVE_SIZE||abs((int)diff.y)>MIN_MOVE_SIZE)
	{
		m_bIsToucheMoved = true;
	}
	else
	{
		return;
	}
	CCPoint currentPos = getPosition();
	/*float winSizeX = CCDirector::sharedDirector()->getWinSize().width;
	float winSizeY = CCDirector::sharedDirector()->getWinSize().height;*/
	//限制拖拽范围
	CCPoint command = ccpAdd(currentPos,diff);
	//限制向左拖拽
	/*float minX =  winSizeX- m_pTMXTiledMap->getContentSize().width;*/
	command.x = command.x<m_fMinPositionX?m_fMinPositionX:command.x;
	//限制向右拖拽
	command.x = command.x>m_fMaxPositionX?m_fMaxPositionX:command.x;
	//限制向下拖拽
	//float minY =  winSizeY - m_pTMXTiledMap->getContentSize().height;
	command.y = command.y<m_fMinPositionY?m_fMinPositionY:command.y;
	//限制向上拖拽
	command.y = command.y>m_fMaxPositionY?m_fMaxPositionY:command.y;
	setPosition(command);
	ShowRangeMenu* pShowRange = (ShowRangeMenu*)getParent()->getChildByTag(LAYER_SHOWRANGE_MENU_ID);
	if(pShowRange!=NULL)
	{
		pShowRange->Refresh();
	}
}

void BattleMap ::ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent)
{
	if(!m_bIsToucheMoved)
	{
		CCPoint touchedPoint = pTouch->getPreviousLocationInView();
		touchedPoint = CCDirector::sharedDirector()->convertToGL(touchedPoint);
		touchedPoint = convertToNodeSpace(touchedPoint);//将触屏点转换为自身
		CCSprite* pSubRange =(CCSprite*)getChildByTag(LAYER_SUBRANGE_ID);
		if(pSubRange!=NULL)
		{
			/*int x = (int)touchedPoint.x/SQUARE_LENTH;
			int y = (int)touchedPoint.y/SQUARE_LENTH;
			pSubRange->setPositionX((x+0.5f)*SQUARE_LENTH);
			pSubRange->setPositionY((y+0.5f)*SQUARE_LENTH)*/;
		return;

		}
		//对玩家的单位进行操作
		for(unsigned int index = 0;index<m_pUnits->count();index++)
		{
			BattleCharacter* pChar = (BattleCharacter*)m_pUnits->objectAtIndex(index);
			if(pChar->PerformFromTouch(touchedPoint))
			{
				return;
			}
		}
		if(BattleMenu::instance()->Usable())
		{
			BattleMenu::instance()->CheckTouchedCommand(touchedPoint);
			return;
		}
		else if(0==m_pShowRangeObjects->count())
		{
			getParent()->addChild(new MainMenu(),MAX_LAYER_ZORDER,LAYER_MAINMENU_LAYER_ID);
		}
		//for(unsigned int index = 0;index<m_pPlayerUnits->count();index++)
		//{
		//	BattleCharacter* pPlayerUnit =(BattleCharacter*) m_pPlayerUnits->objectAtIndex(index);
		//	if(pPlayerUnit->m_pMapSprite->boundingBox().containsPoint(touchedPoint))
		//	{	
		//		 pPlayerUnit->OnOperate();
		//		 return;
		//	}
		//}
	}

}

bool BattleMap::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	} while (0);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	RunLoading();
	InitBattleSceneFrameResoures();
	return bRet;
}
void BattleMap::InitGame()
{
	//设置角色菜单
	CharacterStatus::m_pMap = this;
	BattleCharacter::m_pMap = this;
	BattleMenu* pMenu = BattleMenu::instance();
	pMenu->SetParent(this);
	//预加载背景音乐
	char chStr[50];
	EventHandler::instance()->GetPlayerTurnBGMName(m_strPlayerTurnMusic);
	EventHandler::instance()->GetEnermyTurnBGMName(m_strEnermyTurnMusic);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(m_strPlayerTurnMusic.c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(m_strPlayerTurnMusic.c_str());

	//加载tmx地图	
	sprintf(chStr,"%s%d.tmx",STAGE_FILE_FULLPATH,m_nID);
	m_pTMXTiledMap = CCTMXTiledMap::create(chStr);
	addChild(m_pTMXTiledMap,-MAX_LAYER_ZORDER, BATTLEMAP_TMX_TAG);
	m_pTMXTiledMap->setPosition(ccp(0,0));
	//初始化地图拖拽范围
	m_fMaxPositionX = 0;
	m_fMaxPositionY = 0;
	float winSizeX = CCDirector::sharedDirector()->getWinSize().width;
	float winSizeY = CCDirector::sharedDirector()->getWinSize().height;
	m_fMinPositionX =  winSizeX- m_pTMXTiledMap->getContentSize().width;
	m_fMinPositionY =  winSizeY- m_pTMXTiledMap->getContentSize().height;
	//初始化方格动画
	sprintf(chStr,"%s.png",SQUARE_ICON_FILE_FULLPATH);
	m_pSquareBatchNode = CCSpriteBatchNode::create(chStr);
	addChild(m_pSquareBatchNode,-1);
	//初始化数字和角色
	sprintf(chStr,"%s.png",NUMS_FILE_FULLPATH);
	m_pNumsBatchNode = CCSpriteBatchNode::create(chStr);
	addChild(m_pNumsBatchNode,MAX_LAYER_ZORDER,LAYER_MAPNUMS_ID);
	////通过tmx地图数据初始化角色
	//CCArray* characterDices = CCArray::createWithArray(SearchArrayFromTMXLayer("character"));
	//for(unsigned int index = 0;index<characterDices->count();index++)
	//{
	//	CCDictionary* dic =  (CCDictionary*)characterDices->objectAtIndex(index);
	//	if (dic->objectForKey("Turn")==NULL||dic->objectForKey("Turn")==1)
	//	{
	//		m_pUnits->addObject(new BattleCharacter());
	//		((BattleCharacter*)m_pUnits->lastObject())->BuildCharacter(this,((CCString*)dic->objectForKey("ID"))->intValue(),((CCString*)dic->objectForKey("x"))->intValue()+SQUARE_LENTH/2,((CCString*)dic->objectForKey("y"))->intValue()+SQUARE_LENTH/2,((CCString*)dic->objectForKey("FORCE"))->intValue());
	//	}	
	//}

	TurnBegin(this,NULL);
	////播放背景音乐
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_strPlayerTurnMusic.c_str(),true);
	//设置战场地图的下雪天气，默认为第一个场景下雪
	if(EventHandler::instance()->GetSceneID()==1)
	{
		CCNode * pRain = CCParticleRain::createWithTotalParticles(123);
		
		this->getParent()->addChild(pRain,200);
	}
	//pRain->setPositionX(m_pTMXTiledMap->getPositionX());
	//pRain->setPositionY(m_pTMXTiledMap->getPositionY());



	setTouchEnabled(true);
}

void BattleMap::InitBattleSceneImageResoures()
{
	char chPathStr[50] = {0};
	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	SEL_CallFuncO pCallFunc =callfuncO_selector(BattleMap::LoadImageCallBack);
	//初始化血条
	sprintf(chPathStr,"%s.png",HPOFF_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	sprintf(chPathStr,"%s.png",HPBAR_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化决斗地图背景
	sprintf(chPathStr,"%s%d.png",DUEL_BACKGROUND_FILE_FULLPATH,m_nID);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化决斗场景UI
	sprintf(chPathStr,"%s.png",DUEL_LAYERUI_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//TOAL 4
}

void BattleMap::InitBattleSceneFrameResoures()
{
	char chPathStr[50] = {0};
	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	SEL_CallFuncO pCallFunc = NULL;
	//初始化战斗地图主菜单
	sprintf(chPathStr,"%s.png",MAINMENU_MENU_FILE_FULLPATH);
	pCallFunc = callfuncO_selector(BattleMap::LoadMainMenuCALLBACK);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化范围菜单
	pCallFunc = callfuncO_selector(BattleMap::LoadRangeMenuCALLBACK);
	sprintf(chPathStr,"%s.png",RANGEMENU_LAYER_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化角色菜单
	pCallFunc = callfuncO_selector(BattleMap::LoadBattleMenuCALLBACK);
	sprintf(chPathStr,"%s.png",BATTLEMENU_MENU_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化切换回合动画
	pCallFunc = callfuncO_selector(BattleMap::LoadTurnPassCALLBACK);
	sprintf(chPathStr,"%s.png",TURNPASS_LAYER_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化数字动画 
	pCallFunc = callfuncO_selector(BattleMap::LoadNumsCALLBACK); 
	sprintf(chPathStr,"%s.png",NUMS_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化角色对话
	pCallFunc = callfuncO_selector(BattleMap::LoadDialogMenuCALLBACK); 
	sprintf(chPathStr,"%s.png",DIALOG_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化角色动画
	//id=1
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar1CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,1);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar1CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,1);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=2
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar2CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,2);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar2CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,2);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=3
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar3CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,3);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar3CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,3);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=4
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar4CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,4);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar4CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,4);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=5
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar5CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,5);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar5CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,5);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=6
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar6CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,6);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar6CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,6);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=7
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar7CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,7);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar7CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,7);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=8
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar8CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,8);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar8CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,8);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=9
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar9CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,9);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar9CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,9);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=10
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar10CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,10);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar10CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,10);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=11
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar11CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,11);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar11CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,11);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=12
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar12CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,12);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar12CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,12);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=13
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar13CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,13);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar13CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,13);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=14
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar14CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,14);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar14CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,14);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=15
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar15CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,15);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar15CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,15);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=16
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar16CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,16);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar16CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,16);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=17
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar17CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,17);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar17CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,17);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//id=18
	//地图场景精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadMapchar18CALLBACK); 
	sprintf(chPathStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,18);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//决斗场景角精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelchar18CALLBACK); 
	sprintf(chPathStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,18);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	////初始化地图技能动画
	////id = 1
	//pCallFunc = callfuncO_selector(BattleMap::LoadMapSkill1CALLBACK); 
	//sprintf(chPathStr,"%s%d.png",MAP_SKILL_FILE_FULLPATH,1);
	//pCache->addImageAsync(chPathStr,this,pCallFunc);
	////id = 2
	//pCallFunc = callfuncO_selector(BattleMap::LoadMapSkill2CALLBACK); 
	//sprintf(chPathStr,"%s%d.png",MAP_SKILL_FILE_FULLPATH,2);
	//pCache->addImageAsync(chPathStr,this,pCallFunc);
	////id = 3
	//pCallFunc = callfuncO_selector(BattleMap::LoadMapSkill3CALLBACK); 
	//sprintf(chPathStr,"%s%d.png",MAP_SKILL_FILE_FULLPATH,3);
	//pCache->addImageAsync(chPathStr,this,pCallFunc);
	////id = 4
	//pCallFunc = callfuncO_selector(BattleMap::LoadMapSkill4CALLBACK); 
	//sprintf(chPathStr,"%s%d.png",MAP_SKILL_FILE_FULLPATH,4);
	//pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化地图精灵消除动画
	pCallFunc = callfuncO_selector(BattleMap::LoadMapCharacterVanishCALLBACK); 
	sprintf(chPathStr,"%s.png",MAP_CHARACTER_VANISH_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);
	//初始化飞行道具精灵
	pCallFunc = callfuncO_selector(BattleMap::LoadDuelWaveCALLBACK);
	sprintf(chPathStr,"%s.png",DUEL_WAVE_FILE_FULLPATH);
	pCache->addImageAsync(chPathStr,this,pCallFunc);

}

void BattleMap::LoadImageCallBack(CCObject* pTexture)
{
	m_nLoadedImages++;
	if(m_nLoadedImages==(MAX_BATTLEMAP_LOADIMAGES_COUNT))
	{
		m_pLoading->stopAllActions();
		m_pLoading->removeFromParentAndCleanup(true);
		if(m_bLoadInit)
		{
			LoadBattle();
		}
		else
		{
			InitGame();
		}
	}
}


void BattleMap::RunLoading()
{
	AnimationManager* pManager = AnimationManager::instance();
	CCAnimation* pAnimation = pManager->CreateSimpleLoadingAnimation();
	CCSpriteFrame* pFrame = dynamic_cast<CCAnimationFrame*>(pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame();
	m_pLoading = CCSprite::createWithSpriteFrame(pFrame);
	addChild(m_pLoading,MAX_LAYER_ZORDER);
	m_pLoading->setPosition(ccp(50,50));
	m_pLoading->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));
}
//void BattleMap::InvokeEndTurnMenu()
//{
//	setTouchEnabled(false);
//	getParent()->addChild(new ConfirmEndTurnMenu,2,LAYER_ENDTURN_LAYER_ID);
//	BattleMenu::instance()->HangMenu();
//	
//}
//void BattleMap::PauseTurnLogoSEL(float time)
//{
//	/*::CCDirector::sharedDirector()->getActionManager()->pauseTarget( m_pTurnLogo);*/
//	CCMoveBy* move = CCMoveBy::create(1.0f,ccp(VisibleRect::right().x/2+GetSpriteTextureLength( m_pTurnLogo),0));
//	CCArray* pActions = CCArray::createWithCapacity(2);
//	pActions->addObject(move);
//	pActions->addObject(CCCallFuncND::create(this,callfuncND_selector(BattleMap::OnTurnLogoPassed),NULL));
//	m_pTurnLogo->runAction(CCSequence::create(pActions));
//
//}


void BattleMap::EndTurn(CCNode* pSender,void *data)
{
	setTouchEnabled(false);
	ResumeAllCharacters();
	m_nTurns++;
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	while(true)
	{
		m_nForceTurnFlag = (++m_nForceTurnFlag)%FORCE_COUNT;
		bool bFind = false;
		for(unsigned int index = 0;index<m_pUnits->count();index++)
		{
			BattleCharacter* pUnit = (BattleCharacter*)m_pUnits->objectAtIndex(index);
			if(pUnit->GetForceFlag() == m_nForceTurnFlag)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind)
		{
			/*	setTouchEnabled(true);
			return;*/
			continue;
		}
		if(m_nForceTurnFlag==FORCE_PLAYER)
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_strPlayerTurnMusic.c_str(),true);
		}
		else
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_strEnermyTurnMusic.c_str(),true);
		}
		getParent()->addChild( new TurnPassLayer(m_nForceTurnFlag),LAYER_TURNTASS_LAYER_ID,LAYER_TURNTASS_LAYER_ID);
		return;
	}
}
void BattleMap::TurnBegin(CCNode *sender,void* data)
{
	getParent()->removeChildByTag(LAYER_TURNTASS_LAYER_ID,true);
	EventHandler::instance()->OnBattleTurnBegin();
	if(m_nForceTurnFlag == FORCE_PLAYER)
	{
		setTouchEnabled(true);
		/*AIActionsChan();*/
	}
	//else
	//{
	//	setTouchEnabled(true);
	//	/*	CCArray* pLayers = getParent()->getChildren();
	//	for(unsigned index = 0;index<pLayers->count();index++)
	//	{
	//	CCLayer* pLayer = (CCLayer*)pLayers->objectAtIndex(index);
	//	pLayer->setTouchEnabled(true);
	//	}*/
	//}

}
void BattleMap::ResumeAllCharacters()
{
	/*CCArray* resumeArray;
	CHAR_ARRAY_SEL(m_nForceTurnFlag,resumeArray);*/
	for(unsigned int index = 0;index<m_pUnits->count();index++)
	{
		BattleCharacter* pUnit = (BattleCharacter*)m_pUnits->objectAtIndex(index);
		pUnit->Resume();
	}
}



void BattleMap::AIActionsChan()
{
	//根据战斗系统可以将容器排序，暂记于此
	//sort();
	if(EventHandler::instance()->IsEventRunning())
	{
		return ;	
	}
	if(m_pUnits->count()!= 0&&m_nForceTurnFlag!=FORCE_PLAYER)
	{
		for(;m_AIModeIndex <m_pUnits->count();m_AIModeIndex ++)
		{
			BattleCharacter* nextOne =(BattleCharacter*)m_pUnits->objectAtIndex(m_AIModeIndex);
			if(m_nForceTurnFlag == nextOne->GetForceFlag())
			{
				//本回合行动的AI
				m_AIModeIndex ++;
				nextOne->Action();
				return;
			}
		}
		if(!(m_AIModeIndex <m_pUnits->count()))
		{
			m_AIModeIndex  = 0;
			EndTurn(this,NULL);
		}
	}
}
void BattleMap::AcceptShowRangeObject(BattleCharacter *pObject)
{
	if(!m_pShowRangeObjects->containsObject(pObject))
	{
		m_pShowRangeObjects->addObject(pObject);
	}
}
void BattleMap::KickShowRangeObject(BattleCharacter* pObject)
{
	if(m_pShowRangeObjects->containsObject(pObject))
	{
		m_pShowRangeObjects->removeObject(pObject);
	}
}
int BattleMap::GetTurnFlag()
{
	return m_nForceTurnFlag;
}
CCArray* BattleMap::GetUnits()
{
	return m_pUnits;
}


void  BattleMap:: SetCostFromMap(BattleCharacter *pCharacter)
{
	CCArray* pAllMoves = pCharacter->GetRangeArray();

	//CCArray* pMoveableArray = CCArray::createWithCapacity(pObject->GetMoveRangeArray()->count());
	int move = pCharacter->GetMove();
	//将瓦片地图坐标转换为OPENGL坐标进行比较,地图边界检测
	for(unsigned int index = 0;index<pAllMoves->count();index++)
	{
		PathPoint* pPointOnCheck = (PathPoint*)pAllMoves->objectAtIndex(index);
		pPointOnCheck->m_nCost = 0;
		int pathX = pPointOnCheck->m_nX;
		int pathY = pPointOnCheck->m_nY;
		int objectX = pCharacter->GetMapPositionX() + pathX*SQUARE_LENTH;
		int objectY = pCharacter->GetMapPositionY() + pathY*SQUARE_LENTH;
		CCPoint mapPoint = ccp(objectX,objectY);
		//子节点的位置是依赖于父节点,box碰撞检测是相对于OPENGL(世界坐标系)而论.所以应将子节点的坐标在父节点的空间转换函数中转换一下.
		//mapPoint = convertToWorldSpace(mapPoint);
		if(!m_pTMXTiledMap->boundingBox().containsPoint(mapPoint))
		{
			pPointOnCheck->m_nCost = 255;
		}
	}
	//障碍物检测
	CCArray* pAffects = SearchArrayFromTMXLayer("affect");
	for(unsigned int j = 0;j<pAffects->count();j++)
	{
		//找到移动范围内的障碍物
		int affectX = ((CCString*)((CCDictionary*)pAffects->objectAtIndex(j))->objectForKey("x"))->intValue()+SQUARE_LENTH/2;
		int affectY = ((CCString*)((CCDictionary*)pAffects->objectAtIndex(j))->objectForKey("y"))->intValue()+SQUARE_LENTH/2;
		int effection =((CCString*)((CCDictionary*)pAffects->objectAtIndex(j))->objectForKey("offset"))->intValue();					
		ConvertPositionToRange(pCharacter->GetMapPositionX(),pCharacter->GetMapPositionY(),&affectX,&affectY);
		int offsetIndex = ::DrhombusRangeObject::GetRangeOffsetsIndex(affectX,affectY,pCharacter->GetMove());
		if((!pCharacter->IsFlyable())&&offsetIndex!=-1)
		{
			PathPoint* pPointOnCheck = (PathPoint*)pAllMoves->objectAtIndex(offsetIndex);
			pPointOnCheck->m_nCost = effection;
		}
	}
	//人物屏障检测
	for(unsigned int index = 0;index<m_pUnits->count();index++)
	{
		BattleCharacter* pUnit = (BattleCharacter*)m_pUnits->objectAtIndex(index);
		if(pUnit == pCharacter)
		{
			continue;
		}
		int rangeX =pUnit->GetMapPositionX();
		int rangeY =pUnit->GetMapPositionY();
		ConvertPositionToRange(pCharacter->GetMapPositionX(),pCharacter->GetMapPositionY(),&rangeX,&rangeY);
		int rangeIndex = DrhombusRangeObject::GetRangeOffsetsIndex(rangeX,rangeY,pCharacter->GetMove()); 
		if(rangeIndex==-1)
		{
			continue;
		}
		PathPoint* pPoint = (PathPoint*)pAllMoves->objectAtIndex(rangeIndex);
		pPoint->m_nCost = 255;
		//检测ZOC
		if(pUnit->IsZOC(pCharacter))
		{
			for(int i= 0;i<4;i++)
			{	
				PathPoint* twardPoint = pPoint->twardPoints[i];
				if(twardPoint==NULL)
				{
					//越界
					continue;
				}
				if(twardPoint->m_nX == 0&&twardPoint->m_nY == 0)
				{
					//对于接近对方的单位的方格,ZOC不起效果
					continue;
				}
				int nZOC_Cost=pCharacter->GetMove()-(abs(twardPoint->m_nX)+abs(twardPoint->m_nY));
				if(twardPoint->m_nCost<nZOC_Cost)
				{
					pPoint->twardPoints[i]->m_nCost =nZOC_Cost;
				}
			}
		}
	}

	//return pMoveableArray;
}
BattleCharacter* BattleMap::SearchUnitByPosition(int forceFlag,int x,int y,bool bIdentify,bool bSame)
{

	for(unsigned int index = 0;index<m_pUnits->count();index++)
	{
		BattleCharacter* pUnit = (BattleCharacter*)m_pUnits->objectAtIndex(index);
		if(pUnit->MapContains(ccp(x,y)))
		{	
			if(!bIdentify||(bSame?(pUnit->GetForceFlag()==forceFlag):(pUnit->GetForceFlag()!=forceFlag)))
			{
				return pUnit;
			}
			else
			{
				return NULL;
			}
		}
	}
	return NULL;
}


void BattleMap::DrawRanges()
{
	for(unsigned int i = 0;i<m_pShowRangeObjects->count();i++)
	{
		BattleCharacter* pObject=(BattleCharacter *)m_pShowRangeObjects->objectAtIndex(i);
		int offsetX = pObject->GetShowRangePositionX();
		int offsetY = pObject->GetShowRangePositionY();
		CCArray* pRanges = pObject->GetRangeArray();
		DrawRangeRect(pRanges,offsetX,offsetY,ccc4f(0,0,255,0.5));
		//if(pObject->IsShowAtkRange())
		//{
		//	CCArray* pRanges = pObject->GetAtkRangeArray();
		//	DrawRangeLines(pRanges,offsetX,offsetY,ccc4f(0,125,0,125));
		//}
		//if(pObject->IsShowSkillRange())
		//{
		//	
		//	CCArray* pRanges = pObject->GetSkillRangeArray();
		//	DrawRangeLines(pRanges,offsetX,offsetY,ccc4f(0,0,125,255));
		//}
	}
}



void BattleMap::DrawRangeRect(CCArray* pRanges,int offsetX,int offsetY,ccColor4F color)
{
	CHECK_GL_ERROR_DEBUG();
	glLineWidth(0.5f);
	for(unsigned int i = 0;i<pRanges->count();i++)
	{
		if(((PathPoint*)pRanges->objectAtIndex(i))->m_bAvailable)
		{
			int x = ((PathPoint*)pRanges->objectAtIndex(i))->m_nX;
			int y = ((PathPoint*)pRanges->objectAtIndex(i))->m_nY;
			ccDrawSolidRect(ccp(x*SQUARE_LENTH+SQUARE_LENTH/2+offsetX,y*SQUARE_LENTH+SQUARE_LENTH/2+offsetY),ccp(x*SQUARE_LENTH-SQUARE_LENTH/2+offsetX,y*SQUARE_LENTH-SQUARE_LENTH/2+offsetY),color);
		}
	}

	glLineWidth(1);
	ccDrawColor4B(255,255,255,255);
	ccPointSize(1);

	CHECK_GL_ERROR_DEBUG();
}

bool BattleMap::IsShowRange()
{
	return (m_pShowRangeObjects->count()!=0);
}

void BattleMap::SaveBattle()
{
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();
	XMLElement* pRootElement =  pDoc->NewElement("SaveData");
	pDoc->LinkEndChild(pRootElement);
	//储存场景事件节点
	EventHandler::instance()->Save(pDoc);
	//储存地图数据
	XMLElement* pStageElement =pDoc->NewElement("STAGE");
	pStageElement->SetAttribute("ID",m_nID);
	pStageElement->SetAttribute("Turn",m_nTurns);
	pStageElement->SetAttribute("TurnForce",m_nForceTurnFlag);
	pRootElement->LinkEndChild(pStageElement);
	CCObject* pArrayObject;
	CCARRAY_FOREACH(m_pUnits,pArrayObject)
	{
		BattleCharacter* pUnit = (BattleCharacter*)pArrayObject;
		pUnit->Save(pDoc);
	}
	pDoc->SaveFile(SAVE_BATTLE_FILE_NAME);
	delete pDoc;
}


void BattleMap::LoadBattle()
{
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(SAVE_BATTLE_FILE_NAME, "rb", &bufferSize);
	char pCBuffer[MAX_SAVE_DATA_SIZE];
	pCBuffer[0] ='\0';
	sprintf(pCBuffer,"%s%s",(char*)pBuffer,'\0');
	if (pBuffer)
	{
		XMLError bError= pDoc->Parse((const char*)pCBuffer,bufferSize);
		if (bError)
		{
			CCLOG("failed to load BattleSave.save");
			delete pDoc;
			return;
		}
	}
	delete(pBuffer);
	XMLElement* pElement = pDoc->RootElement();
	if (pElement==NULL)
	{
		delete pDoc;
		return;
	}
	//加载事件节点
	if (!EventHandler::instance()->Load(pDoc))
	{
		CCLOG("Event Load failed!");
		return;
	}
	EventHandler::instance()->GetPlayerTurnBGMName(m_strPlayerTurnMusic);
	EventHandler::instance()->GetEnermyTurnBGMName(m_strEnermyTurnMusic);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(m_strPlayerTurnMusic.c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(m_strPlayerTurnMusic.c_str());EventHandler::instance()->GetPlayerTurnBGMName(m_strPlayerTurnMusic);
	pElement = pElement->FirstChildElement(SAVE_DATA_STAGE_NAME);
	if (pElement==NULL)
	{
		delete pDoc;
		return;
	}
	int nID = 0;
	int nTurns = 1;
	int nTurnForceFlag = FORCE_PLAYER;
	pElement->QueryIntAttribute(DATA_ATTRIBUTE_ID,&nID);
	pElement->QueryIntAttribute("Turn",&nTurns);
	pElement->QueryIntAttribute("TurnForce",&nTurnForceFlag);
	CCArray* pUnits = CCArray::createWithCapacity(10);
	pElement = pElement->NextSiblingElement("character");
	if (pElement==NULL)
	{
		CCLOG("Found character node faild!");
		delete pDoc;
		return;
	}
	for (;pElement!=NULL;pElement=pElement->NextSiblingElement("character"))
	{
		BattleCharacter* pUnit = new BattleCharacter();
		if(!pUnit->LoadCharacterFromData(pElement))
		{
			CCLOG("Faild to load character data");
			delete pDoc;
			return;
		}
		pUnits->addObject(pUnit);
	}
	delete pDoc;

	setTouchEnabled(false);
	//所有数据加载完毕，开始布置场景
	//当加载的战斗场景和当前场景一致时，不重新生成TiledMap的纹理图片
	if (nID==m_nID&&m_pTMXTiledMap!=NULL)
	{
		m_pTMXTiledMap->retain();
		m_pTMXTiledMap->removeFromParentAndCleanup(false);
	}
	else
	{
		m_nID = nID;
	}
	m_nTurns = nTurns;
	m_nForceTurnFlag = nTurnForceFlag;
	//清除所有子节点
	removeAllChildrenWithCleanup(true);
	//释放掉角色菜单
	BattleMenu* pMenu = BattleMenu::instance();
	pMenu->clear();
	//获取角色容器
	m_pUnits->release();
	m_pUnits = pUnits;
	m_pUnits->retain();
	//初始化角色地图
	BattleCharacter::m_pMap = this;
	CharacterStatus::m_pMap = this;
	//设置角色菜单
	pMenu = BattleMenu::instance();
	pMenu->SetParent(this);
	//预加载背景音乐
	char chStr[50];
	sprintf(chStr,"%s.mp3",MUSIC_BATTLE1_FILE_FULLPATH);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(chStr);
	sprintf(chStr,"%s.mp3",MUSIC_BATTLE2_FILE_FULLPATH);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(chStr);
	if (m_pTMXTiledMap!=NULL)
	{
		addChild(m_pTMXTiledMap,-MAX_LAYER_ZORDER,BATTLEMAP_TMX_TAG);
		m_pTMXTiledMap->autorelease();
	}
	else
	{
		sprintf(chStr,"%s%d.tmx",STAGE_FILE_FULLPATH,m_nID);
		//加载tmx地图
		m_pTMXTiledMap = CCTMXTiledMap::create(chStr);
		addChild(m_pTMXTiledMap,-MAX_LAYER_ZORDER, BATTLEMAP_TMX_TAG);
		m_pTMXTiledMap->setPosition(ccp(0,0));
	}
	//初始化地图拖拽范围
	m_fMaxPositionX = 0;
	m_fMaxPositionY = 0;
	float winSizeX = CCDirector::sharedDirector()->getWinSize().width;
	float winSizeY = CCDirector::sharedDirector()->getWinSize().height;
	m_fMinPositionX =  winSizeX- m_pTMXTiledMap->getContentSize().width;
	m_fMinPositionY =  winSizeY- m_pTMXTiledMap->getContentSize().height;
	//初始化数字和角色
	sprintf(chStr,"%s.png",NUMS_FILE_FULLPATH);
	m_pNumsBatchNode = CCSpriteBatchNode::create(chStr);
	addChild(m_pNumsBatchNode,MAX_LAYER_ZORDER,LAYER_MAPNUMS_ID);
	//初始化方格动画
	sprintf(chStr,"%s.png",SQUARE_ICON_FILE_FULLPATH);
	m_pSquareBatchNode = CCSpriteBatchNode::create(chStr);
	addChild(m_pSquareBatchNode,-1);
	//加载角色精灵
	for(unsigned int index = 0;index<m_pUnits->count();index++)
	{
		((BattleCharacter*)m_pUnits->objectAtIndex(index))->LoadSpritesAndStatus();
	}
	//播放背景音乐
	sprintf(chStr,"%s.mp3",MUSIC_BATTLE1_FILE_FULLPATH);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(chStr,true);
	//恢复用户操作
	setTouchEnabled(true);
}

void BattleMap::TimingPlayerTurn()
{
	if (m_nForceTurnFlag == FORCE_PLAYER)
	{
		CCObject* pUnit = NULL;
		CCARRAY_FOREACH(m_pUnits,pUnit)
		{
			if(((BattleCharacter*)pUnit)->IsControlable())
			{
				return;
			}
		}
		//没有可以操作的单位，结束回合
		EndTurn(this,NULL);
	}

}

void BattleMap::TimingGameOver()
{

}

void BattleMap::BattleClear()
{
	for(;m_pUnits->count() != 0;)
	{
		BattleCharacter* pUnit = (BattleCharacter*)m_pUnits->lastObject();
		if (pUnit->GetForceFlag() == FORCE_PLAYER)
		{
			PlayerDataManager::instance()->AddPartner(pUnit);
			m_pUnits->removeLastObject(false);
			pUnit->RecoverHPToMax();
			pUnit->RecoverSPToMax();

		}
		else
		{
			m_pUnits->removeLastObject();
		}

	}

}

void BattleMap::RemoveAllEventMarks()
{
	m_vEventMarks.clear();
	m_pSquareBatchNode->removeAllChildrenWithCleanup(true);
}

void BattleMap::SetEventMark( CCPoint* pPoint )
{
	m_vEventMarks.push_back(pPoint);
}

void BattleMap::DrawEventMarks()
{
	for (vector<CCPoint*>::iterator it = m_vEventMarks.begin();it!=m_vEventMarks.end();it++)
	{
		CCPoint* pPoint = *it;
		float x = pPoint->x;
		float y = pPoint->y;
		CCAnimation* pAnimation = AnimationManager::instance()->CreateSquareAnimation();
		CCSprite* pSquare = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
		pSquare->setPosition(ccp(x+SQUARE_LENTH/2,y+SQUARE_LENTH/2));
		m_pSquareBatchNode->addChild(pSquare);
		pSquare->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));
		//ccDrawSolidRect(*pPoint,ccp(x+SQUARE_LENTH,y+SQUARE_LENTH),ccc4f(100,0,30,0.5));
	}
}

void BattleMap::SearchCharacterAndViewLock( int nCharId )
{
	for (unsigned int index = 0;index<m_pUnits->count();index++)
	{
		BattleCharacter* pCharacter = (BattleCharacter*)m_pUnits->objectAtIndex(index);
		if (pCharacter->GetCharID()==nCharId)
		{
			ViewLock(pCharacter);
			return;
		}

	}
}

void BattleMap::onExit()
{
	CCLayer::onExit();

}

void BattleMap::ContinueAfterCharacterAcition()
{
	if (m_nForceTurnFlag == FORCE_PLAYER)
	{
		TimingPlayerTurn();
	}
	else
	{
		AIActionsChan();
	}
}

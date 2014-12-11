#include "GameUI.h"
#include "BattleMapScene.h"
#include "BattleObject.h"
#include "StoryScene.h"
#include "EventHandler.h"
#include <string>
using namespace std;
#define DIALOG_START_X 60
#define DIALOG_END_X 580
#define DIALOG_START_Y 114
#define DIALOG_STR_SHOUW_ROW 28
#define DIALOG_STR_SHOW_LINE 3
#define DIALOG_STR_SHOW_CSTYLE_CAP (DIALOG_STR_SHOUW_ROW*DIALOG_STR_SHOW_LINE)*2+1
//对话阅读的文字时间间隔
#define DIALOG_WORD_DUR 0.03f


//初始化角色战斗菜单单例
DECLARE_SINGLETON_MEMBER(BattleMenu);



//定义BattleMenu

BattleMenu::BattleMenu()
{ 
	m_pInvoker = NULL;
	m_pActivedIcons = CCArray::createWithCapacity(6);
	m_pActivedIcons->retain();

	//初始化精灵
	/*CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	char chPlistStr[100] = {0};
	char chImageStr[100]= {0};
	sprintf(chPlistStr,"logo.plist");
	sprintf(chImageStr,"logo.png");
	cache->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey(chImageStr);	
	CCSpriteBatchNode *sprites = CCSpriteBatchNode::createWithTexture(texture);
	m_pMoveIcon =  CCSprite::createWithSpriteFrameName("movelogo.gif");
	m_pMoveIcon ->setTag(COMM_MOVE);
	m_pActivedIcons->addObject(m_pMoveIcon);*/
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s.png",BATTLEMENU_MENU_FILE_FULLPATH);
	//cache->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey(chImageStr);	
	m_pBatchNode = CCSpriteBatchNode::createWithTexture(texture);
	m_pBatchNode->retain();
	/*初始化移动指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_MOVE_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_MOVE,&m_pMoveIcon);
	/*初始化道具指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_ITEM_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_ITEM,&m_pItemIcon);
	/*初始化攻击指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_ATK_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_ATK,&m_pAtkIcon);
	/*初始化技能指令图标*/	
	sprintf(chImageStr,"%s.png",BATTLEMENU_SKILL_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_SKILL,&m_pSkillIcon);
	///*初始化防御指令图标*/	
	//sprintf(chImageStr,"%s.png",BATTLEMENU_DEF_ICON_NAME);
	//InitActivedIconByFileName(chImageStr,texture,DEF_COMM,&m_pDefIcon);
	//SCALE_UI(m_pDefIcon);
	/*初始化待机指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_STAY_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_STAY,&m_pStayIcon);
	/*初始化撤回指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_BACK_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_BACK,&m_pBackIcon);
	/*初始化查看角色指令图标*/
	sprintf(chImageStr,"%s.png",BATTLEMENU_STATUS_ICON_NAME);
	InitActivedIconByFileName(chImageStr,COMM_Status,&m_pStatusIcon);
	////初始化菜单等级
	m_nMenuLevel = 0;
	//pLevelOneBatchNode = NULL;
}


BattleMenu::~BattleMenu()
{
	m_pAtkIcon->release();
	m_pMoveIcon->release();
	//m_pDefIcon->release();
	m_pSkillIcon->release();
	m_pItemIcon->release();
	m_pStayIcon->release();
	m_pBatchNode->release();
	/*pLevelZeroBatchNode->release();
	pLevelOneBatchNode->release();*/
}
void BattleMenu::CheckTouchedCommand(cocos2d::CCPoint touchedPoint)
{
	for(unsigned int index = 0;index<m_pActivedIcons->count();index++)
	{
		CCSprite* pOnCheckIcon = (CCSprite*)m_pActivedIcons->objectAtIndex(index);
		CCPoint onCheckPoint = touchedPoint;
		if(pOnCheckIcon->getParent()!=NULL&&pOnCheckIcon->boundingBox().containsPoint(onCheckPoint))
		{
			HangMenu();
			m_pInvoker->ExecuteCommand(pOnCheckIcon->getTag());
			return;
		}
	}

	//if(Usable())
	//{
	//	m_pInvoker->ResumeStandby();
	//}

}
void BattleMenu::EnterTopMenu()
{
	m_pActivedIcons->removeAllObjects();
	HangMenu();
	m_nMenuLevel = 0;
	ShowMenu();
}
void BattleMenu::EnterChildMenu(std::vector<int> &vObjects,int nCommand)
{
	m_pActivedIcons->removeAllObjects();
	HangMenu();
	//CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//char chPlistStr[100] = {0};
	char chImageStr[50]= {0};
	switch(nCommand)
	{
	case COMM_SKILL:
		sprintf(chImageStr,"%s",BATTLEMENU_SKILL_ICON_NAME);
		break;
	case COMM_ITEM:
		sprintf(chImageStr,"%s",BATTLEMENU_SKILL_ICON_NAME);
		break;
	}
	assert(chImageStr[0]!=0);
	//sprintf(chPlistStr,"%s.plist",BATTLEMENU_SKILL_FILE_NAME);
	//sprintf(chImageStr,"%s.png",BATTLEMENU_SKILL_FILE_NAME);
	//cache->addSpriteFramesWithFile(chPlistStr,chImageStr);
	//pLevelOneBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	//pLevelOneBatchNode->retain();
	for(vector<int>::iterator it = vObjects.begin();it!=vObjects.end();it++)
	{
		char chFrameStr[50];
		int tag = *it;
		sprintf(chFrameStr,"%s%d.png",chImageStr,tag);
		CCSprite* pIcon = CCSprite::createWithSpriteFrameName(chFrameStr);
		pIcon->setTag(tag);
		m_pActivedIcons->addObject(pIcon);
	}
	m_pActivedIcons->addObject(m_pBackIcon);
	m_nMenuLevel = 1;
	/*m_pMapParent->removeChildByTag(BATCH_TAG,false);
	m_pMapParent->addChild(pLevelOneBatchNode,MAX_LAYER_ZORDER,BATCH_TAG);*/
	ShowMenu();
}
//void BattleMenu:: EnterChildMenu(CCArray* pBattleObjects)
//{
//	m_pActivedIcons->removeAllObjects();
//	HangMenu();
//	//CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
//	//char chPlistStr[100] = {0};
//	char chImageStr[100]= {0};
//	//sprintf(chPlistStr,"%s.plist",BATTLEMENU_SKILL_FILE_NAME);
//	//sprintf(chImageStr,"%s.png",BATTLEMENU_SKILL_FILE_NAME);
//	//cache->addSpriteFramesWithFile(chPlistStr,chImageStr);
//	//pLevelOneBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
//	//pLevelOneBatchNode->retain();
//	for(unsigned int index = 0;index<pBattleObjects->count();index++)
//	{
//		int tag = ((BattleSkill*)pBattleObjects->objectAtIndex(index))->GetID();
//		sprintf(chImageStr,"%s%d.png","skill",tag);
//		CCSprite* pIcon = CCSprite::createWithSpriteFrameName(chImageStr);
//		pIcon->setTag(tag+BATTLEMENU_SKILL_TAG_DUR);
//		m_pActivedIcons->addObject(pIcon);
//	}
//	m_pActivedIcons->addObject(m_pBackIcon);
//	m_nMenuLevel = 1;
//	/*m_pMapParent->removeChildByTag(BATCH_TAG,false);
//	m_pMapParent->addChild(pLevelOneBatchNode,MAX_LAYER_ZORDER,BATCH_TAG);*/
//	ShowMenu();
//}


bool BattleMenu::Usable()
{
	return (m_pInvoker != NULL&&m_pMapParent!=NULL);
}

void BattleMenu::ShowMenu()
{
	if(m_pActivedIcons->count() == 0&&m_nMenuLevel == 0)
	{
		//初始化角色指令
		if(m_pInvoker->IsAbleToAtk())
		{
			m_pActivedIcons->addObject(m_pAtkIcon);
			/*	m_pMapParent->addChild(m_pAtkIcon);*/
		}
		//if(m_pInvoker->IsAbleToDef())
		//{
		//	m_pActivedIcons->addObject(m_pDefIcon);
		//}
		if(m_pInvoker->IsAbleToItem())
		{
			m_pActivedIcons->addObject(m_pItemIcon);
			/*m_pMapParent->addChild(m_pItemIcon);*/
		}
		if(m_pInvoker->IsAbleToSkill())
		{
			m_pActivedIcons->addObject(m_pSkillIcon);
			/*	m_pMapParent->addChild(m_pSkillIcon);*/
		}

		if(m_pInvoker->IsAbleToStay())
		{
			m_pActivedIcons->addObject(m_pStayIcon);
			/*m_pMapParent->addChild(m_pStayIcon);*/
		}
		if(m_pInvoker->IsAbleToMove())
		{
			m_pActivedIcons->addObject(m_pMoveIcon);
			/*		m_pMapParent->addChild(m_pMoveIcon);*/
		}
		m_pActivedIcons->addObject(m_pBackIcon);
		m_pActivedIcons->addObject(m_pStatusIcon);
		//m_pMapParent->removeChildByTag(BATCH_TAG,false);
		//m_pMapParent->addChild(pLevelZeroBatchNode,MAX_LAYER_ZORDER,BATCH_TAG);
	}
	float perAgl = 0;
	int count = m_pActivedIcons->count();
	if(count>1)
	{
		perAgl = 2*MATH_PI/count;
	}
	for(unsigned int index = 0;index<m_pActivedIcons->count();index++)
	{
		float x = SQUARE_LENTH * sinf(perAgl*index);
		float y = SQUARE_LENTH * cosf(perAgl*index);
		CCSprite* pIcon = (CCSprite*)m_pActivedIcons->objectAtIndex(index);
	/*	CCNode* pN = m_pMapParent->getChildByTag(BATCH_TAG);*/
		m_pBatchNode->addChild(pIcon,MAX_LAYER_ZORDER);
		pIcon->setPosition(ccp(m_pInvoker->GetMapPositionX()+x,m_pInvoker->GetMapPositionY()+y));
	}
}
void BattleMenu::HangMenu()
{
	/*CCNode* pBatch = m_pMapParent->getChildByTag(BATCH_TAG);
	if(pBatch ==NULL)
	{
		return;
	}
	unsigned int count =  pBatch->getChildrenCount();
	for(unsigned int index = 0;index<count;index++)
	{
		CCSprite*  pIcon = (CCSprite*)m_pActivedIcons->objectAtIndex(index);
		m_pMapParent->getChildByTag(BATCH_TAG)->removeChild(pIcon,false);
	}*/
	if(m_pBatchNode!=NULL)
	{
		bool bClean =(m_nMenuLevel ==1);
		m_pBatchNode->removeAllChildrenWithCleanup(bClean);
	}
}

void BattleMenu::RemoveMoveCommand()
{
	if(m_pActivedIcons->containsObject(m_pMoveIcon))
	{
		m_pActivedIcons->fastRemoveObject(m_pMoveIcon);
	}
}

void BattleMenu::ResumeMoveCommand()
{
	if(!m_pActivedIcons->containsObject(m_pMoveIcon))
	{
		m_pActivedIcons->addObject(m_pMoveIcon);
	}
}
void BattleMenu::SetParent(CCLayer* pLayer)
{
	m_pMapParent = pLayer;
	m_pMapParent->addChild(m_pBatchNode,MAX_LAYER_ZORDER);
	/*for(unsigned int index = 0;index<m_pActivedIcons->count();index++)
	{
	CCSprite*  pIcon = (CCSprite*)m_pActivedIcons->objectAtIndex(index);
	m_pMapParent->addChild(pIcon);
	pIcon->setVisible(false);

	}*/
}
void BattleMenu::SetInvoker(BattleCharacter * pInvoker)
{
	HangMenu();
	m_pInvoker = pInvoker;
	m_pActivedIcons->removeAllObjects();
}
void BattleMenu::InitActivedIconByFileName(const char* pFileName,int nTag,CCSprite** pIcon)
{
	*pIcon =  CCSprite::createWithSpriteFrameName(pFileName);
	(*pIcon) ->setTag(nTag);
	(*pIcon) ->retain();
}
bool BattleMenu::Back()
{
	m_nMenuLevel = 0;
	return false;
}
void  BattleMenu::Disconnect()
{
	SetInvoker(NULL);
	//m_pMapParent->removeChildByTag(BATCH_TAG,false);
	m_pActivedIcons->removeAllObjects();
	bool bClean = (m_nMenuLevel == 1);
	m_pBatchNode->removeAllChildrenWithCleanup(bClean);
	//pLevelZeroBatchNode->removeAllChildrenWithCleanup(false);
	//if(pLevelOneBatchNode!=NULL)
	//{
	//	pLevelOneBatchNode->removeAllChildrenWithCleanup(true);
	//}
	m_nMenuLevel = 0;
}
void BattleMenu::clear()
{
	delete(_instance);
	_instance = NULL;
}
//TurnPass
TurnPassLayer::TurnPassLayer(int nforceFlag)
{;
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s.png",TURNPASS_LAYER_FILE_FULLPATH);
	m_pBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	if(nforceFlag==FORCE_PLAYER)
	{
		sprintf(chImageStr,"%s.png",TURNPASS_PLAYER_MARK_NAME);
		m_pTurnMark = CCSprite::createWithSpriteFrameName(chImageStr);
		sprintf(chImageStr,"%s.png",TURNPASS_PLAYER_LABEL_NAME);
		m_pTurnLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	}
	else if(nforceFlag== FORCE_ENERMY)
	{
		sprintf(chImageStr,"%s.png",TURNPASS_ENERMY_MARK_NAME);
		m_pTurnMark = CCSprite::createWithSpriteFrameName(chImageStr);
		sprintf(chImageStr,"%s.png",TURNPASS_ENERMY_LABEL_NAME);
		m_pTurnLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	}
	sprintf(chImageStr,"%s.png",TURNPASS_BK_BAR_NAME);
	m_pPassTurnBar = CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s%d.png",TURNPASS_BLINK_BK_FRAME_NAMEHEAD,0);
	m_pPassTurnBlink = CCSprite::createWithSpriteFrameName(chImageStr);
	addChild(m_pBatchNode);
	m_pBatchNode->addChild(m_pPassTurnBar,-2);
	m_pBatchNode->addChild(m_pPassTurnBlink,-3);
	m_pBatchNode->addChild(m_pTurnLabel,-1);
	m_pBatchNode->addChild(m_pTurnMark,-4);
	//摆放控件
	m_pPassTurnBar->setPosition(VisibleRect::center());
	m_pPassTurnBlink->setPosition(VisibleRect::center());
	m_pTurnLabel->setPosition(VisibleRect::center());
	m_pTurnMark->setPosition(VisibleRect::center());
}
void TurnPassLayer::onEnter()
{
	CCLayer::onEnter();
	char fileHead[50];
	sprintf(fileHead,"%s",TURNPASS_BLINK_BK_FRAME_NAMEHEAD);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* frames = CCArray::createWithCapacity(TURNPASS_BLINK_BK_FRAME_COUNT);
	for(int i = 0; i < TURNPASS_BLINK_BK_FRAME_COUNT; i++) 
	{
		char chFileName[50];
		sprintf(chFileName, "%s%d.png",fileHead,i);
		CCSpriteFrame* frame = cache->spriteFrameByName( chFileName ); 
		frames->addObject(frame);  
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.2f );
	CCArray* pActions = CCArray::createWithCapacity(2);
	pActions->addObject(CCAnimate::create(animation));
	BattleMap* pMap = (BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
	pActions->addObject(CCCallFuncND::create(pMap,callfuncND_selector(BattleMap::TurnBegin),NULL));
	CCAction* pSequenceActions = CCSequence::create(pActions);
	m_pPassTurnBlink->runAction(pSequenceActions);
}
TurnPassLayer::~TurnPassLayer()
{

}
//BattleMainMenu
MainMenu::MainMenu()
{
	setTouchEnabled(true);
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s.png",MAINMENU_MENU_FILE_FULLPATH);
	m_pBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	//通过文件名在缓存中获取帧
	//回合结束
	sprintf(chImageStr,"%s.png",MAINMENU_ENDTURN_LABEL_NAME);
	m_pEndTurnLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//游戏结束
	sprintf(chImageStr,"%s.png",MAINMENU_ENDGAME_LABEL_NAME);
	m_pEndGameLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//储存
	sprintf(chImageStr,"%s.png",MAINMENU_SAVE_LABEL_NAME);
	m_pSaveLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//读取
	sprintf(chImageStr,"%s.png",MAINMENU_LOAD_LABEL_NAME);
	m_pLoadLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//胜利条件
	sprintf(chImageStr,"%s.png",MAINMENU_TOVICTORY_LABEL_NAME);
	m_pToVictoryLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//行军
	sprintf(chImageStr,"%s.png",MAINMENU_GOTOAIM_LABEL_NAME);
	m_pGoToAimLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//系统设定
	sprintf(chImageStr,"%s.png",MAINMENU_SYSTEMCONFIG_LABEL_NAME);
	m_pSystemConfigLabel = CCSprite::createWithSpriteFrameName(chImageStr);
	//战场概况
	sprintf(chImageStr,"%s.png",MAINMENU_BATTLEFIELDSTATUS_CONPONENT_NAME);
	m_pBattleFieldStatusConponent = CCSprite::createWithSpriteFrameName(chImageStr);
	//主菜单背景
	sprintf(chImageStr,"%s.png",MAINMENU_MAINMENU_CONPONENT_NAME);
	m_pMainMenuConponent = CCSprite::createWithSpriteFrameName(chImageStr);
	//图层透明背景
	sprintf(chImageStr,"%s.png",MAINMENU_TRANSPARENT_BACKGROUND_NAME);
	m_pBackGround = CCSprite::createWithSpriteFrameName(chImageStr);
	//初始化按钮
	sprintf(chImageStr,"%s.png",MAINMENU_BUTTON_CONPONENT);
	m_pButtonConponent = CCSprite::createWithSpriteFrameName(chImageStr);
	m_pButtonConponent->retain();
	sprintf(chImageStr,"%s.png",MAINMENU_OK_BUTTON_NAME);
	m_pButtonOk= CCSprite::createWithSpriteFrameName(chImageStr);//size(90,154)
	m_pButtonOk->retain();
	sprintf(chImageStr,"%s.png",MAINMENU_CANCEL_BUTTON_NAME);//offset(44.43,44)
	m_pButtonCancel = CCSprite::createWithSpriteFrameName(chImageStr);//offset(45,108)
	m_pButtonCancel->retain();
	//排列节点
	m_pBatchNode->addChild(m_pEndGameLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pEndTurnLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pGoToAimLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pLoadLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pSaveLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pToVictoryLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pSystemConfigLabel,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pBackGround,-1);
	m_pBatchNode->addChild(m_pBattleFieldStatusConponent,0);
	m_pBatchNode->addChild(m_pMainMenuConponent,0);
	//设定位置
	m_pMainMenuConponent->setAnchorPoint(ccp(0,1));
	m_pMainMenuConponent->setPosition(ccp(VisibleRect::leftTop().x+40,VisibleRect::leftTop().y-40));
	int main_x = m_pMainMenuConponent->getPositionX();
	int main_y = m_pMainMenuConponent->getPositionY();
	int main_lenth = GetSpriteTextureLength(m_pMainMenuConponent);
	m_pEndTurnLabel->setPosition(ccp(main_x+main_lenth/2,main_y-40));
	m_pGoToAimLabel->setPosition(ccp(main_x+main_lenth/2,main_y-70));
	m_pSaveLabel->setPosition(ccp(main_x+main_lenth/2,main_y-100));
	m_pLoadLabel->setPosition(ccp(main_x+main_lenth/2,main_y-130));
	m_pSystemConfigLabel->setPosition(ccp(main_x+main_lenth/2,main_y-160));
	m_pToVictoryLabel->setPosition(ccp(main_x+main_lenth/2,main_y-190));
	m_pEndGameLabel->setPosition(ccp(main_x+main_lenth/2,main_y-220));
	m_pBackGround->setAnchorPoint(ccp(0,0));
	m_pBackGround->setPosition(ccp(0,0));
	m_pBattleFieldStatusConponent->setAnchorPoint(ccp(1,0));
	m_pBattleFieldStatusConponent->setPosition(VisibleRect::rightBottom());
	//将菜单按钮初始化为不可见
	CCArray* pLabels =m_pBatchNode->getChildren();
	CCObject* pChild = NULL;
	CCARRAY_FOREACH(pLabels,pChild)
	{
		CCSprite* pLabel = (CCSprite*)pChild;
		if (pLabel!=m_pBackGround&&pLabel!=m_pBattleFieldStatusConponent&&pLabel!=m_pMainMenuConponent)
		{
			pLabel->setVisible(false);
		}	
	}
	//初始化被选中的菜单
	m_pOnSelect = NULL;
	addChild(m_pBatchNode);
}

MainMenu::~MainMenu()
{

}
void MainMenu::CreateButton(int nOK_Tag,float x,float y)
{
	ClearButton();
	m_pBatchNode->addChild(m_pButtonCancel,MAX_LAYER_ZORDER,COMM_MAINMENU_MAINMENU_CANCEL);
	m_pBatchNode->addChild(m_pButtonConponent,-1,COMM_MAINMENU_BUTTON_CONPONENT);
	m_pBatchNode->addChild(m_pButtonOk,MAX_LAYER_ZORDER,nOK_Tag);
	m_pButtonConponent->setPosition(ccp(x+128,y));
	float conpX =m_pButtonConponent->getPositionX();
	float conpY =m_pButtonConponent->getPositionY();
	m_pButtonCancel->setPosition(ccp(conpX ,conpY-31));
	m_pButtonOk->setPosition(ccp(conpX-0.57f,conpY+33));
	m_pButtonOk->setVisible(false);
	m_pButtonCancel->setVisible(false);

}



void MainMenu::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

void MainMenu::ClearButton()
{
	if(m_pButtonConponent->getParent()!=NULL)
	{
		m_pButtonConponent->removeFromParentAndCleanup(false);
	}
	if(m_pButtonOk->getParent()!=NULL)
	{
		m_pButtonOk->removeFromParentAndCleanup(false);
	}
	if(m_pButtonCancel->getParent()!=NULL)
	{
		m_pButtonCancel->removeFromParentAndCleanup(false);
	}
}

bool MainMenu::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	if(m_pOnSelect ==NULL)
	{
		touchedPoint = convertToNodeSpace(touchedPoint);
		if(m_pEndTurnLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pEndTurnLabel->setVisible(true);
			m_pOnSelect = m_pEndTurnLabel;
		}
		else if(m_pGoToAimLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pGoToAimLabel->setVisible(true);
			m_pOnSelect = m_pGoToAimLabel;
		}
		else if (m_pLoadLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pLoadLabel->setVisible(true);
			m_pOnSelect = m_pLoadLabel;
		}
		else if (m_pSaveLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pSaveLabel->setVisible(true);
			m_pOnSelect = m_pSaveLabel;
		}
		else if (m_pSystemConfigLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pSystemConfigLabel->setVisible(true);
			m_pOnSelect = m_pSystemConfigLabel;
		}
		else if (m_pToVictoryLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pToVictoryLabel->setVisible(true);
			m_pOnSelect = m_pToVictoryLabel;
		}
		else if(m_pEndGameLabel->boundingBox().containsPoint(touchedPoint))
		{
			m_pEndGameLabel->setVisible(true);
			m_pOnSelect = m_pEndGameLabel;
		}
		CCArray* pLabels =m_pBatchNode->getChildren();
		CCObject* pChild = NULL;
		CCARRAY_FOREACH(pLabels,pChild)
		{
			CCSprite* pLabel = (CCSprite*)pChild;
			if (pLabel!=m_pBackGround&&pLabel!=m_pBattleFieldStatusConponent&&pLabel!=m_pMainMenuConponent&&pLabel!=m_pOnSelect)
			{
				pLabel->setVisible(false);
			}	
		}
	}
	else if(m_pButtonOk->boundingBox().containsPoint(touchedPoint))
	{
		m_pButtonOk->setVisible(true);
		m_pButtonCancel->setVisible(false);
	}
	else if(m_pButtonCancel->boundingBox().containsPoint(touchedPoint))
	{
		m_pButtonOk->setVisible(false);
		m_pButtonCancel->setVisible(true);
	}
	return true;
}
void MainMenu::ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent)
{
	/*CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it); */
	CCPoint touchedPoint = pTouch->getPreviousLocationInView();
	touchedPoint = CCDirector::sharedDirector()->convertToGL(touchedPoint);
	touchedPoint = convertToNodeSpace(touchedPoint);
	if(m_pEndTurnLabel->boundingBox().containsPoint(touchedPoint)&&m_pEndTurnLabel==m_pOnSelect)
	{
		CreateButton(COMM_MAINMENU_ENDTURN_OK,m_pEndTurnLabel->getPositionX(),m_pEndTurnLabel->getPositionY());
	}
	else if(m_pEndGameLabel->boundingBox().containsPoint(touchedPoint)&&m_pEndGameLabel==m_pOnSelect)
	{
		CreateButton(COMM_MAINMENU_ENDGAME_OK,m_pEndGameLabel->getPositionX(),m_pEndGameLabel->getPositionY());
	}
	else if(m_pSaveLabel->boundingBox().containsPoint(touchedPoint)&&m_pSaveLabel==m_pOnSelect)
	{
		CreateButton(COMM_MAINMENU_SAVEGAME_OK,m_pLoadLabel->getPositionX(),m_pLoadLabel->getPositionY());
	}
	else if (m_pLoadLabel->boundingBox().containsPoint(touchedPoint)&&m_pLoadLabel==m_pOnSelect)
	{
		CreateButton(COMM_MAINMENU_LOADGAME_OK,m_pOnSelect->getPositionX(),m_pOnSelect->getPositionY());
	}
	else if(m_pButtonOk->boundingBox().containsPoint(touchedPoint))
	{
		int nFlag = m_pButtonOk->getTag();
		if(nFlag == COMM_MAINMENU_ENDTURN_OK)
		{
			BattleMap* pMap = (BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
			removeFromParentAndCleanup(true);
			pMap->EndTurn(pMap,NULL);
		}
		else if(nFlag == COMM_MAINMENU_ENDGAME_OK)
		{
			CCDirector::sharedDirector()->end();
		}
		else if(nFlag ==COMM_MAINMENU_SAVEGAME_OK)
		{
			((BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID))->SaveBattle();
			removeFromParentAndCleanup(true);
		}
		else if(nFlag ==COMM_MAINMENU_LOADGAME_OK)
		{
			((BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID))->LoadBattle();
			removeFromParentAndCleanup(true);
		}
	}
	else if(m_pButtonCancel->boundingBox().containsPoint(touchedPoint)&&m_pOnSelect!=NULL)
	{
		ClearButton();
		m_pOnSelect->setVisible(false);
		m_pOnSelect = NULL;
	}
	else
	{

		if(m_pButtonConponent->getParent()!=NULL)
		{
			//当按钮存在时没选取成功将按钮设为不激活状态
			m_pButtonOk->setVisible(false);
			m_pButtonCancel->setVisible(false);
		}
		else if(m_pOnSelect!=NULL)
		{
			//当点击到菜单，选取失败时，取消选取菜单
			m_pOnSelect->setVisible(false);
			m_pOnSelect = NULL;
		}
		else if(m_pMainMenuConponent->boundingBox().containsPoint(touchedPoint))
		{
			//不小心没选中按钮，触碰到了主菜单背景
		}
		else
		{
			removeFromParentAndCleanup(true);
		}
		
	}
}

//ShowStatus

ShowStatusMenu::ShowStatusMenu(BattleCharacter* pInvoker)
{
	setTouchEnabled(true);

	m_pInvoker  = pInvoker;
	char tempStr[50];
	sprintf(tempStr,"%s.png",SHOWSTATUS_MENU_FILE_FULLPATH);
	m_pStatusBackGround = CCSprite::create(tempStr);
	//布置显示状态的背景
	addChild(m_pStatusBackGround,0);
	m_pStatusBackGround->setAnchorPoint(ccp(0,1));
	m_pStatusBackGround->setPosition(ccp(VisibleRect().center().x-GetSpriteTextureLength(m_pStatusBackGround)/2,VisibleRect().top().y));
	float bkX = m_pStatusBackGround->getPositionX();
	float bkY = m_pStatusBackGround->getPositionY();
	//显示最大HP
	sprintf(tempStr,"%d",m_pInvoker->GetMaxHP());
	CCLabelTTF* dataLabel = CCLabelTTF::create(tempStr,"Arial",24);
	addChild(dataLabel,MAX_LAYER_ZORDER);
	dataLabel->setPosition(ccp(bkX+138,bkY-78));
	//显示当前HP
	sprintf(tempStr,"%d",m_pInvoker->GetHP());
	dataLabel = CCLabelTTF::create(tempStr,"Arial",24);
	addChild(dataLabel,MAX_LAYER_ZORDER);
	dataLabel->setPosition(ccp(bkX+70,bkY-78));
	//显示物理攻击力
	sprintf(tempStr,"%d",m_pInvoker->GetAtk());
	dataLabel = CCLabelTTF::create(tempStr,"Arial",24);
	addChild(dataLabel,MAX_LAYER_ZORDER);
	dataLabel->setPosition(ccp(bkX+92,bkY-122));
	//显示防御攻击力
	sprintf(tempStr,"%d",m_pInvoker->GetDef());
	dataLabel = CCLabelTTF::create(tempStr,"Arial",24);
	addChild(dataLabel,MAX_LAYER_ZORDER);
	dataLabel->setPosition(ccp(bkX+92,bkY-144));
}

ShowStatusMenu::~ShowStatusMenu()
{
	removeAllChildrenWithCleanup(true);
}
void ShowStatusMenu::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

bool ShowStatusMenu::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

void ShowStatusMenu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	removeFromParentAndCleanup(true);
}





ShowRangeMenu::ShowRangeMenu(BattleCharacter* pInvoker,int nSubRange,bool bIsCommand)
{
	setTouchEnabled(true);
	m_bIsCommand = bIsCommand;
	m_pInvoker = pInvoker;
	m_tSubPoint = ccp(pInvoker->GetMapPositionX(),pInvoker->GetMapPositionY());
	char chImageStr[100]= {0};
	sprintf(chImageStr,"%s.png",RANGEMENU_LAYER_FILE_FULLPATH);
	m_pBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	sprintf(chImageStr,"%s.png",RANGEMENU_OK_BUTTON_NAME);
	m_pOKButton = CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s.png",RANGEMENU_CANCEL_BUTTON_NAME);
	m_pCancelButton = CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s.png",RANGEMENU_RESET_BUTTON_NAME);
	m_pReset =  CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s.png",RANGEMENU_RESET_BUTTON_NAME);
	m_nSubRange = nSubRange;
	if(m_nSubRange!=0)
	{
		sprintf(chImageStr,"%s%d.png",RANGEMENU_SUBRANGE_X_NAME,m_nSubRange);
		m_pSubRange = CCSprite::createWithSpriteFrameName(chImageStr);
	}
	else
	{
		m_pSubRange = NULL;
	}
	sprintf(chImageStr,"%s.png",RANGEMENU_BACKGROUND_NAME);
	m_pBackGround = CCSprite::createWithSpriteFrameName(chImageStr);
	addChild(m_pBatchNode);
	m_pBatchNode->addChild(m_pCancelButton,100);
	m_pBatchNode->addChild(m_pBackGround,1);
	if(m_bIsCommand == true)
	{
		m_pBatchNode->addChild(m_pOKButton,100);
		m_pBatchNode->addChild(m_pReset,100);
	}
}
ShowRangeMenu::~ShowRangeMenu()
{
	removeAllChildrenWithCleanup(true);
}

void ShowRangeMenu::onEnter()
{
	CCLayer::onEnter();
	Refresh();
	if(m_pSubRange!=NULL)
	{
		CCScene *pScene = (CCScene*)getParent();
		if(pScene ==NULL)
		{
			return;
		}
		CCLayer* pLayer = (CCLayer*)pScene->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
		if(pLayer == NULL)
		{
			return;
		}
		pLayer->addChild(m_pSubRange,0,LAYER_SUBRANGE_ID);
		m_pSubRange->setPosition(m_tSubPoint);
	}

}

bool ShowRangeMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void ShowRangeMenu::ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent)
{

	CCPoint touchedPoint = pTouch->getPreviousLocationInView();
	touchedPoint = CCDirector::sharedDirector()->convertToGL(touchedPoint);
	touchedPoint = convertToNodeSpace(touchedPoint);
	if(m_bIsCommand == true&&m_pOKButton->boundingBox().containsPoint(touchedPoint))
	{
		bool bCommand = m_pInvoker->PerformFromTouch(ccp(m_pSubRange->getPositionX(),m_pSubRange->getPositionY()));
		if(!bCommand)
		{
			return;
		}
		m_pSubRange->removeFromParentAndCleanup(true);
		removeFromParentAndCleanup(true);
		return;
	}
	else if(m_pCancelButton->boundingBox().containsPoint(touchedPoint))
	{
		if(m_pSubRange!=NULL)
		{
			m_pSubRange->removeFromParentAndCleanup(true);
		}
		if(BattleMenu::instance()->Usable())
		{
			m_pInvoker->ExecuteCommand(COMM_BACK);
		}
		m_pInvoker->HangTouchPath();
		removeFromParentAndCleanup(true);
		return;
	}
	else if(m_bIsCommand == true&&m_pReset->boundingBox().containsPoint(touchedPoint))
	{
		m_pSubRange->setPosition(ccp(m_tSubPoint.x,m_tSubPoint.y));
		return;
	}
	else if(m_pSubRange!=NULL)
	{	
		touchedPoint = convertToWorldSpace(touchedPoint);
		CCNode* pStage = getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
		touchedPoint = pStage->convertToNodeSpace(touchedPoint);
		int x = (int)touchedPoint.x/SQUARE_LENTH;
		int y = (int)touchedPoint.y/SQUARE_LENTH;
		x = (x+0.5f)*SQUARE_LENTH;
		y = (y+0.5f)*SQUARE_LENTH;
		int nRangeX = x;
		int nRangeY = y;
		ConvertPositionToRange(m_tSubPoint.x,m_tSubPoint.y,&nRangeX,&nRangeY);
		if((abs(nRangeX)+abs(nRangeY))<=m_pInvoker->GetSkillRange())
		{
			m_pSubRange->setPositionX(x);
			m_pSubRange->setPositionY(y);
		}	
		return;
	}
}

void ShowRangeMenu::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);  //注册并设置当前面板触摸的优先级<BattleMap	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}
void ShowRangeMenu::RefreshBackGround()
{
	CCScene *pScene = (CCScene*)getParent();
	if(pScene ==NULL)
	{
		return;
	}
	CCLayer* pLayer = (CCLayer*)pScene->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
	if(pLayer == NULL)
	{
		return;
	}
	CCTMXTiledMap* pMap = (CCTMXTiledMap*)pLayer->getChildByTag(BATTLEMAP_TMX_TAG);
	if(pMap == NULL)
	{
		return;
	}
	float midX = pMap->getContentSize().width/2;
	float midY = pMap->getContentSize().height/2;
	if(pLayer->getPositionX()<-midX)
	{
		//视角在地图右边，将菜单栏动态左移，以下同理
		m_pBackGround->setPositionX(GetSpriteTextureLength(m_pBackGround)/2);
	}
	else
	{
		m_pBackGround->setPositionX(VisibleRect::right().x-GetSpriteTextureLength(m_pBackGround)/2);
	}
	if(pLayer->getPositionY()<-midY)
	{
		m_pBackGround->setPositionY(GetSpriteTextureHeight(m_pBackGround)/2);
	}
	else
	{
		m_pBackGround->setPositionY(VisibleRect::top().y-GetSpriteTextureHeight(m_pBackGround)/2);
	}


}
void ShowRangeMenu::RefreshButton()
{
	if(m_bIsCommand==true)
	{
		m_pOKButton->setPosition(ccp(m_pBackGround->getPositionX()-40.8f,m_pBackGround->getPositionY()));
		m_pReset->setPosition(ccp(m_pBackGround->getPositionX(),m_pBackGround->getPositionY()));
	}
	m_pCancelButton->setPosition(ccp(m_pBackGround->getPositionX()+40.8f,m_pBackGround->getPositionY()));


}
void ShowRangeMenu::Refresh()
{
	RefreshBackGround();
	RefreshButton();
}



bool DialogMenu::ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
	return true;
}

void DialogMenu::ccTouchesMoved( CCTouch *pTouch,CCEvent *pEvent )
{
	CCPoint touchedPoint = pTouch->getPreviousLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
}


void DialogMenu::ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent)
{
	CCPoint touchedPoint = pTouch->getLocation();
	touchedPoint = convertToNodeSpace(touchedPoint);
	if (m_pMenu!=NULL&&m_pMenu->boundingBox().containsPoint(touchedPoint))
	{
		EventHandler::instance()->OnDialogEnd();
	}
	else if(m_bPause)
	{
		if (m_bPageDown)
		{
			m_bPageDown =false;
		}
		else
		{
			//点击继续之后更新一次头像
			RefreshImage();
		}
		m_bPause = false;
	}
	else if(m_bEnd)
	{
	/*	if(m_nMode == DIALOG_MODE_STORY)
		{
			((StoryScene*)getParent()->getChildByTag(LAYER_STORY_LAYER_ID))->GoNext();
		}*/
		EventHandler::instance()->OnDialogEnd();
	}
}
void DialogMenu ::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -254, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}


DialogMenu::DialogMenu()
{
	m_nReadStartCount = 0;
	m_nReadPassCount = 0;
	m_bPause = false;
	m_bEnd = false;
	m_bPageDown = false;
	//初始化对话框精灵
	char chStr[50] = {0};
	sprintf(chStr,"%s.png",DIALOG_FILE_FULLPATH);
	m_pBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chStr));
	sprintf(chStr,"%s.png",DlALOG_DIALOG_FRAME_NAME);
	CCSprite*  pDialog = CCSprite::createWithSpriteFrameName(chStr);
	m_pBatchNode->addChild(pDialog,-1);
	pDialog->setAnchorPoint(ccp(0,0));
	this->addChild(m_pBatchNode);
	//初始化人物头像精灵
	sprintf(chStr,"%s1l.png",DIALOG_FACE_FAME_NAME);
	m_pLeft =CCSprite::createWithSpriteFrameName(chStr);
	sprintf(chStr,"%s2r.png",DIALOG_FACE_FAME_NAME);
	m_pRight = CCSprite::createWithSpriteFrameName(chStr);
	//m_pRight->setFlipX(true);
	float x = GetSpriteTextureLength(pDialog);
	float y = GetSpriteTextureHeight(pDialog)-20;
	m_pLeft->setAnchorPoint(ccp(0,0));
	m_pLeft->setPosition(ccp(0,y));
	m_pRight->setAnchorPoint(ccp(1,0));
	m_pRight->setPosition(ccp(x,y));
	m_pLeft->setVisible(false);
	m_pRight->setVisible(false);
	m_pBatchNode->addChild(m_pLeft,MAX_LAYER_ZORDER);
	m_pBatchNode->addChild(m_pRight,MAX_LAYER_ZORDER);
	//初始化文字标
	//m_pDialogContent = CCLabelBMFont::create("","Heiti9.fnt");
	//m_pDialogContent = CCLabelAtlas::create("","west_england-64.fnt"); 
	m_pDialogContent = CCLabelTTF::create("","TimesNewRomanPSMT",24,CCSizeMake (DIALOG_END_X-DIALOG_START_X, 20*4), kCCTextAlignmentLeft);
	addChild(m_pDialogContent,MAX_LAYER_ZORDER);
	m_pDialogContent->setAnchorPoint(ccp(0,1));
	m_pDialogContent->setPosition(ccp(DIALOG_START_X,DIALOG_START_Y));
	//跳过按钮
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pMenu = CCSprite::create("skip.png");
	m_pMenu->setPosition(ccp(origin.x + visibleSize.width - m_pMenu->getContentSize().width/2 ,
		origin.y +visibleSize.height - m_pMenu->getContentSize().height/2));
	addChild(m_pMenu,MAX_LAYER_ZORDER+1);
	//读取文本
	const char* chFileName = EventHandler::instance()->GetDialogFileName();
	sprintf(chStr,"%s%s.txt",DIALOG_WORD_FILE_FULLPATH,chFileName);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(chStr, "rb", &bufferSize);
	if (pBuffer==NULL)
	{
		sprintf(m_chWordBuffer,"1-l:DialogFile Missing!");
		return;
	} 
	m_chWordBuffer[0] ='\0';
	sprintf(m_chWordBuffer,"%s%s",(char*)pBuffer,'\0');
	m_nReadPassCount+=3;//去掉utf8文件头EFBBBF
	delete(pBuffer);
	
}
DialogMenu::~DialogMenu()
{
	
}

void DialogMenu::ShiftDialog()
{
	
}

void DialogMenu::ReadUpdate( float fTime )
{
	if(!m_bPause)
	{
		char chWords[DIALOG_STR_SHOW_CSTYLE_CAP];
		memset(chWords,0,DIALOG_STR_SHOW_CSTYLE_CAP*sizeof(char));
		if(m_nReadStartCount!=0)
		{
			memcpy((void*)chWords,(void*)(m_chWordBuffer+m_nReadPassCount),m_nReadStartCount);
		}
		for(int i =0;i<3;i++)
		{
			m_nReadStartCount++;
			
			//检查命令符
			if (m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]=='\n')
			{
				chWords[m_nReadStartCount] = '\0';
				Pause();
				break;
			}
			else if (m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]==0||m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]=='#')
			{	
				chWords[m_nReadStartCount] = '\0';
				m_bEnd = true;
				unscheduleAllSelectors();
				return;
			}
			else
			{
				chWords[m_nReadStartCount-1] = m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1];
				chWords[m_nReadStartCount] = '\0';
				if(m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]==','||m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]=='.'||m_chWordBuffer[m_nReadPassCount+m_nReadStartCount-1]==':')
				{
					break;
				}
				//超过容量，换行
				if (m_nReadStartCount == DIALOG_STR_SHOW_CSTYLE_CAP-1)
				{
					m_bPageDown = true;
					Pause();
					break;
				}
			}
		}
		m_pDialogContent->setString(chWords);
	}	
}

void DialogMenu::onEnter()
{
	CCLayer::onEnter();
	
	RefreshImage();//第一次更新头像
	schedule(schedule_selector(DialogMenu::ReadUpdate),DIALOG_WORD_DUR,kCCRepeatForever,0);
	setTouchEnabled(true);
}

void DialogMenu::Pause()
{
	setTouchEnabled(false);
	m_bPause =true;
	m_nReadPassCount +=m_nReadStartCount;
	m_nReadStartCount = 0;
	setTouchEnabled(true);
}

void DialogMenu::RefreshImage()
{
	int nID = 0;
	char chID[50] = {0};	
	char cKey = m_chWordBuffer[m_nReadPassCount];
	while(cKey=='<')
	{
		int nCount = 0;
		cKey = m_chWordBuffer[m_nReadPassCount+1];
		while(cKey!='-'&&nCount<50)
		{
			chID[nCount] = cKey;
			nCount++;
			cKey = m_chWordBuffer[m_nReadPassCount+nCount+1];
		}
		cKey = m_chWordBuffer[m_nReadPassCount+nCount+2];
		char chTward = cKey;
		chTward = tolower(chTward);
		sscanf(chID,"%d",&nID);
		char chImage[50];
		sprintf(chImage,"%s%d%c.png",DIALOG_FACE_FAME_NAME,nID,chTward);
		CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCSpriteFrame* pCharacterFaceFrame = pCache->spriteFrameByName(chImage);
		if (NULL==pCharacterFaceFrame)
		{
			return;
		}
		BattleMap* pMap = (BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
		if (pMap!=NULL)
		{
			pMap->SearchCharacterAndViewLock(nID);
		}
		m_nReadPassCount+= nCount+4;//'<'数字个数+'-'+单个字符+'>'
		switch(cKey)
		{
		case 'r':
			m_pRight->setDisplayFrame(pCharacterFaceFrame);
			m_pRight->setVisible(true);	
			break;
		case 'R':
			m_pLeft->setVisible(false);
			m_pRight->setDisplayFrame(pCharacterFaceFrame);
			m_pRight->setVisible(true);	
			break;
		case 'L':
			m_pRight->setVisible(false);
			m_pLeft->setDisplayFrame(pCharacterFaceFrame);
			m_pLeft->setVisible(true);	
			break;
		case 'l':
		default:
			m_pLeft->setDisplayFrame(pCharacterFaceFrame);
			m_pLeft->setVisible(true);	
			break;
		}
		//go next loop
		cKey = m_chWordBuffer[m_nReadPassCount];
	}
}

void DialogMenu::menuSkipCallback( CCObject* pSender )
{
	EventHandler::instance()->OnDialogEnd();
}

void MenuHide(CCLayer* menu)
{
	menu->setVisible(false);
	menu->setTouchEnabled(false);
	menu->setVertexZ(-1);
}
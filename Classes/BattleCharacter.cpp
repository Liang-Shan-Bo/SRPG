#include "BattleCharacter.h"
#include "Singleton.h"
#include "GameUI.h"
#include "AnimationManager.h"
#include "DataLoader.h"
#include "BattleAI.h"
#include "BattleMapScene.h"
#include "EventHandler.h"
#include "PlayerDataManager.h"

#define LOAD_XML_CHARACTER_VALUE(_chValue_,_nValue_) \
	pValue = pCharacter->FirstChildElement(_chValue_);\
	if (pValue==NULL)\
{\
	return false;\
}\
else\
{\
	TIXML_SSCANF(pValue->GetText(),"%d",&_nValue_);\
}




BattleCharacter* BattleCharacter::m_pAttacker = NULL;
BattleCharacter* BattleCharacter::m_pDefender = NULL;
BattleMap* BattleCharacter::m_pMap = NULL;
//初始化自身参数
BattleCharacter::BattleCharacter()
{
	m_pStatus = NULL;
	m_nLevel =1;
	m_nCharID = 0;
	m_nStr = 0;
	m_nDef = 0;
	m_nLuk = 0;
	m_nSpd = 0;
	m_nHP = 0;
	m_nMag = 0;
	m_nMove = 0;
	m_nStepTargetX = 0;
	m_nStepTargetY = 0;
	m_nStartX = 0;
	m_nStartY = 0;
	m_nCommandX = 0;
	m_nCommandY = 0;
	m_nExp = 0;
	m_nExpGot = 0;
	m_nHPoffset = 0;
	m_nBaseExp = 10;
	m_nActionCount = 1;
	m_nMaxAtkRange = 1;
	m_nMinAtkRange = 1;
	m_bMoved = false;
	m_pMapSprite = NULL;
	m_pMapShadow = NULL;
	m_pRange = NULL;
	m_nCommandFlag = COMM_NONE;
	m_nUnionFlag = -1;
	m_bKeyCharacter = false;
	m_bTargetCharacter = false;
}
//释放掉retain过的容器
BattleCharacter::~BattleCharacter()
{
	m_pMapActions->release();
	m_pStatus->release();
}


bool BattleCharacter::IsMoved()
{
	return m_bMoved;
}


bool BattleCharacter::IsZOC(BattleCharacter* pAnotherChar)
{
	return (!InSameRace(pAnotherChar));
}



void BattleCharacter::RefreshHPBar()
{
	float percent = (m_nHP*100/m_nMaxHP);
	m_pHPBar->setPercentage(percent);
	m_pHPBar->setPosition(ccp(m_pMapSprite->getPositionX()-SQUARE_LENTH/2,m_pMapSprite->getPositionY()-SQUARE_LENTH/2));
	m_pHPBarOff->setPosition(ccp(m_pHPBar->getPositionX(),m_pHPBar->getPositionY()));
}



void BattleCharacter::BuildCharacter(CCDictionary* dic)
{	
	int nID = ((CCString*)dic->objectForKey("ID"))->intValue();
	int x = ((CCString*)dic->objectForKey("x"))->intValue()+SQUARE_LENTH/2;
	int y = ((CCString*)dic->objectForKey("y"))->intValue()+SQUARE_LENTH/2;
	int nForceFlag = ((CCString*)dic->objectForKey("FORCE"))->intValue();
	//初始化角色属性
	m_nCharID = nID;
	CharacterDataLoader* pData = CharacterDataLoader::instance();
	pData->GetCharacterValue(DATA_CHARACTER_STR,m_nCharID,&m_nStr);
	pData->GetCharacterValue(DATA_CHARACTER_DEF,m_nCharID,&m_nDef);
	pData->GetCharacterValue(DATA_CHARACTER_SPD,m_nCharID,&m_nSpd);
	pData->GetCharacterValue(DATA_CHARACTER_HP,m_nCharID,&m_nMaxHP);
	pData->GetCharacterValue(DATA_CHARACTER_SP,m_nCharID,&m_nMaxSP);
	m_nSP = m_nMaxSP;
	m_nHP = m_nMaxHP;
	pData->GetCharacterValue(DATA_CHARACTER_MAG,m_nCharID,&m_nMag);
	pData->GetCharacterValue(DATA_CHARACTER_DEX,m_nCharID,&m_nDex);
	pData->GetCharacterValue(DATA_CHARACTER_LUK,m_nCharID,&m_nLuk);
	pData->GetCharacterValue(DATA_CHARACTER_MOVE,m_nCharID,&m_nMove);
	pData->GetCharacterValue(DATA_CHARACTER_MINRANGE,m_nCharID,&m_nMinAtkRange);
	pData->GetCharacterValue(DATA_CHARACTER_MAXRANGE,m_nCharID,&m_nMaxAtkRange);
	m_nForceFlag = nForceFlag;
	CCString* pLevel = (CCString*)dic->objectForKey("Level");
	if (pLevel!=NULL)
	{
		int nLevel  = pLevel->intValue();
		LevelUp(nLevel-1);
		RecoverSPToMax();
		RecoverHPToMax();
	}	
	//初始化角色技能、能力
	pData->GetLevelSkills(m_nCharID,m_nLevel,m_vSkills);
	pData->GetLevelAbilities(m_nCharID,m_nLevel,m_vAbilities);
	//初始化角色状态
	InitSpritesAndStatus(x,y);
}

bool BattleCharacter::LoadCharacterFromData(XMLElement* pCharacter)
{
	if(pCharacter==NULL)
	{
		return false;
	}
	XMLError nError = pCharacter->QueryIntAttribute(DATA_ATTRIBUTE_ID,&m_nCharID);
	if (nError!=XML_NO_ERROR)
	{
		return false;
	}
	XMLElement* pValue = NULL;
	//初始化角色属性
	LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MAXHP,m_nMaxHP)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_HP,m_nHP)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MAXSP,m_nMaxSP)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_SP,m_nSP)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_DEX,m_nDex)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_STR,m_nStr)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_DEF,m_nDef)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_SPD,m_nSpd)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_LUK,m_nLuk)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MAG,m_nMag)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_LEVEL,m_nLevel)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_LEVEL,m_nExp)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_FORCE,m_nForceFlag)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MOVE,m_nMove)
		//初始化角色攻击范围
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MAXRANGE,m_nMaxAtkRange)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MINRANGE,m_nMinAtkRange)
		//获取角色位置
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_X,m_nStartX)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_Y,m_nStartY)
		//初始化角色技能、能力
		XMLElement* pSkills = pCharacter->FirstChildElement(DATA_CHARACTER_SKILLS);
	if (pSkills==NULL)
	{
		return false;
	}
	if(!pSkills->NoChildren())
	{
		XMLElement* pSkill = pSkills->FirstChildElement(DATA_CHARACTER_SKILL);
		if (pSkill==NULL)
		{
			return false;
		}
		for(;pSkill!=NULL;pSkill=pSkill->NextSiblingElement(DATA_CHARACTER_SKILL))
		{
			int nID = 0;
			pSkill->QueryIntAttribute(DATA_ATTRIBUTE_ID,&nID);
			m_vSkills.push_back(nID);
		}
	}
	CharacterDataLoader::instance()->GetLevelAbilities(m_nCharID,m_nLevel,m_vAbilities);
	return true;
}

void BattleCharacter::RefreshMapZ()
{
	m_pMapSprites->getParent()->reorderChild(m_pMapSprites,MAX_LAYER_ZORDER-GetMapPositionY());
}

void BattleCharacter::RefreshShadow()
{
	m_pMapShadow->setPosition(ccp(m_pMapSprite->getPositionX()-2,m_pMapSprite->getPositionY()-SQUARE_LENTH/2/*GetSpriteTextureHeight(m_pMapSprite)/2+10*/));
}

void BattleCharacter::ShadowRefresh()
{
	char frameName[50];
	CCArray* pFrames  = CCArray::createWithCapacity(2);
	for(unsigned int index = 1;index<=2;index++)
	{
		sprintf(frameName,"%s%d%s%d.png",MAPCHAR_FRAME_NAME,m_nCharID,MAP_SHADOW_FRAME_NAME,index);
		pFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(pFrames,_MAPSPRITE_FLESH_RATE_*2.8f);
	m_pMapShadow->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
}
void BattleCharacter::ResumeActionCount()
{
	m_nActionCount = 1;
	m_nCommandFlag = COMM_NONE;
}

void BattleCharacter::SetMapPosition(CCPoint pos)
{
	m_pMapSprite->setPosition(pos);
}
/*未实现装备系统暂缺*/

void BattleCharacter::ConfirmSkillRange()
{

}
void BattleCharacter::ConfirmWeapon()
{

}
void BattleCharacter::ConfirmAtkRange()
{
	ConfirmWeapon();
}




void BattleCharacter::OnOperate()
{
	m_pStatus->OnOperate();
}
bool BattleCharacter::IsControlable()
{
	return m_pStatus->IsControlable();
}

void BattleCharacter::Resume()
{
	m_pStatus->Resume();
}

/*将自身移动按方格来划分ACTION*/
void BattleCharacter::MoveStep(twards_enum twards)
{

	int offsetY = 0;
	int offsetX = 0;
	int actFlag;
	switch(twards)
	{
	case up_enum:
		m_nStepTargetY +=SQUARE_LENTH;
		offsetY +=SQUARE_LENTH; 
		actFlag = ACTION_MAPSTEPUP;
		break;
	case down_enum:
		m_nStepTargetY -=SQUARE_LENTH;
		offsetY -=SQUARE_LENTH; 
		actFlag = ACTION_MAPSTEPDOWN;
		break;
	case right_enum:
		m_nStepTargetX +=SQUARE_LENTH;
		offsetX +=SQUARE_LENTH; 
		actFlag = ACTION_MAPSTEPRIGHT;
		break;
	case left_enum:
		m_nStepTargetX -=SQUARE_LENTH;
		offsetX -=SQUARE_LENTH; 
		actFlag = ACTION_MAPSTEPLEFT;
		break;
	default:
		break;
	}
	//移动依据方向创建地图角色动画
	CCAnimation* animation = AnimationManager::instance()->CreateMapActionAnimation(m_nCharID,actFlag);
	float dur = animation->getDuration();
	CCActionInterval* MoveTo = CCMoveTo::create(dur,ccp(m_nStepTargetX ,m_nStepTargetY));
	//将移动以及角色动画使用CCSpawn混合，装载到移动动作队列中
	m_pMapActions->addObject( CCSpawn::create( CCAnimate::create(animation),MoveTo,NULL) );
	m_pMapShadowFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));
	m_pHPBarOffFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));
	m_pHPBarFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));


}
//显示显示范围的基点调整至移动之前
void BattleCharacter::FomatBeforeMovePos()
{
	m_nShowRangeX = m_nStartX;
	m_nShowRangeY = m_nStartY;
}
//将显示范围的基点调整至移动之后
void BattleCharacter::FomatMovedPos()
{
	m_nShowRangeX = GetMapPositionX();
	m_nShowRangeY = GetMapPositionY();
}


//执行地图角色精灵移动动作，在传入的回调函数中执行之后的任务
void BattleCharacter::Move(SEL_CallFuncND callFuncND)
{
	//锁定视角

	m_pMap->LockCharacterView(this);
	//停止之前的动画
	m_pMapSprite->stopAllActions();	
	m_pMapShadow->stopAllActions();
	m_pHPBar->stopAllActions();
	m_pHPBarOff->stopAllActions();
	m_pMapActions->removeAllObjects();
	m_pMapShadowFollowActions->removeAllObjects();
	m_pHPBarOffFollowActions->removeAllObjects();
	m_pHPBarFollowActions->removeAllObjects();

	InitStep();
	while(!m_tPaths.empty())
	{
		twards_enum tward = m_tPaths.back();
		m_tPaths.pop_back();
		MoveStep(tward);
	}
	if(callFuncND!=NULL)
	{
		m_pMapActions->addObject(CCCallFuncND::create(m_pStatus,callFuncND,(void*)m_pStatus));
	}
	CCAction* pActions = CCSequence::create(m_pMapActions);
	m_pMapSprite->runAction(pActions);
	if(m_pHPBarFollowActions->count()!=0)
	{
		m_pHPBar->runAction(CCSequence::create(m_pHPBarFollowActions));
		m_pHPBarOff->runAction( CCSequence::create(m_pHPBarOffFollowActions));
		m_pMapShadow->runAction( CCSequence::create(m_pMapShadowFollowActions));
	}
	m_nCommandFlag = COMM_NONE;
	//自身已经移动过了
	m_bMoved = true;
}
/************************************************************************/
/* 锁住自身视角，以及解锁自身视角                                                                     */
/************************************************************************/
void BattleCharacter::FocusView()
{

	m_pMap->ViewLock(this);
}
void BattleCharacter::UnlockView()
{

	m_pMap->UnLockCharacterView();
}


void BattleCharacter::MapShowHPOffset(int dam)
{

	CCSpriteBatchNode* pNumBatchNode = (CCSpriteBatchNode*)m_pMap->getChildByTag(LAYER_MAPNUMS_ID);
	CCArray* numSprites = NULL;
	if (dam<0)
	{
		numSprites = AnimationManager::instance()->CreateSizeNums(-dam,NUMS_SMALL_RED);
	}
	else 
	{
		numSprites = AnimationManager::instance()->CreateSizeNums(dam,NUMS_SMALL_BLACK);
	}
	unsigned int count = numSprites->count();
	//计算当前前数字个数，决定显示位置的偏移量。当个数为奇数时，不做办个数字的偏移。当数字为偶数时，需要向右偏移半个数字精灵的图片宽度。
	float distance = NUMS_SMALL_DISTANCE;
	float half = count%2==0?0:distance/2;
	for(unsigned int index = 0;index<count;index++)
	{
		CCSprite* numSprite = (CCSprite*)numSprites->objectAtIndex(index);
		pNumBatchNode->addChild(numSprite,MAX_LAYER_ZORDER);
		numSprite->setPosition(ccp(GetMapPositionX()+half+(count/2-index)*distance,GetMapPositionY()));
		numSprite->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(NUMS_MAPFALL_DUR,ccp(0,SQUARE_LENTH/2)),CCCallFuncND::create(this,callfuncND_selector(AnimationManager::RemoveAniamationSprite),(void*)numSprite)));
	}

}
//与ShowMapDam实现方案相同，只是将字体换为黑色---暂定
void BattleCharacter::RefreshPlayer()
{
	if (m_nForceFlag!=FORCE_PLAYER)
	{
		return;
	}

	CCArray* numSprites = AnimationManager::instance()->CreateSizeNums(m_nExpGot,NUMS_SMALL_BLACK);
	CCSpriteBatchNode* pNumBatchNode = (CCSpriteBatchNode*)m_pMap->getChildByTag(LAYER_MAPNUMS_ID);
	unsigned int count = numSprites->count();
	float distance = NUMS_SMALL_DISTANCE;
	float half = count%2==0?0:distance/2;
	for(unsigned int index = 0;index<count;index++)
	{
		CCSprite* numSprite = (CCSprite*)numSprites->objectAtIndex(index);
		pNumBatchNode->addChild(numSprite,MAX_LAYER_ZORDER);
		numSprite->setPosition(ccp(GetMapPositionX()+half+(count/2-index)*distance,GetMapPositionY()+SQUARE_LENTH/2));
		numSprite->runAction(CCSequence::create(CCMoveBy::create(NUMS_MAPFALL_DUR,ccp(0,-SQUARE_LENTH/2)),CCCallFuncND::create(this,callfuncND_selector(AnimationManager::RemoveAniamationSprite),(void*)numSprite),
			CCCallFuncND::create(this,callfuncND_selector(BattleCharacter::ExpForLevelCallBack),NULL),NULL));
	}
}
//返回攻击范围
int BattleCharacter::GetAtkRange()
{
	ConfirmWeapon();
	return 2;
}

//获取当前正在使用的技能施法范围
int BattleCharacter::GetSkillRange()
{
	int nRange = 0;
	SkillDataLoader::instance()->GetSkillValue(DATA_SKILL_RANGE,m_nOnUseID,&nRange);
	return nRange;
}


bool BattleCharacter::IsShowRange()
{
	return (m_pRange!=NULL);
}
bool BattleCharacter::MapContains(CCPoint touchPoint)
{
	return (m_pMapSprite->boundingBox().containsPoint(touchPoint));
}

void BattleCharacter::Action()
{
	m_pStatus->Action();
}
/************************************************************************/
/* 当玩家移动角色之后，点击返回指令会被调用。将状态重置为未移动时，并刷新影子动画、血条、影子和Z轴优先度。                                                                     */
/************************************************************************/
void BattleCharacter::BackToStart()
{
	BattleMenu* pMenu = BattleMenu::instance();
	pMenu->HangMenu();
	pMenu->ResumeMoveCommand();
	m_bMoved = false;
	m_pMapSprite->setPosition(ccp(m_nStartX,m_nStartY));
	RefreshShadow();
	RefreshHPBar();
	RefreshMapZ();
	FocusView();
	pMenu->ShowMenu();

}
//移动之前会被调用，用来初始化坐标记录数据。
void BattleCharacter::InitStep()
{
	m_nStepTargetX =  GetMapPositionX();
	m_nStepTargetY =  GetMapPositionY();
	m_nStartX = GetMapPositionX();
	m_nStartY = GetMapPositionY();
}
void BattleCharacter::SetCommandFlag(int nFlag)
{
	m_nCommandFlag = nFlag;
}
//改变自身状态。此函数会释放掉原来的状态。
void BattleCharacter::SetStatus(CharacterStatus* pStatus)
{
	if(m_pStatus!=NULL)
	{
		m_pStatus->release();
	}
	m_pStatus = pStatus;
	m_pStatus->retain();
}

void BattleCharacter::CheckMoveRange()
{
	m_pRange = new DrhombusRangeObject(m_nMove);

	m_pMap->SetCostFromMap(this);
	//将移动范围初始化为全部不可移动。
	m_pRange->RefreshAvailableRange(false);
	//通过A*算法检测移动范围
	m_pRange->CheckMoveable();
}
void BattleCharacter::ShowMoveRange()
{
	BattleMenu::instance()->HangMenu();
	FomatBeforeMovePos();
	CheckMoveRange();

	m_pMap->AcceptShowRangeObject(this);
}
void BattleCharacter::ShowAtkRange()
{
	FomatMovedPos();	
	BattleMenu::instance()->HangMenu();

	m_pRange = new DrhombusRangeObject(m_nMaxAtkRange);
	//将攻击范围初始化
	m_pRange->RefreshAvailableRange(false);
	m_pRange->RefreshRangeTo(true,m_nMinAtkRange,m_nMaxAtkRange);
	m_pMap->AcceptShowRangeObject(this);



}
void BattleCharacter::ShowSkillRange()
{
	FomatMovedPos();
	SkillDataLoader* pSkillData = SkillDataLoader::instance();
	int nRange = 0;
	int nSubRange = 0;
	//通过文档数据获取技能范围
	pSkillData->GetSkillValue(DATA_SKILL_RANGE,m_nOnUseID,&nRange);
	m_pRange = new DrhombusRangeObject(nRange);
	/*m_pRange = new DrhombusRangeObject(m_pOnUseObject->GetRange());*/
	BattleMenu::instance()->HangMenu();

	m_pMap->AcceptShowRangeObject(this);
	m_pRange->RefreshAvailableRange(true);
	pSkillData->GetSkillValue(DATA_SKILL_SUBRANGE,m_nOnUseID,&nSubRange);
	m_pMap->getParent()->addChild(new ShowRangeMenu(this,nSubRange,true),MAX_LAYER_ZORDER,LAYER_SHOWRANGE_MENU_ID);

}
int BattleCharacter::GetAtk()
{
	return m_nStr;
}
bool BattleCharacter::InSameRace(BattleCharacter* pCharacter)
{
	bool bSame = (m_nForceFlag == pCharacter->GetForceFlag());
	return bSame;
}


bool BattleCharacter::IsAbleToAtk()
{
	return true;
}

bool BattleCharacter::IsAbleToDef()
{
	return true;
}

bool BattleCharacter::IsAbleToItem()
{
	return true;
}

bool BattleCharacter::IsAbleToSkill()
{
	return true;
}

bool BattleCharacter::IsAbleToStay()
{
	return true;
}

bool BattleCharacter::IsAbleToMove()
{
	return (!m_bMoved);
}

CCPoint BattleCharacter::GetMapPosition()
{
	return ccp(m_pMapSprite->getPositionX(),m_pMapSprite->getPositionY());
}

int BattleCharacter::GetShowRangePositionX()
{
	return m_nShowRangeX;
}
int BattleCharacter::GetShowRangePositionY()
{
	return m_nShowRangeY;
}

int BattleCharacter::GetMapPositionX()
{
	return m_pMapSprite->getPositionX();
}

int BattleCharacter::GetMapPositionY()
{
	return m_pMapSprite->getPositionY();
}

CCArray* BattleCharacter::GetRangeArray()
{
	return m_pRange->GetOffsets();
}



void BattleCharacter::HangTouchPath()
{

	//m_nCommandFlag = COMM_NONE;
	m_pMap->KickShowRangeObject(this);
	delete m_pRange;
	m_pRange = NULL;
}


void BattleCharacter::CaculateMovePath(PathPoint* pRangePoint)
{
	m_pRange->SearchTarget(pRangePoint);
	CreateMovePath(pRangePoint);
}
/*返回值为true代表该指令生效*/
bool BattleCharacter::PerformFromTouch(CCPoint touchPoint)
{

	m_nCommandX = touchPoint.x;
	m_nCommandY = touchPoint.y;
	if(m_pMap->GetTurnFlag() == m_nForceFlag)
	{
		//使用A*算法估算产生的移动路径
		if(m_nCommandFlag == COMM_MOVE)
		{
			int offsetX = (int)touchPoint.x;
			int offsetY = (int)touchPoint.y;
			ConvertTouchPointToRange(GetMapPositionX(),GetMapPositionY(),&offsetX,&offsetY);
			if((abs(offsetX)+abs(offsetY))>m_nMove)
			{
				return false;//超出范围
			}

			for(unsigned int index = 0;index<m_pRange->GetOffsets()->count();index++)
			{
				PathPoint* pRangePoint = (PathPoint*)m_pRange->GetOffsets()->objectAtIndex(index);
				int pathX = pRangePoint->m_nX;
				int pathY = pRangePoint->m_nY;
				if(offsetX == pathX&&offsetY==pathY&&pRangePoint->m_bAvailable)
				{
					//挂起范围菜单
					ShowRangeMenu* showRange = (ShowRangeMenu*)m_pMap->getParent()->getChildByTag(LAYER_SHOWRANGE_MENU_ID);
					if(showRange!=NULL)
					{
						showRange->removeFromParentAndCleanup(true);
					}
					CaculateMovePath(pRangePoint);
					Action();
					m_nCommandFlag = COMM_NONE;
					return true;
				}

			}
		}
		//执行攻击指令，当目标不存在时，保持状态。
		if(m_nCommandFlag == COMM_ATK)
		{
			int pathX = touchPoint.x;
			int pathY = touchPoint.y;
			ConvertTouchPointToRange(GetMapPositionX(),GetMapPositionY(),&pathX,&pathY);
			int nIndex = DrhombusRangeObject::GetRangeOffsetsIndex(pathX,pathY,GetMaxAtkRange());
			if(nIndex!=-1)
			{
				PathPoint* pPoint = (PathPoint*)m_pRange->GetOffsets()->objectAtIndex(nIndex);
				if(!pPoint->m_bAvailable)
				{
					return false;
				}
				BattleCharacter* pDefender = m_pMap->SearchUnitByPosition(m_nForceFlag,touchPoint.x,touchPoint.y,true,false);
				if(pDefender!= NULL)
				{
					HangTouchPath();
					//挂起范围菜单
					ShowRangeMenu* showRange = (ShowRangeMenu*)m_pMap->getParent()->getChildByTag(LAYER_SHOWRANGE_MENU_ID);
					if(showRange!=NULL)
					{
						showRange->removeFromParentAndCleanup(true);
					}
					BattleCharacter::SimulationAttackAction(this,pDefender);
					m_nActionCount--;
					//m_pMap->CheckTurnEnd();
					if(m_nForceFlag==FORCE_PLAYER)
					{
						SetStatus(new StayStatus(this));
					}				
					m_nCommandFlag = COMM_NONE;
					return true;
				}
			}
		}
		//执行范围技能指令
		if(m_nCommandFlag == COMM_SKILL_RANGE)
		{
			SkillDataLoader* pData = SkillDataLoader::instance();
			int nSubRange = 0; 
			pData->GetSkillValue(DATA_SKILL_SUBRANGE,m_nOnUseID,&nSubRange);
			CCArray* pTargets = CCArray::createWithCapacity(2*(nSubRange-1)*(nSubRange));

			CCNode* pSubRange = m_pMap->getChildByTag(LAYER_SUBRANGE_ID);
			/*将菱形范围内的所有角色加入目标队列*/
			for(int i = 0;i<=nSubRange; i++)
			{
				for(int ix = 0;ix<=i;ix++)
				{
					BattleCharacter* pTarget = NULL;
					int x = touchPoint.x;
					int y = touchPoint.y;
					ConvertRangeToPosition(ix,i-ix,&x,&y);
					pTarget = m_pMap->SearchUnitByPosition(GetForceFlag(),x,y,false,false);
					if(pTarget!= NULL)
					{
						pTargets->addObject(pTarget);
					}
					if(ix!=0)
					{
						x = touchPoint.x;
						y = touchPoint.y;
						ConvertRangeToPosition(-ix,i-ix,&x,&y);
						pTarget = m_pMap->SearchUnitByPosition(GetForceFlag(),x,y,false,false);
						if(pTarget!= NULL)
						{
							pTargets->addObject(pTarget);
						}
					}
					if(ix!=i)
					{
						x = touchPoint.x;
						y = touchPoint.y;
						ConvertRangeToPosition(ix,ix-i,&x,&y);
						pTarget = m_pMap->SearchUnitByPosition(GetForceFlag(),x,y,false,false);
						if(pTarget!= NULL)
						{
							pTargets->addObject(pTarget);
						}
					}
					if(ix!=i&&ix!=0)
					{
						x = touchPoint.x;
						y = touchPoint.y;
						ConvertRangeToPosition(-ix,ix-i,&x,&y);
						pTarget = m_pMap->SearchUnitByPosition(GetForceFlag(),x,y,false,false);
						if(pTarget!= NULL)
						{
							pTargets->addObject(pTarget);
						}
					}

				}
			}
			/*执行释放技能函数*/
			bool bCommand = BattleCharacter::SimulationSkill(this,pTargets);
			if(!bCommand)
			{
				/*当技能释放无效时，返回*/
				return false;
			}
			m_nActionCount--;
			//m_pMap->CheckTurnEnd();
			if(m_nForceFlag==FORCE_PLAYER)
			{
				SetStatus(new StayStatus(this));
			}				
			m_nCommandFlag = COMM_NONE;
			return true;
		}
	}
	//自身地图精灵被点击，并且地图没有显示范围，也没有战斗菜单在使用时，调用被操作的函数。
	if(m_pMapSprite->boundingBox().containsPoint(touchPoint)&&(!m_pMap->IsShowRange())&&(!BattleMenu::instance()->Usable()))
	{	
		OnOperate();
		return true;
	}
	return false;
}
/*从当前点获取可移动的朝向*/

void BattleCharacter::CreateMovePath(PathPoint* pTarget)
{
	if(!m_tPaths.empty())
	{
		m_tPaths.clear();
	}
	if(pTarget->m_pParent == NULL)
	{
		return;
	}
	PathPoint* pParentPoint = pTarget;
	for(int move = 0;move<m_nMove;move++)
	{
		m_tPaths.push_back(pParentPoint->m_nTward);
		pParentPoint = pParentPoint->m_pParent;
		if(pParentPoint->m_pParent ==NULL)
		{
			break;
		}

	}

}
void BattleCharacter::ShowBattleMenu()
{
	BattleMenu* pMenu = BattleMenu::instance();
	pMenu->SetInvoker(this);
	pMenu->ShowMenu();
}


void BattleCharacter::RefreshTurnStatus()
{
	Stand();
}

/*通过菜单执行各种指令*/
void BattleCharacter::ExecuteCommand(int commandFlag)
{

	m_pMap->ViewLock(this);
	if(m_nForceFlag == FORCE_PLAYER)
	{

		if(m_nCommandFlag == COMM_NONE)
		{
			switch(commandFlag)
			{
			case COMM_MOVE:
				m_nCommandFlag = commandFlag;
				ShowMoveRange();
				m_pMap->getParent()->addChild(new ShowRangeMenu(this,0,false),MAX_LAYER_ZORDER,LAYER_SHOWRANGE_MENU_ID);
				break;
			case COMM_STAY:
				SetStatus(new StayStatus(this));
				EventHandler::instance()->OnCharacterActionEnd(this);
				//m_pMap->TimingPlayerTurn();
				break;
			case COMM_ATK:
				m_nCommandFlag = commandFlag;
				ShowAtkRange();
				m_pMap->getParent()->addChild(new ShowRangeMenu(this,0,false),MAX_LAYER_ZORDER,LAYER_SHOWRANGE_MENU_ID);
				break;
			case COMM_SKILL:
				m_nCommandFlag = commandFlag;
				BattleMenu::instance()->EnterChildMenu(m_vSkills,m_nCommandFlag);
				break;
			case COMM_Status:
				m_nCommandFlag = COMM_NONE;
				Resume();
				m_pMap->getParent()->addChild(new ShowStatusMenu(this),MAX_LAYER_ZORDER,LAYER_SHOWSTATUS_MENU_ID);
				break;
			case COMM_BACK:
				m_nCommandFlag = COMM_NONE;
				if(m_bMoved ==false)
				{
					Resume();
				}
				else
				{
					BackToStart();
				}

				break;
			default:
				m_nCommandFlag = COMM_NONE;
				Resume();
				break;
			}
		}
		else if(commandFlag==COMM_BACK)
		{
			if(m_nCommandFlag == COMM_SKILL_RANGE)
			{
				HangTouchPath();	
				m_nCommandFlag = COMM_SKILL;
				BattleMenu::instance()->EnterChildMenu(m_vSkills,m_nCommandFlag);

			}
			else if(m_nCommandFlag == COMM_SKILL)
			{	
				m_nCommandFlag = COMM_NONE; 
				BattleMenu::instance()->HangMenu();
				BattleMenu::instance()->EnterTopMenu();

			}
			else if(m_nCommandFlag ==COMM_MOVE||m_nCommandFlag == COMM_ATK)
			{
				m_nCommandFlag = COMM_NONE;
				HangTouchPath();
				BattleMenu::instance()->EnterTopMenu();

			}
			else
			{
				BattleMenu::instance()->Back();
			}

		}
		else if(m_nCommandFlag == COMM_SKILL)
		{
			m_nOnUseID = commandFlag;
			m_nCommandFlag = COMM_SKILL_RANGE;
			ShowSkillRange();	
		}

	}
}
void BattleCharacter::Stand()
{
	m_pMapSprite->stopAllActions();
	CCAnimation* animation  = AnimationManager::instance()->CreateMapActionAnimation(m_nCharID,ACTION_MAPSTAND);
	m_pMapSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
}



void BattleCharacter::Standby()
{
	m_pMapSprite->stopAllActions();
	char frameStr[50]= {0};
	sprintf(frameStr,"%s%d%s%d%s0.png",MAPCHAR_FRAME_NAME,m_nCharID,MAPCHAR_ACTION_FILE_NAME,ACTION_MAPSTAND/MAX_ANIMATION_FRAME,MAPCHAR_FRAME_COUNT_NAME);
	CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameStr); 
	m_pMapSprite->setDisplayFrame(frame);
	m_bMoved = false;
}

void BattleCharacter::ResumeStandby()
{
	m_nActionCount++;
	m_bMoved = false;
	SetStatus(new StandbyStatus(this));
}
void BattleCharacter::Stay()
{
	//动画置为stay
	m_pMapSprite->stopAllActions();
	char frameStr[50]= {0};
	sprintf(frameStr,"%s%d%s%d%s0.png",MAPCHAR_FRAME_NAME,m_nCharID,MAPCHAR_ACTION_FILE_NAME,ACTION_MAPSTAY/MAX_ANIMATION_FRAME,MAPCHAR_FRAME_COUNT_NAME);
	CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameStr); 
	m_pMapSprite->setDisplayFrame(frame);
	//设置点坐标
	InitStep();
}



//战斗计算
void BattleCharacter::SimulationAttackAction(BattleCharacter *pAttacker, cocos2d::CCArray *pDefenders)
{
	for(unsigned int index = 0;index<pDefenders->count();index++)
	{
		SimulationAttackAction(pAttacker,(BattleCharacter*)pDefenders->objectAtIndex(index));
	}
}
void BattleCharacter::SimulationAttackAction(BattleCharacter *pAttacker, BattleCharacter *pDefender)
{

	int atkOffset = GetAttackHPOffset(pAttacker,pDefender);
	int fightbackOffset = 0;
	//记录防御方战斗前的状态
	BattleRecordTable* pDefenderTable = new BattleRecordTable();
	pDefenderTable->m_nHP = pDefender->m_nHP;
	pDefenderTable->m_nHPOffset = atkOffset;
	pDefenderTable->m_nSP = pDefender->GetSP();
	//更新防御方数值
	pDefender->m_nHP -= atkOffset;
	//还击
	bool isFightBack  =false;
	if(pDefender->IsBattlable())
	{
		//当防御方处于生存状态，并且攻击处于其射程内时,反击可能
		int pathX = pAttacker->GetMapPositionX();
		int pathY = pAttacker->GetMapPositionY();
		ConvertTouchPointToRange(pDefender->GetMapPositionX(),pDefender->GetMapPositionY(),&pathX,&pathY);
		int nRange = abs(pathX)+abs(pathY);
		if(nRange<=pDefender->GetMaxAtkRange()&&nRange>=pDefender->GetMinAtkRange())
		{
			isFightBack = true;
			fightbackOffset = GetAttackHPOffset(pDefender,pAttacker);
		}
	}

	/*记录战斗前进攻方角色状态*/
	BattleRecordTable* pAttackerTable = new BattleRecordTable();
	pAttackerTable->m_nHP = pAttacker->m_nHP;
	pAttackerTable->m_nHPOffset =fightbackOffset;
	pAttackerTable->m_nSP = pAttacker->GetSP();
	//更新进攻方数值
	pAttacker->m_nHP -= fightbackOffset;
	/*进入决斗场景前判定当前的决斗模式*/
	int nMode =0;
	if(pAttacker->m_nForceFlag ==FORCE_ENERMY)
	{
		if(isFightBack)
		{
			nMode = DUELMODE_ATK_ENERMY;
		}
		else
		{
			nMode = DUELMODE_ATK_ENERMY_ONLY;
		}
	}
	else
	{
		if(isFightBack)
		{
			nMode = DUELMODE_ATK_PLAYER;
		}
		else
		{
			nMode = DUELMODE_ATK_PLAYER_ONLY;
		}

	}





	//进入决斗场景
	CCScene* scene = (CCScene*)pAttacker->m_pMapSprites->getParent()->getParent();
	DuelLayer *pDuelLayer = new DuelLayer(pAttacker,pDefender,pAttackerTable,pDefenderTable,nMode);
	scene->addChild(pDuelLayer,MAX_LAYER_ZORDER,LAYER_DUEL_LAYER_ID);
	int nCount =0;
	m_pAttacker = pAttacker;
	m_pDefender = pDefender;

	//更新经验值 
	//if(pAttacker->GetForceFlag()==FORCE_PLAYER)
	//{
	//	BattleCharacter::AquireExpFromAttack(pAttacker,pDefender);
	//}
	//else if(pDefender->GetForceFlag()==FORCE_PLAYER)
	//{
	//	BattleCharacter::AquireExpFromAttack(pDefender,pAttacker);
	//}
	////更新地图血条
	//pAttacker->RefreshHPBar();
	//pDefender->RefreshHPBar();
	////更新角色数据
	//pAttacker->RefeshAfterAction();
	//pDefender->RefeshAfterAction();
}
/************************************************************************/
/* 经验值随着敌我等级差递减，敌我等级差在各个区间段的递减幅度
-1~-3  减少40%
-3~-4  减少60%
-4~    减少90%
*/
/************************************************************************/
void BattleCharacter::AquireExpFromAttack(BattleCharacter* pPlayer,BattleCharacter* pOpposite)
{
	if(pPlayer->m_nForceFlag!=FORCE_PLAYER&&pPlayer->IsBattlable())
	{
		return;
	}
	int nPlayerLevel = pPlayer->GetLevel();
	int nOppositeLevel = pOpposite->GetLevel();
	int nLevelOffset = nOppositeLevel -nPlayerLevel;
	if(!pPlayer->InSameRace(pOpposite))
	{
		float factor = nLevelOffset<0?(nLevelOffset<-2?(nLevelOffset<-4?0.1f:0.4f):0.6f):1.0f;
		pPlayer->m_nExpGot += pOpposite->GetBaseExp()*factor;
	}
}
//攻击伤害公式 攻击力-防御力    ---暂定
int BattleCharacter::GetAttackHPOffset(BattleCharacter *pAttacker, BattleCharacter *pDefender)
{
	int offset =  pAttacker->GetAtk() - pDefender->GetDef();
	offset = offset>0?offset:0;
	return offset;
}
/************************************************************************/
/* 通过判断技能的类型、伤害、治疗效果、作用对象等判断技能是否释放成功。当释放成功时将数据更新角色数据                                                                   */
/************************************************************************/
bool BattleCharacter::SimulationSkill(BattleCharacter* pInvoker,CCArray* pTargets)
{	
	CCArray* pEffectTargets =CCArray::createWithCapacity(pTargets->count());
	pEffectTargets->retain();
	bool bCommandSuc = false;
	for(unsigned int index = 0;index<pTargets->count();index++)
	{
		SkillDataLoader* pData = SkillDataLoader::instance();
		int nID = pInvoker->m_nOnUseID;
		int nType = 0;
		pData->GetSkillValue(DATA_SKILL_TYPE,nID,&nType);
		BattleCharacter* pTarget = (BattleCharacter*)pTargets->objectAtIndex(index);
		//判断是否产生伤害
		bool bDamage = (nType == SKILL_TYPE_DAMAGE)||(nType == SKILL_TYPE_DAMAGE_AND_HEALING);
		bool bHealing = (nType == SKILL_TYPE_HEALING)||(nType == SKILL_TYPE_DAMAGE_AND_HEALING);
		if(bDamage&&(!pInvoker->InSameRace(pTarget)))
		{
			int nDamage = BattleCharacter::GetSkillHPDam(pInvoker,pTarget);	
			pTarget->m_nHP += -nDamage;
			pTarget->m_nHPoffset += -nDamage;
			//pTarget->ShowMapDam(nDamage);
			//pTarget->RefreshHPBar();
			bCommandSuc = true;
			AquireExpFromAttack(pInvoker,pTarget);
			if (!pEffectTargets->containsObject(pTarget))
			{
				pEffectTargets->addObject(pTarget);
			}
		}
		else if(bHealing&&pInvoker->InSameRace(pTarget))
		{
			int nHealing = BattleCharacter::GetSkillHPHeal(pInvoker,pTarget);
			pTarget->m_nHP += nHealing;
			pTarget->m_nHPoffset += nHealing;
			bCommandSuc = true;
			AquireExpFromAttack(pInvoker,pTarget);
			if (!pEffectTargets->containsObject(pTarget))
			{
				pEffectTargets->addObject(pTarget);
			}
		}
	}
	if(bCommandSuc)
	{
		CCArray* pAnimations = AnimationManager::instance()->CreateMapSkillAnimations(pInvoker->m_nOnUseID);
		CCAnimation* pMapSkillAnimation = (CCAnimation*)pAnimations->objectAtIndex(0);
		CCSprite* pMapSkillSprite = CCSprite::createWithSpriteFrame(dynamic_cast<CCAnimationFrame*>(pMapSkillAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
		CCSprite* pMapSkillBackgroundSprite = NULL;
		CCAnimation* pMapSkillBackgroundAniation  = NULL;
		if (pAnimations->count()>1)
		{
			pMapSkillBackgroundAniation = (CCAnimation*)pAnimations->lastObject();
			pMapSkillBackgroundSprite = CCSprite::createWithSpriteFrame(dynamic_cast<CCAnimationFrame*>(pMapSkillBackgroundAniation->getFrames()->objectAtIndex(0))->getSpriteFrame());
		}
		if (pMapSkillBackgroundSprite!=NULL)
		{
			pMapSkillBackgroundSprite->setPosition(ccp(pInvoker->m_nCommandX,pInvoker->m_nCommandY));
			CharacterStatus::m_pMap->addChild(pMapSkillBackgroundSprite,MAX_LAYER_ZORDER-1,LAYER_MAPSKILLBACKGROUND_ID);
			pMapSkillBackgroundSprite->runAction(CCRepeatForever::create(CCAnimate::create(pMapSkillBackgroundAniation)));
			pMapSkillSprite->setPosition(ccp(pMapSkillBackgroundSprite->getPositionX(),pMapSkillBackgroundSprite->getPositionY()+GetSpriteTextureHeight(pMapSkillBackgroundSprite)));
		}
		else
		{
			pMapSkillSprite->setPosition(ccp(pInvoker->m_nCommandX,pInvoker->m_nCommandY));
		}
		CharacterStatus::m_pMap->addChild(pMapSkillSprite,MAX_LAYER_ZORDER);
		pMapSkillSprite->runAction(CCSequence::create(CCAnimate::create(pMapSkillAnimation),CCCallFuncND::create(pInvoker,callfuncND_selector(BattleCharacter::MapSkillEffectsCallBack),(void*)pEffectTargets),NULL));


	}
	return bCommandSuc;
}
//技能伤害 = 技能基本伤害 --暂定
int BattleCharacter::GetSkillHPDam(BattleCharacter *pInvoker, BattleCharacter *pTarget)
{
	SkillDataLoader* pData = SkillDataLoader::instance();
	int nID = pInvoker->m_nOnUseID;
	int nDamage = 0;
	pData->GetSkillValue(DATA_SKILL_DAMAGE,nID,&nDamage);
	return nDamage;
}
//技能治疗效果 = 魔力值+基本效果 --暂定
int BattleCharacter::GetSkillHPHeal(BattleCharacter *pInvoker, BattleCharacter *pTarget)
{
	SkillDataLoader* pData = SkillDataLoader::instance();
	int nID = pInvoker->m_nOnUseID;
	int nHealPecent = 0;
	int nHealing = 0;
	pData->GetSkillValue(DATA_SKILL_HEALING,nID,&nHealPecent);
	nHealing = pInvoker->GetMag()+(nHealPecent*pTarget->GetMaxHP())/100;
	return nHealing;
}
//行动结果判定
bool BattleCharacter::IsBattlable ()
{
	return m_nHP>0;
}
bool BattleCharacter::IsEndTurn()
{
	return false;
}

void BattleCharacter::ExpForLevelCallBack( CCNode* pSender,void* pData )
{
	m_nExp += m_nExpGot;
	int nLevelUp = m_nExp/100;
	m_nExp = m_nExp%100;
	//当exp>100时升级
	//清空获取的exp
	m_nExpGot = 0;
	if(nLevelUp>0)
	{
		LevelUp(nLevelUp);
		char chImage[50] = {0};
		sprintf(chImage,"%s.png",LEVELUP_LOGO_NAME);
		CCSprite* pLogo = CCSprite::createWithSpriteFrameName(chImage);
		m_pMapSprites->getParent()->addChild(pLogo,MAX_LAYER_ZORDER);
		pLogo->setPosition(ccp(GetMapPositionX(),GetMapPositionY()));
		pLogo->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(LEVELUP_LOGO_MOVE_DUR,ccp(0,SQUARE_LENTH/2)),CCCallFuncND::create(this,callfuncND_selector(BattleCharacter::RemoveLevelLogoCallBack),(void*)pLogo)));
	}
	//else
	//{
	//	//判断事件
	//	EventHandler::instance()->OnCharacterActionEnd(this);
	//}
}
void BattleCharacter::RemoveLevelLogoCallBack(CCNode* pSender,void* pData)
{
	((CCSprite*)pData)->removeFromParentAndCleanup(true);
	//刷新地图血血条
	RefreshHPBar();
}

void BattleCharacter::LevelUp(int nUp)
{
	//从角色数据文件中获取升级数据,将他们附加到角色上
	m_nLevel += nUp;
	int nAtrUp;//上升的属性
	CharacterDataLoader* pData = CharacterDataLoader::instance();
	pData->GetCharacterValue(DATA_CHARACTER_HP_PLUS,GetCharID(),&nAtrUp);
	m_nMaxHP +=nAtrUp*nUp;
	pData->GetCharacterValue(DATA_CHARACTER_SPD_PLUS,GetCharID(),&nAtrUp);
	m_nSpd += nAtrUp*nUp;
	pData->GetCharacterValue(DATA_CHARACTER_DEX_PLUS,GetCharID(),&nAtrUp);
	m_nDex += nAtrUp*nUp;
	pData->GetCharacterValue(DATA_CHARACTER_LUK_PLUS,GetCharID(),&nAtrUp);
	m_nLuk += nAtrUp*nUp;
	pData->GetCharacterValue(DATA_CHARACTER_STR_PLUS,GetCharID(),&nAtrUp);
	m_nStr += nAtrUp*nUp;
	pData->GetCharacterValue(DATA_CHARACTER_DEF_PLUS,GetCharID(),&nAtrUp);
	m_nDef += nAtrUp*nUp;
	//获取该等级的技能、能力
	pData->GetLevelAbilities(m_nCharID,m_nLevel,m_vAbilities);
	pData->GetLevelSkills(m_nCharID,m_nLevel,m_vSkills);
}

vector<int>& BattleCharacter::GetSkillsIDVector()
{
	return m_vSkills;
}

void BattleCharacter::InitSpritesAndStatus(int x,int y)
{
	//精灵动画
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,m_nCharID);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey(chImageStr);	
	//初始化精灵
	m_pMapSprites = CCSpriteBatchNode::createWithTexture(texture);
	m_pMap->addChild(m_pMapSprites,MAX_LAYER_ZORDER-y);
	char frameStr[50]= {0};
	sprintf(frameStr,"%s%da0f0.png",MAPCHAR_FRAME_NAME,m_nCharID);
	m_pMapSprite = CCSprite::createWithSpriteFrameName(frameStr);
	m_pMapSprites->addChild(m_pMapSprite,y,0);
	m_pMapSprite->setPosition(ccp(x,y));	
	m_pMapActions =CCArray::createWithCapacity(10);
	m_pMapShadowFollowActions = CCArray::createWithCapacity(10);
	m_pHPBarFollowActions = CCArray::createWithCapacity(10);
	m_pHPBarOffFollowActions = CCArray::createWithCapacity(10);
	m_pMapActions->retain();
	m_pMapShadowFollowActions->retain();
	m_pHPBarFollowActions->retain();
	m_pHPBarOffFollowActions->retain();
	//初始化地图角色影子
	sprintf(frameStr,"%s%d%s1.png",MAPCHAR_FRAME_NAME,m_nCharID,MAP_SHADOW_FRAME_NAME);
	m_pMapShadow = CCSprite::createWithSpriteFrameName(frameStr);
	m_pMapSprites->addChild(m_pMapShadow,-MAX_LAYER_ZORDER);
	RefreshShadow();
	//初始化血条
	sprintf(chImageStr,"%s.png",HPBAR_FILE_FULLPATH);
	m_pHPBar = CCProgressTimer::create(CCSprite::create(chImageStr));
	sprintf(chImageStr,"%s.png",HPOFF_FILE_FULLPATH);
	m_pHPBarOff = CCSprite::create(chImageStr);
	m_pMap->addChild(m_pHPBar,MAX_LAYER_ZORDER-1);
	m_pMap->addChild(m_pHPBarOff,MAX_LAYER_ZORDER-2);
	m_pHPBar->setAnchorPoint(ccp(0,0));
	m_pHPBar->setType(kCCProgressTimerTypeBar);
	m_pHPBar->setMidpoint(ccp(0,1));
	m_pHPBar->setBarChangeRate(ccp(1,0));
	m_pHPBarOff->setAnchorPoint(ccp(0,0));
	RefreshHPBar();
	//初始化移动起点
	InitStep();

	//生成状态
	if(m_nForceFlag == FORCE_PLAYER)
	{
		m_pStatus = new StandbyStatus(this);
	}
	else
	{
		m_pStatus = new AIBasedATKStatus(this);
	}

}

void BattleCharacter::LoadSpritesAndStatus()
{
	InitSpritesAndStatus(m_nStartX,m_nStartY);
}

void BattleCharacter::RemoveFromBattle()
{
	EventHandler* pHandler = EventHandler::instance();
	AnimationManager* pManger = AnimationManager::instance();
	CCAnimation* pVanishAnimation =pManger->CreateMapCharacterVanishAnimation();
	CCSprite* pVanish = CCSprite::createWithSpriteFrame(dynamic_cast<CCAnimationFrame*>(pVanishAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
	pVanish->setPosition(ccp(this->GetMapPositionX(),this->GetMapPositionY()));
	//注册事件
	EventHandler::instance()->OnCharacterRemoveFromBattle(this);
	//移除自身地图精灵
	m_pMapSprites->removeFromParentAndCleanup(true);
	m_pHPBarOff->removeFromParentAndCleanup(true);
	m_pHPBar->removeFromParentAndCleanup(true);
	//加入消除精灵
	m_pMap->addChild(pVanish);
	pVanish->runAction(CCSequence::create(CCAnimate::create(pVanishAnimation),CCCallFuncND::create(pManger,callfuncND_selector(AnimationManager::RemoveAniamationSprite),(void*)pVanish),NULL));
	m_pMap->GetUnits()->removeObject(this);

}

void BattleCharacter::MapSkillEffectsCallBack(CCNode* pSender,void* pData)
{

	CCSprite* pMapSkillBackgorund  =(CCSprite*)m_pMap->getChildByTag(LAYER_MAPSKILLBACKGROUND_ID);
	if (pMapSkillBackgorund!=NULL)
	{
		pMapSkillBackgorund->stopAllActions();
		pMapSkillBackgorund->removeFromParentAndCleanup(true);
	}
	pSender->stopAllActions();
	pSender->removeFromParentAndCleanup(true);
	CCArray* pEffectTargets  = (CCArray*)pData;
	CCObject* pObjcet = NULL;
	CCARRAY_FOREACH(pEffectTargets,pObjcet)
	{
		BattleCharacter* pTarget = (BattleCharacter*)pObjcet;
		pTarget->MapShowHPOffset(pTarget->m_nHPoffset);
		if (!pTarget->IsBattlable())
		{
			pTarget->RemoveFromBattle();
			continue;
		}
		pTarget->RefreshHPBar();
	}
	pEffectTargets->release();
	RefeshAfterAction();
}

void BattleCharacter::RefeshAfterAction()
{
	m_pStatus->RefreshAfterAction();
}

void BattleCharacter::RefeshAfterDuel()
{
	CCLOG("DUEL DONE!");
	EventHandler* pHandler = EventHandler::instance();
	BattleCharacter::AquireExpFromAttack(m_pAttacker,m_pDefender);
	BattleCharacter::AquireExpFromAttack(m_pDefender,m_pAttacker);
	//更新地图血条
	m_pAttacker->RefreshHPBar();
	m_pDefender->RefreshHPBar();
	//更新角色数据	
	bool bAlive = m_pAttacker->IsBattlable();
	m_pAttacker->RefeshAfterAction();
	m_pDefender->RefeshAfterAction();
	if (bAlive)
	{	
		pHandler->OnCharacterActionEnd(m_pAttacker);
	}
	else
	{
		CharacterStatus::m_pMap->ContinueAfterCharacterAcition();
	}
	
}

bool BattleCharacter::IsFlyable()
{
	std::vector<int>::iterator it;
	it = find(m_vAbilities.begin(),m_vAbilities.end(),4);
	return (it!= m_vAbilities.end());
}

void BattleCharacter::Save(tinyxml2::XMLDocument* pDoc)
{
	XMLElement* pCharElement = pDoc->NewElement("character");
	char chStr[50] = {0};
	//记录角色节点
	pDoc->RootElement()->LinkEndChild(pCharElement);
	//记录人物ID
	sprintf(chStr,"%d",GetCharID());
	pCharElement->SetAttribute("ID",GetCharID());
	XMLElement* pValueElement = NULL;
#define SAVE_CHARACTER_XML_VALUE(_chName_,_nValue_) \
	pValueElement = pDoc->NewElement(_chName_);\
	sprintf(chStr,"%d",_nValue_);\
	pValueElement->SetValue(chStr);\
	pCharElement->LinkEndChild(pValueElement);
	//记录人物力量值
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_STR,GetStr());
	//记录人物敏捷
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_DEX,GetDex());
	//记录人物防御力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_DEF,GetDef());
	//记录人物速度
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_SPD,GetSpd());
	//记录人物运气
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_LUK,GetLuk());
	//记录人物魔力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAG,GetLuk());
	//记录人物最大生命
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXHP,GetMaxHP());
	//记录人物当前生命
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_HP,GetHP());
	//记录人物最大精力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXSP,GetMaxSP());
	//记录人物当前精力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_SP,GetSP());
	//记录人物经验值
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_EXP,GetExp());
	//记录人物等级
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_LEVEL,GetLevel());
	//记录人物移动力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MOVE,GetMove());
	//记录人物势力
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_FORCE,GetForceFlag());
	//记录人物最大攻击范围
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXRANGE,GetMaxAtkRange());
	//记录人物最小攻击范围
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MINRANGE,GetMinAtkRange());
	//记录人物当前x坐标
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_X,GetMapPositionX());
	//记录人物当前y坐标
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_Y,GetMapPositionY());
	pValueElement = pDoc->NewElement("SKILLS");
	pCharElement->LinkEndChild(pValueElement);
	for(vector<int>::iterator it = GetSkillsIDVector().begin();it!=GetSkillsIDVector().end();it++)
	{
		XMLElement* pSkillElement = pDoc->NewElement("SKILL");
		pSkillElement->SetAttribute("ID",*it);
		pValueElement->LinkEndChild(pSkillElement);
	}
	pValueElement = pDoc->NewElement("Abilities");
	pCharElement->LinkEndChild(pValueElement);
	for(vector<int>::iterator it = m_vAbilities.begin();it!=m_vAbilities.end();it++)
	{
		XMLElement* pAbilityElement = pDoc->NewElement("Ability");
		pAbilityElement->SetAttribute("ID",*it);
		pValueElement->LinkEndChild(pAbilityElement);
	}
#undef SAVE_CHARACTER_XML_VALUE
}

void BattleCharacter::RecoverSPToMax()
{
	m_nHP = m_nMaxHP;
}

void BattleCharacter::RecoverHPToMax()
{
	m_nSP = m_nMaxSP;
}







/*
Status
*/
BattleMap* CharacterStatus::m_pMap = NULL;
MovingStatus::MovingStatus(BattleCharacter* pObject):CharacterStatus(pObject)
{
	BattleMenu::instance()->HangMenu();
	m_pCharacter->HangTouchPath();
	m_pCharacter->Move(callfuncND_selector(MovingStatus::OnMoveFinished));
}
//角色行进过程中若接受到恢复指令，则返回原地并处于被操作状态。
void MovingStatus::Resume()
{
	m_pCharacter->BackToStart();
	m_pCharacter->SetStatus(new OperateStatus(m_pCharacter));

}

void MovingStatus::Action()
{
	return;
}
void MovingStatus::OnOperate()
{
	return;
}
//当操作角色移动之后执行的回调函数
void MovingStatus::OnMoveFinished(CCNode *sender,void* data)
{
	//更新地图角色精灵Z轴
	m_pCharacter->RefreshMapZ();
	//解除视角锁定
	m_pCharacter->UnlockView();
	//进入到可操作状态。
	m_pCharacter->SetStatus(new OperateStatus(m_pCharacter));
	//m_pObject->m_pMapSprites->resumeSchedulerAndActions();
}
OperateStatus::OperateStatus(BattleCharacter* pObject):CharacterStatus(pObject)
{
	m_pCharacter->FocusView();
	m_pCharacter->ShowBattleMenu();
	m_pCharacter->Stand();
	m_pCharacter->ShadowRefresh();

}

void OperateStatus::Resume()
{
	m_pCharacter->SetStatus(new StandbyStatus(m_pCharacter));
}
void OperateStatus::Action()
{
	m_pCharacter->SetStatus(new MovingStatus(m_pCharacter));
}

void OperateStatus::OnOperate()
{
	//m_pObject->SetStatus(new StandbyStatus(m_pObject));
}
StandbyStatus::StandbyStatus(BattleCharacter* pObject):CharacterStatus(pObject)
{	
	Resume();
}
void StandbyStatus::Action()
{

}
void StandbyStatus::OnOperate()
{
	m_pCharacter->SetStatus(new OperateStatus(m_pCharacter));
}

void StandbyStatus::Resume()
{
	m_pCharacter->HangTouchPath();
	BattleMenu::instance()->Disconnect();
	m_pCharacter->m_pMapSprite->stopAllActions();
	//m_pObject->m_nCommandFlag = COMM_NONE;
	m_pCharacter->Stand();
	m_pCharacter->ShadowRefresh();
}

StayStatus::StayStatus(BattleCharacter *pObject):CharacterStatus(pObject)
{
	m_pCharacter->HangTouchPath();
	BattleMenu::instance()->Disconnect();
	m_pCharacter->Stay();
}

void StayStatus::Resume()
{
	m_pCharacter->ResumeStandby();
}
void StayStatus::Action()
{

}
void StayStatus::OnOperate()
{
	m_pMap->getParent()->addChild(new ShowStatusMenu(m_pCharacter),MAX_LAYER_ZORDER,LAYER_SHOWSTATUS_MENU_ID);
}

void CharacterStatus::RefreshAfterAction()
{
	if (!m_pCharacter->IsBattlable())
	{
		m_pCharacter->RemoveFromBattle();
		return;
	}
	m_pCharacter->RefreshPlayer();
}

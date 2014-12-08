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
//��ʼ���������
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
//�ͷŵ�retain��������
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
	//��ʼ����ɫ����
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
	//��ʼ����ɫ���ܡ�����
	pData->GetLevelSkills(m_nCharID,m_nLevel,m_vSkills);
	pData->GetLevelAbilities(m_nCharID,m_nLevel,m_vAbilities);
	//��ʼ����ɫ״̬
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
	//��ʼ����ɫ����
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
		//��ʼ����ɫ������Χ
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MAXRANGE,m_nMaxAtkRange)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_MINRANGE,m_nMinAtkRange)
		//��ȡ��ɫλ��
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_X,m_nStartX)
		LOAD_XML_CHARACTER_VALUE(DATA_CHARACTER_Y,m_nStartY)
		//��ʼ����ɫ���ܡ�����
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
/*δʵ��װ��ϵͳ��ȱ*/

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

/*�������ƶ�������������ACTION*/
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
	//�ƶ����ݷ��򴴽���ͼ��ɫ����
	CCAnimation* animation = AnimationManager::instance()->CreateMapActionAnimation(m_nCharID,actFlag);
	float dur = animation->getDuration();
	CCActionInterval* MoveTo = CCMoveTo::create(dur,ccp(m_nStepTargetX ,m_nStepTargetY));
	//���ƶ��Լ���ɫ����ʹ��CCSpawn��ϣ�װ�ص��ƶ�����������
	m_pMapActions->addObject( CCSpawn::create( CCAnimate::create(animation),MoveTo,NULL) );
	m_pMapShadowFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));
	m_pHPBarOffFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));
	m_pHPBarFollowActions->addObject( CCMoveBy::create(dur,ccp(offsetX,offsetY)));


}
//��ʾ��ʾ��Χ�Ļ���������ƶ�֮ǰ
void BattleCharacter::FomatBeforeMovePos()
{
	m_nShowRangeX = m_nStartX;
	m_nShowRangeY = m_nStartY;
}
//����ʾ��Χ�Ļ���������ƶ�֮��
void BattleCharacter::FomatMovedPos()
{
	m_nShowRangeX = GetMapPositionX();
	m_nShowRangeY = GetMapPositionY();
}


//ִ�е�ͼ��ɫ�����ƶ��������ڴ���Ļص�������ִ��֮�������
void BattleCharacter::Move(SEL_CallFuncND callFuncND)
{
	//�����ӽ�

	m_pMap->LockCharacterView(this);
	//ֹ֮ͣǰ�Ķ���
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
	//�����Ѿ��ƶ�����
	m_bMoved = true;
}
/************************************************************************/
/* ��ס�����ӽǣ��Լ����������ӽ�                                                                     */
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
	//���㵱ǰǰ���ָ�����������ʾλ�õ�ƫ������������Ϊ����ʱ������������ֵ�ƫ�ơ�������Ϊż��ʱ����Ҫ����ƫ�ư�����־����ͼƬ��ȡ�
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
//��ShowMapDamʵ�ַ�����ͬ��ֻ�ǽ����廻Ϊ��ɫ---�ݶ�
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
//���ع�����Χ
int BattleCharacter::GetAtkRange()
{
	ConfirmWeapon();
	return 2;
}

//��ȡ��ǰ����ʹ�õļ���ʩ����Χ
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
/* ������ƶ���ɫ֮�󣬵������ָ��ᱻ���á���״̬����Ϊδ�ƶ�ʱ����ˢ��Ӱ�Ӷ�����Ѫ����Ӱ�Ӻ�Z�����ȶȡ�                                                                     */
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
//�ƶ�֮ǰ�ᱻ���ã�������ʼ�������¼���ݡ�
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
//�ı�����״̬���˺������ͷŵ�ԭ����״̬��
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
	//���ƶ���Χ��ʼ��Ϊȫ�������ƶ���
	m_pRange->RefreshAvailableRange(false);
	//ͨ��A*�㷨����ƶ���Χ
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
	//��������Χ��ʼ��
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
	//ͨ���ĵ����ݻ�ȡ���ܷ�Χ
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
/*����ֵΪtrue�����ָ����Ч*/
bool BattleCharacter::PerformFromTouch(CCPoint touchPoint)
{

	m_nCommandX = touchPoint.x;
	m_nCommandY = touchPoint.y;
	if(m_pMap->GetTurnFlag() == m_nForceFlag)
	{
		//ʹ��A*�㷨����������ƶ�·��
		if(m_nCommandFlag == COMM_MOVE)
		{
			int offsetX = (int)touchPoint.x;
			int offsetY = (int)touchPoint.y;
			ConvertTouchPointToRange(GetMapPositionX(),GetMapPositionY(),&offsetX,&offsetY);
			if((abs(offsetX)+abs(offsetY))>m_nMove)
			{
				return false;//������Χ
			}

			for(unsigned int index = 0;index<m_pRange->GetOffsets()->count();index++)
			{
				PathPoint* pRangePoint = (PathPoint*)m_pRange->GetOffsets()->objectAtIndex(index);
				int pathX = pRangePoint->m_nX;
				int pathY = pRangePoint->m_nY;
				if(offsetX == pathX&&offsetY==pathY&&pRangePoint->m_bAvailable)
				{
					//����Χ�˵�
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
		//ִ�й���ָ���Ŀ�겻����ʱ������״̬��
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
					//����Χ�˵�
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
		//ִ�з�Χ����ָ��
		if(m_nCommandFlag == COMM_SKILL_RANGE)
		{
			SkillDataLoader* pData = SkillDataLoader::instance();
			int nSubRange = 0; 
			pData->GetSkillValue(DATA_SKILL_SUBRANGE,m_nOnUseID,&nSubRange);
			CCArray* pTargets = CCArray::createWithCapacity(2*(nSubRange-1)*(nSubRange));

			CCNode* pSubRange = m_pMap->getChildByTag(LAYER_SUBRANGE_ID);
			/*�����η�Χ�ڵ����н�ɫ����Ŀ�����*/
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
			/*ִ���ͷż��ܺ���*/
			bool bCommand = BattleCharacter::SimulationSkill(this,pTargets);
			if(!bCommand)
			{
				/*�������ͷ���Чʱ������*/
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
	//�����ͼ���鱻��������ҵ�ͼû����ʾ��Χ��Ҳû��ս���˵���ʹ��ʱ�����ñ������ĺ�����
	if(m_pMapSprite->boundingBox().containsPoint(touchPoint)&&(!m_pMap->IsShowRange())&&(!BattleMenu::instance()->Usable()))
	{	
		OnOperate();
		return true;
	}
	return false;
}
/*�ӵ�ǰ���ȡ���ƶ��ĳ���*/

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

/*ͨ���˵�ִ�и���ָ��*/
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
	//������Ϊstay
	m_pMapSprite->stopAllActions();
	char frameStr[50]= {0};
	sprintf(frameStr,"%s%d%s%d%s0.png",MAPCHAR_FRAME_NAME,m_nCharID,MAPCHAR_ACTION_FILE_NAME,ACTION_MAPSTAY/MAX_ANIMATION_FRAME,MAPCHAR_FRAME_COUNT_NAME);
	CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameStr); 
	m_pMapSprite->setDisplayFrame(frame);
	//���õ�����
	InitStep();
}



//ս������
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
	//��¼������ս��ǰ��״̬
	BattleRecordTable* pDefenderTable = new BattleRecordTable();
	pDefenderTable->m_nHP = pDefender->m_nHP;
	pDefenderTable->m_nHPOffset = atkOffset;
	pDefenderTable->m_nSP = pDefender->GetSP();
	//���·�������ֵ
	pDefender->m_nHP -= atkOffset;
	//����
	bool isFightBack  =false;
	if(pDefender->IsBattlable())
	{
		//����������������״̬�����ҹ��������������ʱ,��������
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

	/*��¼ս��ǰ��������ɫ״̬*/
	BattleRecordTable* pAttackerTable = new BattleRecordTable();
	pAttackerTable->m_nHP = pAttacker->m_nHP;
	pAttackerTable->m_nHPOffset =fightbackOffset;
	pAttackerTable->m_nSP = pAttacker->GetSP();
	//���½�������ֵ
	pAttacker->m_nHP -= fightbackOffset;
	/*�����������ǰ�ж���ǰ�ľ���ģʽ*/
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





	//�����������
	CCScene* scene = (CCScene*)pAttacker->m_pMapSprites->getParent()->getParent();
	DuelLayer *pDuelLayer = new DuelLayer(pAttacker,pDefender,pAttackerTable,pDefenderTable,nMode);
	scene->addChild(pDuelLayer,MAX_LAYER_ZORDER,LAYER_DUEL_LAYER_ID);
	int nCount =0;
	m_pAttacker = pAttacker;
	m_pDefender = pDefender;

	//���¾���ֵ 
	//if(pAttacker->GetForceFlag()==FORCE_PLAYER)
	//{
	//	BattleCharacter::AquireExpFromAttack(pAttacker,pDefender);
	//}
	//else if(pDefender->GetForceFlag()==FORCE_PLAYER)
	//{
	//	BattleCharacter::AquireExpFromAttack(pDefender,pAttacker);
	//}
	////���µ�ͼѪ��
	//pAttacker->RefreshHPBar();
	//pDefender->RefreshHPBar();
	////���½�ɫ����
	//pAttacker->RefeshAfterAction();
	//pDefender->RefeshAfterAction();
}
/************************************************************************/
/* ����ֵ���ŵ��ҵȼ���ݼ������ҵȼ����ڸ�������εĵݼ�����
-1~-3  ����40%
-3~-4  ����60%
-4~    ����90%
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
//�����˺���ʽ ������-������    ---�ݶ�
int BattleCharacter::GetAttackHPOffset(BattleCharacter *pAttacker, BattleCharacter *pDefender)
{
	int offset =  pAttacker->GetAtk() - pDefender->GetDef();
	offset = offset>0?offset:0;
	return offset;
}
/************************************************************************/
/* ͨ���жϼ��ܵ����͡��˺�������Ч�������ö�����жϼ����Ƿ��ͷųɹ������ͷųɹ�ʱ�����ݸ��½�ɫ����                                                                   */
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
		//�ж��Ƿ�����˺�
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
//�����˺� = ���ܻ����˺� --�ݶ�
int BattleCharacter::GetSkillHPDam(BattleCharacter *pInvoker, BattleCharacter *pTarget)
{
	SkillDataLoader* pData = SkillDataLoader::instance();
	int nID = pInvoker->m_nOnUseID;
	int nDamage = 0;
	pData->GetSkillValue(DATA_SKILL_DAMAGE,nID,&nDamage);
	return nDamage;
}
//��������Ч�� = ħ��ֵ+����Ч�� --�ݶ�
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
//�ж�����ж�
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
	//��exp>100ʱ����
	//��ջ�ȡ��exp
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
	//	//�ж��¼�
	//	EventHandler::instance()->OnCharacterActionEnd(this);
	//}
}
void BattleCharacter::RemoveLevelLogoCallBack(CCNode* pSender,void* pData)
{
	((CCSprite*)pData)->removeFromParentAndCleanup(true);
	//ˢ�µ�ͼѪѪ��
	RefreshHPBar();
}

void BattleCharacter::LevelUp(int nUp)
{
	//�ӽ�ɫ�����ļ��л�ȡ��������,�����Ǹ��ӵ���ɫ��
	m_nLevel += nUp;
	int nAtrUp;//����������
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
	//��ȡ�õȼ��ļ��ܡ�����
	pData->GetLevelAbilities(m_nCharID,m_nLevel,m_vAbilities);
	pData->GetLevelSkills(m_nCharID,m_nLevel,m_vSkills);
}

vector<int>& BattleCharacter::GetSkillsIDVector()
{
	return m_vSkills;
}

void BattleCharacter::InitSpritesAndStatus(int x,int y)
{
	//���鶯��
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s%d.png",MAPCHAR_FILE_FULLPATH,m_nCharID);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey(chImageStr);	
	//��ʼ������
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
	//��ʼ����ͼ��ɫӰ��
	sprintf(frameStr,"%s%d%s1.png",MAPCHAR_FRAME_NAME,m_nCharID,MAP_SHADOW_FRAME_NAME);
	m_pMapShadow = CCSprite::createWithSpriteFrameName(frameStr);
	m_pMapSprites->addChild(m_pMapShadow,-MAX_LAYER_ZORDER);
	RefreshShadow();
	//��ʼ��Ѫ��
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
	//��ʼ���ƶ����
	InitStep();

	//����״̬
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
	//ע���¼�
	EventHandler::instance()->OnCharacterRemoveFromBattle(this);
	//�Ƴ������ͼ����
	m_pMapSprites->removeFromParentAndCleanup(true);
	m_pHPBarOff->removeFromParentAndCleanup(true);
	m_pHPBar->removeFromParentAndCleanup(true);
	//������������
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
	//���µ�ͼѪ��
	m_pAttacker->RefreshHPBar();
	m_pDefender->RefreshHPBar();
	//���½�ɫ����	
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
	//��¼��ɫ�ڵ�
	pDoc->RootElement()->LinkEndChild(pCharElement);
	//��¼����ID
	sprintf(chStr,"%d",GetCharID());
	pCharElement->SetAttribute("ID",GetCharID());
	XMLElement* pValueElement = NULL;
#define SAVE_CHARACTER_XML_VALUE(_chName_,_nValue_) \
	pValueElement = pDoc->NewElement(_chName_);\
	sprintf(chStr,"%d",_nValue_);\
	pValueElement->SetValue(chStr);\
	pCharElement->LinkEndChild(pValueElement);
	//��¼��������ֵ
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_STR,GetStr());
	//��¼��������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_DEX,GetDex());
	//��¼���������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_DEF,GetDef());
	//��¼�����ٶ�
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_SPD,GetSpd());
	//��¼��������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_LUK,GetLuk());
	//��¼����ħ��
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAG,GetLuk());
	//��¼�����������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXHP,GetMaxHP());
	//��¼���ﵱǰ����
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_HP,GetHP());
	//��¼���������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXSP,GetMaxSP());
	//��¼���ﵱǰ����
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_SP,GetSP());
	//��¼���ﾭ��ֵ
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_EXP,GetExp());
	//��¼����ȼ�
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_LEVEL,GetLevel());
	//��¼�����ƶ���
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MOVE,GetMove());
	//��¼��������
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_FORCE,GetForceFlag());
	//��¼������󹥻���Χ
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MAXRANGE,GetMaxAtkRange());
	//��¼������С������Χ
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_MINRANGE,GetMinAtkRange());
	//��¼���ﵱǰx����
	SAVE_CHARACTER_XML_VALUE(DATA_CHARACTER_X,GetMapPositionX());
	//��¼���ﵱǰy����
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
//��ɫ�н������������ܵ��ָ�ָ��򷵻�ԭ�ز����ڱ�����״̬��
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
//��������ɫ�ƶ�֮��ִ�еĻص�����
void MovingStatus::OnMoveFinished(CCNode *sender,void* data)
{
	//���µ�ͼ��ɫ����Z��
	m_pCharacter->RefreshMapZ();
	//����ӽ�����
	m_pCharacter->UnlockView();
	//���뵽�ɲ���״̬��
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

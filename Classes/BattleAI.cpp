#include "BattleAI.h"
#include "GameUI.h"
#include "EventHandler.h"
#define  AI_CHAN_DUR  0.4f



AIBasedStatus::AIBasedStatus(BattleCharacter* pObject):CharacterStatus(pObject)
{
	m_pCharacter->Stand();
	m_pCharacter->ShadowRefresh();
	m_pMoveTarget = NULL;
	m_pTargetChar = NULL;
}

AIStayStatus::AIStayStatus(BattleCharacter* pObject):AIBasedStatus(pObject)
{
	
}
void AIStayStatus::Action()
{
	m_pMap->AIActionsChan();
}


void AIBasedStatus::OnOperate()
{
	m_pCharacter->ShowMoveRange();
	m_pMap->getParent()->addChild(new ShowRangeMenu(m_pCharacter,0,false),MAX_LAYER_ZORDER,LAYER_SHOWRANGE_MENU_ID);
	m_pMap->getParent()->addChild(new ShowStatusMenu(m_pCharacter),MAX_LAYER_ZORDER,LAYER_SHOWSTATUS_MENU_ID);
	//m_pMap->AcceptShowRangeObject(m_pObject);
}
void AIBasedStatus::Action()
{
    ConfirmTargetAndCommand();
	FilterDecision();
	ExecuteCommand();
}
void AIBasedStatus::OnMoveFinished(CCNode *sender,void* data)
{
	//����Z��
	m_pCharacter->RefreshMapZ();

	//�����ӽ�����
	m_pCharacter->UnlockView();
	//������ʾAI�ƶ���Χ
	m_pMap->KickShowRangeObject(m_pCharacter);
	//��������ƶ�������µ���ǰλ��
	m_pCharacter->InitStep();
	
	//����û��Ŀ�����ѡ��
	if(m_pMoveTarget==NULL)
	{
		//����ͼ����״̬�������ж���ϵ�״̬
		m_pCharacter->m_pMapSprite->stopAllActions();
		m_pCharacter->Stay();
		EventHandler::instance()->OnCharacterActionEnd(m_pCharacter);
		return;
	}	
	if(m_nObjectiveCommand == COMM_ATK&&m_bExecuteCommand)
	{
		m_pCharacter->ShowAtkRange();
	}	
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AIBasedStatus::OnShowCommandRangeFinished),this,0,false,AI_CHAN_DUR,false);

}
/************************************************************************/
/* �غϽ���ʱ���߱��û����ʱ��ᱻ���ã�����˵����ƶ���Χ                                                                    */
/************************************************************************/
void AIBasedStatus::Resume()
{
	m_pCharacter->HangTouchPath();
	BattleMenu::instance()->Disconnect();
	m_pCharacter->Stand();
}
//��ʾָ�Χ��ִ�е�ǰָ��
void AIBasedStatus::OnShowCommandRangeFinished(float time)
{
	ExecuteCommandDirect();
	m_pMap->KickShowRangeObject(m_pCharacter);
}
void AIBasedStatus::OnCommandFisinshed(CCNode *sender,void* data)
{
	
}

void AIBasedStatus::ExecuteCommandDirect()
{	
	
	/*if(m_nObjectiveCommand == COMM_ATK)
	{
		SimulationAttackAction(m_pCharacter,m_pTargetChar);
	}*/
	if (m_bExecuteCommand)
	{
		m_pCharacter->SetCommandFlag(m_nObjectiveCommand);
		if(!m_pCharacter->PerformFromTouch(ccp(m_nCommandPosX,m_nCommandPosY)))
		{
			m_pCharacter->m_pMapSprite->stopAllActions();
			m_pCharacter->Stay();
			EventHandler::instance()->OnCharacterActionEnd(m_pCharacter);
	/*		m_pMap->AIActionsChan();*/
		}
	}
	else
	{
		m_pCharacter->m_pMapSprite->stopAllActions();
		m_pCharacter->Stay();
		EventHandler::instance()->OnCharacterActionEnd(m_pCharacter);
		/*m_pMap->AIActionsChan();*/
	}
	
	
}

void AIBasedStatus::RefreshAfterAction()
{
	if (!m_pCharacter->IsBattlable())
	{
		m_pCharacter->RemoveFromBattle();
		return;
	}
	else
	{
		m_pCharacter->stopAllActions();
		m_pCharacter->Stay();
	}
}

/*�ڿ����ƶ��������ķ�Χ�ڿɹ�����Ŀ��,��û��Ŀ����Թ���ʱ���ý�ɫ�ᾲֹ*/

void AIBasedATKStatus::ConfirmTargetAndCommand()
{
	m_nObjectiveCommand = COMM_ATK;
	m_pTargetChar = NULL;
	m_pMoveTarget = NULL;
	m_bExecuteCommand = false;
	m_pCharacter->CheckMoveRange();
	CCArray* pUnits = m_pMap->GetUnits();
	CCArray* pTargets = CCArray::createWithCapacity(pUnits->count());
	for(unsigned int index = 0 ;index<pUnits->count();index++)
	{
		BattleCharacter* pUnit = (BattleCharacter*)pUnits->objectAtIndex(index);
		if (!pUnit->InSameRace(m_pCharacter))
		{
			pTargets->addObject(pUnits->objectAtIndex(index));
		}
		
	}
	for(;pTargets->count()!=0;)
	{
		int toal = pTargets->count();
		int randSel = 0;
		do 
		{	
			randSel = CCRANDOM_0_1()*toal;
		} while (randSel==toal);
		
		BattleCharacter* pUnit = (BattleCharacter*)pTargets->objectAtIndex(randSel);
		//if(pUnit->InSameRace(m_pCharacter))
		//{
		//	pTargets->removeObjectAtIndex(randSel);
		//	continue;//���ܻ����ɱ,����Ѱ��Ŀ��
		//}
		int x = pUnit->GetMapPositionX();
		int y = pUnit->GetMapPositionY();
		ConvertPositionToRange(m_pCharacter->GetMapPositionX(),m_pCharacter->GetMapPositionY(),&x,&y);
		if( x+y > (m_pCharacter->GetMove()+m_pCharacter->GetAtkRange()))
		{
			pTargets->removeObjectAtIndex(randSel);
			continue;//���Գ�����̣�����Ѱ��Ŀ��
		}
		int nMoveRange = m_pCharacter->GetMove();
		int nMaxRange = m_pCharacter->GetMaxAtkRange();
		int nMinRange =m_pCharacter->GetMinAtkRange();
		CCArray* moveableRanges = CCArray::createWithCapacity(4*nMaxRange);
		CCArray* pAllMoves = m_pCharacter->GetRangeArray();
		for(int nAtkRange = nMaxRange;nAtkRange>=nMinRange;nAtkRange--)
		{
			for(int tempRangeX = 0;tempRangeX<=nAtkRange ;tempRangeX++)
			{
				int tempRangeY = nAtkRange  - tempRangeX;
				int offsetIndex = DrhombusRangeObject::GetRangeOffsetsIndex(x+tempRangeX,y+tempRangeY,nMoveRange);
				if(offsetIndex!=-1)
				{
					PathPoint* pPoint = (PathPoint*)pAllMoves->objectAtIndex(offsetIndex);
					if(pPoint->m_bAvailable)
					{
						moveableRanges->addObject(pPoint);
					}
				}
				offsetIndex = DrhombusRangeObject::GetRangeOffsetsIndex(x-tempRangeX,y+tempRangeY,nMoveRange);
				if(tempRangeX!=0&&offsetIndex!=-1)
				{
					PathPoint* pPoint = (PathPoint*)pAllMoves->objectAtIndex(offsetIndex);
					if(pPoint->m_bAvailable)
					{
						moveableRanges->addObject(pPoint);
					}
				}
				offsetIndex = DrhombusRangeObject::GetRangeOffsetsIndex(x+tempRangeX,y-tempRangeY,nMoveRange);
				if(tempRangeY!=0&&offsetIndex!=-1)
				{
					PathPoint* pPoint = (PathPoint*)pAllMoves->objectAtIndex(offsetIndex);
					if(pPoint->m_bAvailable)
					{
						moveableRanges->addObject(pPoint);
					}
				}
				offsetIndex = DrhombusRangeObject::GetRangeOffsetsIndex(x-tempRangeX,y-tempRangeY,nMoveRange);
				if(tempRangeX!=0&&tempRangeY!=0&&offsetIndex !=-1)
				{
					PathPoint* pPoint = (PathPoint*)pAllMoves->objectAtIndex(offsetIndex);
					if(pPoint->m_bAvailable)
					{
						moveableRanges->addObject(pPoint);
					}
				}
			}
		}
		
		if(moveableRanges->count()==0)
		{
			pTargets->removeObjectAtIndex(randSel);
			continue;//��̷�Χ��û����ŵ�
		}
		//�������ϼ������Ŀ�������Ϊ����Ŀ��
		m_pTargetChar = pUnit;
		m_bExecuteCommand = true;
		m_nCommandPosX = pUnit->GetMapPositionX();
		m_nCommandPosY = pUnit->GetMapPositionY();
		toal = moveableRanges->count();
		do 
		{	
			randSel = CCRANDOM_0_1()*toal;
		} while (randSel==toal);
		m_pMoveTarget = (PathPoint*)moveableRanges->objectAtIndex(randSel);
		return;
	}
	
}
/*˼���ж�����,���ڴ�AIΪ��һ���ƶ�~����ģʽ����Ϊ��*/
void AIBasedATKStatus::FilterDecision()
{
	
}

/*˼����������ʼִ��ָ�ͬʱ����ص�����ʹai���ж�����*/

void AIBasedATKStatus::ExecuteCommand()
{	
	if(m_pMoveTarget!= NULL)
	{
		m_pCharacter->CaculateMovePath(m_pMoveTarget);
		m_pCharacter->FomatBeforeMovePos();
		m_pMap->AcceptShowRangeObject(m_pCharacter);
		m_pCharacter->Move(callfuncND_selector(AIBasedStatus::OnMoveFinished));
	}
	else
	{
		OnMoveFinished(this,NULL);
	}
	
	//m_pObject->InitStep();
	//while(!m_pObject->m_tPaths.empty())
	//{
	//	twards_enum tward = m_pObject->m_tPaths.back();
	//	m_pObject->m_tPaths.pop_back();
	//	m_pObject->MoveStep(tward);
	//}
	//m_pObject->m_pMapActions->addObject(CCCallFuncND::create(m_pObject,callfuncND_selector(AIBasedStatus::OnMoveFinished),(void*)this));
	//if(m_pObject->m_pMapActions->count()!=0)
	//{
	//	CCAction* pActions = CCSequence::create(m_pObject->m_pMapActions);
	//	m_pObject->m_pMapSprite->runAction(pActions);
	//}
}

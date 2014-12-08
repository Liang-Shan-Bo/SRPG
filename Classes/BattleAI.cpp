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
	//更新Z轴
	m_pCharacter->RefreshMapZ();

	//解锁视角锁定
	m_pCharacter->UnlockView();
	//不再显示AI移动范围
	m_pMap->KickShowRangeObject(m_pCharacter);
	//将自身的移动坐标更新到当前位置
	m_pCharacter->InitStep();
	
	//附近没有目标可以选择
	if(m_pMoveTarget==NULL)
	{
		//将地图精灵状态更新至行动完毕的状态
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
/* 回合结束时或者被用户点击时后会被调用，挂起菜单和移动范围                                                                    */
/************************************************************************/
void AIBasedStatus::Resume()
{
	m_pCharacter->HangTouchPath();
	BattleMenu::instance()->Disconnect();
	m_pCharacter->Stand();
}
//显示指令范围后执行当前指令
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

/*在可以移动并攻击的范围内可攻击的目标,当没有目标可以攻击时，该角色会静止*/

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
		//	continue;//不能互相残杀,继续寻找目标
		//}
		int x = pUnit->GetMapPositionX();
		int y = pUnit->GetMapPositionY();
		ConvertPositionToRange(m_pCharacter->GetMapPositionX(),m_pCharacter->GetMapPositionY(),&x,&y);
		if( x+y > (m_pCharacter->GetMove()+m_pCharacter->GetAtkRange()))
		{
			pTargets->removeObjectAtIndex(randSel);
			continue;//明显超出射程，继续寻找目标
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
			continue;//射程范围内没有落脚点
		}
		//经过以上检测后，这个目标可以作为攻击目标
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
/*思考行动策略,由于此AI为单一的移动~攻击模式，故为空*/
void AIBasedATKStatus::FilterDecision()
{
	
}

/*思考结束，开始执行指令，同时加入回调函数使ai的行动有序化*/

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

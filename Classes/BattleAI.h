#ifndef __Battle_AI_H__
#define __Battle_AI_H__
#include "cocos2d.h"
#include "BattleMapScene.h"
#include "BattleCharacter.h"
#include "Singleton.h"


USING_NS_CC;
/*电脑战斗AI,通过实现角色状态定义的ACTION完成实现AI的战斗动作。各个AI需要实现：将要实施确认指令以及指令的目标、思考向何处移动、以及验证并执行指令等几个纯虚函数*/


/*
	AI状态类的父类，它定义了执行何种指令、指令的地点、移动目的地等成员变量。提供了AI的思考接口以及一些动画执行后回调函数*/
class AIBasedStatus:public CharacterStatus
{
public:
	AIBasedStatus(BattleCharacter* pObject);
	virtual void RefreshAfterAction();
	virtual void OnOperate();
	virtual void Action();
	virtual bool IsControlable()
	{
		return false;
	}
	PathPoint* m_pMoveTarget;
	int m_nObjectiveCommand;
	int m_nCommandPosX;
	int m_nCommandPosY;
	bool m_bExecuteCommand;
	BattleCharacter* m_pTargetChar;
	void OnMoveFinished(CCNode *sender,void* data);
	void OnCommandFisinshed(CCNode *sender,void* data);
	void OnShowCommandRangeFinished(float time);
	void ExecuteCommandDirect();

protected:
	virtual void ConfirmTargetAndCommand()=0;//确认指令以及目标
	virtual void FilterDecision()=0;//思考向何处移动
	virtual void ExecuteCommand()=0;//执行指令
	virtual void Resume();
};

/*什么也不做，待机的AI*/
class AIStayStatus:public AIBasedStatus
{
public:
	AIStayStatus(BattleCharacter* pObject);
	virtual void Action();
private:
	AIBasedStatus* m_pStatusToResume;
};
/*一个被动型的随机攻击范围内目标的AI。它会随机选取移动范围+攻击范围内可攻击到的一个作为攻击对象的目标展开攻击。*/

class AIBasedATKStatus:public AIBasedStatus
{
public:
	AIBasedATKStatus(BattleCharacter* pObject):AIBasedStatus(pObject)
	{
		
	}
protected:
	virtual void ConfirmTargetAndCommand();//确认指令以及目标队列
	virtual void FilterDecision();//思考向何处移动
	virtual void ExecuteCommand();//矫正目标队列范围，执行指令
};

#endif
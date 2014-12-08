#ifndef __Battle_AI_H__
#define __Battle_AI_H__
#include "cocos2d.h"
#include "BattleMapScene.h"
#include "BattleCharacter.h"
#include "Singleton.h"


USING_NS_CC;
/*����ս��AI,ͨ��ʵ�ֽ�ɫ״̬�����ACTION���ʵ��AI��ս������������AI��Ҫʵ�֣���Ҫʵʩȷ��ָ���Լ�ָ���Ŀ�ꡢ˼����δ��ƶ����Լ���֤��ִ��ָ��ȼ������麯��*/


/*
	AI״̬��ĸ��࣬��������ִ�к���ָ�ָ��ĵص㡢�ƶ�Ŀ�ĵصȳ�Ա�������ṩ��AI��˼���ӿ��Լ�һЩ����ִ�к�ص�����*/
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
	virtual void ConfirmTargetAndCommand()=0;//ȷ��ָ���Լ�Ŀ��
	virtual void FilterDecision()=0;//˼����δ��ƶ�
	virtual void ExecuteCommand()=0;//ִ��ָ��
	virtual void Resume();
};

/*ʲôҲ������������AI*/
class AIStayStatus:public AIBasedStatus
{
public:
	AIStayStatus(BattleCharacter* pObject);
	virtual void Action();
private:
	AIBasedStatus* m_pStatusToResume;
};
/*һ�������͵����������Χ��Ŀ���AI���������ѡȡ�ƶ���Χ+������Χ�ڿɹ�������һ����Ϊ���������Ŀ��չ��������*/

class AIBasedATKStatus:public AIBasedStatus
{
public:
	AIBasedATKStatus(BattleCharacter* pObject):AIBasedStatus(pObject)
	{
		
	}
protected:
	virtual void ConfirmTargetAndCommand();//ȷ��ָ���Լ�Ŀ�����
	virtual void FilterDecision();//˼����δ��ƶ�
	virtual void ExecuteCommand();//����Ŀ����з�Χ��ִ��ָ��
};

#endif
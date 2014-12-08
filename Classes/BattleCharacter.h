#ifndef __BATTLE_CHARACTER_H__
#define __BATTLE_CHARACTER_H__

#include "cocos2d.h"
#include "RangeObject.h"
#include <vector>
#include <algorithm>
#include "BattleObject.h"
#include "slg_tools.h"
#include "tinyxml2.h"

//define commands
#define COMM_NONE 0
#define COMM_ATK 1
#define COMM_DEF 2
#define COMM_SKILL 3
#define COMM_MOVE 4
#define COMM_ITEM 5
#define COMM_STAY 6
#define COMM_SKILL_RANGE 7
#define COMM_ITEM_RANGE  8
#define COMM_BACK 9
#define COMM_Status 10
#define COMM_CANCEL 99

//define force
#define FORCE_PLAYER 0
#define FORCE_ENERMY 1
#define FORCE_NPC 2

#define FORCE_COUNT 2


USING_NS_CC;
using namespace std;
using namespace tinyxml2;



class CharacterStatus;//״̬�ุ��
class MovingStatus;//�������ƶ���
class OperateStatus;//����ѡ��
class StandbyStatus; //����������״̬
class AIBasedStatus;//

class BattleMap;




class BattleCharacter : public CCNode 
{
	IntGetter(Level)//��ɫ�ȼ�
	IntGetter(Move)//��ɫ�ƶ���
	IntGetter(ForceFlag)//��ɫ����
	IntGetter(CharID)//��ɫID
	IntGetter(Exp)//��ɫ����ֵ
	IntGetter(HP)//��ɫ��ǰѪ��
	IntGetter(MaxHP)//��ɫ���Ѫ��
	IntGetter(SP)//��ǰ��ɫ����
	IntGetter(MaxSP)//��ɫ�������ֵ
	IntGetter(Str)//��ɫ����ֵ
	IntGetter(Mag)//��ɫħ��
	IntGetter(Spd)//��ɫ�ٶ�
	IntGetter(Def)//��ɫ����
	IntGetter(Luk)//��ɫ����
	IntGetter(Dex)//��ɫ����
	IntGetter(ActionCount)//�ж�����
	IntGetter(BaseExp)//ս���ɻ�õĻ�������
	IntGetter(OnUseID)//����ʹ�õ���Ʒ�����ܵ�ID
	IntGetter(MaxAtkRange)//��󹥻�����
	IntGetter(MinAtkRange)//��С��������
	IntGetter(UnionFlag)
	
private:
	CharacterStatus* m_pStatus;//ս����ɫ״̬
	RangeObject* m_pRange;//ս����ɫ��ͼ��ʾ����ʽ


	int m_nStepTargetX;//ÿһ���ƶ��ĺ�������
	int m_nStepTargetY;//ÿһ���ƶ�����������
	int m_nStartX;//��ִ���ƶ�����ǰ�ĺ���λ��
	int m_nStartY;//��ִ���ƶ�����ǰ������λ��
	int m_nShowRangeX;//��ʾ��Χ
	int m_nShowRangeY;//��ʾ��Χ
	int m_nCommandFlag;//��ɫ���ں�������
	int m_nCommandX;//ָ��ִ�еĺ�����
	int m_nCommandY;//ָ��ִ�е�������
	int m_nExpGot;//��õľ���ֵ
	int m_nHPoffset;//��õ��˺�������Ч��
	bool m_bMoved;//�Ƿ�ִ�й��ƶ�ָ��
	bool m_bKeyCharacter;//�Ƿ���ս���������ɫ
	bool m_bTargetCharacter;//�Ƿ�Ϊ����boss,ս������ʤ��
	vector<int> m_vSkills;//�����ɫ���õļ���ID
	vector<int> m_vAbilities;//�����ɫ������ID
	CCProgressTimer* m_pHPBar;//Ѫ��
	CCSprite* m_pHPBarOff;//Ѫ�۱���
	vector<twards_enum> m_tPaths;//��ɫ�ƶ��ķ���·��	
	void MoveStep(twards_enum twards);//�����ƶ��Ķ���
	//void OnMoveFinished(CCNode* sender,void* data);//��ɫ����ƶ��Ļص�����
	void ConfirmAtkRange();//ȷ�Ϲ������
	void ConfirmWeapon();//ȷ������
	void ConfirmSkillRange();//ȷ�ϼ������
	void RefeshAfterAction();//���ж�֮���������״̬����ʾ����
	void ShowMove();
	/*bool m_bClickAble;*/
	void FomatMovedPos();
	void ShowBattleMenu();//��ʾս���˵�
	void RefreshShadow();
	/*void ShowTouchPath();*/
	void CreateMovePath(PathPoint* pTarget);//�����ƶ�·��(A*�㷨)
	CCSpriteBatchNode* m_pMapSprites;//BatchNode,����Ч�ʵ���Ⱦ
	CCSprite *m_pMapSprite;//��ͼ����
	CCSprite *m_pMapShadow;//��ͼ��ɫ��Ӱ��
	CCArray* m_pMapActions; //�����б�
	//Ӱ�ӣ�Ѫ�۵ȸ��涯��
	CCArray* m_pMapShadowFollowActions;
	CCArray* m_pHPBarOffFollowActions;
	CCArray* m_pHPBarFollowActions;
	void LevelUp(int nUp);//��������������
	void MapSkillEffectsCallBack(CCNode* pSender,void* pData);//��ͼ����Ӱ��ص�����
	void ExpForLevelCallBack(CCNode* pSender,void* pData);//�ж��������Լ��������������Ļص�����
	void RemoveLevelLogoCallBack(CCNode* pSender,void* pData);//����levelup��ͼ��
	/************************************************************************/
	/* ս�����㺯��������                                                                     */
	/************************************************************************/
	static void SimulationAttackAction(BattleCharacter* pAttacker,CCArray* pDefenders);
	static void SimulationAttackAction(BattleCharacter* pAttacker,BattleCharacter* pDefender);
	static bool SimulationSkill(BattleCharacter* pInvoker,CCArray* pTargets);
	static int GetAttackHPOffset(BattleCharacter *pAttacker, BattleCharacter *pDefender);
	static int GetSkillHPDam(BattleCharacter *pInvoker, BattleCharacter *pTarget);
	static int GetSkillHPHeal(BattleCharacter *pInvoker, BattleCharacter *pTarget);
	static void AquireExpFromAttack(BattleCharacter* pPlayer,BattleCharacter* pOpposite);
	static BattleCharacter* m_pAttacker;
	static BattleCharacter* m_pDefender;
	
	
	
	
	
public:
	static BattleMap* m_pMap;
	int GetAtk();//��ȡ���﹥����
	int GetAtkRange();
	int GetSkillRange();//��ȡ���＼�ܷ�Χ
	vector<int>& GetSkillsIDVector();//���ؽ�ɫ����ID��vector����
	void RefreshMapZ();//���µ�ͼ����z�����ȼ�
	void FocusView();//�����Ӿ�������
	void UnlockView();//�����ӽ�
	void RefreshPlayer();//���������������
	void MapShowHPOffset(int dam);
	void ShowMoveRange();//��ʾ�ƶ���Χ
	void ShowSkillRange();//��ʾ���ܷ�Χ
	void FomatBeforeMovePos();
	void ShowAtkRange();//��ʾ������Χ	
	void SetCommandFlag(int nFlag); 
	void ShadowRefresh();
	void RefreshHPBar();
	void SetStatus(CharacterStatus* pStatus);//�л���ɫ״̬
	void Move(SEL_CallFuncND callFuncND);//�ƶ�
	void InitStep();//��ʼ�������ƶ�������
	void CaculateMovePath(PathPoint* pRangePoint);
	void CheckMoveRange();//��������ƶ���Χ�������ݴ�ŵ�m_pRange�С�
	bool PerformFromTouch(CCPoint touchPoint);//���ݴ�����������Ӧ�Ķ���
	void RefreshTurnStatus();
	void ExecuteCommand(int commandFlag);
	int GetMapPositionX();//��ȡ��ͼ����ĺ�����λ��
	int GetMapPositionY();//��ȡ��ͼ�����������λ��
	int GetShowRangePositionX();//��ȡ�ƶ��ĺ�����
	int GetShowRangePositionY();//��ȡ�ƶ�֮���������
	CCPoint GetMapPosition();//��ȡ��ͼλ��
	void SetMapPosition(CCPoint pos);//���õ�ͼ�����λ��
	CCArray* GetRangeArray(); //��ȡ��ʾ��Χ������
	void OnOperate();//��ѡȡ����Ķ���
	bool IsControlable();//�Ƿ���Ա���Ҳ���
	void Resume();//���ý�ɫ�Ĳ���״̬
	void ResumeActionCount();//�ָ��ƶ�����
	BattleCharacter();//���캯��
	~BattleCharacter();	//��������
	bool IsShowRange();//�Ƿ�������ʾ��Χ
	bool MapContains(CCPoint touchPoint);
	bool IsAbleToAtk();//�Ƿ����ʹ�ù���ָ��
	bool IsAbleToItem();//�Ƿ����ʹ����Ʒָ��
	bool IsAbleToDef();//�Ƿ����ʹ�÷���ָ��
	bool IsAbleToSkill();//�Ƿ����ʹ�ü���ָ��
	bool IsAbleToStay();//�Ƿ����ʹ�ô���ָ��
	bool IsAbleToMove();//�Ƿ����ʹ���ƶ�ָ��
	bool IsMoved();//�Ƿ�ִ�й��ƶ�ָ��
	bool IsFlyable();//�Ƿ�Ϊ���е�λ
	bool IsZOC(BattleCharacter* pAnotherChar);//�Ƿ����ZOC
	void BuildCharacter(CCDictionary* dic );//����һ����ɫ��ʾ��ս����ͼ��
	bool LoadCharacterFromData(XMLElement* pCharacter);
	void Action();//�ж�
	void Stand();//վ������ʾ������ҵĿ���
	void Standby();//׼����������
	void Stay();//����
	void ResumeStandby();//�ظ�����
	void HangTouchPath();//������ʾ��Χ
	void BackToStart();//�ص�ִ���ƶ�ָ��ǰ��ԭ��
	bool InSameRace(BattleCharacter* pCharacter);
	void LoadSpritesAndStatus();//����ͼƬ������ʾ��ս����ͼ��
	void InitSpritesAndStatus(int x,int y);
	void RemoveFromBattle();//ս�����ܣ�����ս��
	void RecoverHPToMax();
	void RecoverSPToMax();
	//�ж�����ж�
	bool IsBattlable ();
	bool IsEndTurn();
	void Save(tinyxml2::XMLDocument* pDoc);
	static void RefeshAfterDuel();
	friend class MovingStatus;
	friend class StandbyStatus;
	friend class OperateStatus;
	friend class BattleManager;
	friend class AIBasedStatus;
};
class CharacterStatus : public CCNode
{
protected:

	BattleCharacter* m_pCharacter;
public:
	static BattleMap* m_pMap;
	CharacterStatus(BattleCharacter* pObject)
	{
		m_pCharacter = pObject;
	}
	virtual void RefreshAfterAction();
	virtual bool IsControlable() = 0;
	virtual void OnOperate() = 0;
	virtual void Action() = 0;
	virtual void Resume() = 0;
};

class MovingStatus: public CharacterStatus
{
public:
	MovingStatus(BattleCharacter* pObject);
	virtual void Action();
	virtual void OnOperate();
	virtual void Resume();
	void OnMoveFinished(CCNode *sender,void* data);
	virtual bool IsControlable()
	{
		return true;
	}
};

class StandbyStatus: public CharacterStatus
{
public:
	StandbyStatus(BattleCharacter* pObject);
	virtual void OnOperate();
	virtual void Action();
	virtual void Resume();
	virtual bool IsControlable()
	{
		return true;
	}
};
class OperateStatus: public CharacterStatus
{
public:
	OperateStatus(BattleCharacter* pObject);
	virtual void OnOperate();
	virtual void Action();
	virtual void Resume();
	virtual bool IsControlable()
	{
		return true;
	}
};

class StayStatus:public CharacterStatus
{
public:
	StayStatus(BattleCharacter* pObject);
	virtual void OnOperate();
	virtual void Action();
	virtual void Resume();
	virtual bool IsControlable()
	{
		return false;
	}

};



#endif  
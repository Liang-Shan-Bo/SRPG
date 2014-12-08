#ifndef __DUEL_SCENE_H__
#define __DUEL_SCENE_H__

#include "cocos2d.h"
#include "BattleCharacter.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

//ս����ֵ�䶯��
struct BattleRecordTable:public CCObject
{
	int m_nHP;
	int m_nHPOffset;
	int m_nSP;
	int m_nSPOffset;
	BattleRecordTable()
	{
		m_nSPOffset = 0;
		m_nSP = 0;
		m_nHPOffset = 0;
		m_nHP = 0;
	}
};

//����������
struct CombatTable:public CCObject
{
	int m_nOccureFrame;//��������֡��
	int m_nWaveID;//����֡��
	float m_fWavePosX;//��������λ��
	float m_fWavePosY;
	int m_nDamScope;//�ܴ��̶�
	bool m_bKeepWave;//�������Ƿ���Ϊ���е���
	float m_fMoveOffsetX;//��ɫ���������ƶ�
	float m_fMoveOffsetY;//��ɫ���������ƶ�
	int m_nMoveDur; //�ƶ�����֡��
	CombatTable(int occureFrame,int waveID,float wavePosX,float wavePosY,int damScopet,bool bKeepWave,int dur,float moveX,float moveY);
};
class DuelLayer : public cocos2d::CCLayer
{
public:
	


    DuelLayer(BattleCharacter* pAttacker,BattleCharacter* pDefender,BattleRecordTable* pAttackerTable,BattleRecordTable* pDefenderTable,int DuelMode);
	~DuelLayer();
	//~ DuelLayer();
    //virtual void draw();
	//virtual bool init();
	virtual void onEnter();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
	virtual void onExit();

private:
	void CreateAtkCallFunc(CCNode *sender,void* tableData);
	void KillWaveCallFun(CCNode *sender,void* data);
	void EndPlayCallBack(CCNode *sender,void* data);
	void DefHitRecover(CCNode *sender,void* data);
	void AtkHitRecover(CCNode *sender,void* data);
	void CreateFightBackCallFunc(CCNode *sender,void* tableData);
	void FomatTableActions(CCArray* &charActions,CCArray* &shadowActions,int charID,int occureFrameOffset,SEL_CallFuncND callFunc,CCSprite* pChar);
	void RefreshHPCounter(int nFrom,int nTo,bool bLeft = false);
	CCSpriteBatchNode* m_pAttackerBatchNode;
	CCSprite* m_pAttacker;
	CCSprite* m_pAttackerShadow;
	CCSpriteBatchNode* m_pDefenderBatchNode;
	CCSprite* m_pDefender;
	CCSprite* m_pDefenderShadow;
	CCSprite* m_pBackGround;
	CCSprite* m_pPlayer;
	CCSprite* m_pEnermy;
	CCSprite* m_pPlayerShadow;
	CCSprite* m_pEnermyShadow;
    CCPoint m_tBeginPos;
    CCMenu* m_pItemMenu;
	CCSprite* m_pLeftBarBK;	
	CCSprite* m_pRightBarBK;
	CCProgressTimer* m_pLeftHP;
	CCProgressTimer* m_pLeftMP;
	CCProgressTimer* m_pRightHP;
	CCProgressTimer* m_pRightMP;
	CCSpriteBatchNode* m_pNumsBatchNode;
	CCSprite* m_pLeftHPCounter;
	CCSprite* m_pRightHPCounter;
	void  GetAtkTableFromID(int nID,int nLevel);
	CCArray* m_pCombatTable;
	int m_nToalDamScope;
	int m_nToalHits;
	int m_nDamage;
	int m_nHits;
	BattleCharacter* m_pAttackerChar;
	BattleCharacter* m_pDefenderChar;
	BattleCharacter* m_pPlayerChar;
	BattleCharacter* m_pEnermyChar;
	BattleRecordTable* m_pAttackerTable;
	BattleRecordTable* m_pDefenderTable;
	BattleRecordTable* m_pEnermyTable;
	BattleRecordTable* m_pPlayerTable;
	int m_nMode;
	void InitCounter(int nTag,float fStartX,float fStartY,unsigned nNum);
	void InitDuelPosition();
	void BeginActionChan();
	void BeginFightBackChan(CCNode *sender,void* data);
	void WaveCollisionUpdate(float time);
	void ShowNums(CCPoint &tStartPoint,int num);
	CCArray* m_pAttackerActions;
	CCArray* m_pAttackerShadowActions;
	CCArray* m_pDefenderActions;
	CCArray* m_pDefenderShadowActions;
	CCArray* m_pWaves;
};

#endif  
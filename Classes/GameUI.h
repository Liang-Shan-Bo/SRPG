#ifndef __GAME_UI_H__
#define __GAME_UI_H__
#include "cocos2d.h"
USING_NS_CC;
#include "SimpleAudioEngine.h"
#include "Singleton.h"
#include "BattleCharacter.h"
#include "BattleResources.h"
#include "slg_tools.h"
#include <vector>
using namespace std;

#define BATCH_TAG 8001
#define MAX_DIALOG_BUFFER_SIZE 4000 

class BattleMenu: public Singleton<BattleMenu>
{
private:
	BattleCharacter *m_pInvoker;
	CCLayer* m_pMapParent;
	CCSprite* m_pAtkIcon;
	/*CCSprite* m_pDefIcon;*/
	CCSprite* m_pMoveIcon;
	CCSprite* m_pSkillIcon;
	CCSprite* m_pStayIcon;
	CCSprite* m_pItemIcon;
	CCSprite* m_pBackIcon;
	CCSprite* m_pStatusIcon;
	CCSpriteBatchNode* m_pBatchNode;
	/*CCSpriteBatchNode* pLevelZeroBatchNode;
	CCSpriteBatchNode* pLevelOneBatchNode;*/
	int m_nMenuLevel;
	CCArray* m_pActivedIcons;	
	void InitActivedIconByFileName(const char* pFileName,int nTag,CCSprite** pIcon);
public:	
	BattleMenu();
	~BattleMenu();
	//bool AddChildCommand(int commandFlag,CCSprite* pIcon);
	void EnterTopMenu();
	void CheckTouchedCommand(CCPoint touchedPoint);
	void SetParent(CCLayer* pLayer);
	void SetInvoker(BattleCharacter * pInvoker);
	void ShowMenu();
	void HangMenu();
	bool Usable();
	void RemoveMoveCommand();
	void ResumeMoveCommand();
	void EnterChildMenu(vector<int> &vObjects,int nCommand);
	//void EnterChildMenu(CCArray* pBattleObejcts);
	bool Back();
	void Disconnect();
	void clear();
	//void AddAtkCommand();
	//void AddDefCommand();
	//void AddMoveCommand();
	//void AddSkillCommand();
	//void AddWaitCommand();
	//void AddCancelCommand();
};
class TurnPassLayer:public CCLayer
{
private:
	CCSpriteBatchNode* m_pBatchNode;
	CCSprite* m_pTurnLabel;
	CCSprite* m_pTurnMark;
	CCSprite* m_pPassTurnBar;
	CCSprite* m_pPassTurnBlink;
public:
	virtual void onEnter();
	TurnPassLayer(int nforceFlag);
	~TurnPassLayer();
};


class MainMenu:public CCLayer
{
private:
	CCSpriteBatchNode* m_pBatchNode;
	CCSprite* m_pEndGameLabel;//������Ϸ�˵�
	CCSprite* m_pSystemConfigLabel;//ϵͳ�趨�˵�
	CCSprite* m_pBackGround;//����
	CCSprite* m_pEndTurnLabel;//�����غϲ˵�
	CCSprite* m_pToVictoryLabel;//ʤ�������˵�
	CCSprite* m_pSaveLabel;//������Ϸ�˵�
	CCSprite* m_pLoadLabel;//������Ϸ�˵�
	CCSprite* m_pGoToAimLabel;//�о��˵�
	CCSprite* m_pMainMenuConponent;//���˵�����
	CCSprite* m_pBattleFieldStatusConponent;//��ʾ��ǰս��״̬���
	CCSprite* m_pButtonOk;//ȷ�ϰ�ť
	CCSprite* m_pButtonCancel;//ȡ����ť
	CCSprite* m_pButtonConponent;//ȷ�ϡ�ȡ����ť�ı���
	CCSprite* m_pOnSelect;//���ڱ�ѡȡ�Ĳ˵�
     void  CreateButton(int nOK_Tag,float x,float y);//����ȷ�Ϻ�ȡ����ť
	 void ClearButton();//�Ƴ�ȷ�Ϻ�ȡ����ť
public:
	/*virtual bool init();  
	virtual void update(float dt)*/;
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
	 MainMenu();
	~MainMenu(); 

};
class ShowStatusMenu:public CCLayer
{
private:
	BattleCharacter* m_pInvoker;
	CCSprite* m_pStatusBackGround;//��ʾ����״̬�ı���
	CCSprite* m_pItemBackGround;//��ʾ������Ʒ�ı���
	CCSprite* m_pSkillBackGround;//��ʾ���＼�ܱ���
public:
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
	ShowStatusMenu(BattleCharacter* pInvoker);
	~ShowStatusMenu();
};





class ShowRangeMenu:public CCLayer
{
private:
	CCSpriteBatchNode* m_pBatchNode;
	CCSprite* m_pBackGround;
	CCSprite* m_pSubRange;//���Ա���ק��һ����Χͼ��
	int m_nSubRange;
	bool m_bIsCommand;
	CCSprite* m_pReset;
	CCSprite* m_pOKButton;
	CCSprite* m_pCancelButton;
	void RefreshBackGround();
	void RefreshButton();
	CCPoint m_tSubPoint;
	BattleCharacter* m_pInvoker;
public:
	virtual void onEnter();
	ShowRangeMenu(BattleCharacter* pInvoker,int nSubRange,bool bIsCommand);
	~ShowRangeMenu();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent); 
	void Refresh();
};

class DialogMenu:public CCLayer
{
private:
	int m_nMode;
	int m_nReadStartCount;
	int m_nReadPassCount;
	bool m_bPause;
	bool m_bEnd;
	bool m_bPageDown;
	CCSprite* m_pMenu;
	CCSprite* m_pLeft;
	CCSprite* m_pRight;
	char m_chWordBuffer[MAX_DIALOG_BUFFER_SIZE];
	CCSpriteBatchNode* m_pBatchNode;
	//CCLabelBMFont* m_pDialogContent;
	CCLabelTTF* m_pDialogContent;
	void menuSkipCallback(CCObject* pSender);

	void ShiftDialog();
	void ReadUpdate(float fTime);
    void Pause();
	void RefreshImage();
public:
	DialogMenu();
	~DialogMenu();
protected: 
	virtual void onEnter();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchesMoved(CCTouch *pTouch,CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
};

void MenuHide(CCLayer* menu);

#endif
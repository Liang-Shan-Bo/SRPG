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
	CCSprite* m_pEndGameLabel;//结束游戏菜单
	CCSprite* m_pSystemConfigLabel;//系统设定菜单
	CCSprite* m_pBackGround;//背景
	CCSprite* m_pEndTurnLabel;//结束回合菜单
	CCSprite* m_pToVictoryLabel;//胜利条件菜单
	CCSprite* m_pSaveLabel;//储存游戏菜单
	CCSprite* m_pLoadLabel;//载入游戏菜单
	CCSprite* m_pGoToAimLabel;//行军菜单
	CCSprite* m_pMainMenuConponent;//主菜单背景
	CCSprite* m_pBattleFieldStatusConponent;//显示当前战场状态面板
	CCSprite* m_pButtonOk;//确认按钮
	CCSprite* m_pButtonCancel;//取消按钮
	CCSprite* m_pButtonConponent;//确认、取消按钮的背景
	CCSprite* m_pOnSelect;//正在被选取的菜单
     void  CreateButton(int nOK_Tag,float x,float y);//创建确认和取消按钮
	 void ClearButton();//移除确认和取消按钮
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
	CCSprite* m_pStatusBackGround;//显示人物状态的背景
	CCSprite* m_pItemBackGround;//显示人物物品的背景
	CCSprite* m_pSkillBackGround;//显示人物技能背景
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
	CCSprite* m_pSubRange;//可以被拖拽的一个范围图标
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
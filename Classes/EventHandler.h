#ifndef  _EVENT_HANDLER_
#define  _EVENT_HANDLER_
#include "Singleton.h"
#include "BattleMapScene.h"
#include "BattleCharacter.h"
#include "StoryScene.h"
#include <vector>
#include <string>
#include <algorithm>
#include "tinyxml2.h"
#include "GameUI.h"
using namespace tinyxml2;
using namespace std;
class EventHandler:public Singleton<EventHandler>
{
	IntGetter(SceneID);
private:
	void CheckBattleStartEvent();
	void CheckStoryStartEvent();
	bool CheckCharacterArrival();
	vector<string> m_vBattleEventNameKeys;
	StoryScene* m_pEventStory;
	BattleMap* m_pEventBattle;
	BattleCharacter* m_pEventCharacter;
	string m_strSceneMode;
	string m_strDialogFileName;
	tinyxml2::XMLDocument* m_pLocalDocument;
	XMLElement* m_pEntry;
	XMLElement* m_pSelect;
	XMLElement* m_pEvent;
	XMLElement* m_pOcurrenceEvent;
	bool m_bIsRunningEvent;
	bool m_bContinueBattle;
	void MoveToNextEntry();
	void SearchEventSelect();
	void UnlockEvent();
	void LockEvent();
	void RefreshArrivalShowOnMap();
	bool IsOcurrenceLocked();
	void HandleBattle();
	void HandleStory();
public:
	EventHandler()
	{
		m_strSceneMode = "Start";
		m_pEvent = NULL;
		m_pEntry =NULL;
		m_pSelect = NULL;
		m_pOcurrenceEvent = NULL;
		m_nSceneID = 1;
		m_vBattleEventNameKeys.clear();
		m_bIsRunningEvent = false;
	}
	void init();
	const char* GetDialogFileName();
	void GetPlayerTurnBGMName(string& strName);
	void GetEnermyTurnBGMName(string& strName);
	void GetDefaultBGMName(string& strName); 
	~EventHandler()
	{
		delete m_pLocalDocument;
	}
	void OnBattleStart();
	void OnStoryStart();
	void OnCharacterRemoveFromBattle(BattleCharacter* pCharacter);
	void OnCharacterActionEnd(BattleCharacter* pCharacter);
	void OnDialogEnd();
	void OnBattleTurnBegin();
	void FomatNextScene();
	void InitBattleCharacterOnTurnBegin();
	bool IsEventRunning();
	void Save(tinyxml2::XMLDocument* pDoc);
	bool Load(tinyxml2::XMLDocument* pDoc);
};
#endif
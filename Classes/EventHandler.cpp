#include "EventHandler.h"
#include "StartScene.h"
#include "PlayerDataManager.h"
DECLARE_SINGLETON_MEMBER(EventHandler)
#define MAX_EVENET_HANDLER_BUFFER 40000

void EventHandler::OnBattleStart(  )
{
	CheckBattleStartEvent();
}

void EventHandler::CheckBattleStartEvent()
{
	m_pEventBattle->getParent()->addChild(new DialogMenu());
	
}

void EventHandler::FomatNextScene()
{
	MoveToNextEntry();
	CCScene* pScene =NULL;
	if(m_strSceneMode =="Battle")
	{
		pScene = BattleMap::InitBattleMapScene();
		m_pEventBattle = (BattleMap*)pScene->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
	}
	else if (m_strSceneMode == "Story")
	{
		pScene = StoryScene::scene();
		m_pEventStory = (StoryScene*)pScene->getChildByTag(LAYER_STORY_LAYER_ID);
		
	}
	else if (m_strSceneMode == "Start")
	{
		pScene = StartNow::scene();
	}

	if (pScene!=NULL)
	{
		//清空带锁事件的钥匙
		m_vBattleEventNameKeys.clear();
		m_bIsRunningEvent = false;
		if(CCDirector::sharedDirector()->getRunningScene()!=NULL)
		{
			 CCDirector::sharedDirector()->replaceScene(pScene);
		}
		else
		{
			CCDirector::sharedDirector()->runWithScene(pScene);
		}
		
	}
	
}

void EventHandler::init()
{
	m_pLocalDocument = new tinyxml2::XMLDocument();
	char dataPath[50];
	sprintf(dataPath,"%s",EVENTHANDLER_DOCUMENT_FULLPATH);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(dataPath, "rb", &bufferSize);
	char pCBuffer[MAX_EVENET_HANDLER_BUFFER];
	pCBuffer[0] ='\0';
	sprintf(pCBuffer,"%s%s",(char*)pBuffer,'\0');
	if (pBuffer)
	{
		XMLError bError= m_pLocalDocument->Parse((const char*)pCBuffer,bufferSize);
		assert(bError==XML_NO_ERROR);
	}
	delete(pBuffer);
	m_pEntry = m_pLocalDocument->RootElement()->FirstChildElement("Entries")->FirstChildElement("Entry");
	assert(m_pEntry != NULL);
	m_pSelect = m_pEntry->FirstChildElement("Select");
	m_pEvent = m_pEntry->FirstChildElement("Event");
}

void EventHandler::MoveToNextEntry()
{
	if (m_pSelect==NULL)
	{
		m_pSelect = m_pEntry->FirstChildElement("Select");
		assert(m_pSelect != NULL);
	}
	int nNextEntryID = 1;
	m_pSelect->QueryIntAttribute("NextID",&nNextEntryID);
	XMLElement* pNextEntry = m_pEntry->Parent()->FirstChildElement("Entry");
	if (pNextEntry == NULL)
	{
		pNextEntry = pNextEntry->Parent()->FirstChildElement("Entry");
		CCLog("Entry Missing!");
	}

	while(pNextEntry!=NULL)
	{
		int nEntryId =1;
		pNextEntry->QueryIntAttribute("ID",&nEntryId);
		if (nEntryId==nNextEntryID)
		{
			break;
		}
		pNextEntry = pNextEntry->NextSiblingElement("Entry");
	}
	if (pNextEntry ==NULL)
	{
		return;
	}
	m_pEntry = pNextEntry;
	string strName = m_pSelect->Attribute("Name");
	if (m_strSceneMode == "Battle"&&!(strName=="GameOver"))
	{
		m_pEventBattle->BattleClear();
	}
	m_pSelect = m_pEntry->FirstChildElement("Select");
	m_strSceneMode = m_pEntry->Attribute("SceneType");
	XMLError bResult = m_pEntry->QueryIntAttribute("SceneID",&m_nSceneID);
	assert(m_strSceneMode.data()!=NULL);
	assert(bResult==XML_NO_ERROR);
}

void EventHandler::OnDialogEnd()
{
	string strTransform ;
	strTransform = m_pEvent->Attribute("Transform");
	if (strTransform =="True"||strTransform=="true")
	{
		FomatNextScene();
	}
	else 
	{
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(LAYER_DIALOG_LAYER_ID,true);
		m_bIsRunningEvent = false;
		if (m_bContinueBattle)
		{
			m_pEventBattle->ContinueAfterCharacterAcition();
		}
		
	}
}	

void EventHandler::OnCharacterRemoveFromBattle( BattleCharacter* pCharacter )
{
	m_bContinueBattle =false;
	XMLElement* pEvent =m_pEntry->FirstChildElement("Event");
	m_pEventCharacter = pCharacter;
	while(pEvent!=NULL)
	{	
		XMLElement* pOcurrence = pEvent->FirstChildElement("Ocurrence");
		assert(pOcurrence != NULL);
		XMLElement* pCharacterDefeatedEvent =pOcurrence->FirstChildElement("CharacterDefeated");
		while(pCharacterDefeatedEvent!=NULL)
		{
			int nCharacterID = -1;
			pCharacterDefeatedEvent->QueryIntAttribute("CharacterID",&nCharacterID);
		    if (nCharacterID!=pCharacter->GetCharID())
			{
				
			}
			else
			{
				m_pEvent = pEvent;
				m_pOcurrenceEvent  = pCharacterDefeatedEvent;
				HandleBattle();
				return;
			}
			pCharacterDefeatedEvent = pCharacterDefeatedEvent->NextSiblingElement("CharacterDefeated");
			
		}
		pEvent = pEvent->NextSiblingElement("Event");
	}
}

const char* EventHandler::GetDialogFileName()
{
	char chFileName[50] = {0};
	XMLElement* pDialog = m_pOcurrenceEvent->FirstChildElement("Dialog");
	if (pDialog == NULL)
	{
		return NULL;
	}
	m_strDialogFileName = pDialog->Attribute("FileName");
	return m_strDialogFileName.c_str();
}

void EventHandler::OnStoryStart()
{
	CheckStoryStartEvent();
}

void EventHandler::CheckStoryStartEvent()
{
	XMLElement* pEvent = m_pEntry->FirstChildElement("Event");
	while(pEvent!=NULL)
	{	
		XMLElement* pOcurrence = pEvent->FirstChildElement("Ocurrence");
		assert(pOcurrence != NULL);
		XMLElement* pStartEvent = pOcurrence->FirstChildElement("SceneStart");
		while (pStartEvent!=NULL)
		{
				if (pStartEvent == NULL)
				{
					continue;
				}
				else
				{
					m_pEvent = pEvent;
					m_pOcurrenceEvent  = pStartEvent;
					HandleStory();
					return;
				}	
			pStartEvent = pOcurrence->NextSiblingElement("SceneStart");
		}
		pEvent = pEvent->NextSiblingElement("Event");		
	}
}

void EventHandler::SearchEventSelect()
{
	const char* chSelect = m_pEvent->Attribute("Select");
	if(chSelect!=NULL)
	{
		string strSelect =chSelect;
		XMLElement* pSelect = m_pEntry->FirstChildElement("Select");
		while (pSelect!=NULL)
		{
			if (strSelect == pSelect->Attribute("Name"))
			{
				m_pSelect = pSelect;
				return;
			}
			pSelect = pSelect->NextSiblingElement("Select");
		}
	}
}

void EventHandler::OnBattleTurnBegin()
{
	m_bContinueBattle = true;
	InitBattleCharacterOnTurnBegin();
	XMLElement* pEvent = m_pEntry->FirstChildElement("Event");
	while(pEvent!=NULL)
	{	
		XMLElement* pOcurrence = pEvent->FirstChildElement("Ocurrence");
		assert(pOcurrence != NULL);
		XMLElement* pTurnBeginEvent = pOcurrence->FirstChildElement("TurnBegin");
		while (pTurnBeginEvent!=NULL)
		{
			
				int nTurn = -1;
				pTurnBeginEvent->QueryIntAttribute("Turn",&nTurn);
				if(nTurn!=m_pEventBattle->GetTurns())
				{
				
				}
				else
				{
					m_pEvent = pEvent;
					m_pOcurrenceEvent  = pTurnBeginEvent;
					HandleBattle();
					return;
				}	
			
			pTurnBeginEvent = pOcurrence->NextSiblingElement("TurnBegin");
		}
		pEvent = pEvent->NextSiblingElement("Event");
	}
	//没有事件发生
	m_pEventBattle->ContinueAfterCharacterAcition();
}

void EventHandler::InitBattleCharacterOnTurnBegin()
{
	//通过tmx地图数据初始化角色
	CCArray* characterDices = CCArray::createWithArray(m_pEventBattle->SearchArrayFromTMXLayer("character"));
	for(unsigned int index = 0;index<characterDices->count();index++)
	{
		CCDictionary* dic =  (CCDictionary*)characterDices->objectAtIndex(index);
		CCObject* pTurn =dic->objectForKey("Turn");
		int nTurn = -1;
		if (pTurn!=NULL)
		{	
			nTurn = ((CCString*)pTurn)->intValue();
		}
		
		if (nTurn == m_pEventBattle->GetTurns())
		{
			CCArray* pUnits =m_pEventBattle->GetUnits();
			int nCharID = ((CCString*)dic->objectForKey("ID"))->intValue();
			BattleCharacter* pUnit  = NULL;
			if (dic->objectForKey("Partner")!=NULL)
			{
				pUnit = PlayerDataManager::instance()->GetPartnerFromCharID(nCharID);
			}
			if (pUnit != NULL)
			{
				int x = ((CCString*)dic->objectForKey("x"))->intValue()+SQUARE_LENTH/2;
				int y = ((CCString*)dic->objectForKey("y"))->intValue()+SQUARE_LENTH/2;
				pUnit->InitSpritesAndStatus(x,y);
			}
			else
			{
				pUnit = new BattleCharacter();
				pUnit->BuildCharacter(dic);
			}
			pUnits->addObject(pUnit);
		}	
	}
}

void EventHandler::OnCharacterActionEnd( BattleCharacter* pCharacter )
{
	if (m_bIsRunningEvent)
	{
		CCLog("Another event is running!");
		return;
	}
	m_bContinueBattle = true;
	m_pEventCharacter = pCharacter;
	if(!CheckCharacterArrival())
	{
		m_pEventBattle->ContinueAfterCharacterAcition();
	}

}

bool EventHandler::CheckCharacterArrival()
{
	bool bResult = false;
	XMLElement* pEvent = m_pEntry->FirstChildElement("Event");
	while(pEvent!=NULL)
	{	
		XMLElement* pOcurrence = pEvent->FirstChildElement("Ocurrence");
		assert(pOcurrence != NULL);
		XMLElement* pArrivalEvent = pOcurrence->FirstChildElement("Arrival");
		while (pArrivalEvent!=NULL)
		{
			int nCharacterID = -1;
		/*	pArrivalEvent->QueryIntAttribute("CharacterID",&nCharacterID);
			(nCharacterID==m_pEventCharacter->GetCharID());*/
			bResult = false;
			CCArray* pEventDices = m_pEventBattle->SearchArrayFromTMXLayer("event");
			for(unsigned int index = 0;index<pEventDices->count();index++)
			{
				CCDictionary* dic =  (CCDictionary*)pEventDices->objectAtIndex(index);
				CCString* pEventKey =(CCString*)dic->objectForKey("GoalEvent");
				if (pEventKey==NULL)
				{
					continue;
				}	
				float x = ((CCString*)dic->objectForKey("x"))->floatValue()+SQUARE_LENTH/2;
				float y = ((CCString*)dic->objectForKey("y"))->floatValue()+SQUARE_LENTH/2;
				bResult = (x==m_pEventCharacter->GetMapPositionX()&&y==m_pEventCharacter->GetMapPositionY());
				if (bResult)
				{
					m_pEvent = pEvent;
					m_pOcurrenceEvent  = pArrivalEvent;
					HandleBattle();
					return bResult;
				}
			}	
			pArrivalEvent = pOcurrence->NextSiblingElement("TurnBegin");
		}
		pEvent = pEvent->NextSiblingElement("Event");
	}
	return bResult;
}

void EventHandler::UnlockEvent()
{
	XMLElement* pToUnLock = m_pOcurrenceEvent->FirstChildElement("ToUnlock");
	string key;
	while (pToUnLock!=NULL)
	{
		key = pToUnLock->GetText();
		vector<string>::iterator it;
		it = find(m_vBattleEventNameKeys.begin(),m_vBattleEventNameKeys.end(),key);
		if (m_vBattleEventNameKeys.end()==it)
		{
			m_vBattleEventNameKeys.push_back(key);
		}	
		pToUnLock = pToUnLock->NextSiblingElement("ToUnlock");
	}
}

void EventHandler::LockEvent()
{
	XMLElement* pToLock = m_pOcurrenceEvent->FirstChildElement("ToLock");
	string key;
	while (pToLock!=NULL)
	{
		key = pToLock->GetText();
		vector<string>::iterator it;
		it = find(m_vBattleEventNameKeys.begin(),m_vBattleEventNameKeys.end(),key);
		if (m_vBattleEventNameKeys.end()!=it)
		{
			m_vBattleEventNameKeys.erase(it);
		}	
		pToLock = pToLock->NextSiblingElement("ToLock");
	}
}

void EventHandler::RefreshArrivalShowOnMap()
{
	m_pEventBattle->RemoveAllEventMarks();
	CCArray* pEventDices = CCArray::createWithArray(m_pEventBattle->SearchArrayFromTMXLayer("event"));
	for(unsigned int index = 0;index<pEventDices->count();index++)
	{
		CCDictionary* dic =  (CCDictionary*)pEventDices->objectAtIndex(index);
		CCString* pEventKey =(CCString*)dic->objectForKey("ShowEvent");
		if (pEventKey==NULL)
		{
			continue;
		}
		vector<string>::iterator it;
		it = find(m_vBattleEventNameKeys.begin(),m_vBattleEventNameKeys.end(),pEventKey->m_sString);
		if (m_vBattleEventNameKeys.end()!=it)
		{
			float x = ((CCString*)dic->objectForKey("x"))->floatValue();
			float y = ((CCString*)dic->objectForKey("y"))->floatValue();
			m_pEventBattle->SetEventMark(new CCPoint(x,y));
		}	
	}	
	m_pEventBattle->DrawEventMarks();
}

void EventHandler::HandleBattle()
{
	if (!IsOcurrenceLocked())
	{
		if (m_bIsRunningEvent)
		{
			CCLog("Another event is running!");
			return;
		}
		m_bIsRunningEvent = true;
		SearchEventSelect();
		LockEvent();
		UnlockEvent();
		RefreshArrivalShowOnMap();
		m_pEventBattle->getParent()->addChild(new DialogMenu(),MAX_LAYER_ZORDER,LAYER_DIALOG_LAYER_ID);
	}
}

bool EventHandler::IsOcurrenceLocked()
{
	XMLElement* pLocked = m_pOcurrenceEvent->FirstChildElement("Locked");
	if (pLocked==NULL)
	{
		return false;
	}
	const char* chName =  m_pOcurrenceEvent->Attribute("Name");
	if (chName == NULL)
	{
		CCLOG("Ocurrence Name not found");
		return true;
	}
	string strName = chName;
	vector<string>::iterator it;
	it = find(m_vBattleEventNameKeys.begin(),m_vBattleEventNameKeys.end(),chName);
	if (m_vBattleEventNameKeys.end()!=it)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void EventHandler::HandleStory()
{
	if (!IsOcurrenceLocked())
	{
		SearchEventSelect();
		LockEvent();
		UnlockEvent();
		m_pEventStory->getParent()->addChild(new DialogMenu(),MAX_LAYER_ZORDER,LAYER_DIALOG_LAYER_ID);
	}
}

bool EventHandler::IsEventRunning()
{
	return m_bIsRunningEvent
		;
}

void EventHandler::GetEnermyTurnBGMName( string& strName )
{
	XMLElement* pMusic = m_pEntry->FirstChildElement("Music");
	if(pMusic==NULL)
	{
		return;
	}
	XMLElement* pEnermyTurnBGM = pMusic->FirstChildElement("Enermy");
	char chBGM[50] = {0};
	sprintf(chBGM,"%s%s",MUSIC_FILE_PATH_NAME,pEnermyTurnBGM->GetText());
	if (chBGM!=NULL)
	{
		strName = chBGM;
	}
}

 void EventHandler::GetPlayerTurnBGMName( string& strName )
{
	XMLElement* pMusic = m_pEntry->FirstChildElement("Music");
	if(pMusic==NULL)
	{
		return;
	}
	XMLElement* pPlayerTurnBGM = pMusic->FirstChildElement("Player");
	char chBGM[50] = {0};
	sprintf(chBGM,"%s%s",MUSIC_FILE_PATH_NAME,pPlayerTurnBGM->GetText());
	if (chBGM!=NULL)
	{
		strName = chBGM;
	}
}

 void  EventHandler::Save( tinyxml2::XMLDocument* pDoc )
 {
	XMLElement* pRootElement = pDoc->RootElement();
	if (pDoc == NULL)
	{
		return;
	}
	//储存场景入口ID
	XMLElement* pEntry =pDoc->NewElement("Entry");
	pEntry->SetAttribute("ID",m_pEntry->Attribute("ID"));
	pRootElement->InsertFirstChild(pEntry);
	//储存事件key
	XMLElement* pKeys = pDoc->NewElement("Keys");
	pRootElement->InsertEndChild(pKeys);
	for (vector<string>::iterator key = m_vBattleEventNameKeys.begin();key!=m_vBattleEventNameKeys.end();key++)
	{
		XMLElement* pKey = pDoc->NewElement("Key");
		pKey->SetValue(key->c_str());
		pKeys->InsertEndChild(pKey);
	}
 }

 bool EventHandler::Load( tinyxml2::XMLDocument* pDoc )
 {
	 XMLElement* pRootElement = pDoc->RootElement();
	 if (pDoc == NULL)
	 {
		 return false;
	 }
	 //载入场景入口
	 XMLElement* pEntry =pRootElement->FirstChildElement("Entry");
	 if (pEntry==NULL)
	 {
		 return false;
	 }
	 int nFindID = 1;
	 XMLError nResult = pEntry->QueryIntAttribute("ID",&nFindID);
	 if (nResult!=XML_NO_ERROR)
	 {
		 return false;
	 }
	 m_pEntry = m_pEntry->Parent()->FirstChildElement("Entry");
	 while(m_pEntry!=NULL)
	 {
		int nID = 1;
		nResult = m_pEntry->QueryIntAttribute("ID",&nID);
		if (nResult!=XML_NO_ERROR)
		{
			CCLOG("Handler Entry ID missing!");
			return false;
		}
		if (nID == nFindID)
		{
			break;
		}
		m_pEntry = m_pEntry->NextSiblingElement("Entry");
	 }
	 if (m_pEntry==NULL)
	 {
		 CCLOG("No saved Entry ID be found!");
		 return false;
	 }
	 m_pSelect = m_pEntry->FirstChildElement("Select");
	 m_strSceneMode = m_pEntry->Attribute("SceneType");
	 nResult = m_pEntry->QueryIntAttribute("SceneID",&m_nSceneID);
	 assert(m_strSceneMode.data()!=NULL);
	 assert(nResult==XML_NO_ERROR);	
	 CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
	 if(m_strSceneMode =="Battle")
	 {
		 m_pEventBattle = (BattleMap*)pScene->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
		 if (m_pEventBattle == NULL)
		 {
			 return false;
		 }
	 }
	 else if (m_strSceneMode == "Story")
	 {
		 m_pEventStory = (StoryScene*)pScene->getChildByTag(LAYER_STORY_LAYER_ID);
		 if (m_pEventStory == NULL)
		 {
			 return false;
		 }

	 }
	 else
	 {
		 return false;
	 }
	 //加载事件key
	 XMLElement* pKeys = pRootElement->FirstChildElement("Keys");
	 XMLElement* pKey  = pKeys->FirstChildElement("Key");
	 if (!m_vBattleEventNameKeys.empty())
	 {
		 m_vBattleEventNameKeys.clear();
	 }
	 while(pKey!=NULL)
	 {
		 string key = pKey->GetText();
		m_vBattleEventNameKeys.push_back(key);
	 }
	 return true;
 }

 void EventHandler::GetDefaultBGMName( string& strName )
 {
	 XMLElement* pMusic = m_pEntry->FirstChildElement("Music");
	 if(pMusic==NULL)
	 {
		 return;
	 }
	 XMLElement* pDefualtTurnBGM = pMusic->FirstChildElement("Default");
	 char chBGM[50] = {0};
	 sprintf(chBGM,"%s%s",MUSIC_FILE_PATH_NAME,pDefualtTurnBGM->GetText());
	 if (chBGM!=NULL)
	 {
		 strName = chBGM;
	 }
 }



#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "BattleCharacter.h"
#include "SimpleAudioEngine.h"
#include "BattleResources.h"
#include "DuelScene.h"

//������������Ļ�ȡ�ж�Ȩ�Ļغ�����
#define TURN_CYCLE 2
//����Battle�ж��ٺϲ�ͼƬ�Լ�����ͼƬ��Ҫ����
#define MAX_BATTLEMAP_LOADFRAMES_COUNT 46 
#define MAX_BATTLEMAP_LOADIMAGES_COUNT 4

//#define CHAR_ARRAY_SEL(Flag,Array) if(Flag == FORCE_PLAYER)\
//		{\
//			Array = m_pPlayerUnits;\
//		}\
//		else if(Flag == FORCE_NPC)\
//		{\
//			Array = m_pNPCUnits;\
//		}\
//		else if(Flag == FORCE_ENERMY)\
//		{\
//			Array = m_pEnermyUnits;\
//		}\

#define LOAD_PLIST_CALLBACK(_funcName_,_chFile_,_nID_)  \
public: void Load##_funcName_##CALLBACK(CCObject* pTexture)\
{\
	m_nLoadedFrames++;\
	char chPathStr[50] = {0};\
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();\
	if(_nID_==0)\
	{\
	sprintf(chPathStr,"%s.plist",_chFile_);\
	}\
	else\
	{\
		sprintf(chPathStr,"%s%d.plist",_chFile_,_nID_);\
	}\
	pCache->addSpriteFramesWithFile(chPathStr,(CCTexture2D*)pTexture);\
	if(m_nLoadedFrames==MAX_BATTLEMAP_LOADFRAMES_COUNT-4)\
	{\
		InitBattleSceneImageResoures();\
	}\
}



USING_NS_CC;

class BattleMap : public cocos2d::CCLayer
{
IntGetter(ID);
IntGetter(Turns);
public:
   // void PauseTurnLogoSEL(float time);
	void LockCharacterView(BattleCharacter* pChar);//����������ɫ�ӽ�
	void UnLockCharacterView();//������ɫ�ӽ�
	void ViewLockUpdate(float time);//������ɫ�ӽǵĸ��º���
	void ViewLock(BattleCharacter* pChar);//������ɫ�ӽǣ��ú�����ViewLockUpdata�в��ϵ���
	void SearchCharacterAndViewLock(int nCharId);
	void TurnBegin(CCNode *sender,void* data);//���غϽ���ͼ�꾭��ʱ
    virtual bool init();  //��ʼ����������������������Դ�ĺ�������ʼ������
	virtual void update(float dt);
	string m_strPlayerTurnMusic;
	string m_strEnermyTurnMusic;
    BattleMap();
	~BattleMap();
    static CCScene* InitBattleMapScene();//��ʼ��ս��������
	static CCScene* LoadBattleMapScene();//��ս�����������
	virtual void draw();//ʹ��OPENGL����ͼ�񣬵�ǰ��������ƽ�ɫ�ļ��� ���� �ƶ��ķ�Χ
	void DrawEventMarks();//�����¼��ص�
	void AcceptShowRangeObject(BattleCharacter* pObject);//����һ����Ҫ��ʾ��Χ�Ľ�ɫ
	void KickShowRangeObject(BattleCharacter* pObject);//����ɫ�Ƴ���ʾ��Χ�Ķ���
	/************************************************************************/
	/*    ���㴥������ʵ�ֵ��麯��                            */
	/************************************************************************/
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent); 
	void SetCostFromMap(BattleCharacter *pObject);//����ǰ��ͼ����ת��Ϊ��ɫ�ƶ���Χ���ݵ�cost
	
	/************************************************************************/
	/* Ѱ�ҵ�ͼ������Ϊ(x,y)�Ľ�ɫ����bIdentify��ʶ�Ƿ���Ҫʶ���ɫ��������,ֵΪtrueʱbSame����Ч����ֵbSameΪtrueʱ�����ҵĽ�ɫ��һ�¡�*/                                                                    
	/************************************************************************/
	BattleCharacter*  SearchUnitByPosition(int forceFlag,int x,int y,bool bIdentify,bool bSame); 
	CCArray* GetUnits();//��ȡ��ͼ�ϵ����н�ɫ����
    CREATE_FUNC(BattleMap);//CCLayer����Ĵ�����
	void EndTurn(CCNode* pSender,void *data);//������ǰ�غ�
	int GetTurnFlag();	//��ȡ��ǰ�غϵ�������ʶ
	void AIActionsChan();//ִ��Ai�Ķ�����
	void ContinueAfterCharacterAcition();
	bool IsShowRange();//�Ƿ�����ʾ��Χ
	void SaveBattle(); //ս����Ϸ���ݴ浵
	void LoadBattle();//����
	void TimingPlayerTurn();
	void TimingGameOver();
	void BattleClear();	
	void SetEventMark(CCPoint* pPoint);
	void RemoveAllEventMarks();
	CCDictionary* SearchTMXObject(const char* pStr);
	CCArray* SearchArrayFromTMXLayer(const char* pStr);
	void onExit();
private:
	bool m_bLoadInit;//�Ƿ��ڶ���״̬��ʼ��ս��
	CCArray* m_pShowRangeObjects;
	void InitGame();
	BattleCharacter* m_pViewLocker;
	unsigned int m_AIModeIndex ;
	CCTMXTiledMap* m_pTMXTiledMap;
    CCPoint m_tBeginPos;
	bool m_bIsToucheMoved;
    CCMenu* m_pItemMenu;
	CCArray* m_pUnits;
	vector<CCPoint*> m_vEventMarks;
	CCSprite* m_pLoading;
	CCSpriteBatchNode* m_pNumsBatchNode;
	CCSpriteBatchNode* m_pSquareBatchNode;
	//CCArray* m_pPlayerUnits;
	//CCArray* m_pEnermyUnits;
	//CCArray* m_pNPCUnits;
	int m_nPlayerForceCount;
	int m_nEnermyForceCount;
	int m_nNPCEForceCount;
	int m_nForceTurnFlag;
	int m_nLoadedFrames;
	int m_nLoadedImages;
	float m_fMinPositionX;
	float m_fMinPositionY;
	float m_fMaxPositionX;
	float m_fMaxPositionY;
	void InitBattleSceneImageResoures();
	void InitBattleSceneFrameResoures();
	void DrawSquareLines();
	void DrawRangeRect(CCArray* pRanges,int offsetX,int offsetY,ccColor4F color);
	void DrawRanges();
	/*void InvokeEndTurnMenu();*/
	void ResumeAllCharacters();
	void LoadImageCallBack(CCObject* pTexture);
	void RunLoading();
	/************************************************************************/
	/*                           �첽����plist�Ļص�����                    */
	/************************************************************************/
	//toal 46
	LOAD_PLIST_CALLBACK(Mapchar1,MAPCHAR_FILE_FULLPATH,1)
	LOAD_PLIST_CALLBACK(Mapchar2,MAPCHAR_FILE_FULLPATH,2)
	LOAD_PLIST_CALLBACK(Mapchar3,MAPCHAR_FILE_FULLPATH,3)
	LOAD_PLIST_CALLBACK(Mapchar4,MAPCHAR_FILE_FULLPATH,4)
	LOAD_PLIST_CALLBACK(Mapchar5,MAPCHAR_FILE_FULLPATH,5)
	LOAD_PLIST_CALLBACK(Mapchar6,MAPCHAR_FILE_FULLPATH,6)
	LOAD_PLIST_CALLBACK(Mapchar7,MAPCHAR_FILE_FULLPATH,7)
	LOAD_PLIST_CALLBACK(Mapchar8,MAPCHAR_FILE_FULLPATH,8)
	LOAD_PLIST_CALLBACK(Mapchar9,MAPCHAR_FILE_FULLPATH,9)
	LOAD_PLIST_CALLBACK(Mapchar10,MAPCHAR_FILE_FULLPATH,10)
	LOAD_PLIST_CALLBACK(Mapchar11,MAPCHAR_FILE_FULLPATH,11)
	LOAD_PLIST_CALLBACK(Mapchar12,MAPCHAR_FILE_FULLPATH,12)
	LOAD_PLIST_CALLBACK(Mapchar13,MAPCHAR_FILE_FULLPATH,13)
	LOAD_PLIST_CALLBACK(Mapchar14,MAPCHAR_FILE_FULLPATH,14)
	LOAD_PLIST_CALLBACK(Mapchar15,MAPCHAR_FILE_FULLPATH,15)
	LOAD_PLIST_CALLBACK(Mapchar16,MAPCHAR_FILE_FULLPATH,16)
	LOAD_PLIST_CALLBACK(Mapchar17,MAPCHAR_FILE_FULLPATH,17)
	LOAD_PLIST_CALLBACK(Mapchar18,MAPCHAR_FILE_FULLPATH,18)
	LOAD_PLIST_CALLBACK(Duelchar1,DUEL_CHAR_FILE_FULLPATH,1)
	LOAD_PLIST_CALLBACK(Duelchar2,DUEL_CHAR_FILE_FULLPATH,2)
	LOAD_PLIST_CALLBACK(Duelchar3,DUEL_CHAR_FILE_FULLPATH,3)
	LOAD_PLIST_CALLBACK(Duelchar4,DUEL_CHAR_FILE_FULLPATH,4)
	LOAD_PLIST_CALLBACK(Duelchar5,DUEL_CHAR_FILE_FULLPATH,5)
	LOAD_PLIST_CALLBACK(Duelchar6,DUEL_CHAR_FILE_FULLPATH,6)
	LOAD_PLIST_CALLBACK(Duelchar7,DUEL_CHAR_FILE_FULLPATH,7)
	LOAD_PLIST_CALLBACK(Duelchar8,DUEL_CHAR_FILE_FULLPATH,8)
	LOAD_PLIST_CALLBACK(Duelchar9,DUEL_CHAR_FILE_FULLPATH,9)
	LOAD_PLIST_CALLBACK(Duelchar10,DUEL_CHAR_FILE_FULLPATH,10)
	LOAD_PLIST_CALLBACK(Duelchar11,DUEL_CHAR_FILE_FULLPATH,11)
	LOAD_PLIST_CALLBACK(Duelchar12,DUEL_CHAR_FILE_FULLPATH,12)
	LOAD_PLIST_CALLBACK(Duelchar13,DUEL_CHAR_FILE_FULLPATH,13)
	LOAD_PLIST_CALLBACK(Duelchar14,DUEL_CHAR_FILE_FULLPATH,14)
	LOAD_PLIST_CALLBACK(Duelchar15,DUEL_CHAR_FILE_FULLPATH,15)
	LOAD_PLIST_CALLBACK(Duelchar16,DUEL_CHAR_FILE_FULLPATH,16)
	LOAD_PLIST_CALLBACK(Duelchar17,DUEL_CHAR_FILE_FULLPATH,17)
	LOAD_PLIST_CALLBACK(Duelchar18,DUEL_CHAR_FILE_FULLPATH,18)
	LOAD_PLIST_CALLBACK(MapSkill1,MAP_SKILL_FILE_FULLPATH,1);
	LOAD_PLIST_CALLBACK(MapSkill2,MAP_SKILL_FILE_FULLPATH,2);
	LOAD_PLIST_CALLBACK(MapSkill3,MAP_SKILL_FILE_FULLPATH,3);
	LOAD_PLIST_CALLBACK(MapSkill4,MAP_SKILL_FILE_FULLPATH,4);
	LOAD_PLIST_CALLBACK(MapCharacterVanish,MAP_CHARACTER_VANISH_FILE_FULLPATH,0);
	LOAD_PLIST_CALLBACK(DuelWave,DUEL_WAVE_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(Nums,NUMS_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(RangeMenu,RANGEMENU_LAYER_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(MainMenu,MAINMENU_MENU_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(BattleMenu,BATTLEMENU_MENU_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(TurnPass,TURNPASS_LAYER_FILE_FULLPATH,0)
	LOAD_PLIST_CALLBACK(DialogMenu,DIALOG_FILE_FULLPATH,0)
};

#endif  
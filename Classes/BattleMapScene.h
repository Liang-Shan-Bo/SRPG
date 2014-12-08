#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "BattleCharacter.h"
#include "SimpleAudioEngine.h"
#include "BattleResources.h"
#include "DuelScene.h"

//定义各个势力的获取行动权的回合周期
#define TURN_CYCLE 2
//定义Battle有多少合并图片以及单个图片需要加载
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
	void LockCharacterView(BattleCharacter* pChar);//保持锁定角色视角
	void UnLockCharacterView();//解锁角色视角
	void ViewLockUpdate(float time);//锁定角色视角的更新函数
	void ViewLock(BattleCharacter* pChar);//锁定角色视角，该函数会ViewLockUpdata中不断调用
	void SearchCharacterAndViewLock(int nCharId);
	void TurnBegin(CCNode *sender,void* data);//当回合结束图标经过时
    virtual bool init();  //初始化函数，调用生成纹理资源的函数，初始化场景
	virtual void update(float dt);
	string m_strPlayerTurnMusic;
	string m_strEnermyTurnMusic;
    BattleMap();
	~BattleMap();
    static CCScene* InitBattleMapScene();//初始化战场场景。
	static CCScene* LoadBattleMapScene();//在战场场景外读挡
	virtual void draw();//使用OPENGL绘制图像，当前函数会绘制角色的技能 攻击 移动的范围
	void DrawEventMarks();//绘制事件地点
	void AcceptShowRangeObject(BattleCharacter* pObject);//接受一个需要显示范围的角色
	void KickShowRangeObject(BattleCharacter* pObject);//将角色移除显示范围的队列
	/************************************************************************/
	/*    单点触屏所需实现的虚函数                            */
	/************************************************************************/
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent); 
	void SetCostFromMap(BattleCharacter *pObject);//将当前地图数据转换为角色移动范围数据的cost
	
	/************************************************************************/
	/* 寻找地图上坐标为(x,y)的角色精灵bIdentify标识是否需要识别角色势力查找,值为true时bSame才有效。当值bSame为true时，查找的角色将一致。*/                                                                    
	/************************************************************************/
	BattleCharacter*  SearchUnitByPosition(int forceFlag,int x,int y,bool bIdentify,bool bSame); 
	CCArray* GetUnits();//获取地图上的所有角色容器
    CREATE_FUNC(BattleMap);//CCLayer子类的创建宏
	void EndTurn(CCNode* pSender,void *data);//结束当前回合
	int GetTurnFlag();	//获取当前回合的势力标识
	void AIActionsChan();//执行Ai的动作链
	void ContinueAfterCharacterAcition();
	bool IsShowRange();//是否在显示范围
	void SaveBattle(); //战场游戏数据存档
	void LoadBattle();//读档
	void TimingPlayerTurn();
	void TimingGameOver();
	void BattleClear();	
	void SetEventMark(CCPoint* pPoint);
	void RemoveAllEventMarks();
	CCDictionary* SearchTMXObject(const char* pStr);
	CCArray* SearchArrayFromTMXLayer(const char* pStr);
	void onExit();
private:
	bool m_bLoadInit;//是否处于读档状态初始化战场
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
	/*                           异步加载plist的回调函数                    */
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
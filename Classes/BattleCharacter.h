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



class CharacterStatus;//状态类父类
class MovingStatus;//对象在移动中
class OperateStatus;//对象被选中
class StandbyStatus; //对象处于闲置状态
class AIBasedStatus;//

class BattleMap;




class BattleCharacter : public CCNode 
{
	IntGetter(Level)//角色等级
	IntGetter(Move)//角色移动力
	IntGetter(ForceFlag)//角色势力
	IntGetter(CharID)//角色ID
	IntGetter(Exp)//角色经验值
	IntGetter(HP)//角色当前血量
	IntGetter(MaxHP)//角色最大血量
	IntGetter(SP)//当前角色精力
	IntGetter(MaxSP)//角色精力最大值
	IntGetter(Str)//角色力量值
	IntGetter(Mag)//角色魔力
	IntGetter(Spd)//角色速度
	IntGetter(Def)//角色防御
	IntGetter(Luk)//角色运气
	IntGetter(Dex)//角色敏捷
	IntGetter(ActionCount)//行动次数
	IntGetter(BaseExp)//战斗可获得的基本经验
	IntGetter(OnUseID)//正在使用的物品、技能的ID
	IntGetter(MaxAtkRange)//最大攻击距离
	IntGetter(MinAtkRange)//最小攻击距离
	IntGetter(UnionFlag)
	
private:
	CharacterStatus* m_pStatus;//战斗角色状态
	RangeObject* m_pRange;//战斗角色地图显示的样式


	int m_nStepTargetX;//每一步移动的横向坐标
	int m_nStepTargetY;//每一步移动的纵向坐标
	int m_nStartX;//在执行移动命令前的横轴位置
	int m_nStartY;//在执行移动命令前的纵轴位置
	int m_nShowRangeX;//显示范围
	int m_nShowRangeY;//显示范围
	int m_nCommandFlag;//角色处于何种命令
	int m_nCommandX;//指令执行的横坐标
	int m_nCommandY;//指令执行的纵坐标
	int m_nExpGot;//获得的经验值
	int m_nHPoffset;//获得的伤害或治疗效果
	bool m_bMoved;//是否执行过移动指令
	bool m_bKeyCharacter;//是否是战斗必须存活角色
	bool m_bTargetCharacter;//是否为场景boss,战斗宣告胜利
	vector<int> m_vSkills;//储存角色可用的技能ID
	vector<int> m_vAbilities;//储存角色能力的ID
	CCProgressTimer* m_pHPBar;//血槽
	CCSprite* m_pHPBarOff;//血槽背景
	vector<twards_enum> m_tPaths;//角色移动的方向路径	
	void MoveStep(twards_enum twards);//按步移动的动作
	//void OnMoveFinished(CCNode* sender,void* data);//角色完成移动的回调函数
	void ConfirmAtkRange();//确认攻击射程
	void ConfirmWeapon();//确认武器
	void ConfirmSkillRange();//确认技能射程
	void RefeshAfterAction();//在行动之后更新自身状态，显示动画
	void ShowMove();
	/*bool m_bClickAble;*/
	void FomatMovedPos();
	void ShowBattleMenu();//显示战斗菜单
	void RefreshShadow();
	/*void ShowTouchPath();*/
	void CreateMovePath(PathPoint* pTarget);//创建移动路径(A*算法)
	CCSpriteBatchNode* m_pMapSprites;//BatchNode,更有效率的渲染
	CCSprite *m_pMapSprite;//地图精灵
	CCSprite *m_pMapShadow;//地图角色的影子
	CCArray* m_pMapActions; //动作列表
	//影子，血槽等跟随动作
	CCArray* m_pMapShadowFollowActions;
	CCArray* m_pHPBarOffFollowActions;
	CCArray* m_pHPBarFollowActions;
	void LevelUp(int nUp);//升级并更新数据
	void MapSkillEffectsCallBack(CCNode* pSender,void* pData);//地图技能影响回调函数
	void ExpForLevelCallBack(CCNode* pSender,void* pData);//判定升级，以及产生升级动画的回调函数
	void RemoveLevelLogoCallBack(CCNode* pSender,void* pData);//消除levelup的图像
	/************************************************************************/
	/* 战斗计算函数、对象                                                                     */
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
	int GetAtk();//获取人物攻击力
	int GetAtkRange();
	int GetSkillRange();//获取人物技能范围
	vector<int>& GetSkillsIDVector();//返回角色技能ID的vector容器
	void RefreshMapZ();//更新地图人物z轴优先级
	void FocusView();//锁定视觉至自身
	void UnlockView();//解锁视角
	void RefreshPlayer();//更新玩家势力数据
	void MapShowHPOffset(int dam);
	void ShowMoveRange();//显示移动范围
	void ShowSkillRange();//显示技能范围
	void FomatBeforeMovePos();
	void ShowAtkRange();//显示攻击范围	
	void SetCommandFlag(int nFlag); 
	void ShadowRefresh();
	void RefreshHPBar();
	void SetStatus(CharacterStatus* pStatus);//切换角色状态
	void Move(SEL_CallFuncND callFuncND);//移动
	void InitStep();//初始化按步移动的坐标
	void CaculateMovePath(PathPoint* pRangePoint);
	void CheckMoveRange();//检查自身移动范围，将数据存放到m_pRange中。
	bool PerformFromTouch(CCPoint touchPoint);//依据触屏点作出相应的动作
	void RefreshTurnStatus();
	void ExecuteCommand(int commandFlag);
	int GetMapPositionX();//获取地图精灵的横坐标位置
	int GetMapPositionY();//获取地图精灵的纵坐标位置
	int GetShowRangePositionX();//获取移动的横坐标
	int GetShowRangePositionY();//获取移动之后的纵坐标
	CCPoint GetMapPosition();//获取地图位置
	void SetMapPosition(CCPoint pos);//设置地图精灵的位置
	CCArray* GetRangeArray(); //获取表示范围的容器
	void OnOperate();//被选取到后的动作
	bool IsControlable();//是否可以被玩家操作
	void Resume();//重置角色的操作状态
	void ResumeActionCount();//恢复移动次数
	BattleCharacter();//构造函数
	~BattleCharacter();	//析构函数
	bool IsShowRange();//是否正在显示范围
	bool MapContains(CCPoint touchPoint);
	bool IsAbleToAtk();//是否可以使用攻击指令
	bool IsAbleToItem();//是否可以使用物品指令
	bool IsAbleToDef();//是否可以使用防御指令
	bool IsAbleToSkill();//是否可以使用技能指令
	bool IsAbleToStay();//是否可以使用待机指令
	bool IsAbleToMove();//是否可以使用移动指令
	bool IsMoved();//是否执行过移动指令
	bool IsFlyable();//是否为飞行单位
	bool IsZOC(BattleCharacter* pAnotherChar);//是否具有ZOC
	void BuildCharacter(CCDictionary* dic );//创建一个角色显示在战斗地图上
	bool LoadCharacterFromData(XMLElement* pCharacter);
	void Action();//行动
	void Stand();//站立，表示处于玩家的控制
	void Standby();//准备就绪动作
	void Stay();//待机
	void ResumeStandby();//回复就绪
	void HangTouchPath();//挂起显示范围
	void BackToStart();//回到执行移动指令前的原点
	bool InSameRace(BattleCharacter* pCharacter);
	void LoadSpritesAndStatus();//加载图片精灵显示在战斗地图上
	void InitSpritesAndStatus(int x,int y);
	void RemoveFromBattle();//战斗不能，撤出战场
	void RecoverHPToMax();
	void RecoverSPToMax();
	//行动结果判定
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
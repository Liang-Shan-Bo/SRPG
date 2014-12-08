#include "BattleManager.h"
#include "DuelScene.h"
#include "DataLoader.h"
DECLARE_SINGLETON_MEMBER(BattleManager);

BattleManager::BattleManager()
{

}
BattleManager::~BattleManager()
{

}
void BattleManager::SimulationAttackAction(BattleCharacter *pAttacker, cocos2d::CCArray *pDefenders)
{
	for(unsigned int index = 0;index<pDefenders->count();index++)
	{
		SimulationAttackAction(pAttacker,(BattleCharacter*)pDefenders->objectAtIndex(index));
	}
}
void BattleManager::SimulationAttackAction(BattleCharacter *pAttacker, BattleCharacter *pDefender)
{

	int atkOffset = GetAttackHPOffset(pAttacker,pDefender);
	int fightbackOffset = 0;
	//还击
	bool isFightBack  =false;
	int pathX = pAttacker->GetMapPositionX();
	int pathY = pAttacker->GetMapPositionY();
	ConvertTouchPointToRange(pDefender->GetMapPositionX(),pDefender->GetMapPositionY(),&pathX,&pathY);
	if((abs(pathX)+abs(pathY))<=pDefender->GetAtkRange())
	{
		isFightBack = true;
		fightbackOffset = GetAttackHPOffset(pDefender,pAttacker);
	}
	int nMode =0;
	if(pAttacker->m_nForceFlag ==FORCE_ENERMY)
	{
		if(isFightBack)
		{
			nMode = DUELMODE_ATK_ENERMY;
		}
		else
		{
			nMode = DUELMODE_ATK_ENERMY_ONLY;
		}
	}
	else
	{
		if(isFightBack)
		{
			nMode = DUELMODE_ATK_PLAYER;
		}
		else
		{
			nMode = DUELMODE_ATK_PLAYER_ONLY;
		}

	}
	CCScene* scene = (CCScene*)pAttacker->m_pMapSprites->getParent()->getParent();
	BattleRecordTable* pAttackerTable = new BattleRecordTable();
	pAttackerTable->m_nHP = pAttacker->m_nHP;
	pAttackerTable->m_nHPOffset =fightbackOffset;
	BattleRecordTable* pDefenderTable = new BattleRecordTable();
	pDefenderTable->m_nHP = pDefender->m_nHP;
	pDefenderTable->m_nHPOffset = atkOffset;
	scene->addChild(new DuelLayer(pAttacker,pDefender,pAttackerTable,pDefenderTable,nMode),MAX_LAYER_ZORDER);
	//更新数值
	pDefender->m_nHP -= atkOffset;
	pAttacker->m_nHP -= fightbackOffset;
	pAttacker->RefreshHPBar();
	pDefender->RefreshHPBar();
}
int BattleManager::GetAttackHPOffset(BattleCharacter *pAttacker, BattleCharacter *pDefender)
{
	int offset =  pAttacker->GetAtk() - pDefender->GetDef();
	offset = offset>0?offset:0;
	return offset;
}
void BattleManager::SimulationSkill(BattleCharacter* pInvoker,CCArray* pTargets)
{
	int nID = pInvoker->m_nOnUseID;
	SkillDataLoader* pData = SkillDataLoader::instance();
	for(unsigned int index = 0;index<pTargets->count();index++)
	{
		BattleCharacter* pTarget = (BattleCharacter*)pTargets->objectAtIndex(index);
		//判断是否产生伤害
		if(pInvoker->m_nForceFlag!=pTarget->GetForceFlag())
		{
			int nDamage = 0;
			pData->GetSkillValue(DATA_SKILL_DAMAGE,nID,&nDamage);
			pTarget->m_nHP += -nDamage;
			pTarget->MapShowHPOffset(nDamage);
			pTarget->RefreshHPBar();
		}
	}
}

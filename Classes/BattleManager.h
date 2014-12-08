#ifndef __BATTLE_MANAGER_H__
#define __BATTLE_MANAGER_H__
#include "cocos2d.h"

#include "SimpleAudioEngine.h"
#include "Singleton.h"
#include "BattleCharacter.h"
class BattleManager: public Singleton<BattleManager>
{
private:
	int GetAttackHPOffset(BattleCharacter *pAttacker, BattleCharacter *pDefender);
public:	
	BattleManager();
	~BattleManager();
	void SimulationAttackAction(BattleCharacter* pAttacker,CCArray* pDefenders);
	void SimulationAttackAction(BattleCharacter* pAttacker,BattleCharacter* pDefender);
	void SimulationSkill(BattleCharacter* pInvoker,CCArray* pTargets);
	void IsDead();
	void IsEndTurn();
	
};


#endif
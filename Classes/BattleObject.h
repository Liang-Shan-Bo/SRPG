#ifndef _BATTLE_OBJECT_H_
#define _BATTLE_OBJECT_H_
#include "cocos2d.h"
USING_NS_CC;
class BattleObject:public CCObject
{
protected:
	int m_nID;
	int m_nRange;
	int m_nSubRange;
	int m_nHPCost;
	int m_nMPCost;
	int m_nColdDown;
	CCSprite* m_pIcon;
	CCArray* m_pChars;
public:
	void SetIconPosition(float x,float y);
	float GetIconPostionX();
	float GetIconPostionY();
	int GetID()
	{
		return m_nID;
	}
	int GetRange()
	{
		return m_nRange;
	}
	int GetSubRange()
	{
		return m_nSubRange;
	}
};


class BattleItem:public BattleObject
{
	static CCLabelTTF* m_pCountLabel;
	static CCSprite* m_pEquipedIcon;
protected:
	int m_nDam;
	int m_nHeal;
public:
};

class BattleSkill:public BattleItem
{
protected:
	int Delay;
public:
	BattleSkill(int ID)
	{
		m_nID = ID;
		m_nRange = 4;
		m_nSubRange = ID;
	}
};

class BattleBuff:public BattleObject
{
private:
	bool m_bIsForever;
	int m_nATK;
	int m_nDEF;
	int m_nSPD;
};



#endif
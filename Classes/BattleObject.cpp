#include "BattleObject.h"

void BattleObject::SetIconPosition(float x, float y)
{
	m_pIcon->setPositionX(x);
	m_pIcon->setPositionY(y);
}

float BattleObject::GetIconPostionX()
{
	return m_pIcon->getPositionX();
}

float BattleObject::GetIconPostionY()
{
	return m_pIcon->getPositionY();
}
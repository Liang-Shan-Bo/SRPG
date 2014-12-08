#include "PlayerDataManager.h"
DECLARE_SINGLETON_MEMBER(PlayerDataManager)

PlayerDataManager::PlayerDataManager(void)
{
	m_pPartners = CCArray::createWithCapacity(10);
	m_pPartners->retain();
}

PlayerDataManager::~PlayerDataManager(void)
{
	m_pPartners->release();
}

void PlayerDataManager::AddPartner( BattleCharacter* pCharacter )
{
	
	for (unsigned int index = 0;index<m_pPartners->count();index++)
	{
		BattleCharacter* pUnit  = (BattleCharacter*)m_pPartners->objectAtIndex(index);
		if (pUnit->GetCharID() == pCharacter->GetCharID())
		{
			m_pPartners->removeObjectAtIndex(index);
			break;
		}
	}
	m_pPartners->addObject(pCharacter);
}

void PlayerDataManager::Save()
{
	
}

bool PlayerDataManager::Load()
{
	return true;
}

BattleCharacter* PlayerDataManager::GetPartnerFromCharID( int nCharID )
{
	BattleCharacter* pPartner = NULL;
	for (unsigned int index = 0;index<m_pPartners->count();index++)
	{
		BattleCharacter* pUnit  = (BattleCharacter*)m_pPartners->objectAtIndex(index);
		if (pUnit->GetCharID() == nCharID)
		{
			pPartner = pUnit;
			break;
		}
	}
	return pPartner;
}

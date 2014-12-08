#ifndef  _PLAYER_DATA_MANAGER_
#define  _PLAYER_DATA_MANAGER_
#include "Singleton.h"
#include "BattleCharacter.h"

class PlayerDataManager:public Singleton<PlayerDataManager>
{
public:
	PlayerDataManager(void);
	virtual ~PlayerDataManager(void);
	void AddPartner(BattleCharacter* pCharacter);
	BattleCharacter* GetPartnerFromCharID(int nCharID);
	void Save();
	bool Load();
private:
	 CCArray* m_pPartners;
	 vector<int>  m_StockItems;
};
#endif

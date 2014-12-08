#ifndef __DATA_LOADER_
#define __DATA_LOADER_
#ifdef _WIN32

	#if defined( _MSC_VER )
		#include <direct.h>		// _mkdir
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#elif defined(MINGW32) || defined(__MINGW32__)
		#include <io.h>  // mkdir
	#else
		#include <sys/stat.h>	// mkdir
	#endif
#endif
#include "Singleton.h"
#include <vector>
#include "BattleMapScene.h"
using namespace std;
using namespace tinyxml2;
class DataLoader
{
public:
	//����·������ȡ��Դ��ʹ��tinyxml��ȡ�ĵ����ڴ�
	virtual void Init() = 0;
	DataLoader();
	~DataLoader();
	tinyxml2::XMLDocument* m_pDataDocument;//�ĵ�ʵ��
	XMLElement* m_pCoreElement;//��ǰ�ĵ��ĺ���Ԫ��
	//TiXmlDocument* m_pDataDocument;//�ĵ�ʵ��
	//XMLElement* m_pCoreElement;//��ǰ�ĵ��ĺ���Ԫ��
protected:
	virtual void DataEncryption(char* key);//�ĵ�����---����
	virtual void FindElement(char* chElement,int nID);//�����ַ���ID����root�µĺ���Ԫ��
};

class CharacterDataLoader:public Singleton<CharacterDataLoader>,public DataLoader
{
public: 
	virtual void Init();
	//��ȡ��ɫ����
	void GetCharacterValue(char* chValueName,int nID,int* pValue);
	void GetCharacterValue(char* chValueName,int nID,char* chValue);
	//��ȡ���ﵱǰ�ȼ���ѧ��ļ���ID
	void GetLevelSkills(int nID,int nLevel,vector<int>& vSkills);
	void GetLevelAbilities(int nID,int nLevel,vector<int>& vAbilities);
};

class SkillDataLoader:public Singleton<SkillDataLoader>,public DataLoader
{
public:
	virtual void Init();
	//��ȡ��������
	void GetSkillValue(char* chValueName,int nID,int* pValue);
	void GetSkillValue(char* chValueName,int nID,char* chValue);
};

class AbilityDataLoader:public Singleton<SkillDataLoader>,public DataLoader
{
public:
	virtual void Init();
	//��ȡ��������
	void GetAbilityValue(char* chValueName,int nID,int* pValue);
	void GetAbilityValue(char* chValueName,int nID,char* chValue);
};
#endif	

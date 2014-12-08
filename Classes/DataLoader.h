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
	//依据路径名获取资源，使用tinyxml读取文档至内存
	virtual void Init() = 0;
	DataLoader();
	~DataLoader();
	tinyxml2::XMLDocument* m_pDataDocument;//文档实例
	XMLElement* m_pCoreElement;//当前文档的核心元素
	//TiXmlDocument* m_pDataDocument;//文档实例
	//XMLElement* m_pCoreElement;//当前文档的核心元素
protected:
	virtual void DataEncryption(char* key);//文档解密---暂置
	virtual void FindElement(char* chElement,int nID);//依据字符和ID查找root下的核心元素
};

class CharacterDataLoader:public Singleton<CharacterDataLoader>,public DataLoader
{
public: 
	virtual void Init();
	//获取角色属性
	void GetCharacterValue(char* chValueName,int nID,int* pValue);
	void GetCharacterValue(char* chValueName,int nID,char* chValue);
	//获取人物当前等级能学会的技能ID
	void GetLevelSkills(int nID,int nLevel,vector<int>& vSkills);
	void GetLevelAbilities(int nID,int nLevel,vector<int>& vAbilities);
};

class SkillDataLoader:public Singleton<SkillDataLoader>,public DataLoader
{
public:
	virtual void Init();
	//获取技能数据
	void GetSkillValue(char* chValueName,int nID,int* pValue);
	void GetSkillValue(char* chValueName,int nID,char* chValue);
};

class AbilityDataLoader:public Singleton<SkillDataLoader>,public DataLoader
{
public:
	virtual void Init();
	//获取技能数据
	void GetAbilityValue(char* chValueName,int nID,int* pValue);
	void GetAbilityValue(char* chValueName,int nID,char* chValue);
};
#endif	

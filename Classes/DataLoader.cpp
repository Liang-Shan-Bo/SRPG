#include "dataloader.h"
#include "BattleResources.h"
USING_NS_CC;
using namespace tinyxml2;
DECLARE_SINGLETON_MEMBER(CharacterDataLoader);
DECLARE_SINGLETON_MEMBER(SkillDataLoader);

#define  MAX_CHARACTERS_FILE_SIZE 40000
#define  MAX_SKILLS_FILE_SIZE 40000
#define  MAX_ABILITIES_FILE_SIZE 40000


//采用遍历查询方法搜索节点
void DataLoader::FindElement(char* chElement,int nID)
{
	
	assert(m_pCoreElement!=NULL);
	char chID[50] = {0};			
	sprintf(chID,"%d",nID);
	//若当前查找节点的ID不等于核心节点的ID，则采取遍历查询
	m_pCoreElement = m_pCoreElement->Parent()->FirstChildElement();
	while(m_pCoreElement->Attribute("ID",chID)==0)
	{		
		m_pCoreElement = m_pCoreElement->NextSiblingElement(chElement);
		assert(m_pCoreElement!=NULL);
		
	}

	CCLog("Found charID = %s",chID);
	//int diff = nID-nCoreID;
	//if(diff>0)
	//{
	//	//获取的节点在当前节点之后
	//	for(;diff!=0;diff--)
	//	{
	//		
	//		m_pCoreElement = m_pCoreElement->NextSiblingElement(chElement);
	//		assert(m_pCoreElement!=NULL);
	//	}
	//	 
	//}
	//else 
	//{
	//	for(;diff!=0;diff++)
	//	{
	//		m_pCoreElement = m_pCoreElement->PreviousSibling(chElement)->ToElement();
	//		assert(m_pCoreElement!=NULL);
	//	}
	//	
	//}
}


void DataLoader::DataEncryption(char* key)
{

}
DataLoader::DataLoader()
{
	m_pDataDocument = NULL;
	m_pCoreElement = NULL;
}
DataLoader::~DataLoader()
{
	if(m_pDataDocument!=NULL)
	{
		delete m_pDataDocument;
	}
}


void CharacterDataLoader::Init()
{
	m_pDataDocument =new XMLDocument();
	char dataPath[50];
	sprintf(dataPath,"%s",DATA_CHARACTER_FULLPATH);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(dataPath, "rb", &bufferSize);
	char pCBuffer[MAX_CHARACTERS_FILE_SIZE];
	pCBuffer[0] ='\0';
	sprintf(pCBuffer,"%s%s",(char*)pBuffer,'\0');
	if (pBuffer)
	{
		XMLError bError= m_pDataDocument->Parse((const char*)pCBuffer,bufferSize);
		assert(bError==XML_NO_ERROR);
	}
	delete(pBuffer);
	XMLElement* pRoot = m_pDataDocument->RootElement();
	m_pCoreElement = pRoot->FirstChildElement();
}



void  CharacterDataLoader::GetCharacterValue(char* chValueName,int nID,int* pValue)
{
	FindElement("character",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%d",pValue);
	CCLog("%d",*pValue);
}


void  CharacterDataLoader::GetCharacterValue(char* chValueName,int nID,char* chValue)
{
	FindElement("character",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%s",chValue);
	CCLog("%s",chValue);
}

void CharacterDataLoader::GetLevelSkills(int nID,int nLevel,vector<int>& vSkills)
{
	FindElement("character",nID);
	XMLElement* pSkillsElement = m_pCoreElement->FirstChildElement("SKILLS");
	if(pSkillsElement==NULL||pSkillsElement->NoChildren())
	{
		return;
	}
	XMLElement* pSkillElement = pSkillsElement->FirstChildElement("SKILL");
	while(pSkillElement!=NULL)
	{
		int nSkillLevel = 0; 
		int nSkillID = 0;
		TIXML_SSCANF(pSkillElement->Attribute("LEVEL"),"%d",&nSkillLevel);
		if(nSkillLevel<=nLevel)
		{
			TIXML_SSCANF(pSkillElement->Attribute("ID"),"%d",&nSkillID);
			vSkills.push_back(nSkillID);
		}
		pSkillElement = pSkillElement->NextSiblingElement("SKILL");
	}

}
void CharacterDataLoader::GetLevelAbilities(int nID,int nLevel,vector<int>& vSkills)
{
	FindElement("character",nID);
	XMLElement* pAbilitiesElement = m_pCoreElement->FirstChildElement("Abilities");
	if(pAbilitiesElement==NULL||pAbilitiesElement->NoChildren())
	{
		return;
	}
	XMLElement* pAbilityElement = pAbilitiesElement->FirstChildElement("Ability");
	while(pAbilityElement!=NULL)
	{
		int nAbilityLevel = 0; 
		int nAbilityID = 0;
		TIXML_SSCANF(pAbilityElement->Attribute("LEVEL"),"%d",&nAbilityLevel);
		if(nAbilityLevel<=nLevel)
		{
			TIXML_SSCANF(pAbilityElement->Attribute("ID"),"%d",&nAbilityID);
			vSkills.push_back(nAbilityID);
		}
		pAbilityElement = pAbilityElement->NextSiblingElement("Ability");
	}

}

void SkillDataLoader::Init()
{
	m_pDataDocument =new XMLDocument();
	char dataPath[50];
	sprintf(dataPath,"%s",DATA_SKILL_FULLPATH);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(dataPath, "rb", &bufferSize);
	char pCBuffer[MAX_SKILLS_FILE_SIZE];
	pCBuffer[0] ='\0';
	sprintf(pCBuffer,"%s%s",(char*)pBuffer,'\0');
	if (pBuffer)
	{
		XMLError bError= m_pDataDocument->Parse((const char*)pCBuffer,bufferSize);
		assert(bError==XML_NO_ERROR);
	}
	delete(pBuffer);
	XMLElement* pRoot = m_pDataDocument->RootElement();
	m_pCoreElement = pRoot->FirstChildElement();
	//char dataPath[50];
	////sprintf(dataPath,"%s%s%s",DATA_FILE_PATH,PLAYFORM_SEPERATOR,DATA_SKILL_FILE_NAME);
	//sprintf(dataPath,"%s",DATA_SKILL_FULLPATH);
	//m_pDataDocument =new XMLDocument();
	//XMLError bError =m_pDataDocument->LoadFile(dataPath);
	//assert(bError==XML_NO_ERROR);
	//XMLElement* pRoot = m_pDataDocument->RootElement();
	//m_pCoreElement = pRoot->FirstChildElement();
	////XMLElement* test = m_pCoreElement->NextSiblingElement("character")->PreviousSibling()->ToElement();
	////CCLog("%s",test->FirstChild()->FirstChild()->Value());
}



void SkillDataLoader::GetSkillValue(char* chValueName,int nID,int* pValue)
{
	FindElement("SKILL",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%d",pValue);
	CCLog("%d",*pValue);
}

void  SkillDataLoader::GetSkillValue(char *chValueName,int nID,char*chValue)
{
	FindElement("SKILL",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%s",chValue);
	CCLog("%s",chValue);
}


void AbilityDataLoader::Init()
{
	m_pDataDocument =new XMLDocument();
	char dataPath[50];
	sprintf(dataPath,"%s",DATA_SKILL_FULLPATH);
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(dataPath, "rb", &bufferSize);
	char pCBuffer[MAX_ABILITIES_FILE_SIZE];
	pCBuffer[0] ='\0';
	sprintf(pCBuffer,"%s%s",(char*)pBuffer,'\0');
	if (pBuffer)
	{
		XMLError bError= m_pDataDocument->Parse((const char*)pCBuffer,bufferSize);
		assert(bError==XML_NO_ERROR);
	}
	delete(pBuffer);
	XMLElement* pRoot = m_pDataDocument->RootElement();
	m_pCoreElement = pRoot->FirstChildElement();
	//char dataPath[50];
	////sprintf(dataPath,"%s%s%s",DATA_FILE_PATH,PLAYFORM_SEPERATOR,DATA_SKILL_FILE_NAME);
	//sprintf(dataPath,"%s",DATA_SKILL_FULLPATH);
	//m_pDataDocument =new XMLDocument();
	//XMLError bError =m_pDataDocument->LoadFile(dataPath);
	//assert(bError==XML_NO_ERROR);
	//XMLElement* pRoot = m_pDataDocument->RootElement();
	//m_pCoreElement = pRoot->FirstChildElement();
	////XMLElement* test = m_pCoreElement->NextSiblingElement("character")->PreviousSibling()->ToElement();
	////CCLog("%s",test->FirstChild()->FirstChild()->Value());
}

void AbilityDataLoader::GetAbilityValue(char* chValueName,int nID,int* pValue)
{
	FindElement("Ability",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%d",pValue);
	CCLog("%d",*pValue);
}

void  AbilityDataLoader::GetAbilityValue(char *chValueName,int nID,char*chValue)
{
	FindElement("Ability",nID);
	XMLElement* pValueElement = m_pCoreElement->FirstChildElement(chValueName);
	assert(pValueElement!=NULL);
	TIXML_SSCANF(pValueElement->FirstChild()->Value(),"%s",chValue);
	CCLog("%s",chValue);
}


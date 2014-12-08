#include <vector>
#include <algorithm>
#include "AnimationManager.h"
#define DUEL_ACTION_FRAME_DUR 0.091f
#define DUEL_WAVE_FRAME_DUR 0.07f
#define NUMS_RUNTO_FRAME_DUR 0.04f

DECLARE_SINGLETON_MEMBER(AnimationManager);
using namespace std;

AnimationManager::AnimationManager()
{
	
}
AnimationManager::~AnimationManager()
{
      
}
/*依据文字宏定义的资源路径，将资源填充到FrameCache中。之后通过获取Cache中的SpriteFrame创建并返回Animation。*/


CCAnimation* AnimationManager::CreateSimpleLoadingAnimation()
{
	char chPlistStr[50] = {0};
	char chImageStr[50]= {0};
	sprintf(chPlistStr,"%s.plist",LOADING_ICON_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",LOADING_ICON_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* pFrames = CCArray::createWithCapacity(4);
	for(int i = 1;i<MAX_ANIMATION_FRAME; i++) 
	{
		char chFileName[50];
		sprintf(chFileName,"%s%d.png",LOADING_ICON_FRAME_NAME,i);
		CCSpriteFrame* pFrame = NULL;
		pFrame = cache->spriteFrameByName(chFileName);
		if(pFrame ==NULL)
		{
			break;
		}
		pFrames->addObject(pFrame); 
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(pFrames,LOADING_ANIMATION_DUR);
	return animation;
}

CCAnimation* AnimationManager::CreateTitleBackGroundAnimation()
{
	char chPathStr[50] = {0};
	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	CCArray* pFrames = CCArray::createWithCapacity(MAX_TITLE_BACKGROUND_FRAME);
	for(int i = 1;i<=MAX_TITLE_BACKGROUND_FRAME;i++)
	{
		sprintf(chPathStr,"%s%d.png",TITLE_BACKGROUND_FILE_FULLPATH,i);
		CCTexture2D* pTexture = pCache->textureForKey(chPathStr);
		CCSpriteFrame*  pFrame = NULL;
		assert(pTexture!=NULL);
		pFrame = CCSpriteFrame::createWithTexture(pTexture,CCRect(0,0,pTexture->getPixelsWide(),pTexture->getPixelsHigh()));
		assert(pFrame!=NULL);
		pFrames->addObject(pFrame);
	}
	CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrames,TITLE_BACKGROUND_ANI_DUR);
	return pAnimation;
}

CCAnimation* AnimationManager::CreateMapActionAnimation(int charFlag,int actFlag)
{
	char fileHead[50];
	sprintf(fileHead,"%s%d%s%d",MAPCHAR_FRAME_NAME,charFlag,MAPCHAR_ACTION_FILE_NAME,actFlag/MAX_ANIMATION_FRAME);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* frames = CCArray::createWithCapacity(actFlag%MAX_ANIMATION_FRAME);
	for(int i = 0; i < MAX_ANIMATION_FRAME; i++) 
	{
		char chFileName[50];
		sprintf(chFileName, "%s%s%d.png",fileHead,MAPCHAR_FRAME_COUNT_NAME,i);
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName( chFileName ); 
		if(frame == NULL)
		{
			break;
		}
		frames->addObject(frame);  
	}
	float dur = _MAPSPRITE_FLESH_RATE_;
	if(actFlag/MAX_ANIMATION_FRAME == 0)
	{
		 dur = _MAPSPRITE_FLESH_RATE_ *1.8f;
	}
	else
	{
		dur = _MAPSPRITE_FLESH_RATE_*(4.0f/frames->count());
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, dur);
	return animation;
}

void AnimationManager::CreateDuelActionAnimation(CCAnimation* &charAni,CCAnimation* &shadowAni,int charFlag, int actFlag,int startFrame,int endFrame)
{
	charAni = NULL;
	shadowAni = NULL;
	char charFileHead[50];
	char shadowFileHead[50];
	sprintf(charFileHead,"%s%d%s%d",DUEL_CHAR_FRAME_NAME,charFlag,DUEL_ACTION_FILE_NAME,actFlag);
	sprintf(shadowFileHead,"%s%d%s%d",DUEL_CHARSHADOW_FILE_NAME,charFlag,DUEL_ACTION_FILE_NAME,actFlag);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* charFrames = CCArray::createWithCapacity(endFrame - startFrame);
	CCArray* shadowFrames = CCArray::createWithCapacity(endFrame - startFrame); 
	for(int i = startFrame;i<= endFrame; i++) 
	{
		char chFileName[50];
		sprintf(chFileName, "%s%s%d.png",charFileHead,DUEL_FRAME_FILE_NAME,i);
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName(chFileName);
		if(frame ==NULL)
		{
			break;//超出帧范围
		}
		charFrames->addObject(frame); 
		sprintf(chFileName, "%s%s%d.png",shadowFileHead,DUEL_FRAME_FILE_NAME,i);
		frame = cache->spriteFrameByName(chFileName);
		if(frame!=NULL)
		{
			shadowFrames->addObject(frame);
		}
		
	}
	float fDur = DUEL_ACTION_FRAME_DUR;
	if (actFlag == ACTION_DUEL_STAND)
	{
		fDur = fDur*1.8;
	}
	if(charFrames->count()>0)
	{
		charAni = CCAnimation::createWithSpriteFrames(charFrames, fDur);
		shadowAni = CCAnimation::createWithSpriteFrames(shadowFrames, fDur);
	}
}
CCAnimation* AnimationManager::CreateDuelWaveAnimation(int waveID)
{
	char chPlistStr[50] = {0};
	char chImageStr[50]= {0};
	sprintf(chPlistStr,"%s.plist",DUEL_WAVE_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",DUEL_WAVE_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* waveFrames = CCArray::createWithCapacity(4);
	for(int i = 0;i<MAX_ANIMATION_FRAME; i++) 
	{
		char chFileName[50];
		sprintf(chFileName, "%s%d%s%d.png",DUEL_WAVE_ID_NAME,waveID,DUEL_WAVE_FRAME_NAME,i);
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName(chFileName);
		if(frame ==NULL)
		{
			break;//超出帧范围
		}
		waveFrames->addObject(frame); 
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(waveFrames, DUEL_WAVE_FRAME_DUR );
	return animation;
}

CCSprite* AnimationManager::CreateSingleNum(unsigned int num, char *pTag)
{
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s.png",NUMS_FILE_FULLPATH);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if(num>9)
	{
		//大于9的数字强制转换成个位数
		num = num%10;
	}
	sprintf(chImageStr,"%s%d.png",pTag,num);
	CCSprite* numSprite = CCSprite::createWithSpriteFrameName(chImageStr);
	return numSprite;	
}

CCArray* AnimationManager::CreateSizeNums(unsigned int num, char *pTag,bool bBlank)
{
	CCArray* numSprites= CCArray::createWithCapacity(10);
	//定义除数,赋予初值10用来筛选个位数
	unsigned int divisor = 10;
	//定义余数
	unsigned int remainder = 0;
	int nCount = 0;
	do
	{
		remainder = num%divisor;
		numSprites->addObject(CreateSingleNum(remainder,pTag));
		num = num/divisor;
		nCount++;
	}while(num!=0||(bBlank&&nCount<10));
	return numSprites;

}


void AnimationManager::RemoveAniamationSprite(cocos2d::CCNode *sender, void *data)
{
	((CCSprite*)data)->removeFromParentAndCleanup(true);
}

CCAnimation* AnimationManager::CreateMapCharacterVanishAnimation()
{
	char chPlistStr[50] = {0};
	char chImageStr[50]= {0};
	
	sprintf(chPlistStr,"%s.plist",MAP_CHARACTER_VANISH_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",MAP_CHARACTER_VANISH_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* skillFrames = CCArray::createWithCapacity(4);
	for(int i =1;i<MAX_ANIMATION_FRAME; i++) 
	{
		char chFileName[50];
		//每一帧的图片名
		sprintf(chFileName,"%s%d.png",MAP_CHARACTER_VANISH_FRAME_NAME,i);
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName(chFileName);
		if(frame ==NULL)
		{
			break;//超出帧范围
		}
		skillFrames->addObject(frame); 
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(skillFrames,MAP_CHARACTER_VANISH_FRAME_DUR );
	return animation;
}

CCArray* AnimationManager::CreateMapSkillAnimations( unsigned int nSkillID )
{
	CCArray* pSkillAnimations =CCArray::createWithCapacity(4);//盛放技能主动画和技能背景动画的容器，返回值
	char fileHead[50];
	sprintf(fileHead,"%s%d%s",MAP_SKILL_FRAME_NAME,nSkillID,MAP_SKILL_FRAME_COUNT_NAME);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* skillFrames = CCArray::createWithCapacity(40);
	CCArray* backFrames = CCArray::createWithCapacity(4);
	char chFrameName[50];
	//创建技能主动画
	for(int i = 1; i < MAX_ANIMATION_FRAME; i++) 
	{	
		sprintf(chFrameName, "%s%d.png",fileHead,i);
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName( chFrameName ); 
		if(frame == NULL)
		{
			break;
		}
		skillFrames->addObject(frame);  
	}
	CCAnimation* skillAnimation = CCAnimation::createWithSpriteFrames(skillFrames, MAP_SKILL_FRAME_DUR);
	pSkillAnimations->addObject(skillAnimation);
	//创建技能背景动画
	for(int i = 1; i < MAX_ANIMATION_FRAME; i++) 
	{	
		sprintf(chFrameName, "%s%d%s%d.png",MAP_SKILL_FRAME_NAME,nSkillID,MAP_SKILL_BACKGROUND_FRAME_NAME,i);	
		CCSpriteFrame* frame = NULL;
		frame = cache->spriteFrameByName( chFrameName ); 
		if(frame == NULL)
		{
			break;
		}
		backFrames->addObject(frame);  
	}
	if (backFrames->count()>0)
	{
		CCAnimation* backAnimation = CCAnimation::createWithSpriteFrames(backFrames, MAP_SKILL_FRAME_DUR);
		pSkillAnimations->addObject(backAnimation);
	}
	return pSkillAnimations;
	
}

CCAnimation* AnimationManager::CreateNumsRunToAnimation(unsigned int nFrom,unsigned int nTo ,char *pTag)
{
	nTo = nTo%10;
	char chStr[50] = {0};
	CCArray* pFrames = CCArray::createWithCapacity(10);
	CCSpriteFrame* pNumFrame = NULL;	
	if (nTo!= nFrom)
	{
		vector<unsigned int> vToal;
		for(unsigned int i= 0;i<nTo;i++)
		{
			vToal.push_back(i);
		}
		random_shuffle(vToal.begin(),vToal.end());
		
	
		for(vector<unsigned int>::iterator it = vToal.begin();it!=vToal.end();it++)
		{
			sprintf(chStr,"%s%d.png",pTag,*it);
			pNumFrame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(chStr);
			pFrames->addObject(pNumFrame);
		}
	}
	//将到生成的目标数字放到最后一帧
	sprintf(chStr,"%s%d.png",pTag,nTo);
	pNumFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(chStr);
	pFrames->addObject(pNumFrame);
	CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(pFrames,NUMS_RUNTO_FRAME_DUR);
	return pAnimation;
}

CCAnimation* AnimationManager::CreateSquareAnimation()
{
	char chPlistStr[50] = {0};
	char chImageStr[50]= {0};
	sprintf(chPlistStr,"%s.plist",SQUARE_ICON_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",SQUARE_ICON_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* pFrames = CCArray::createWithCapacity(24);
	for(int i = 1;i<MAX_ANIMATION_FRAME; i++) 
	{
		char chFileName[50];
		sprintf(chFileName,"%s%d.png",SQUARE_ICON_FRAME_NAME,i);
		CCSpriteFrame* pFrame = NULL;
		pFrame = cache->spriteFrameByName(chFileName);
		if(pFrame ==NULL)
		{
			break;
		}
		pFrames->addObject(pFrame); 
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(pFrames,SQUARE_ANIMATION_DUR);
	return animation;
}

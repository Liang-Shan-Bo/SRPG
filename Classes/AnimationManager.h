
#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__
#include "cocos2d.h"
#include "Singleton.h"
#include "BattleResources.h"

//定义动画键宏,1-8位代表动画帧数 9-16位代表角色动作 17-32位代表精灵位图


USING_NS_CC;
/*
	创建游戏动画
					*/
class AnimationManager:public Singleton<AnimationManager>,public CCObject
{
public:
        AnimationManager();
        ~AnimationManager();
		CCAnimation* CreateSquareAnimation();
		CCAnimation* CreateSimpleLoadingAnimation();//创建简单的loading动画
		CCAnimation* CreateTitleBackGroundAnimation();//创建标题画面的背景动画
		CCAnimation* CreateMapActionAnimation(int charFlag,int actFlag);//创建地图角色的站立、静止动画
		void CreateDuelActionAnimation(CCAnimation* &charAni,CCAnimation* &ShadowAni,int charFlag, int actFlag,int startFrame,int endFrame);//创建决斗动画
		CCAnimation* CreateDuelWaveAnimation(int waveID);//创建决斗场景的“斗气”动画
		CCSprite* CreateSingleNum(unsigned int num,char* pTag);//创建单个数字精灵
		CCArray* CreateSizeNums(unsigned int  num,char* pTag,bool bBlank=false);//创建当前数字对应的数字精灵图片集
		CCAnimation* CreateNumsRunToAnimation(unsigned int nFrom,unsigned int nTo, char *pTag);
		void RemoveAniamationSprite(cocos2d::CCNode *sender, void *data);//将数字移出当前场景的回调函数
		CCArray* CreateMapSkillAnimations(unsigned int nSkillID);
		CCAnimation* CreateMapCharacterVanishAnimation();
};
#endif


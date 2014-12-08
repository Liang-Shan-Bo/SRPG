
#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__
#include "cocos2d.h"
#include "Singleton.h"
#include "BattleResources.h"

//���嶯������,1-8λ������֡�� 9-16λ�����ɫ���� 17-32λ������λͼ


USING_NS_CC;
/*
	������Ϸ����
					*/
class AnimationManager:public Singleton<AnimationManager>,public CCObject
{
public:
        AnimationManager();
        ~AnimationManager();
		CCAnimation* CreateSquareAnimation();
		CCAnimation* CreateSimpleLoadingAnimation();//�����򵥵�loading����
		CCAnimation* CreateTitleBackGroundAnimation();//�������⻭��ı�������
		CCAnimation* CreateMapActionAnimation(int charFlag,int actFlag);//������ͼ��ɫ��վ������ֹ����
		void CreateDuelActionAnimation(CCAnimation* &charAni,CCAnimation* &ShadowAni,int charFlag, int actFlag,int startFrame,int endFrame);//������������
		CCAnimation* CreateDuelWaveAnimation(int waveID);//�������������ġ�����������
		CCSprite* CreateSingleNum(unsigned int num,char* pTag);//�����������־���
		CCArray* CreateSizeNums(unsigned int  num,char* pTag,bool bBlank=false);//������ǰ���ֶ�Ӧ�����־���ͼƬ��
		CCAnimation* CreateNumsRunToAnimation(unsigned int nFrom,unsigned int nTo, char *pTag);
		void RemoveAniamationSprite(cocos2d::CCNode *sender, void *data);//�������Ƴ���ǰ�����Ļص�����
		CCArray* CreateMapSkillAnimations(unsigned int nSkillID);
		CCAnimation* CreateMapCharacterVanishAnimation();
};
#endif


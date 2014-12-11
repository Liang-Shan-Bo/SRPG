#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "BattleResources.h"
#include "AnimationManager.h"

#include "SimpleAudioEngine.h"


USING_NS_CC;

class StartNow : public cocos2d::CCLayer
{
public:
   
    virtual bool init();  
	virtual void onEnter();
    
    static cocos2d::CCScene* scene();
	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    //virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchesMoved(CCTouch *pTouch,CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent); 
	virtual void registerWithTouchDispatcher();
	void LoadingCallBack(CCObject* pSender);
    CREATE_FUNC(StartNow);
	virtual void onExit();
private:
	int m_nLoadCount;
	CCSprite* m_pLoading;
	CCSprite* m_pStarGame;
	CCSprite* m_pLoadGame;
	CCSprite* m_pContinue;
	CCSprite* m_pQuitGame;
	CCSpriteBatchNode* m_pBatchNode;
	void RunLoading();
	void InitStartSceneResoures();
	

	//bool m_bGetStart;
	
	//CCLabelTTF* m_pPassStartLabel;
	//void updateDialog(float time);
	//CCLabelTTF* m_pNewGameLabel;
	//CCLabelTTF* m_pExitLabel;
 //   CCPoint m_tBeginPos;
 //   CCMenu* m_pItemMenu;
};

#endif  
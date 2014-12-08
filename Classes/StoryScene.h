
#ifndef __STORY_SCENE_H__
#define  __STORY_SCENE_H__
#include "cocos2d.h"
#include <string>

/************************************************************************/
/* ¾çÇé³¡¾°                                                                     */
/************************************************************************/





USING_NS_CC;
using namespace std;
class StoryScene : public cocos2d::CCLayer
{
public:
	virtual bool init();  
	virtual void onEnter();
	static cocos2d::CCScene* scene();
	virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
	virtual void ccTouchesMoved(CCTouch *pTouch,CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent); 
	virtual void registerWithTouchDispatcher();
	void InitResource();
	void LoadBackgroundCallBack(CCObject* pSender);
	void LoadDialogCallBack(CCObject* pSender);
	CREATE_FUNC(StoryScene);
	void GoNext();
	virtual void onExit();
	void InitStory();
private:
	int m_nID;
	int m_nLoadCount;
	string m_strBGMName;
	CCSprite* m_pBackGround;
	//bool m_bGetStart;

	//CCLabelTTF* m_pPassStartLabel;
	//CCLabelTTF* m_pNewGameLabel;
	//CCLabelTTF* m_pExitLabel;
	//   CCPoint m_tBeginPos;
	//   CCMenu* m_pItemMenu;
};
#endif

#include "DuelScene.h"
#include "AnimationManager.h"
#include "slg_tools.h"
#include "DualATKActionDefine.h"
#include "BattleMapScene.h"
#include "EventHandler.h"
#include "json/json.h"
#include "BattleResources.h"
#define NUMS_CREATE_POSITION_X 53
#define NUMS_CREATE_POSITION_Y 106

#define L_HP_NUM_0 0
#define L_HP_NUM_9 9
#define R_HP_NUM_0 10
#define R_HP_NUM_9 19
#define L_SP_NUM_0 20
#define L_SP_NUM_9 29
#define R_SP_NUM_0 30
#define R_SP_NUM_9 39

#define HP_COUNTER_START_X 200
#define HP_COUNTER_START_Y 32

#define SP_COUNTER_START_X 200
#define SP_COUNTER_START_Y 12

#define DUEL_LAYER_NUMS_BATCH_TAG 3001

#include <fstream>

CombatTable::CombatTable(int occureFrame,int waveID,float wavePosX,float wavePosY,int damScopet,bool bKeepWave,int dur,float moveX,float moveY)
{
	m_nOccureFrame = occureFrame;
	m_nWaveID = waveID;
	m_fWavePosX = wavePosX;
	m_fWavePosY = wavePosY; 
	m_nDamScope = damScopet;
	m_bKeepWave = bKeepWave;
	m_nMoveDur = dur;
	m_fMoveOffsetX = moveX;
	m_fMoveOffsetY = moveY;
}
DuelLayer::~DuelLayer()
{
	m_pAttackerActions->release();
	m_pAttackerShadowActions->release();
	m_pDefenderActions->release();
	m_pDefenderShadowActions->release();
	if(m_pCombatTable!=NULL)
	{
		m_pCombatTable->release();
		m_pCombatTable = NULL; 
	}

}
DuelLayer::DuelLayer(BattleCharacter* pAttacker,BattleCharacter* pDefender,BattleRecordTable* pAttackerTable,BattleRecordTable* pDefenderTable,int DuelMode)
{
	CCLayer* pRunningLayer = (CCLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
	if (pRunningLayer!=NULL)
	{
		pRunningLayer->setTouchEnabled(false);
	}
	m_pCombatTable =NULL;
	m_nToalDamScope = 1;
	m_nToalHits = 1;
	m_nHits = 0;
	m_nDamage= 0;
	m_nMode = DuelMode;
	m_pAttackerChar = pAttacker;
	m_pDefenderChar = pDefender;
	m_pAttackerTable = pAttackerTable;
	m_pDefenderTable = pDefenderTable;
	EventHandler* pHandler = EventHandler::instance();
	
	//初始化背景图
	char chImageStr[50]= {0};
	sprintf(chImageStr,"%s%d.png",DUEL_BACKGROUND_FILE_FULLPATH,pHandler->GetSceneID());
	m_pBackGround = CCSprite::create(chImageStr);
	m_pBackGround->setAnchorPoint(ccp(0,0));
	m_pCombatTable = NULL;
	addChild(m_pBackGround);
	//初始化数值条
	char chPlistStr[50] = {0};
	
	sprintf(chPlistStr,"%s.plist",DUEL_LAYERUI_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	CCSpriteBatchNode* barNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	CCSprite* temp = NULL;
	addChild(barNode,MAX_LAYER_ZORDER-1);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_LEFTBARBK_NAME);
	m_pLeftBarBK = CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_RIGHTBARBK_NAME);
	m_pRightBarBK = CCSprite::createWithSpriteFrameName(chImageStr);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_LEFTHP_NAME);
	temp =  CCSprite::createWithSpriteFrameName(chImageStr);
	m_pLeftHP = CCProgressTimer::create(temp);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_LEFTMP_NAME);
	temp =  CCSprite::createWithSpriteFrameName(chImageStr);
	m_pLeftMP = CCProgressTimer::create(temp);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_RIGHTHP_NAME);
	temp =  CCSprite::createWithSpriteFrameName(chImageStr);
	m_pRightHP =CCProgressTimer::create(temp);
	sprintf(chImageStr,"%s.png",DUEL_LAYERUI_RIGHTMP_NAME);
	temp =  CCSprite::createWithSpriteFrameName(chImageStr);
	m_pRightMP = CCProgressTimer::create(temp);
	barNode->addChild(m_pLeftBarBK,-1);
	barNode->addChild(m_pRightBarBK,-1);
	addChild(m_pLeftHP,0);
	addChild(m_pLeftMP,0);
	addChild(m_pRightHP,0);
	addChild(m_pRightMP,0);
	//初始化数值条位置
	m_pLeftBarBK->setAnchorPoint(ccp(0,0));
	m_pLeftBarBK->setPosition(ccp(0,0));
	m_pRightBarBK->setAnchorPoint(ccp(0,0));
	m_pRightBarBK->setPosition(ccp(m_pLeftBarBK->getPositionX()+VisibleRect::right().x-GetSpriteTextureLength(m_pRightBarBK),0));
	m_pLeftHP->setPositionX(m_pLeftBarBK->getPositionX()+178);
	m_pLeftHP->setPositionY(m_pLeftBarBK->getPositionY()+(GetSpriteTextureHeight(m_pLeftBarBK)-75));
	m_pLeftMP->setPositionX(m_pLeftBarBK->getPositionX()+144);
	m_pLeftMP->setPositionY(m_pLeftBarBK->getPositionY()+(GetSpriteTextureHeight(m_pLeftBarBK)-97.75));
	m_pRightHP->setPositionX(m_pRightBarBK->getPositionX()+116);
	m_pRightHP->setPositionY(m_pRightBarBK->getPositionY()+(GetSpriteTextureHeight(m_pRightBarBK)-75));
	m_pRightMP->setPositionX(m_pRightBarBK->getPositionX()+151);
	m_pRightMP->setPositionY(m_pRightBarBK->getPositionY()+(GetSpriteTextureHeight(m_pRightBarBK)-97.75));
	//初始化动作链表
	m_pAttackerActions = CCArray::createWithCapacity(5);
	m_pAttackerShadowActions = CCArray::createWithCapacity(5);
	m_pDefenderActions = CCArray::createWithCapacity(5);
	m_pDefenderShadowActions = CCArray::createWithCapacity(5);
	m_pAttackerActions->retain();
	m_pAttackerShadowActions->retain();
	m_pDefenderActions->retain();
	m_pDefenderShadowActions->retain();
	//初始化飞行道具列表
	m_pWaves = CCArray::createWithCapacity(5);
	m_pWaves->retain();
	//初始化攻击方plist至缓存
	sprintf(chPlistStr,"%s%d.plist",DUEL_CHAR_FILE_FULLPATH,m_pAttackerChar->GetCharID());
	sprintf(chImageStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,m_pAttackerChar->GetCharID());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	m_pAttackerBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	char frameStr[50]= {0};
	sprintf(frameStr,"%s%da0f0.png",DUEL_CHAR_FRAME_NAME,m_pAttackerChar->GetCharID());
	m_pAttacker = CCSprite::createWithSpriteFrameName(frameStr);
	sprintf(frameStr,"%s%da0f0.png",DUEL_CHARSHADOW_FILE_NAME,m_pAttackerChar->GetCharID());
	m_pAttackerShadow = CCSprite::createWithSpriteFrameName(frameStr);
	m_pAttackerBatchNode->addChild(m_pAttacker);
	m_pAttackerBatchNode->addChild(m_pAttackerShadow);
	//初始化防御方plist至缓存
	sprintf(chPlistStr,"%s%d.plist",DUEL_CHAR_FILE_FULLPATH,m_pDefenderChar->GetCharID());
	sprintf(chImageStr,"%s%d.png",DUEL_CHAR_FILE_FULLPATH,m_pDefenderChar->GetCharID());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	m_pDefenderBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	sprintf(frameStr,"%s%da0f0.png",DUEL_CHAR_FRAME_NAME,m_pDefenderChar->GetCharID());
	m_pDefender = CCSprite::createWithSpriteFrameName(frameStr);
	sprintf(frameStr,"%s%da0f0.png",DUEL_CHARSHADOW_FILE_NAME,m_pDefenderChar->GetCharID());
	m_pDefenderShadow = CCSprite::createWithSpriteFrameName(frameStr);
	m_pDefenderBatchNode->addChild(m_pDefender);
	m_pDefenderBatchNode->addChild(m_pDefenderShadow);
	addChild(m_pDefenderBatchNode);
	addChild(m_pAttackerBatchNode);
	//依据模式初始化  势力标识
	int playerHP = 0;
	int enermyHP = 0;
	if(m_nMode == DUELMODE_ATK_ENERMY||m_nMode == DUELMODE_ATK_ENERMY_ONLY||m_nMode == DUELMODE_ATK_ENERMY_RANGE||m_nMode == DUELMODE_ATK_ENERMY_RANGE_ONLY)
	{
		m_pPlayer = m_pDefender;
		m_pEnermy = m_pAttacker;
		m_pPlayerShadow = m_pDefenderShadow;
		m_pEnermyShadow = m_pAttackerShadow;
		m_pEnermyChar = m_pAttackerChar;
		m_pPlayerChar = m_pDefenderChar;
		m_pEnermyTable = m_pAttackerTable;
		m_pPlayerTable = m_pDefenderTable;
		enermyHP = m_pAttackerTable->m_nHP;
		playerHP = m_pDefenderTable->m_nHP;
		
	}
	else
	{
		m_pPlayer = m_pAttacker;
		m_pEnermy = m_pDefender;
		m_pPlayerShadow = m_pAttackerShadow;
		m_pEnermyShadow = m_pDefenderShadow;
		m_pPlayerChar = m_pAttackerChar;
		m_pEnermyChar = m_pDefenderChar;
		m_pPlayerTable = m_pAttackerTable;
		m_pEnermyTable = m_pDefenderTable;
		playerHP = m_pAttackerTable->m_nHP;
		enermyHP = m_pDefenderTable->m_nHP;
		
	}
	//初始化血条、魔条显示
	m_pLeftHP->setType(kCCProgressTimerTypeBar);
	m_pLeftHP->setMidpoint(ccp(0,1));//从左向右更新
	m_pLeftHP->setBarChangeRate(ccp(1,0));//只有x轴更新
	m_pLeftHP->setType(kCCProgressTimerTypeBar);
	m_pLeftMP->setMidpoint(ccp(0,1));
	m_pLeftMP->setBarChangeRate(ccp(1,0));
	m_pRightHP->setType(kCCProgressTimerTypeBar);
	m_pRightHP->setMidpoint(ccp(1,1));//从右向左更新
	m_pRightHP->setBarChangeRate(ccp(1,0));
	m_pRightMP->setType(kCCProgressTimerTypeBar);
	m_pRightMP->setMidpoint(ccp(1,1));
	m_pRightMP->setBarChangeRate(ccp(1,0));
	m_pRightHP->setPercentage((playerHP*100)/m_pPlayerChar->GetMaxHP());
	m_pLeftHP->setPercentage((enermyHP*100)/m_pEnermyChar->GetMaxHP());	
	m_pRightMP->setPercentage((m_pPlayerTable->m_nSP*100)/m_pPlayerChar->GetMaxSP());
	m_pLeftMP->setPercentage((m_pEnermyTable->m_nSP*100)/m_pEnermyChar->GetMaxSP());
	CCLog("%f",m_pRightHP->getPercentage());
	//初始化数值显示器
	sprintf(chPlistStr,"%s.plist",NUMS_FILE_FULLPATH);
	sprintf(chImageStr,"%s.png",NUMS_FILE_FULLPATH);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(chPlistStr,chImageStr); 
	m_pNumsBatchNode = CCSpriteBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(chImageStr));
	this->addChild(m_pNumsBatchNode,MAX_LAYER_ZORDER,DUEL_LAYER_NUMS_BATCH_TAG);
	//显示左方(敌方HP)
	float fStartPositionX = m_pLeftBarBK->getPositionX()+HP_COUNTER_START_X;
	float fStartPositionY = m_pLeftBarBK->getPositionY()+HP_COUNTER_START_Y;
	InitCounter(L_HP_NUM_0,fStartPositionX,fStartPositionY,enermyHP);
	
	//显示右方(我方HP)
	fStartPositionX = m_pRightBarBK->getPositionX()+HP_COUNTER_START_X;
    fStartPositionY = m_pRightBarBK->getPositionY()+HP_COUNTER_START_Y;
	InitCounter(R_HP_NUM_0,fStartPositionX,fStartPositionY,playerHP);
	//显示敌方SP
	fStartPositionX = m_pLeftBarBK->getPositionX()+SP_COUNTER_START_X;
	fStartPositionY = m_pLeftBarBK->getPositionY()+SP_COUNTER_START_Y;
	InitCounter(L_SP_NUM_0,fStartPositionX,fStartPositionY,m_pEnermyTable->m_nSP);
	//显示我方SP
	fStartPositionX = m_pRightBarBK->getPositionX()+SP_COUNTER_START_X;
	fStartPositionY = m_pRightBarBK->getPositionY()+SP_COUNTER_START_Y;
	InitCounter(R_SP_NUM_0,fStartPositionX,fStartPositionY,m_pPlayerTable->m_nSP);


	InitDuelPosition();
	setTouchEnabled(true);
}

void DuelLayer::WaveCollisionUpdate(float time)
{
	if(m_pWaves->count()!=0)
	{

		CCAnimation* charAnimation = NULL;
		CCAnimation* shadowAnimation = NULL;
		for(unsigned int index = 0;index<m_pWaves->count();index++)
		{
			CCSprite* pWave = (CCSprite*)m_pWaves->objectAtIndex(index);
			//我方势力标识与飞行道具不一致
			if(m_pPlayerChar->GetForceFlag()!=pWave->getTag())
			{
				if(m_pPlayer->boundingBox().containsPoint(ccp(pWave->getPositionX(),pWave->getPositionY())))
				{
					//飞行道具命中目标，将飞行道具移除队列并释放掉
					m_pWaves->removeObject(pWave);
					pWave->stopAllActions();
					pWave->runAction(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::KillWaveCallFun),(void*)pWave));
					AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pPlayerChar->GetCharID(),ACTION_DUEL_HIT,0,MAX_ANIMATION_FRAME);
					//硬直动作容器
					CCArray* defActions = CCArray::createWithCapacity(2);
					//设置硬直以及恢复动作
					defActions->addObject(CCAnimate::create(charAnimation));	
					if(m_pPlayer == m_pAttacker)
					{
						defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::AtkHitRecover),NULL));
					}
					else
					{
						defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::DefHitRecover),NULL));
					}
					//我方被飞行道具打到了
					m_pPlayer->stopAllActions();
					m_pPlayerShadow->stopAllActions();
					m_pPlayer->runAction(CCSequence::create(defActions));
					m_pPlayerShadow->runAction(CCAnimate::create(shadowAnimation));

					//受到伤害，打击计数+1
					m_nHits++;
					//显示伤害数值,更新血条
					int toalDamage = 0;
					int damage = 0;
					int playerHP = 0;
					if(m_pPlayer == m_pAttacker)
					{
						toalDamage = m_pAttackerTable->m_nHPOffset;
						playerHP = m_pAttackerTable->m_nHP;
					}
					else
					{
						toalDamage = m_pDefenderTable->m_nHPOffset;
						playerHP = m_pDefenderTable->m_nHP;
					}
				
					if(m_nHits == m_nToalHits)
					{
						damage = toalDamage - m_nDamage;
					}
					else
					{
						damage = toalDamage/m_nToalDamScope;
					}
					m_nDamage += damage;
					CCPoint tStartPoint = ccp(0,0);
					tStartPoint.x = m_pRightBarBK->getPositionX()+GetSpriteTextureLength(m_pRightBarBK)-NUMS_CREATE_POSITION_X;
					tStartPoint.y = m_pRightBarBK->getPositionY()+GetSpriteTextureHeight(m_pRightBarBK);
					m_pRightHP->setPercentage(((playerHP-m_nDamage)*100)/m_pPlayerChar->GetMaxHP());
					RefreshHPCounter(playerHP-(m_nDamage-damage),playerHP-m_nDamage);
					ShowNums(tStartPoint,damage);

				}
			}
			//敌方势力标识与飞行道具不一致
			else
			{
				if(m_pEnermy->boundingBox().containsPoint(ccp(pWave->getPositionX(),pWave->getPositionY())))
				{
					//飞行道具命中目标后，移除出队列并释放掉
					m_pWaves->removeObject(pWave);
					pWave->stopAllActions();
					pWave->runAction(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::KillWaveCallFun),(void*)pWave));
					AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pEnermyChar->GetCharID(),ACTION_DUEL_HIT,0,MAX_ANIMATION_FRAME);
					CCArray* defActions = CCArray::createWithCapacity(2);
					//设置被敌方的硬直以及恢复动作
					defActions->addObject(CCAnimate::create(charAnimation));	
					if(m_pEnermy == m_pDefender)
					{
						defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::DefHitRecover),NULL));
					}
					else
					{
						defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::AtkHitRecover),NULL));
					}
					
					//敌方被飞行道具命中，开始响应	
					m_pEnermy->stopAllActions();
					m_pEnermyShadow->stopAllActions();
					m_pEnermy->runAction(CCSequence::create(defActions));
					m_pEnermyShadow->runAction(CCAnimate::create(shadowAnimation));
					//受到伤害，打击计数+1
					m_nHits++;
					//显示伤害数值,更新血条
					int toalDamage = 0;
					int damage = 0;
					int enermyHP = 0;
					if(m_pEnermy == m_pAttacker)
					{
						toalDamage = m_pAttackerTable->m_nHPOffset;
						enermyHP = m_pAttackerTable->m_nHP;
					}
					else
					{
						toalDamage = m_pDefenderTable->m_nHPOffset;
						enermyHP = m_pDefenderTable->m_nHP;
					}
					if(m_nHits == m_nToalHits)
					{
						damage = toalDamage - m_nDamage;
					}
					else
					{
						damage = toalDamage/m_nToalDamScope;
					}
					m_nDamage += damage;
					CCPoint tStartPoint = ccp(0,0);
					tStartPoint.x = m_pLeftBarBK->getPositionX()+NUMS_CREATE_POSITION_X;
					tStartPoint.y = m_pLeftBarBK->getPositionY()+GetSpriteTextureHeight(m_pLeftBarBK);
					m_pLeftHP->setPercentage(((enermyHP-m_nDamage)*100)/m_pEnermyChar->GetMaxHP());
					RefreshHPCounter(enermyHP-(m_nDamage-damage),enermyHP-m_nDamage,true);
					ShowNums(tStartPoint,damage);

				}
			}
		}
	}
	else
	{
		unschedule(schedule_selector(DuelLayer::WaveCollisionUpdate));

	}
}

void DuelLayer::FomatTableActions(CCArray* &charActions,CCArray* &shadowActions,int charID,int occureFrameOffset,SEL_CallFuncND callFunc,CCSprite* pChar)
{
	CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	if(m_pCombatTable!=NULL)
	{
		for(unsigned int index = 0;index<m_pCombatTable->count();index++)
		{
			CombatTable* table = (CombatTable*)m_pCombatTable->objectAtIndex(index);
			//将攻击动作执行到攻击奏效的发生的前一帧
			if(occureFrameOffset<table->m_nOccureFrame)
			{
				AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,charID,ACTION_DUEL_ATK,occureFrameOffset,table->m_nOccureFrame-1);
				charActions->addObject(CCAnimate::create(charAnimation));
				shadowActions->addObject(CCAnimate::create(shadowAnimation));
				//将帧数调整至响应帧
				occureFrameOffset = table->m_nOccureFrame;
			}
			//处理战斗所带来的响应动画
			if(table->m_nDamScope>=0||table->m_nWaveID>=0)
			{
				//该攻击动作会造成硬直、伤害
				charActions->addObject(CCCallFuncND::create(this,callFunc,(void*)table));
			}
			if(table->m_nMoveDur>0)
			{
				////该动作会带来角色位移,那么执行后边的相应帧数的动画，并作位移处理
				charActions->addObject(CCPlace::create(ccp(pChar->getPositionX()+table->m_fMoveOffsetX,pChar->getPositionY()+table->m_fMoveOffsetY)));
				shadowActions->addObject(CCPlace::create(ccp(pChar->getPositionX()+table->m_fMoveOffsetX,pChar->getPositionY())));
			}
		}
	}
	//将攻击动作剩下的帧数执行完毕
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,charID,ACTION_DUEL_ATK,occureFrameOffset,MAX_ANIMATION_FRAME);
	if(charAnimation!=NULL&&shadowAnimation!=NULL)
	{
		charActions->addObject(CCAnimate::create(charAnimation));
		shadowActions->addObject(CCAnimate::create(shadowAnimation));
	}
}
void DuelLayer::BeginActionChan()
{
	CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	//攻击预备动作   只有少量角色适用
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pAttackerChar->GetCharID(),ACTION_DUEL_ATKSTANDBY,0,MAX_ANIMATION_FRAME);
	if(charAnimation!=NULL)
	{
		m_pAttackerActions->addObject(CCAnimate::create(charAnimation));
		m_pAttackerShadowActions->addObject(CCAnimate::create(shadowAnimation));
	}
	//跑到防御方的面前
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pAttackerChar->GetCharID(),ACTION_DUEL_RUN,0,MAX_ANIMATION_FRAME);
	float offsetX = m_pDefender->getPositionX() - m_pAttacker->getPositionX();
	float textureOffset = GetSpriteTextureLength( m_pAttacker);
	if(offsetX<0)
	{
		textureOffset = textureOffset*(-1);
	}
	if(charAnimation!=NULL)
	{
		CCActionInterval* charMoveBy = CCMoveBy::create(charAnimation->getDuration(),ccp(offsetX - textureOffset,0));
		CCActionInterval* shadowMoveBy = CCMoveBy::create(shadowAnimation->getDuration(),ccp(offsetX - textureOffset,0));
		m_pAttackerActions->addObject(CCSpawn::create(CCAnimate::create(charAnimation),charMoveBy,NULL));
		m_pAttackerShadowActions->addObject(CCSpawn::create(CCAnimate::create(shadowAnimation),shadowMoveBy,NULL));
	}
	else if(m_pAttackerChar->GetMinAtkRange()==1&&m_pAttackerChar->GetCharID()!=1)
	{
		m_pAttacker->setPosition(ccp(m_pAttacker->getPositionX()+offsetX - textureOffset,m_pAttacker->getPositionY()));
		m_pAttackerShadow->setPosition(ccp(m_pAttackerShadow->getPositionX()+offsetX - textureOffset,m_pAttackerShadow->getPositionY()));
	}
	//设置攻击动作以及它所造成的影响所造成的响应

	GetAtkTableFromID(m_pAttackerChar->GetCharID(),DUEL_COMBAT_STYLE_1);
	int occureFrameOffset = 0;	
	FomatTableActions(m_pAttackerActions,m_pAttackerShadowActions,m_pAttackerChar->GetCharID(), occureFrameOffset,callfuncND_selector(DuelLayer::CreateAtkCallFunc),m_pAttacker);
	//在攻击方演示攻击动作之后判断还击
	if(m_nMode == DUELMODE_ATK_ENERMY||m_nMode ==DUELMODE_ATK_PLAYER)
	{
		//还击开始
		m_pAttackerActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::BeginFightBackChan),NULL));
	}
	else
	{
		//结束场景
		m_pAttackerActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::EndPlayCallBack),NULL));
	}


	//动作列表安排完毕，开始执行
	m_pAttacker->runAction(CCSequence::create(m_pAttackerActions));
	m_pAttackerShadow->runAction(CCSequence::create(m_pAttackerShadowActions));
	//防御方站立

	DefHitRecover(this,NULL);
	/*CCUserDefault* save =  CCUserDefault::sharedUserDefault();

	save->setBoolForKey("bool_value",true); 
	save->setDoubleForKey("double_value",0.1); 
	save->setFloatForKey("float_value",0.1f); 
	save->setIntegerForKey("integer_value",1); 
	save->setStringForKey("string_value","test");
	save->flush();*/
}
void DuelLayer::KillWaveCallFun(CCNode *sender,void* data)
{
	CCSprite *pSprite = (CCSprite*)data;
	((CCSprite*)data)->removeFromParentAndCleanup(true);
}

void DuelLayer::EndPlayCallBack(CCNode *sender,void* data)
{
	//if(m_nMode == DUELMODE_ATK_ENERMY||m_nMode == DUELMODE_ATK_ENERMY_ONLY||m_nMode == DUELMODE_ATK_ENERMY_RANGE||m_nMode == DUELMODE_ATK_ENERMY_RANGE_ONLY)
	//{
	//	((BattleMap*)getParent()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID))->AIActionsChan();
	//}
	///*if(m_pPlayerChar->IsBattlable())
	//{
	//	m_pPlayerChar->RefreshPlayer();
	//}
	//else
	//{
	//	m_pPlayerChar->RemoveFromBattle();
	//}
	//if (!m_pEnermyChar->IsBattlable())
	//{
	//	m_pEnermyChar->RemoveFromBattle();
	//}*/
	//}
	removeFromParentAndCleanup(true);
}

void DuelLayer::DefHitRecover(CCNode *sender,void* data)
{
	CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pDefenderChar->GetCharID(),ACTION_DUEL_STAND,0,MAX_ANIMATION_FRAME);
	m_pDefender->stopAllActions();
	m_pDefenderShadow->stopAllActions();
	m_pDefender->runAction(CCRepeatForever::create(CCAnimate::create(charAnimation)));
	m_pDefenderShadow->runAction(CCRepeatForever::create(CCAnimate::create(shadowAnimation)));

}

void DuelLayer::AtkHitRecover(CCNode *sender,void* data)
{
	CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pAttackerChar->GetCharID(),ACTION_DUEL_STAND,0,MAX_ANIMATION_FRAME);
	m_pAttacker->stopAllActions();
	m_pAttackerShadow->stopAllActions();
	m_pAttacker->runAction(CCRepeatForever::create(CCAnimate::create(charAnimation)));
	m_pAttackerShadow->runAction(CCRepeatForever::create(CCAnimate::create(shadowAnimation)));

}
void DuelLayer::BeginFightBackChan(CCNode *sender,void* data)
{
	//获取还击角色的攻击列表
	//尝试调用第二种攻击动作
	int style = DUEL_COMBAT_STYLE_2;
	GetAtkTableFromID(m_pDefenderChar->GetCharID(),style);
	if(m_pCombatTable == NULL||0==m_pCombatTable->count())
	{
		//该角色只存在一种攻击动作
		style = DUEL_COMBAT_STYLE_1;
		GetAtkTableFromID(m_pDefenderChar->GetCharID(),style);
	}
	CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	//根据情况调用攻击动作
	int occureFrameOffset = 0;
	if(style == DUEL_COMBAT_STYLE_2)
	{
		occureFrameOffset = DUEL_ATK_STYLE_FRAME;
	}
	FomatTableActions(m_pDefenderActions,m_pDefenderShadowActions,m_pDefenderChar->GetCharID(),occureFrameOffset,callfuncND_selector(DuelLayer::CreateFightBackCallFunc),m_pDefender);
	m_pDefenderActions->addObject(::CCCallFuncND::create(this,callfuncND_selector(DuelLayer::EndPlayCallBack),NULL));

	//防御方的还击动作容器的生成完毕，开始执行动作
	while(m_pWaves->count()>0)
	{
		
	}
	m_pDefender->stopAllActions();
	m_pDefenderShadow->stopAllActions();
	m_pDefender->runAction(CCSequence::create(m_pDefenderActions));
	m_pDefenderShadow->runAction(CCSequence::create(m_pDefenderShadowActions));

	//攻击方开始“待机”
	AtkHitRecover(this,NULL);
}

void DuelLayer::CreateFightBackCallFunc(CCNode *sender,void* tableData)
{
	CombatTable* table = (CombatTable*)tableData; 
	CCSprite* wave = NULL;
	CCAnimation* waveAni = NULL;
	//是否有“波”产生
	if(table->m_nWaveID!= DUEL_UNWAVE_ID)
	{
		//产生防御方的"波"
		char frameStr[50];
		waveAni = AnimationManager::instance()->CreateDuelWaveAnimation(table->m_nWaveID);
		sprintf(frameStr,"%s%d%s0.png",DUEL_WAVE_ID_NAME,table->m_nWaveID,DUEL_WAVE_FRAME_NAME);
		wave = CCSprite::createWithSpriteFrameName(frameStr);
		wave->setTag(m_pDefenderChar->GetForceFlag());
		addChild(wave,MAX_LAYER_ZORDER);
		wave->setPosition(ccp(m_pDefender->getPositionX()+table->m_fWavePosX,m_pDefender->getPositionY()+table->m_fWavePosY));
		CCArray* pWaveActions = CCArray::createWithCapacity(2);
		if(table->m_bKeepWave==true)
		{

			//将“波”视为飞行道具
			m_pWaves->addObject(wave);
			schedule(schedule_selector(DuelLayer::WaveCollisionUpdate));
			//飞行道具按直线行进
			float winX = CCDirector::sharedDirector()->getWinSize().width;
			float targetX = wave->getPositionX()<winX/2?winX:0;
			pWaveActions->addObject(CCSpawn::createWithTwoActions(CCAnimate::create(waveAni),CCMoveTo::create(WAVE_SPEED_HIGH,ccp(targetX,wave->getPositionY()))));
		}
		else
		{
			pWaveActions->addObject(CCAnimate::create(waveAni));
		}
		pWaveActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::KillWaveCallFun),(void*)wave));
		wave->runAction(CCSequence::create(pWaveActions));
	}

	if(table->m_nDamScope>=0)
	{



		//根据攻击表中的硬直等数据使被攻击方进行响应
		CCAnimation* charAnimation = NULL;
		CCAnimation* shadowAnimation = NULL;
		int nScope = table->m_nDamScope;
		//产生硬直动画
		do 
		{
			AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pAttackerChar->GetCharID(),ACTION_DUEL_HIT,nScope*10,MAX_ANIMATION_FRAME);
			nScope--;	
		} while (charAnimation==NULL&&nScope>=0);
		
		//硬直动作容器
		CCArray* defActions = CCArray::createWithCapacity(2);
		//设置硬直以及恢复动作
		defActions->addObject(CCAnimate::create(charAnimation));	
		defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::AtkHitRecover),NULL));
		//攻击方 开始应对 防御方的还击响应
		m_pAttacker->stopAllActions();
		m_pAttackerShadow->stopAllActions();
		m_pAttacker->runAction(CCSequence::create(defActions));
		m_pAttackerShadow->runAction(CCAnimate::create(shadowAnimation));
		//受到伤害，打击计数+1
		m_nHits++;
		//显示伤害数值,更新血条
		int damage = 0;
		if(m_nHits == m_nToalHits)
		{
			damage = m_pAttackerTable->m_nHPOffset - m_nDamage;
		}
		else
		{
			damage = m_pAttackerTable->m_nHPOffset*(table->m_nDamScope+1)/m_nToalDamScope;
		}
		m_nDamage += damage;
		CCPoint tStartPoint = ccp(0,0);
		if(m_pAttacker == m_pEnermy)
		{
			tStartPoint.x = m_pLeftBarBK->getPositionX()+NUMS_CREATE_POSITION_X;
			tStartPoint.y = m_pLeftBarBK->getPositionY()+NUMS_CREATE_POSITION_Y;
			m_pLeftHP->setPercentage(((m_pAttackerTable->m_nHP-m_nDamage)*100)/m_pEnermyChar->GetMaxHP());
			RefreshHPCounter(m_pAttackerTable->m_nHP-(m_nDamage-damage),m_pAttackerTable->m_nHP-m_nDamage,true);
		}
		else
		{
			tStartPoint.x = m_pRightBarBK->getPositionX()+GetSpriteTextureLength(m_pRightBarBK)-NUMS_CREATE_POSITION_X;
			tStartPoint.y = m_pRightBarBK->getPositionY()+NUMS_CREATE_POSITION_Y;
			m_pRightHP->setPercentage(((m_pAttackerTable->m_nHP-m_nDamage)*100)/m_pPlayerChar->GetMaxHP());
			RefreshHPCounter(m_pAttackerTable->m_nHP-(m_nDamage-damage),m_pAttackerTable->m_nHP-m_nDamage);
		}
		ShowNums(tStartPoint,damage);
	}
}


void DuelLayer::CreateAtkCallFunc(CCNode *sender,void* tableData)
{
	CombatTable* table = (CombatTable*)tableData; 
	CCSprite* wave = NULL;
	CCAnimation* waveAni = NULL;
	//是否有“波”产生
	if(table->m_nWaveID!= DUEL_UNWAVE_ID)
	{
		char frameStr[50];
		waveAni = AnimationManager::instance()->CreateDuelWaveAnimation(table->m_nWaveID);
		sprintf(frameStr,"%s%d%s0.png",DUEL_WAVE_ID_NAME,table->m_nWaveID,DUEL_WAVE_FRAME_NAME);
		wave = CCSprite::createWithSpriteFrameName(frameStr);
		wave->setTag(m_pAttackerChar->GetForceFlag());
		addChild(wave,MAX_LAYER_ZORDER);
		wave->setPosition(ccp(m_pAttacker->getPositionX()+table->m_fWavePosX,m_pAttacker->getPositionY()+table->m_fWavePosY));
		CCArray* pWaveActions = CCArray::createWithCapacity(2);
		if(table->m_bKeepWave==true)
		{
			//将“波”视为飞行道具
			m_pWaves->addObject(wave);
			schedule(schedule_selector(DuelLayer::WaveCollisionUpdate));
			//飞行道具按直线行进
			float winX = CCDirector::sharedDirector()->getWinSize().width;
			float targetX = wave->getPositionX()<winX/2?winX:0;
			pWaveActions->addObject(CCSpawn::createWithTwoActions(CCAnimate::create(waveAni),CCMoveTo::create(WAVE_SPEED_HIGH,ccp(targetX,wave->getPositionY()))));
		}
		else
		{
			pWaveActions->addObject(CCAnimate::create(waveAni));
		}
		pWaveActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::KillWaveCallFun),(void*)wave));
		wave->runAction(CCSequence::create(pWaveActions));
	}
	if(table->m_nDamScope>=0)
	{
		//根据攻击表中的硬直等数据使被攻击方进行响应
		CCAnimation* charAnimation = NULL;
		CCAnimation* shadowAnimation = NULL;
		int nScope = table->m_nDamScope;
		//产生硬直动画
		do 
		{
			AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pDefenderChar->GetCharID(),ACTION_DUEL_HIT,nScope*10,MAX_ANIMATION_FRAME);
			nScope--;	
		} while (charAnimation==NULL&&nScope>=0);
		CCArray* defActions = CCArray::createWithCapacity(2);
		//设置被攻击方的硬直以及恢复动作
		defActions->addObject(CCAnimate::create(charAnimation));	
		defActions->addObject(CCCallFuncND::create(this,callfuncND_selector(DuelLayer::DefHitRecover),NULL));
		//防御方开始响应	
		m_pDefender->stopAllActions();
		m_pDefenderShadow->stopAllActions();
		m_pDefender->runAction(CCSequence::create(defActions));
		m_pDefenderShadow->runAction(CCAnimate::create(shadowAnimation));

		//受到伤害，打击计数+1
		m_nHits++;
		//显示伤害数值,更新血条
		int damage = 0;
		if(m_nHits == m_nToalHits)
		{
			damage = m_pDefenderTable->m_nHPOffset - m_nDamage;
		}
		else
		{
			damage = m_pDefenderTable->m_nHPOffset*(table->m_nDamScope+1)/m_nToalDamScope;
		}
		m_nDamage += damage;
		CCPoint tStartPoint = ccp(0,0);
		if(m_pDefender== m_pEnermy)
		{
			tStartPoint.x = m_pLeftBarBK->getPositionX()+NUMS_CREATE_POSITION_X;
			tStartPoint.y = m_pLeftBarBK->getPositionY()+NUMS_CREATE_POSITION_Y;
			m_pLeftHP->setPercentage(((m_pDefenderTable->m_nHP-m_nDamage)*100)/m_pEnermyChar->GetMaxHP());
			RefreshHPCounter(m_pDefenderTable->m_nHP-(m_nDamage-damage),m_pDefenderTable->m_nHP-m_nDamage,true);
		}
		else
		{
			tStartPoint.x = m_pRightBarBK->getPositionX()+GetSpriteTextureLength(m_pRightBarBK)-NUMS_CREATE_POSITION_X;
			tStartPoint.y = m_pRightBarBK->getPositionY()+NUMS_CREATE_POSITION_Y;
			m_pRightHP->setPercentage(((m_pDefenderTable->m_nHP-m_nDamage)*100)/m_pPlayerChar->GetMaxHP());
			RefreshHPCounter(m_pDefenderTable->m_nHP-(m_nDamage-damage),m_pDefenderTable->m_nHP-m_nDamage);
		}
		ShowNums(tStartPoint,damage);
	}
}

void DuelLayer::ShowNums(CCPoint &tStartPoint,int num)
{
	CCArray* numSprites = AnimationManager::instance()->CreateSizeNums(num,NUMS_BIG_BLACK);
	unsigned int count = numSprites->count();
	float distance = NUMS_BIG_DISTANCE;
	float half = count%2==0?0:distance/2;
	for(unsigned int index = 0;index<count;index++)
	{
		CCSprite* numSprite = (CCSprite*)numSprites->objectAtIndex(index);
		addChild(numSprite,MAX_LAYER_ZORDER);
		numSprite->setPosition(ccp(tStartPoint.x+half+(count/2-index)*distance,tStartPoint.y));
		numSprite->runAction(CCSequence::create(CCEaseSineIn::create(CCMoveBy::create(NUMS_DUELFALL_DUR,ccp(0,-NUMS_CREATE_POSITION_Y/2))),CCFadeOut::create(NUMS_FADEOUT_DUR),CCCallFuncND::create(this,callfuncND_selector(AnimationManager::RemoveAniamationSprite),(void*)numSprite),NULL));
	}
}

void DuelLayer::InitDuelPosition()
{
	if(m_nMode == DUELMODE_ATK_ENERMY||m_nMode ==DUELMODE_ATK_ENERMY_ONLY||m_nMode == DUELMODE_ATK_PLAYER||m_nMode ==DUELMODE_ATK_PLAYER_ONLY)
	{
		m_pPlayer->setAnchorPoint(ccp(0.5,0));
		m_pPlayer->setPosition(ccp(550,DUAL_STANDPOSITION));
		m_pPlayerShadow->setAnchorPoint(ccp(0.5,1));
		m_pPlayerShadow->setPosition(ccp(m_pPlayer->getPositionX(),m_pPlayer->getPositionY()));
		m_pEnermy->setAnchorPoint(ccp(0.5,0));
		m_pEnermy->setPosition(ccp(190,DUAL_STANDPOSITION));
		m_pEnermyShadow->setAnchorPoint(ccp(0.5,1));
		m_pEnermyShadow->setPosition(ccp(m_pEnermy->getPositionX(),m_pEnermy->getPositionY()));
	}
	else if(m_nMode ==DUELMODE_ATK_PLAYER_RANGE||m_nMode ==DUELMODE_ATK_PLAYER_RANGE_ONLY||m_nMode ==DUELMODE_ATK_ENERMY_RANGE||m_nMode ==DUELMODE_ATK_ENERMY_RANGE_ONLY)
	{

	}
}

void DuelLayer::GetAtkTableFromID(int nID,int nLevel)
{
	//重置打击计数
	m_nHits = 0;
	m_nToalHits = 0;
	//重置伤害统计
	m_nDamage = 0;
	m_nToalDamScope = 0;
	std::ifstream fileinput;
	//清空动作表
	if(m_pCombatTable!=NULL)
	{
		m_pCombatTable->removeAllObjects();
	}else{
		m_pCombatTable= CCArray::createWithCapacity(10);
		m_pCombatTable->retain();
	}
	//解析动作表json数据
	unsigned long length = 3000;
	unsigned char* jsondata = CCFileUtils::sharedFileUtils()->getFileData("DATA/DuelTable.json","r",&length);

	Json::Reader reader;
	Json::Value root;
	reader.parse((char*)jsondata,(char*)(jsondata+length-1),root);
	//解析之后 回收缓存
	delete jsondata;
	//reader.parse(fileinput,root,false);
	Json::Value table = root["table"];
	if (table.isNull()){
		CCLOG("table is null!");
		return;
	}
    Json::Value character = table[nID-1]["character"];
	Json::Value style =character[ (unsigned int)0]["style"];
	Json::Value actionIndex = style[nLevel-1];
	//查找当前攻击模式的json节点
	if (actionIndex.isNull())
	{
		CCLOG("actionIndex is null");
		return;
	}
	Json::Value action = actionIndex["action"];
	if(action.isNull()){
		CCLOG("action is null");
		
	}


	
	//获取action节点，装配table
	for(Json::Value::iterator it = action.begin();it!=action.end();it++){	  
			int occuredFrame = atoi((*it)["occuredFrame"].asCString());
			CCLOG("------%d-----",occuredFrame);
			int waveID =  atoi((*it)["waveID"].asCString());
			int wavePosX = atoi((*it)["wavePosX"].asCString());
			int wavePosY = atoi((*it)["wavePosY"].asCString());
			int moveX = atoi((*it)["moveX"].asCString());
			int moveY =  atoi((*it)["moveY"].asCString());
			bool keepWave =  (*it)["keepWave"].asString()=="true"?true:false;
			int moveDur =  atoi((*it)["moveDur"].asCString());
		  int damScope =  atoi((*it)["damScope"].asCString());
			CombatTable *pTable =new CombatTable(occuredFrame,waveID,wavePosX,wavePosY,damScope,keepWave,moveDur,moveX,moveY);
			m_nToalDamScope += damScope+1;
			if(waveID>=0&&keepWave){
				m_nToalDamScope++;
			}
			if(damScope>=0){
				m_nToalHits++;
			}
			m_pCombatTable->addObject(pTable);
			
	}
	

	/*if(m_pCombatTable!=NULL)
	{
		m_pCombatTable->release();
		m_pCombatTable = NULL; 
	}
	if(nLevel == DUEL_COMBAT_STYLE_2)
	{
		if(nID ==1)
		{
			m_nToalHits = DUEL_CHAR1_S2_HITS;
			m_nToalDamScope =  DUEL_CHAR1_S2_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR1_S2_TIMES);
			m_pCombatTable->retain();
			CombatTable* table = new CombatTable(
				DUEL_CHAR1_S2_N1_OCCFRAME,
				DUEL_CHAR1_S2_N1_WAVEID,
				DUEL_CHAR1_S2_N1_WAVEPOSX,
				DUEL_CHAR1_S2_N1_WAVEPOSY ,
				DUEL_CHAR1_S2_N1_DAMSCOPE ,
				DUEL_CHAR1_S2_N1_KEEPWAVE,
				DUEL_CHAR1_S2_N1_MOVEDUR,
				DUEL_CHAR1_S2_N1_MOVEX, 
				DUEL_CHAR1_S2_N1_MOVEY 
				);
			m_pCombatTable->addObject(table);
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N2_OCCFRAME,
				DUEL_CHAR1_S2_N2_WAVEID,
				DUEL_CHAR1_S2_N2_WAVEPOSX,
				DUEL_CHAR1_S2_N2_WAVEPOSY ,
				DUEL_CHAR1_S2_N2_DAMSCOPE ,
				DUEL_CHAR1_S2_N2_KEEPWAVE,
				DUEL_CHAR1_S2_N2_MOVEDUR,
				DUEL_CHAR1_S2_N2_MOVEX, 
				DUEL_CHAR1_S2_N2_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N3_OCCFRAME,
				DUEL_CHAR1_S2_N3_WAVEID,
				DUEL_CHAR1_S2_N3_WAVEPOSX,
				DUEL_CHAR1_S2_N3_WAVEPOSY ,
				DUEL_CHAR1_S2_N3_DAMSCOPE ,
				DUEL_CHAR1_S2_N3_KEEPWAVE,
				DUEL_CHAR1_S2_N3_MOVEDUR,
				DUEL_CHAR1_S2_N3_MOVEX, 
				DUEL_CHAR1_S2_N3_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N4_OCCFRAME,
				DUEL_CHAR1_S2_N4_WAVEID,
				DUEL_CHAR1_S2_N4_WAVEPOSX,
				DUEL_CHAR1_S2_N4_WAVEPOSY ,
				DUEL_CHAR1_S2_N4_DAMSCOPE ,
				DUEL_CHAR1_S2_N4_KEEPWAVE,
				DUEL_CHAR1_S2_N4_MOVEDUR,
				DUEL_CHAR1_S2_N4_MOVEX, 
				DUEL_CHAR1_S2_N4_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N5_OCCFRAME,
				DUEL_CHAR1_S2_N5_WAVEID,
				DUEL_CHAR1_S2_N5_WAVEPOSX,
				DUEL_CHAR1_S2_N5_WAVEPOSY ,
				DUEL_CHAR1_S2_N5_DAMSCOPE ,
				DUEL_CHAR1_S2_N5_KEEPWAVE,
				DUEL_CHAR1_S2_N5_MOVEDUR,
				DUEL_CHAR1_S2_N5_MOVEX, 
				DUEL_CHAR1_S2_N5_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N6_OCCFRAME,
				DUEL_CHAR1_S2_N6_WAVEID,
				DUEL_CHAR1_S2_N6_WAVEPOSX,
				DUEL_CHAR1_S2_N6_WAVEPOSY ,
				DUEL_CHAR1_S2_N6_DAMSCOPE ,
				DUEL_CHAR1_S2_N6_KEEPWAVE,
				DUEL_CHAR1_S2_N6_MOVEDUR,
				DUEL_CHAR1_S2_N6_MOVEX, 
				DUEL_CHAR1_S2_N6_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N7_OCCFRAME,
				DUEL_CHAR1_S2_N7_WAVEID,
				DUEL_CHAR1_S2_N7_WAVEPOSX,
				DUEL_CHAR1_S2_N7_WAVEPOSY ,
				DUEL_CHAR1_S2_N7_DAMSCOPE ,
				DUEL_CHAR1_S2_N7_KEEPWAVE,
				DUEL_CHAR1_S2_N7_MOVEDUR,
				DUEL_CHAR1_S2_N7_MOVEX, 
				DUEL_CHAR1_S2_N7_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N8_OCCFRAME,
				DUEL_CHAR1_S2_N8_WAVEID,
				DUEL_CHAR1_S2_N8_WAVEPOSX,
				DUEL_CHAR1_S2_N8_WAVEPOSY ,
				DUEL_CHAR1_S2_N8_DAMSCOPE ,
				DUEL_CHAR1_S2_N8_KEEPWAVE,
				DUEL_CHAR1_S2_N8_MOVEDUR,
				DUEL_CHAR1_S2_N8_MOVEX, 
				DUEL_CHAR1_S2_N8_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N9_OCCFRAME,
				DUEL_CHAR1_S2_N9_WAVEID,
				DUEL_CHAR1_S2_N9_WAVEPOSX,
				DUEL_CHAR1_S2_N9_WAVEPOSY ,
				DUEL_CHAR1_S2_N9_DAMSCOPE ,
				DUEL_CHAR1_S2_N9_KEEPWAVE,
				DUEL_CHAR1_S2_N9_MOVEDUR,
				DUEL_CHAR1_S2_N9_MOVEX, 
				DUEL_CHAR1_S2_N9_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S2_N10_OCCFRAME,
				DUEL_CHAR1_S2_N10_WAVEID,
				DUEL_CHAR1_S2_N10_WAVEPOSX,
				DUEL_CHAR1_S2_N10_WAVEPOSY ,
				DUEL_CHAR1_S2_N10_DAMSCOPE ,
				DUEL_CHAR1_S2_N10_KEEPWAVE,
				DUEL_CHAR1_S2_N10_MOVEDUR,
				DUEL_CHAR1_S2_N10_MOVEX, 
				DUEL_CHAR1_S2_N10_MOVEY 
				));
			return;
		}
		else if (nID == 16)
		{
			m_nToalHits = DUEL_CHAR16_S2_HITS;
			m_nToalDamScope =  DUEL_CHAR16_S2_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR16_S2_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR16_S2_N1_OCCFRAME,
				DUEL_CHAR16_S2_N1_WAVEID,
				DUEL_CHAR16_S2_N1_WAVEPOSX,
				DUEL_CHAR16_S2_N1_WAVEPOSY ,
				DUEL_CHAR16_S2_N1_DAMSCOPE ,
				DUEL_CHAR16_S2_N1_KEEPWAVE,
				DUEL_CHAR16_S2_N1_MOVEDUR,
				DUEL_CHAR16_S2_N1_MOVEX, 
				DUEL_CHAR16_S2_N1_MOVEY 
				));
		}
	}
	else if(nLevel == DUEL_COMBAT_STYLE_1)
	{
		if(nID == 1)
		{
			m_nToalHits = DUEL_CHAR1_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR1_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR1_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N1_OCCFRAME,
				DUEL_CHAR1_S1_N1_WAVEID,
				DUEL_CHAR1_S1_N1_WAVEPOSX,
				DUEL_CHAR1_S1_N1_WAVEPOSY ,
				DUEL_CHAR1_S1_N1_DAMSCOPE ,
				DUEL_CHAR1_S1_N1_KEEPWAVE,
				DUEL_CHAR1_S1_N1_MOVEDUR,
				DUEL_CHAR1_S1_N1_MOVEX, 
				DUEL_CHAR1_S1_N1_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N2_OCCFRAME,
				DUEL_CHAR1_S1_N2_WAVEID,
				DUEL_CHAR1_S1_N2_WAVEPOSX,
				DUEL_CHAR1_S1_N2_WAVEPOSY ,
				DUEL_CHAR1_S1_N2_DAMSCOPE ,
				DUEL_CHAR1_S1_N2_KEEPWAVE,
				DUEL_CHAR1_S1_N2_MOVEDUR,
				DUEL_CHAR1_S1_N2_MOVEX, 
				DUEL_CHAR1_S1_N2_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N3_OCCFRAME,
				DUEL_CHAR1_S1_N3_WAVEID,
				DUEL_CHAR1_S1_N3_WAVEPOSX,
				DUEL_CHAR1_S1_N3_WAVEPOSY ,
				DUEL_CHAR1_S1_N3_DAMSCOPE ,
				DUEL_CHAR1_S1_N3_KEEPWAVE,
				DUEL_CHAR1_S1_N3_MOVEDUR,
				DUEL_CHAR1_S1_N3_MOVEX, 
				DUEL_CHAR1_S1_N3_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N4_OCCFRAME,
				DUEL_CHAR1_S1_N4_WAVEID,
				DUEL_CHAR1_S1_N4_WAVEPOSX,
				DUEL_CHAR1_S1_N4_WAVEPOSY ,
				DUEL_CHAR1_S1_N4_DAMSCOPE ,
				DUEL_CHAR1_S1_N4_KEEPWAVE,
				DUEL_CHAR1_S1_N4_MOVEDUR,
				DUEL_CHAR1_S1_N4_MOVEX, 
				DUEL_CHAR1_S1_N4_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N5_OCCFRAME,
				DUEL_CHAR1_S1_N5_WAVEID,
				DUEL_CHAR1_S1_N5_WAVEPOSX,
				DUEL_CHAR1_S1_N5_WAVEPOSY ,
				DUEL_CHAR1_S1_N5_DAMSCOPE ,
				DUEL_CHAR1_S1_N5_KEEPWAVE,
				DUEL_CHAR1_S1_N5_MOVEDUR,
				DUEL_CHAR1_S1_N5_MOVEX, 
				DUEL_CHAR1_S1_N5_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N6_OCCFRAME,
				DUEL_CHAR1_S1_N6_WAVEID,
				DUEL_CHAR1_S1_N6_WAVEPOSX,
				DUEL_CHAR1_S1_N6_WAVEPOSY ,
				DUEL_CHAR1_S1_N6_DAMSCOPE ,
				DUEL_CHAR1_S1_N6_KEEPWAVE,
				DUEL_CHAR1_S1_N6_MOVEDUR,
				DUEL_CHAR1_S1_N6_MOVEX, 
				DUEL_CHAR1_S1_N6_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N7_OCCFRAME,
				DUEL_CHAR1_S1_N7_WAVEID,
				DUEL_CHAR1_S1_N7_WAVEPOSX,
				DUEL_CHAR1_S1_N7_WAVEPOSY ,
				DUEL_CHAR1_S1_N7_DAMSCOPE ,
				DUEL_CHAR1_S1_N7_KEEPWAVE,
				DUEL_CHAR1_S1_N7_MOVEDUR,
				DUEL_CHAR1_S1_N7_MOVEX, 
				DUEL_CHAR1_S1_N7_MOVEY 
				));
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR1_S1_N8_OCCFRAME,
				DUEL_CHAR1_S1_N8_WAVEID,
				DUEL_CHAR1_S1_N8_WAVEPOSX,
				DUEL_CHAR1_S1_N8_WAVEPOSY ,
				DUEL_CHAR1_S1_N8_DAMSCOPE ,
				DUEL_CHAR1_S1_N8_KEEPWAVE,
				DUEL_CHAR1_S1_N8_MOVEDUR,
				DUEL_CHAR1_S1_N8_MOVEX, 
				DUEL_CHAR1_S1_N8_MOVEY 
				));
		}
		else if(nID == 2)
		{
			m_nToalHits = DUEL_CHAR2_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR2_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR2_S1_TIMES);
			m_pCombatTable->retain();
			CombatTable* table = new CombatTable(
				DUEL_CHAR2_S1_N1_OCCFRAME,
				DUEL_CHAR2_S1_N1_WAVEID,
				DUEL_CHAR2_S1_N1_WAVEPOSX,
				DUEL_CHAR2_S1_N1_WAVEPOSY,
				DUEL_CHAR2_S1_N1_DAMSCOPE,
				DUEL_CHAR2_S1_N1_KEEPWAVE,
				DUEL_CHAR2_S1_N1_MOVEDUR,
				DUEL_CHAR2_S1_N1_MOVEX, 
				DUEL_CHAR2_S1_N1_MOVEY 
				);
			m_pCombatTable->addObject(table);
		}
		else if(nID == 3)
		{
			m_nToalHits = DUEL_CHAR3_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR3_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR3_S1_TIMES);
			m_pCombatTable->retain();

			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR3_S1_N1_OCCFRAME,
				DUEL_CHAR3_S1_N1_WAVEID,
				DUEL_CHAR3_S1_N1_WAVEPOSX,
				DUEL_CHAR3_S1_N1_WAVEPOSY,
				DUEL_CHAR3_S1_N1_DAMSCOPE,
				DUEL_CHAR3_S1_N1_KEEPWAVE,
				DUEL_CHAR3_S1_N1_MOVEDUR,
				DUEL_CHAR3_S1_N1_MOVEX, 
				DUEL_CHAR3_S1_N1_MOVEY 
				));
		}
		else if (nID == 4)
		{
			m_nToalHits = DUEL_CHAR4_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR4_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR4_S1_TIMES);
			m_pCombatTable->retain();

			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR4_S1_N1_OCCFRAME,
				DUEL_CHAR4_S1_N1_WAVEID,
				DUEL_CHAR4_S1_N1_WAVEPOSX,
				DUEL_CHAR4_S1_N1_WAVEPOSY,
				DUEL_CHAR4_S1_N1_DAMSCOPE,
				DUEL_CHAR4_S1_N1_KEEPWAVE,
				DUEL_CHAR4_S1_N1_MOVEDUR,
				DUEL_CHAR4_S1_N1_MOVEX, 
				DUEL_CHAR4_S1_N1_MOVEY 
				));
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR4_S1_N2_OCCFRAME,
				DUEL_CHAR4_S1_N2_WAVEID,
				DUEL_CHAR4_S1_N2_WAVEPOSX,
				DUEL_CHAR4_S1_N2_WAVEPOSY,
				DUEL_CHAR4_S1_N2_DAMSCOPE,
				DUEL_CHAR4_S1_N2_KEEPWAVE,
				DUEL_CHAR4_S1_N2_MOVEDUR,
				DUEL_CHAR4_S1_N2_MOVEX, 
				DUEL_CHAR4_S1_N2_MOVEY 
				));
		}
		else if (nID == 5)
		{
			m_nToalHits = DUEL_CHAR5_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR5_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR5_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR5_S1_N1_OCCFRAME,
				DUEL_CHAR5_S1_N1_WAVEID,
				DUEL_CHAR5_S1_N1_WAVEPOSX,
				DUEL_CHAR5_S1_N1_WAVEPOSY,
				DUEL_CHAR5_S1_N1_DAMSCOPE,
				DUEL_CHAR5_S1_N1_KEEPWAVE,
				DUEL_CHAR5_S1_N1_MOVEDUR,
				DUEL_CHAR5_S1_N1_MOVEX, 
				DUEL_CHAR5_S1_N1_MOVEY 
				));
		}
		else if (nID== 6)
		{
			m_nToalHits = DUEL_CHAR6_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR6_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR6_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR6_S1_N1_OCCFRAME,
				DUEL_CHAR6_S1_N1_WAVEID,
				DUEL_CHAR6_S1_N1_WAVEPOSX,
				DUEL_CHAR6_S1_N1_WAVEPOSY,
				DUEL_CHAR6_S1_N1_DAMSCOPE,
				DUEL_CHAR6_S1_N1_KEEPWAVE,
				DUEL_CHAR6_S1_N1_MOVEDUR,
				DUEL_CHAR6_S1_N1_MOVEX, 
				DUEL_CHAR6_S1_N1_MOVEY 
				));
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR6_S1_N2_OCCFRAME,
				DUEL_CHAR6_S1_N2_WAVEID,
				DUEL_CHAR6_S1_N2_WAVEPOSX,
				DUEL_CHAR6_S1_N2_WAVEPOSY,
				DUEL_CHAR6_S1_N2_DAMSCOPE,
				DUEL_CHAR6_S1_N2_KEEPWAVE,
				DUEL_CHAR6_S1_N2_MOVEDUR,
				DUEL_CHAR6_S1_N2_MOVEX, 
				DUEL_CHAR6_S1_N2_MOVEY 
				));
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR6_S1_N3_OCCFRAME,
				DUEL_CHAR6_S1_N3_WAVEID,
				DUEL_CHAR6_S1_N3_WAVEPOSX,
				DUEL_CHAR6_S1_N3_WAVEPOSY,
				DUEL_CHAR6_S1_N3_DAMSCOPE,
				DUEL_CHAR6_S1_N3_KEEPWAVE,
				DUEL_CHAR6_S1_N3_MOVEDUR,
				DUEL_CHAR6_S1_N3_MOVEX, 
				DUEL_CHAR6_S1_N3_MOVEY 
				));
		}
		else if(nID == 7)
		{	
			m_nToalHits = DUEL_CHAR7_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR7_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR7_S1_TIMES);
			m_pCombatTable->retain();
			CombatTable* table = new CombatTable(
				DUEL_CHAR7_S1_N1_OCCFRAME
				,DUEL_CHAR7_S1_N1_WAVEID,
				DUEL_CHAR7_S1_N1_WAVEPOSX,
				DUEL_CHAR7_S1_N1_WAVEPOSY ,
				DUEL_CHAR7_S1_N1_DAMSCOPE ,
				DUEL_CHAR7_S1_N1_KEEPWAVE,
				DUEL_CHAR7_S1_N1_MOVEDUR,
				DUEL_CHAR7_S1_N1_MOVEX, 
				DUEL_CHAR7_S1_N1_MOVEY 
				);
			m_pCombatTable->addObject(table);
		}
		else if (nID == 8)
		{
			m_nToalHits = DUEL_CHAR8_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR8_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR8_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR8_S1_N1_OCCFRAME,
				DUEL_CHAR8_S1_N1_WAVEID,
				DUEL_CHAR8_S1_N1_WAVEPOSX,
				DUEL_CHAR8_S1_N1_WAVEPOSY,
				DUEL_CHAR8_S1_N1_DAMSCOPE,
				DUEL_CHAR8_S1_N1_KEEPWAVE,
				DUEL_CHAR8_S1_N1_MOVEDUR,
				DUEL_CHAR8_S1_N1_MOVEX, 
				DUEL_CHAR8_S1_N1_MOVEY 
				));
		}
		else if (nID == 9)
		{
			m_nToalHits = DUEL_CHAR9_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR9_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR9_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR9_S1_N1_OCCFRAME,
				DUEL_CHAR9_S1_N1_WAVEID,
				DUEL_CHAR9_S1_N1_WAVEPOSX,
				DUEL_CHAR9_S1_N1_WAVEPOSY,
				DUEL_CHAR9_S1_N1_DAMSCOPE,
				DUEL_CHAR9_S1_N1_KEEPWAVE,
				DUEL_CHAR9_S1_N1_MOVEDUR,
				DUEL_CHAR9_S1_N1_MOVEX, 
				DUEL_CHAR9_S1_N1_MOVEY 
				));
		}
		else if(nID == 10)
		{	
			m_nToalHits = DUEL_CHAR10_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR10_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR10_S1_TIMES);
			m_pCombatTable->retain();
			CombatTable* table = new CombatTable(
				DUEL_CHAR10_S1_N1_OCCFRAME,
				DUEL_CHAR10_S1_N1_WAVEID,
				DUEL_CHAR10_S1_N1_WAVEPOSX,
				DUEL_CHAR10_S1_N1_WAVEPOSY ,
				DUEL_CHAR10_S1_N1_DAMSCOPE ,
				DUEL_CHAR10_S1_N1_KEEPWAVE,
				DUEL_CHAR10_S1_N1_MOVEDUR,
				DUEL_CHAR10_S1_N1_MOVEX, 
				DUEL_CHAR10_S1_N1_MOVEY 
				);
			m_pCombatTable->addObject(table);
			m_pCombatTable->addObject(new CombatTable(
				DUEL_CHAR10_S1_N2_OCCFRAME,
				DUEL_CHAR10_S1_N2_WAVEID,
				DUEL_CHAR10_S1_N2_WAVEPOSX,
				DUEL_CHAR10_S1_N2_WAVEPOSY,
				DUEL_CHAR10_S1_N2_DAMSCOPE,
				DUEL_CHAR10_S1_N2_KEEPWAVE,
				DUEL_CHAR10_S1_N2_MOVEDUR,
				DUEL_CHAR10_S1_N2_MOVEX, 
				DUEL_CHAR10_S1_N2_MOVEY 
				));
		}
		else if (nID == 11)
		{
			m_nToalHits = DUEL_CHAR11_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR11_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR11_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR11_S1_N1_OCCFRAME,
				DUEL_CHAR11_S1_N1_WAVEID,
				DUEL_CHAR11_S1_N1_WAVEPOSX,
				DUEL_CHAR11_S1_N1_WAVEPOSY,
				DUEL_CHAR11_S1_N1_DAMSCOPE,
				DUEL_CHAR11_S1_N1_KEEPWAVE,
				DUEL_CHAR11_S1_N1_MOVEDUR,
				DUEL_CHAR11_S1_N1_MOVEX, 
				DUEL_CHAR11_S1_N1_MOVEY 
				));
		}
		else if (nID == 12)
		{
			m_nToalHits = DUEL_CHAR12_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR12_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR12_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR12_S1_N1_OCCFRAME,
				DUEL_CHAR12_S1_N1_WAVEID,
				DUEL_CHAR12_S1_N1_WAVEPOSX,
				DUEL_CHAR12_S1_N1_WAVEPOSY,
				DUEL_CHAR12_S1_N1_DAMSCOPE,
				DUEL_CHAR12_S1_N1_KEEPWAVE,
				DUEL_CHAR12_S1_N1_MOVEDUR,
				DUEL_CHAR12_S1_N1_MOVEX, 
				DUEL_CHAR12_S1_N1_MOVEY 
				));
		}
		else if (nID == 13)
		{
			m_nToalHits = DUEL_CHAR13_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR13_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR13_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR13_S1_N1_OCCFRAME,
				DUEL_CHAR13_S1_N1_WAVEID,
				DUEL_CHAR13_S1_N1_WAVEPOSX,
				DUEL_CHAR13_S1_N1_WAVEPOSY,
				DUEL_CHAR13_S1_N1_DAMSCOPE,
				DUEL_CHAR13_S1_N1_KEEPWAVE,
				DUEL_CHAR13_S1_N1_MOVEDUR,
				DUEL_CHAR13_S1_N1_MOVEX, 
				DUEL_CHAR13_S1_N1_MOVEY 
				));
		}
		else if(nID == 14)
		{	
			m_nToalHits = DUEL_CHAR14_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR14_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR14_S1_TIMES);
			m_pCombatTable->retain();
			CombatTable* table = new CombatTable(
				DUEL_CHAR14_S1_N1_OCCFRAME,
				DUEL_CHAR14_S1_N1_WAVEID,
				DUEL_CHAR14_S1_N1_WAVEPOSX,
				DUEL_CHAR14_S1_N1_WAVEPOSY ,
				DUEL_CHAR14_S1_N1_DAMSCOPE ,
				DUEL_CHAR14_S1_N1_KEEPWAVE,
				DUEL_CHAR14_S1_N1_MOVEDUR,
				DUEL_CHAR14_S1_N1_MOVEX, 
				DUEL_CHAR14_S1_N1_MOVEY 
				);
			m_pCombatTable->addObject(table);
		}
		else if (nID == 15)
		{
			m_nToalHits = DUEL_CHAR15_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR15_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR15_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR15_S1_N1_OCCFRAME,
				DUEL_CHAR15_S1_N1_WAVEID,
				DUEL_CHAR15_S1_N1_WAVEPOSX,
				DUEL_CHAR15_S1_N1_WAVEPOSY,
				DUEL_CHAR15_S1_N1_DAMSCOPE,
				DUEL_CHAR15_S1_N1_KEEPWAVE,
				DUEL_CHAR15_S1_N1_MOVEDUR,
				DUEL_CHAR15_S1_N1_MOVEX, 
				DUEL_CHAR15_S1_N1_MOVEY 
				));
		}
		else if (nID == 16)
		{
			m_nToalHits = DUEL_CHAR16_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR16_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR16_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR16_S1_N1_OCCFRAME,
				DUEL_CHAR16_S1_N1_WAVEID,
				DUEL_CHAR16_S1_N1_WAVEPOSX,
				DUEL_CHAR16_S1_N1_WAVEPOSY,
				DUEL_CHAR16_S1_N1_DAMSCOPE,
				DUEL_CHAR16_S1_N1_KEEPWAVE,
				DUEL_CHAR16_S1_N1_MOVEDUR,
				DUEL_CHAR16_S1_N1_MOVEX, 
				DUEL_CHAR16_S1_N1_MOVEY 
				));
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR16_S1_N2_OCCFRAME,
				DUEL_CHAR16_S1_N2_WAVEID,
				DUEL_CHAR16_S1_N2_WAVEPOSX,
				DUEL_CHAR16_S1_N2_WAVEPOSY,
				DUEL_CHAR16_S1_N2_DAMSCOPE,
				DUEL_CHAR16_S1_N2_KEEPWAVE,
				DUEL_CHAR16_S1_N2_MOVEDUR,
				DUEL_CHAR16_S1_N2_MOVEX, 
				DUEL_CHAR16_S1_N2_MOVEY 
				));
		}
		else if (nID == 17)
		{
			m_nToalHits = DUEL_CHAR17_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR17_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR17_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR17_S1_N1_OCCFRAME,
				DUEL_CHAR17_S1_N1_WAVEID,
				DUEL_CHAR17_S1_N1_WAVEPOSX,
				DUEL_CHAR17_S1_N1_WAVEPOSY,
				DUEL_CHAR17_S1_N1_DAMSCOPE,
				DUEL_CHAR17_S1_N1_KEEPWAVE,
				DUEL_CHAR17_S1_N1_MOVEDUR,
				DUEL_CHAR17_S1_N1_MOVEX, 
				DUEL_CHAR17_S1_N1_MOVEY 
				));
		}
		else if (nID == 18)
		{
			m_nToalHits = DUEL_CHAR18_S1_HITS;
			m_nToalDamScope =  DUEL_CHAR18_S1_TOALDAMSCOPE;
			m_pCombatTable = CCArray::createWithCapacity( DUEL_CHAR18_S1_TIMES);
			m_pCombatTable->retain();
			m_pCombatTable->addObject( new CombatTable(
				DUEL_CHAR18_S1_N1_OCCFRAME,
				DUEL_CHAR18_S1_N1_WAVEID,
				DUEL_CHAR18_S1_N1_WAVEPOSX,
				DUEL_CHAR18_S1_N1_WAVEPOSY,
				DUEL_CHAR18_S1_N1_DAMSCOPE,
				DUEL_CHAR18_S1_N1_KEEPWAVE,
				DUEL_CHAR18_S1_N1_MOVEDUR,
				DUEL_CHAR18_S1_N1_MOVEX, 
				DUEL_CHAR18_S1_N1_MOVEY 
				));
		}
	}*/


}


/*bool DuelLayer::init()
{
bool  bRet = CCLayer::init();


return bRet;
}*/

void DuelLayer::onEnter()
{
	CCLayer::onEnter();
	/*CCAnimation* charAnimation = NULL;
	CCAnimation* shadowAnimation = NULL;
	AnimationManager::instance()->CreateDuelActionAnimation(charAnimation,shadowAnimation,m_pAttackerChar->GetCharID(),ACTION_DUEL_ATK,0,15);
	m_pAttacker->runAction(CCRepeatForever::create(CCAnimate::create(charAnimation)));
	m_pAttackerShadow->runAction(CCRepeatForever::create(CCAnimate::create(shadowAnimation)));*/
	BeginActionChan();
}

void DuelLayer::RefreshHPCounter(int nFrom,int nTo,bool bLeft)
{
	if (nTo<0)
	{
		nTo = 0;
	}
	int nTag = bLeft?L_HP_NUM_0:R_HP_NUM_0;
	int nTagend = bLeft?L_HP_NUM_9:R_HP_NUM_9;
	int nDev = 10;
	for (;nTag<=nTagend;nTag++)
	{
		CCSprite* pNum = (CCSprite*)m_pNumsBatchNode->getChildByTag(nTag);
		pNum->stopAllActions();
		pNum->runAction(CCAnimate::create(AnimationManager::instance()->CreateNumsRunToAnimation(nFrom%nDev,nTo%nDev,NUMS_SMALL_BLACK)));
		nFrom = nFrom/nDev;
		nTo = nTo/nDev;
	}
}

void DuelLayer::InitCounter( int nTag,float fStartX,float fStartY,unsigned nNum)
{
	float fOffset = NUMS_SMALL_DISTANCE;
	CCArray* pNums = AnimationManager::instance()->CreateSizeNums(nNum,NUMS_SMALL_BLACK,true);
	CCObject* pObject = NULL;
	int nCount = 0;
	CCARRAY_FOREACH(pNums,pObject)
	{	
		CCSprite* pNum = (CCSprite*)pObject;
		m_pNumsBatchNode->addChild(pNum,0,nTag+nCount);
		pNum->setPositionX(fStartX-nCount*fOffset);
		pNum->setPositionY(fStartY);
		nCount++;
		if (nCount>9)
		{
			break;
		}
	}
}

void DuelLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);  //注册并设置当前面板触摸的优先	
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	CCLayer* pRunningLayer = (CCLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LAYER_BATTLEMAP_LAYER_ID);
	if (pRunningLayer!=NULL)
	{
		pRunningLayer->setTouchEnabled(true);
	}
}

bool DuelLayer::ccTouchBegan( CCTouch* pTouch,CCEvent* pEvent )
{
	return true;
}

void DuelLayer::ccTouchEnded( CCTouch* pTouch,CCEvent* pEvent )
{
	
}

void DuelLayer::onExit()
{
	CCLayer::onExit();
	BattleCharacter::RefeshAfterDuel();
}



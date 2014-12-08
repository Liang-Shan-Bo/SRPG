#ifndef _RANGE_OBJECT_H_
#define _RANGE_OBJECT_H_

#include "cocos2d.h"
#include <vector>
#include <algorithm> 
#include "BattleResources.h"

#define CACULATE_FVALUE(a) abs((a)->m_nX)+abs((a)->m_nY)+(a)->m_nCost
enum twards_enum{up_enum = 2 ,down_enum = 1,left_enum = 4,right_enum = 3};


using namespace std;


USING_NS_CC;
enum range_enum{
	
	drhombus_range_enum//菱形
	
};
void ConvertTouchPointToRange(int pointX,int pointY,int* rangeX,int* rangeY);
void ConvertRangeToPosition(int rangeX,int rangeY,int* pointX,int* pointY);
void ConvertPositionToRange(int pointX,int pointY,int* rangeX,int* rangeY);

struct PathPoint:public CCObject
{
	int m_nY;//节点的横坐标
	int m_nX;//节点的纵坐标
	int m_nCost;//经过当前节点的额外消耗
	bool m_bAvailable;//是否可以使用，作为寻路算法中是否可以作为移动点使用。
	int m_nFValue;//节点的A*算法F评价值
	int m_nSumCost;//当建立搜寻算法时，提供当前节点和父节们点的总消耗
	twards_enum m_nTward;//A*算法完成之后，当前节点的朝向
	PathPoint* m_pParent;//A*算法完成之后，当前节点路径的父节点
	PathPoint* twardPoints[4];//代表着自身4个方向的节点
	//节点构造函数
	PathPoint(int x,int y)
	{
		m_nSumCost = 0;
		m_pParent = NULL;
		m_nFValue = 0;
		m_nX = x;
		m_nY = y;
		m_nCost = 0;
		m_bAvailable = false;
		for(int i = 0;i<4;i++)
		{
			twardPoints[i] = NULL;
		}
	}
	
};


class RangeObject
{
protected:
	CCArray *m_pOffsets;//节点容器;
	int m_nRange;//节点基础范围
	int m_nCapcity;//节点容器容量
	virtual void  CaculateArea() = 0;//计算范围，创建节点加入容器。建立节点之间的联系
	virtual void  CaculateCapcity() = 0;//计算自身容器容量

public:
	virtual void RefreshAvailableRange(bool bAvailable) = 0;//刷新容器节点可使用状态
	virtual void RefreshRangeAt(bool bAvailable,int nRange) = 0;
	virtual void RefreshRangeTo(bool bAvailable,int nRangeStart,int nRangeTo) = 0;
	virtual void CheckMoveable() = 0;//查询路径范围
	virtual bool SearchTarget(PathPoint* pTargetPoint) = 0;//查询路径
	void SetRange(int range)
	{
		m_nRange = range;
	}
	int GetRange()
	{
		return m_nRange;
	}
	int GetCapcity()
	{
		return m_nCapcity;
	}
	virtual CCArray * GetOffsets()
	{
		return 0;
	}
};

/*
经过初始化后的范围索引m_pOffsets是一个自中心点向外延伸的排列顺序的线性结构。如果要获取其中一个范围开始的索引,可以使用计算公式“2*(Range-1)*(Range)+1”来获取该范围的第一个元素，索引范围为4*Range,例如:
获取行动范围为3的边界索引: 2*2*3+1=13.也就是说m_pOffsets结构中，索引为13的点为范围菱形范围为3的边界点的起始索引,范围是4*3,一共为12个点。示意图如下:

										    3
                                          2 o 4
								        1 o x o 5 
									  	o x x x o 6
									  o x x x x x o
									  7	o x	x x o
										8 o	x o 12 
										  9 o 11
                                            10
*/

class DrhombusRangeObject:public RangeObject
{
protected:
	virtual void CaculateCapcity();
	virtual void CaculateArea();
private:
	
	void CreateEquiMoves();//创建菱形范围中所有节点
	void LinkPathNodes();//链接节点方向
	CCArray* m_pUpEquiMovelines;//范围值相等的上边线
	CCArray* m_pDownEquiMovelines;//范围值相等的下边线
	
	bool FixMoveableConditions(PathPoint* pPointOnCheck);//检测当前节点的总消耗点数是否超过容器范围最大值，若没超过则节点可用。
public:
    static int GetRangeOffsetsIndex(int rangeX,int rangeY,int maxRange);//获取自身的某个路径点在容器中的位置
	virtual void RefreshAvailableRange(bool bAvailable);//将容器中所有点的使用状态重置为bAvailable
	virtual void RefreshRangeAt(bool bAvailable,int nRange);
	virtual void RefreshRangeTo(bool bAvailable,int nRangeStart,int nRangeTo);
	virtual bool SearchTarget(PathPoint* pTargetPoint);//A*算法查找Target
	virtual CCArray * GetOffsets();//获取自身容器
	virtual void CheckMoveable();//4叉树深度遍历查询查询可使用的路径点
	DrhombusRangeObject(int range)
	{
		SetRange(range);
		m_pUpEquiMovelines = CCArray::createWithCapacity(m_nRange+1);
		m_pUpEquiMovelines->retain();
	    m_pDownEquiMovelines = CCArray::createWithCapacity(m_nRange+1);
		m_pDownEquiMovelines->retain();
		CaculateCapcity();
		CaculateArea();

	}

	~DrhombusRangeObject()
	{
		m_pDownEquiMovelines->release();
		m_pUpEquiMovelines->release();
		m_pOffsets->release();
	}


};

#endif  
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
	
	drhombus_range_enum//����
	
};
void ConvertTouchPointToRange(int pointX,int pointY,int* rangeX,int* rangeY);
void ConvertRangeToPosition(int rangeX,int rangeY,int* pointX,int* pointY);
void ConvertPositionToRange(int pointX,int pointY,int* rangeX,int* rangeY);

struct PathPoint:public CCObject
{
	int m_nY;//�ڵ�ĺ�����
	int m_nX;//�ڵ��������
	int m_nCost;//������ǰ�ڵ�Ķ�������
	bool m_bAvailable;//�Ƿ����ʹ�ã���ΪѰ·�㷨���Ƿ������Ϊ�ƶ���ʹ�á�
	int m_nFValue;//�ڵ��A*�㷨F����ֵ
	int m_nSumCost;//��������Ѱ�㷨ʱ���ṩ��ǰ�ڵ�͸����ǵ��������
	twards_enum m_nTward;//A*�㷨���֮�󣬵�ǰ�ڵ�ĳ���
	PathPoint* m_pParent;//A*�㷨���֮�󣬵�ǰ�ڵ�·���ĸ��ڵ�
	PathPoint* twardPoints[4];//����������4������Ľڵ�
	//�ڵ㹹�캯��
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
	CCArray *m_pOffsets;//�ڵ�����;
	int m_nRange;//�ڵ������Χ
	int m_nCapcity;//�ڵ���������
	virtual void  CaculateArea() = 0;//���㷶Χ�������ڵ���������������ڵ�֮�����ϵ
	virtual void  CaculateCapcity() = 0;//����������������

public:
	virtual void RefreshAvailableRange(bool bAvailable) = 0;//ˢ�������ڵ��ʹ��״̬
	virtual void RefreshRangeAt(bool bAvailable,int nRange) = 0;
	virtual void RefreshRangeTo(bool bAvailable,int nRangeStart,int nRangeTo) = 0;
	virtual void CheckMoveable() = 0;//��ѯ·����Χ
	virtual bool SearchTarget(PathPoint* pTargetPoint) = 0;//��ѯ·��
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
������ʼ����ķ�Χ����m_pOffsets��һ�������ĵ��������������˳������Խṹ�����Ҫ��ȡ����һ����Χ��ʼ������,����ʹ�ü��㹫ʽ��2*(Range-1)*(Range)+1������ȡ�÷�Χ�ĵ�һ��Ԫ�أ�������ΧΪ4*Range,����:
��ȡ�ж���ΧΪ3�ı߽�����: 2*2*3+1=13.Ҳ����˵m_pOffsets�ṹ�У�����Ϊ13�ĵ�Ϊ��Χ���η�ΧΪ3�ı߽�����ʼ����,��Χ��4*3,һ��Ϊ12���㡣ʾ��ͼ����:

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
	
	void CreateEquiMoves();//�������η�Χ�����нڵ�
	void LinkPathNodes();//���ӽڵ㷽��
	CCArray* m_pUpEquiMovelines;//��Χֵ��ȵ��ϱ���
	CCArray* m_pDownEquiMovelines;//��Χֵ��ȵ��±���
	
	bool FixMoveableConditions(PathPoint* pPointOnCheck);//��⵱ǰ�ڵ�������ĵ����Ƿ񳬹�������Χ���ֵ����û������ڵ���á�
public:
    static int GetRangeOffsetsIndex(int rangeX,int rangeY,int maxRange);//��ȡ�����ĳ��·�����������е�λ��
	virtual void RefreshAvailableRange(bool bAvailable);//�����������е��ʹ��״̬����ΪbAvailable
	virtual void RefreshRangeAt(bool bAvailable,int nRange);
	virtual void RefreshRangeTo(bool bAvailable,int nRangeStart,int nRangeTo);
	virtual bool SearchTarget(PathPoint* pTargetPoint);//A*�㷨����Target
	virtual CCArray * GetOffsets();//��ȡ��������
	virtual void CheckMoveable();//4������ȱ�����ѯ��ѯ��ʹ�õ�·����
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
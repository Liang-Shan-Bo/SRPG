#include "RangeObject.h"

/* * */
void ConvertTouchPointToRange(int pointX,int pointY,int *rangeX,int *rangeY)
{
	int countX	= (*rangeX - pointX)/(SQUARE_LENTH/2);
	int countY  = (*rangeY - pointY)/(SQUARE_LENTH/2);
	//�������ƫ��
	if(countX > 0)
	{
		*rangeX = (countX-1)/2 + 1;
	}
	else if(countX<0)
	{
		*rangeX = (countX+1)/2 - 1;
	}
	else
	{
		*rangeX = countX;
	}
	//��������ƫ��
	
	if(countY>0)
	{
		*rangeY = (countY-1)/2 + 1;
	}
	else if(countY<0)
	{
		*rangeY = (countY+1)/2 - 1;
	}
	else
	{
		*rangeY = countY;
	}

}
/* * */
void ConvertRangeToPosition(int rangeX,int rangeY,int* pointX,int* pointY)
{
	
	*pointX = *pointX+(SQUARE_LENTH)*rangeX;
	*pointY = *pointY+(SQUARE_LENTH)*rangeY;
	
}

void ConvertPositionToRange(int pointX,int pointY,int* rangeX,int* rangeY)
{
	*rangeX = (*rangeX-pointX)/(SQUARE_LENTH);
	*rangeY = (*rangeY-pointY)/(SQUARE_LENTH);
}



bool CompareF(PathPoint* pA,PathPoint* pB)
{
	return pA->m_nFValue>pB->m_nFValue;
}



CCArray* DrhombusRangeObject::GetOffsets()
{
		return m_pOffsets;
}

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
int DrhombusRangeObject::GetRangeOffsetsIndex(int rangeX,int rangeY,int maxRange)
{
	int toalRange =abs(rangeX) +abs(rangeY);
	if(toalRange>maxRange)
	{
		return -1;
	}
	else if(toalRange==0)
	{
		return 0;
	}
	int baseIndex = 2*(toalRange-1)*(toalRange);
	int indexCount = 4*toalRange;
	int indexOffset = 0;
	//m_pOffsets��һ����������x~(-range,range]��������x~[range,-range)˳���ʼ�������Խṹ
	if(rangeY>0)
	{
		indexOffset = rangeX+toalRange;
		
	}
	else if(rangeY<0)
	{
		indexOffset = rangeX+3*toalRange+1;
	}
	else if(rangeX<0)
	{
		indexOffset = 2*toalRange+1;
	}
	else
	{
		indexOffset = 2*toalRange;
	}
	return baseIndex+indexOffset;
}
void DrhombusRangeObject::RefreshAvailableRange(bool bAvailable)
{
	//������·�����ʼ��Ϊ"δ����"״̬
	PathPoint* pPointOnCheck = NULL;
	for(unsigned int index = 0;index <m_pOffsets->count();index++)
	{
		pPointOnCheck =((PathPoint*)m_pOffsets->objectAtIndex(index));
		pPointOnCheck->m_bAvailable = bAvailable;
	}
	
}
void DrhombusRangeObject::CreateEquiMoves()
{
	//��ʼ�����ĵ�
	CCArray* pUpMoveLine = CCArray::createWithCapacity(1);
	CCArray* pDownMoveLine  = pUpMoveLine;
	PathPoint* pOriginPoint = new PathPoint(0,0);
	pUpMoveLine->addObject(pOriginPoint);	
	m_pOffsets->addObject(pOriginPoint);
	m_pUpEquiMovelines->addObject(pUpMoveLine);
	m_pDownEquiMovelines->addObject(pDownMoveLine);
	/*ʹ����������upLine(-move,move]��downLine[-move,move)��ʾ��Χ��ȵĵ�,���������ʼ��upLine�Լ�downLine*/
	for(int move = 1;move<=m_nRange;move++)
	{
		//��ʼ��upLines
		CCArray* pUpMoveLine = CCArray::createWithCapacity(move+1);
		//�����������
		for(int ix = -move+1;ix<=move;ix++)
		{
			PathPoint* pPoint = new PathPoint(ix,move-abs(ix));
			//pPoint->autorelease();
			pUpMoveLine->addObject(pPoint);
			m_pOffsets->addObject(pPoint);
		}
		m_pUpEquiMovelines->addObject(pUpMoveLine);
		//��ʼ��downLines
		CCArray* pDownMoveLine = CCArray::createWithCapacity(move+1);
		//�����������
		for(int ix= -move;ix<move;ix++)
		{
			PathPoint* pPoint = new PathPoint(ix,abs(ix)-move);
			//pPoint->autorelease();
			pDownMoveLine->addObject(pPoint);
			m_pOffsets->addObject(pPoint);
		}
		m_pDownEquiMovelines->addObject(pDownMoveLine);
	}
	
}

void DrhombusRangeObject::LinkPathNodes()
{	//��ʼ�����ĵ�����
	PathPoint* pOriginPoint = (PathPoint*)((CCArray*)m_pUpEquiMovelines->objectAtIndex(0))->objectAtIndex(0);
	PathPoint* pUpPoint = (PathPoint*) ((CCArray*)m_pUpEquiMovelines->objectAtIndex(1))->objectAtIndex(0);
	pOriginPoint->twardPoints[((int)up_enum)-1]  = pUpPoint ;
	pUpPoint->twardPoints[((int)down_enum)-1] = pOriginPoint;
	PathPoint* pDownPoint = (PathPoint*)((CCArray*)m_pDownEquiMovelines->objectAtIndex(1))->objectAtIndex(1);
	pOriginPoint->twardPoints[((int)down_enum)-1]  = pDownPoint ;
	pDownPoint->twardPoints[((int)up_enum)-1] = pOriginPoint;
	PathPoint* pLeftPoint = (PathPoint*)((CCArray*)m_pDownEquiMovelines->objectAtIndex(1))->objectAtIndex(0);
	pOriginPoint->twardPoints[((int)left_enum)-1]  = pLeftPoint ;
	pLeftPoint->twardPoints[((int)right_enum)-1] = pOriginPoint;;
	PathPoint* pRightPoint = (PathPoint*)((CCArray*)m_pUpEquiMovelines->objectAtIndex(1))->objectAtIndex(1);
	pOriginPoint->twardPoints[((int)right_enum)-1]  = pRightPoint ;
	pRightPoint->twardPoints[((int)left_enum)-1] = pOriginPoint;


	//�����������Ӹ���·����
	for(int move =m_nRange;move>1;move--)
	{
		CCArray* pUpOutsideMoveLine =(CCArray*) m_pUpEquiMovelines->objectAtIndex(move);
		CCArray* pUpInsideMoveLine =(CCArray*) m_pUpEquiMovelines->objectAtIndex(move-1);
		CCArray* pDownOutsideMoveLine =(CCArray*) m_pDownEquiMovelines->objectAtIndex(move);
		CCArray* pDownInsideMoveLine =(CCArray*) m_pDownEquiMovelines->objectAtIndex(move-1);
		for(int index =0;index<2*move;index++)
		{
			PathPoint* pUpOutsidePoint = (PathPoint*)pUpOutsideMoveLine->objectAtIndex(index);
			PathPoint* pDownOutsidePoint = (PathPoint*)pDownOutsideMoveLine->objectAtIndex(index);
			//������Χ����Χ���᷽���·����----����ͷȥβ��(-move,move)
			
			if(0<index&&index<2*move-1)
			{
				//�����������������(-move+1,move-1)
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-1);
				pUpOutsidePoint->twardPoints[((int)down_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)up_enum)-1] = pUpOutsidePoint;
				//�����������������(-move+1,move-1)
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index-1);
				pDownOutsidePoint->twardPoints[((int)up_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)down_enum)-1] = pDownOutsidePoint;
			}
			//������������һ�������ڲ������ߵ�"β"����
			if(index==2*move-1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-2);
				pDownOutsidePoint->twardPoints[((int)up_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)down_enum)-1] = pDownOutsidePoint;
			}
			//��������ߵ�һ���������ڲ����ߵ�"ͷ"����
			if(index == 0)
			{
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index);
				pUpOutsidePoint->twardPoints[((int)down_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)up_enum)-1] = pUpOutsidePoint;
			}

			//������Χ����Χ���᷽���·����
			//���������������(-move,0)
			if(index<move-1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index);
				pUpOutsidePoint->twardPoints[((int)right_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)left_enum)-1] = pUpOutsidePoint;
			}
			//�������Ҳ�������
			if(index>move-1&&move>1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-2);
				pUpOutsidePoint->twardPoints[((int)left_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)right_enum)-1] = pUpOutsidePoint;
			}
			//���������������
			if(index<move)
			{
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index);
				pDownOutsidePoint->twardPoints[((int)right_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)left_enum)-1] = pDownOutsidePoint;
			}
			//�������Ҳ�������
			if(index>move&&move>1)
			{
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index-2);
				pDownOutsidePoint->twardPoints[((int)left_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)right_enum)-1] = pDownOutsidePoint;;
			}

		}

	}
}
void DrhombusRangeObject::CaculateCapcity()
{
	//2��ǰn��Ȳ����к�+��n+1��
	m_nCapcity =  2*m_nRange*m_nRange+2*m_nRange+1;
	m_pOffsets = CCArray::createWithCapacity(m_nCapcity);
	m_pOffsets->retain();
}

void DrhombusRangeObject::CaculateArea()
{
	CreateEquiMoves();
	if (m_nRange>0)
	{
		LinkPathNodes();
	}
	
	/*m_pOffsets = CCArray::createWithCapacity(m_nCapcity);
	for(int i = 0;i<= m_nRange;i++)
	{
		for(int j = 0;j<=m_nRange-i;j++)
		{
			m_pOffsets->addObject(new PathPoint(i,j));
			if(j!= 0)
			{
				m_pOffsets->addObject(new PathPoint(i,-j));
			}
			if(i!=0)
			{
				m_pOffsets->addObject(new PathPoint(-i,j));
				if(j!= 0)
				{
					m_pOffsets->addObject(new PathPoint(-i,-j));
				}
			}

		}
		
	}*/
}

void DrhombusRangeObject::CheckMoveable()
{

	//4������ȱ�����ѯ
	
	PathPoint* pPointOnCheck = (PathPoint*)GetOffsets()->objectAtIndex(0);
	pPointOnCheck->m_bAvailable = true;
	pPointOnCheck->m_nSumCost = pPointOnCheck->m_nCost;
	CCArray* pArrayOnCheck = CCArray::createWithObject(pPointOnCheck);
	CCArray* pTempArray =  CCArray::createWithCapacity(10);
	for(int step = 0;step<m_nRange;step++)
	{
		for(unsigned int index = 0;index<pArrayOnCheck->count();index++)
		{
			pPointOnCheck =  (PathPoint*)pArrayOnCheck->objectAtIndex(index);
			for(int tward = 0; tward<4;tward++)
			{
				PathPoint* twardPoint = pPointOnCheck->twardPoints[tward];
				if((twardPoint->m_bAvailable==false)||twardPoint->m_nSumCost>(pPointOnCheck->m_nSumCost+twardPoint->m_nCost+1))
				{
					//������cost
					twardPoint->m_nSumCost = (pPointOnCheck->m_nSumCost)+(twardPoint->m_nCost)+1;
					if(FixMoveableConditions(twardPoint))
					{
						pTempArray->addObject(twardPoint);
					}
				}
			}
		}
		pArrayOnCheck->removeAllObjects();
		pArrayOnCheck->addObjectsFromArray(pTempArray);
		pTempArray->removeAllObjects();
	}
}

bool DrhombusRangeObject::FixMoveableConditions(PathPoint* pPointOnCheck)
{

	if((m_nRange>=pPointOnCheck->m_nSumCost))
	{
		pPointOnCheck->m_bAvailable = true;
	}
	return (pPointOnCheck->m_bAvailable)&&(m_nRange>pPointOnCheck->m_nSumCost);
}


bool DrhombusRangeObject::SearchTarget(PathPoint *pTargetPoint)
{
	PathPoint* pSearchPoint = (PathPoint*)m_pOffsets->objectAtIndex(0);
	pSearchPoint->m_nFValue = CACULATE_FVALUE(pSearchPoint);
	vector<PathPoint*> m_vCloseArray;
	vector<PathPoint*> m_vOpenArray;
	m_vOpenArray.push_back(pSearchPoint);

	while(!m_vOpenArray.empty())
	{
		pSearchPoint = m_vOpenArray.front();
		
		if(pTargetPoint->m_nX == pSearchPoint->m_nX&&pTargetPoint->m_nY == pSearchPoint->m_nY)
		{
			return true;
		}
		//��������Ӵ��б����޳�
		m_vOpenArray.erase(m_vOpenArray.begin());
		for(int tward = 0;tward<4;tward++)
		{
			PathPoint* pTwardPoint = pSearchPoint->twardPoints[tward];
			if(pTwardPoint == NULL)
			{
				break;
			}
		
		      vector<PathPoint*>::iterator OpenArrayResult =m_vOpenArray.begin();  
              for (OpenArrayResult =m_vOpenArray.begin();  
                    OpenArrayResult != m_vOpenArray.end();++OpenArrayResult)  
               {  
				   if((*OpenArrayResult)->m_nX == pTwardPoint->m_nX&&(*OpenArrayResult)->m_nY == pTwardPoint->m_nY)
						break;			
               }
			   vector<PathPoint*>::iterator CloseArrayResult =m_vCloseArray.begin();  
              for (CloseArrayResult =m_vCloseArray.begin();  
                    CloseArrayResult != m_vCloseArray.end();++CloseArrayResult)  
               {  
				   if((*CloseArrayResult)->m_nX == pTwardPoint->m_nX&&(*CloseArrayResult)->m_nY == pTwardPoint->m_nY)
						break;
               }
			//�������㲻�ڹر��б�Ϳ����б���
			  if(CloseArrayResult==m_vCloseArray.end()&&OpenArrayResult==m_vOpenArray.end()&&pTwardPoint->m_bAvailable)
			{
				pTwardPoint->m_nFValue = CACULATE_FVALUE(pTwardPoint);
				m_vOpenArray.push_back(pTwardPoint);
				pTwardPoint->m_pParent = pSearchPoint;
				pTwardPoint->m_nTward  = (twards_enum)(tward+1);
				make_heap(m_vOpenArray.begin(),m_vOpenArray.end(),CompareF);
			}
			
		}	
		//�����ص���뵽�ر��б���
		m_vCloseArray.push_back(pSearchPoint);
	}
	return false;
}

void DrhombusRangeObject::RefreshRangeTo( bool bAvailable,int nRangeStart,int nRangeTo )
{
	if (nRangeStart>nRangeTo||nRangeStart<=0||nRangeTo>m_nRange)
	{
		return;
	}
	for(unsigned int index = 2*(nRangeStart-1)*(nRangeStart)+1;index!=2*(nRangeTo)*(nRangeTo+1)+1;index++)
	{
		PathPoint* pPoint = (PathPoint*)m_pOffsets->objectAtIndex(index);
		pPoint->m_bAvailable = bAvailable;
	}
}

void DrhombusRangeObject::RefreshRangeAt( bool bAvailable,int nRange )
{
	RefreshRangeTo(bAvailable,nRange,nRange);
}


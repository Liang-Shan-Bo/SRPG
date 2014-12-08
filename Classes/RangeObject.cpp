#include "RangeObject.h"

/* * */
void ConvertTouchPointToRange(int pointX,int pointY,int *rangeX,int *rangeY)
{
	int countX	= (*rangeX - pointX)/(SQUARE_LENTH/2);
	int countY  = (*rangeY - pointY)/(SQUARE_LENTH/2);
	//计算横向偏移
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
	//计算纵向偏移
	
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
	//m_pOffsets是一个从上折线x~(-range,range]到下折线x~[range,-range)顺序初始化的线性结构
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
	//将所有路径点初始化为"未检测过"状态
	PathPoint* pPointOnCheck = NULL;
	for(unsigned int index = 0;index <m_pOffsets->count();index++)
	{
		pPointOnCheck =((PathPoint*)m_pOffsets->objectAtIndex(index));
		pPointOnCheck->m_bAvailable = bAvailable;
	}
	
}
void DrhombusRangeObject::CreateEquiMoves()
{
	//初始化中心点
	CCArray* pUpMoveLine = CCArray::createWithCapacity(1);
	CCArray* pDownMoveLine  = pUpMoveLine;
	PathPoint* pOriginPoint = new PathPoint(0,0);
	pUpMoveLine->addObject(pOriginPoint);	
	m_pOffsets->addObject(pOriginPoint);
	m_pUpEquiMovelines->addObject(pUpMoveLine);
	m_pDownEquiMovelines->addObject(pDownMoveLine);
	/*使用两条折线upLine(-move,move]、downLine[-move,move)表示范围相等的点,由内向外初始化upLine以及downLine*/
	for(int move = 1;move<=m_nRange;move++)
	{
		//初始化upLines
		CCArray* pUpMoveLine = CCArray::createWithCapacity(move+1);
		//将点加入线中
		for(int ix = -move+1;ix<=move;ix++)
		{
			PathPoint* pPoint = new PathPoint(ix,move-abs(ix));
			//pPoint->autorelease();
			pUpMoveLine->addObject(pPoint);
			m_pOffsets->addObject(pPoint);
		}
		m_pUpEquiMovelines->addObject(pUpMoveLine);
		//初始化downLines
		CCArray* pDownMoveLine = CCArray::createWithCapacity(move+1);
		//将点加入线中
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
{	//初始化中心点连接
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


	//由外向内连接各个路径点
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
			//连接内围与外围纵轴方向的路径点----“掐头去尾”(-move,move)
			
			if(0<index&&index<2*move-1)
			{
				//上折线内外纵轴对齐(-move+1,move-1)
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-1);
				pUpOutsidePoint->twardPoints[((int)down_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)up_enum)-1] = pUpOutsidePoint;
				//下折线内外纵轴对齐(-move+1,move-1)
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index-1);
				pDownOutsidePoint->twardPoints[((int)up_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)down_enum)-1] = pDownOutsidePoint;
			}
			//外侧下折线最后一个点与内侧上折线的"尾"对齐
			if(index==2*move-1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-2);
				pDownOutsidePoint->twardPoints[((int)up_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)down_enum)-1] = pDownOutsidePoint;
			}
			//外侧上折线第一个点与下内侧折线的"头"对齐
			if(index == 0)
			{
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index);
				pUpOutsidePoint->twardPoints[((int)down_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)up_enum)-1] = pUpOutsidePoint;
			}

			//连接内围与外围横轴方向的路径点
			//上折线左侧横轴对齐(-move,0)
			if(index<move-1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index);
				pUpOutsidePoint->twardPoints[((int)right_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)left_enum)-1] = pUpOutsidePoint;
			}
			//上折线右侧横轴对齐
			if(index>move-1&&move>1)
			{
				PathPoint* pUpInsidePoint =(PathPoint*)pUpInsideMoveLine->objectAtIndex(index-2);
				pUpOutsidePoint->twardPoints[((int)left_enum)-1] = pUpInsidePoint;
				pUpInsidePoint->twardPoints[((int)right_enum)-1] = pUpOutsidePoint;
			}
			//下折线左侧横轴对齐
			if(index<move)
			{
				PathPoint* pDownInsidePoint =(PathPoint*)pDownInsideMoveLine->objectAtIndex(index);
				pDownOutsidePoint->twardPoints[((int)right_enum)-1] = pDownInsidePoint;
				pDownInsidePoint->twardPoints[((int)left_enum)-1] = pDownOutsidePoint;
			}
			//下折线右侧横轴对齐
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
	//2倍前n项等差数列和+第n+1项
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

	//4叉树深度遍历查询
	
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
					//计算总cost
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
		//将搜索点从打开列表中剔除
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
			//若搜索点不在关闭列表和开启列表当中
			  if(CloseArrayResult==m_vCloseArray.end()&&OpenArrayResult==m_vOpenArray.end()&&pTwardPoint->m_bAvailable)
			{
				pTwardPoint->m_nFValue = CACULATE_FVALUE(pTwardPoint);
				m_vOpenArray.push_back(pTwardPoint);
				pTwardPoint->m_pParent = pSearchPoint;
				pTwardPoint->m_nTward  = (twards_enum)(tward+1);
				make_heap(m_vOpenArray.begin(),m_vOpenArray.end(),CompareF);
			}
			
		}	
		//将搜素点加入到关闭列表当中
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


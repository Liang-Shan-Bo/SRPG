#ifndef DUAL_ATK_TABLE
#define DUAL_ATK_TABLE







#define WAVE_SPEED_HIGH 0.3f //高速移动飞行道具的时间间隔
#define WAVE_SPEED_LOW  1.0f //低速移动飞行道具的时间间隔



/*动作列表的定义:
当攻击动作会产生自身的位移，或使攻击目标产生攻击硬直时，使用此表
动作的发生帧定义：
1.每当目标有硬直产生（此时可能产生剑气等），记录此帧。
2.当角色产生位移时，记录下偏移帧数。
3.当1. 2. 同时发生时，首先以使目标硬直的动作为基准，将位移持续帧数分为两段定义
*/





//动作模式
#define DUEL_COMBAT_STYLE_1 1
#define DUEL_COMBAT_STYLE_2 2

#define DUEL_CHAR1_S2_TIMES 2
#define DUEL_CHAR1_S2_TOALDAMSCOPE 9
#define DUEL_CHAR1_S2_HITS 4

#define DUEL_CHAR1_S2_N1_OCCFRAME 32
#define DUEL_CHAR1_S2_N1_WAVEID 6
#define DUEL_CHAR1_S2_N1_WAVEPOSX 0	 
#define DUEL_CHAR1_S2_N1_WAVEPOSY 0
#define DUEL_CHAR1_S2_N1_DAMSCOPE 0  
#define DUEL_CHAR1_S2_N1_KEEPWAVE false
#define DUEL_CHAR1_S2_N1_MOVEX 0
#define DUEL_CHAR1_S2_N1_MOVEY 0
#define DUEL_CHAR1_S2_N1_MOVEDUR 0


#define DUEL_CHAR1_S2_N2_OCCFRAME 37
#define DUEL_CHAR1_S2_N2_WAVEID -1//0
#define DUEL_CHAR1_S2_N2_WAVEPOSX -47 
#define DUEL_CHAR1_S2_N2_WAVEPOSY 30
#define DUEL_CHAR1_S2_N2_DAMSCOPE 1
#define DUEL_CHAR1_S2_N2_KEEPWAVE false
#define DUEL_CHAR1_S2_N2_MOVEX 0
#define DUEL_CHAR1_S2_N2_MOVEY 0 
#define DUEL_CHAR1_S2_N2_MOVEDUR 0

#define DUEL_CHAR1_S2_N3_OCCFRAME 40
#define DUEL_CHAR1_S2_N3_WAVEID -1
#define DUEL_CHAR1_S2_N3_WAVEPOSX 0 
#define DUEL_CHAR1_S2_N3_WAVEPOSY 0
#define DUEL_CHAR1_S2_N3_DAMSCOPE -1
#define DUEL_CHAR1_S2_N3_KEEPWAVE false
#define DUEL_CHAR1_S2_N3_MOVEX 0
#define DUEL_CHAR1_S2_N3_MOVEY 45
#define DUEL_CHAR1_S2_N3_MOVEDUR 1


#define DUEL_CHAR1_S2_N4_OCCFRAME 42
#define DUEL_CHAR1_S2_N4_WAVEID -1//1
#define DUEL_CHAR1_S2_N4_WAVEPOSX 0 
#define DUEL_CHAR1_S2_N4_WAVEPOSY 0
#define DUEL_CHAR1_S2_N4_DAMSCOPE 2
#define DUEL_CHAR1_S2_N4_KEEPWAVE false
#define DUEL_CHAR1_S2_N4_MOVEX -47
#define DUEL_CHAR1_S2_N4_MOVEY 130
#define DUEL_CHAR1_S2_N4_MOVEDUR 0

#define DUEL_CHAR1_S2_N5_OCCFRAME 44
#define DUEL_CHAR1_S2_N5_WAVEID -1
#define DUEL_CHAR1_S2_N5_WAVEPOSX 0 
#define DUEL_CHAR1_S2_N5_WAVEPOSY 0
#define DUEL_CHAR1_S2_N5_DAMSCOPE -1
#define DUEL_CHAR1_S2_N5_KEEPWAVE false
#define DUEL_CHAR1_S2_N5_MOVEX 0
#define DUEL_CHAR1_S2_N5_MOVEY 70
#define DUEL_CHAR1_S2_N5_MOVEDUR 1


#define DUEL_CHAR1_S2_N6_OCCFRAME 46
#define DUEL_CHAR1_S2_N6_WAVEID 7
#define DUEL_CHAR1_S2_N6_WAVEPOSX 0  
#define DUEL_CHAR1_S2_N6_WAVEPOSY 0
#define DUEL_CHAR1_S2_N6_DAMSCOPE 2
#define DUEL_CHAR1_S2_N6_KEEPWAVE false
#define DUEL_CHAR1_S2_N6_MOVEX 0
#define DUEL_CHAR1_S2_N6_MOVEY 0
#define DUEL_CHAR1_S2_N6_MOVEDUR 0


#define DUEL_CHAR1_S2_N7_OCCFRAME 49
#define DUEL_CHAR1_S2_N7_WAVEID -1
#define DUEL_CHAR1_S2_N7_WAVEPOSX 0  
#define DUEL_CHAR1_S2_N7_WAVEPOSY 0
#define DUEL_CHAR1_S2_N7_DAMSCOPE -1
#define DUEL_CHAR1_S2_N7_KEEPWAVE false
#define DUEL_CHAR1_S2_N7_MOVEX -20
#define DUEL_CHAR1_S2_N7_MOVEY 45
#define DUEL_CHAR1_S2_N7_MOVEDUR 1

#define DUEL_CHAR1_S2_N8_OCCFRAME 50
#define DUEL_CHAR1_S2_N8_WAVEID -1
#define DUEL_CHAR1_S2_N8_WAVEPOSX 0  
#define DUEL_CHAR1_S2_N8_WAVEPOSY 0
#define DUEL_CHAR1_S2_N8_DAMSCOPE -1
#define DUEL_CHAR1_S2_N8_KEEPWAVE false
#define DUEL_CHAR1_S2_N8_MOVEX -20
#define DUEL_CHAR1_S2_N8_MOVEY 30
#define DUEL_CHAR1_S2_N8_MOVEDUR 1

#define DUEL_CHAR1_S2_N9_OCCFRAME 51
#define DUEL_CHAR1_S2_N9_WAVEID -1
#define DUEL_CHAR1_S2_N9_WAVEPOSX 0  
#define DUEL_CHAR1_S2_N9_WAVEPOSY 0
#define DUEL_CHAR1_S2_N9_DAMSCOPE -1
#define DUEL_CHAR1_S2_N9_KEEPWAVE false
#define DUEL_CHAR1_S2_N9_MOVEX 0
#define DUEL_CHAR1_S2_N9_MOVEY -6
#define DUEL_CHAR1_S2_N9_MOVEDUR 1

#define DUEL_CHAR1_S2_N10_OCCFRAME 52
#define DUEL_CHAR1_S2_N10_WAVEID -1
#define DUEL_CHAR1_S2_N10_WAVEPOSX 0  
#define DUEL_CHAR1_S2_N10_WAVEPOSY 0
#define DUEL_CHAR1_S2_N10_DAMSCOPE -1
#define DUEL_CHAR1_S2_N10_KEEPWAVE false
#define DUEL_CHAR1_S2_N10_MOVEX 0
#define DUEL_CHAR1_S2_N10_MOVEY 0
#define DUEL_CHAR1_S2_N10_MOVEDUR 1


#define DUEL_CHAR1_S1_TIMES 8
#define DUEL_CHAR1_S1_TOALDAMSCOPE 3
#define DUEL_CHAR1_S1_HITS 2



#define DUEL_CHAR1_S1_N1_OCCFRAME 1
#define DUEL_CHAR1_S1_N1_WAVEID -1
#define DUEL_CHAR1_S1_N1_WAVEPOSX 0  
#define DUEL_CHAR1_S1_N1_WAVEPOSY 0
#define DUEL_CHAR1_S1_N1_DAMSCOPE -1
#define DUEL_CHAR1_S1_N1_KEEPWAVE false
#define DUEL_CHAR1_S1_N1_MOVEX -30
#define DUEL_CHAR1_S1_N1_MOVEY 30
#define DUEL_CHAR1_S1_N1_MOVEDUR 1


#define DUEL_CHAR1_S1_N2_OCCFRAME 2
#define DUEL_CHAR1_S1_N2_WAVEID -1
#define DUEL_CHAR1_S1_N2_WAVEPOSX 0  
#define DUEL_CHAR1_S1_N2_WAVEPOSY 0
#define DUEL_CHAR1_S1_N2_DAMSCOPE -1
#define DUEL_CHAR1_S1_N2_KEEPWAVE false
#define DUEL_CHAR1_S1_N2_MOVEX -90
#define DUEL_CHAR1_S1_N2_MOVEY 70
#define DUEL_CHAR1_S1_N2_MOVEDUR 1

#define DUEL_CHAR1_S1_N3_OCCFRAME 3
#define DUEL_CHAR1_S1_N3_WAVEID -1
#define DUEL_CHAR1_S1_N3_WAVEPOSX 0  
#define DUEL_CHAR1_S1_N3_WAVEPOSY 0
#define DUEL_CHAR1_S1_N3_DAMSCOPE -1
#define DUEL_CHAR1_S1_N3_KEEPWAVE false
#define DUEL_CHAR1_S1_N3_MOVEX -160
#define DUEL_CHAR1_S1_N3_MOVEY 90
#define DUEL_CHAR1_S1_N3_MOVEDUR 1


#define DUEL_CHAR1_S1_N4_OCCFRAME 4
#define DUEL_CHAR1_S1_N4_WAVEID -1
#define DUEL_CHAR1_S1_N4_WAVEPOSX 0  
#define DUEL_CHAR1_S1_N4_WAVEPOSY 0
#define DUEL_CHAR1_S1_N4_DAMSCOPE -1
#define DUEL_CHAR1_S1_N4_KEEPWAVE false
#define DUEL_CHAR1_S1_N4_MOVEX -220
#define DUEL_CHAR1_S1_N4_MOVEY 45
#define DUEL_CHAR1_S1_N4_MOVEDUR 1


#define DUEL_CHAR1_S1_N5_OCCFRAME 5
#define DUEL_CHAR1_S1_N5_WAVEID -1
#define DUEL_CHAR1_S1_N5_WAVEPOSX 0 
#define DUEL_CHAR1_S1_N5_WAVEPOSY 0
#define DUEL_CHAR1_S1_N5_DAMSCOPE -1
#define DUEL_CHAR1_S1_N5_KEEPWAVE false
#define DUEL_CHAR1_S1_N5_MOVEX -250
#define DUEL_CHAR1_S1_N5_MOVEY 30
#define DUEL_CHAR1_S1_N5_MOVEDUR 1


#define DUEL_CHAR1_S1_N6_OCCFRAME 6
#define DUEL_CHAR1_S1_N6_WAVEID -1
#define DUEL_CHAR1_S1_N6_WAVEPOSX 0  
#define DUEL_CHAR1_S1_N6_WAVEPOSY 0
#define DUEL_CHAR1_S1_N6_DAMSCOPE -1
#define DUEL_CHAR1_S1_N6_KEEPWAVE false
#define DUEL_CHAR1_S1_N6_MOVEX -270
#define DUEL_CHAR1_S1_N6_MOVEY 0
#define DUEL_CHAR1_S1_N6_MOVEDUR 1


#define DUEL_CHAR1_S1_N7_OCCFRAME 7
#define DUEL_CHAR1_S1_N7_WAVEID -1//1
#define DUEL_CHAR1_S1_N7_WAVEPOSX -47  
#define DUEL_CHAR1_S1_N7_WAVEPOSY 45
#define DUEL_CHAR1_S1_N7_DAMSCOPE 1
#define DUEL_CHAR1_S1_N7_KEEPWAVE false
#define DUEL_CHAR1_S1_N7_MOVEX 0
#define DUEL_CHAR1_S1_N7_MOVEY 0
#define DUEL_CHAR1_S1_N7_MOVEDUR 0

#define DUEL_CHAR1_S1_N8_OCCFRAME 12
#define DUEL_CHAR1_S1_N8_WAVEID -1//2
#define DUEL_CHAR1_S1_N8_WAVEPOSX -47  
#define DUEL_CHAR1_S1_N8_WAVEPOSY 90
#define DUEL_CHAR1_S1_N8_DAMSCOPE 0
#define DUEL_CHAR1_S1_N8_KEEPWAVE false
#define DUEL_CHAR1_S1_N8_MOVEX 0
#define DUEL_CHAR1_S1_N8_MOVEY 0
#define DUEL_CHAR1_S1_N8_MOVEDUR 0


#define DUEL_CHAR2_S1_TIMES 1
#define DUEL_CHAR2_S1_TOALDAMSCOPE 1
#define DUEL_CHAR2_S1_HITS 1

#define DUEL_CHAR2_S1_N1_OCCFRAME 10
#define DUEL_CHAR2_S1_N1_WAVEID 9
#define DUEL_CHAR2_S1_N1_WAVEPOSX -47  
#define DUEL_CHAR2_S1_N1_WAVEPOSY 90
#define DUEL_CHAR2_S1_N1_DAMSCOPE -1
#define DUEL_CHAR2_S1_N1_KEEPWAVE true
#define DUEL_CHAR2_S1_N1_MOVEX 0
#define DUEL_CHAR2_S1_N1_MOVEY 0
#define DUEL_CHAR2_S1_N1_MOVEDUR 0





#define DUEL_CHAR3_S1_TIMES 1
#define DUEL_CHAR3_S1_TOALDAMSCOPE 2
#define DUEL_CHAR3_S1_HITS 1

#define DUEL_CHAR3_S1_N1_OCCFRAME 6
#define DUEL_CHAR3_S1_N1_WAVEID -1
#define DUEL_CHAR3_S1_N1_WAVEPOSX 0
#define DUEL_CHAR3_S1_N1_WAVEPOSY 0
#define DUEL_CHAR3_S1_N1_DAMSCOPE 1
#define DUEL_CHAR3_S1_N1_KEEPWAVE false
#define DUEL_CHAR3_S1_N1_MOVEX 0
#define DUEL_CHAR3_S1_N1_MOVEY 0
#define DUEL_CHAR3_S1_N1_MOVEDUR 0


#define DUEL_CHAR4_S1_TIMES 2
#define DUEL_CHAR4_S1_TOALDAMSCOPE 3
#define DUEL_CHAR4_S1_HITS 2

#define DUEL_CHAR4_S1_N1_OCCFRAME 0
#define DUEL_CHAR4_S1_N1_WAVEID -1
#define DUEL_CHAR4_S1_N1_WAVEPOSX 0
#define DUEL_CHAR4_S1_N1_WAVEPOSY 0
#define DUEL_CHAR4_S1_N1_DAMSCOPE 0
#define DUEL_CHAR4_S1_N1_KEEPWAVE false
#define DUEL_CHAR4_S1_N1_MOVEX 0
#define DUEL_CHAR4_S1_N1_MOVEY 0
#define DUEL_CHAR4_S1_N1_MOVEDUR 0

#define DUEL_CHAR4_S1_N2_OCCFRAME 10
#define DUEL_CHAR4_S1_N2_WAVEID -1
#define DUEL_CHAR4_S1_N2_WAVEPOSX 0
#define DUEL_CHAR4_S1_N2_WAVEPOSY 0
#define DUEL_CHAR4_S1_N2_DAMSCOPE 1
#define DUEL_CHAR4_S1_N2_KEEPWAVE false
#define DUEL_CHAR4_S1_N2_MOVEX 0
#define DUEL_CHAR4_S1_N2_MOVEY 0
#define DUEL_CHAR4_S1_N2_MOVEDUR 0

#define DUEL_CHAR5_S1_TIMES 1
#define DUEL_CHAR5_S1_TOALDAMSCOPE 2
#define DUEL_CHAR5_S1_HITS 1

#define DUEL_CHAR5_S1_N1_OCCFRAME 0
#define DUEL_CHAR5_S1_N1_WAVEID 13
#define DUEL_CHAR5_S1_N1_WAVEPOSX -50
#define DUEL_CHAR5_S1_N1_WAVEPOSY 0
#define DUEL_CHAR5_S1_N1_DAMSCOPE 1
#define DUEL_CHAR5_S1_N1_KEEPWAVE false
#define DUEL_CHAR5_S1_N1_MOVEX -110
#define DUEL_CHAR5_S1_N1_MOVEY 0
#define DUEL_CHAR5_S1_N1_MOVEDUR 0



#define DUEL_CHAR6_S1_TIMES 1
#define DUEL_CHAR6_S1_TOALDAMSCOPE 4
#define DUEL_CHAR6_S1_HITS 3

#define DUEL_CHAR6_S1_N1_OCCFRAME 5
#define DUEL_CHAR6_S1_N1_WAVEID 15
#define DUEL_CHAR6_S1_N1_WAVEPOSX -50
#define DUEL_CHAR6_S1_N1_WAVEPOSY 0
#define DUEL_CHAR6_S1_N1_DAMSCOPE 1
#define DUEL_CHAR6_S1_N1_KEEPWAVE false
#define DUEL_CHAR6_S1_N1_MOVEX 0
#define DUEL_CHAR6_S1_N1_MOVEY 0
#define DUEL_CHAR6_S1_N1_MOVEDUR 0


#define DUEL_CHAR6_S1_N2_OCCFRAME 11
#define DUEL_CHAR6_S1_N2_WAVEID -1
#define DUEL_CHAR6_S1_N2_WAVEPOSX 0
#define DUEL_CHAR6_S1_N2_WAVEPOSY 0
#define DUEL_CHAR6_S1_N2_DAMSCOPE 1
#define DUEL_CHAR6_S1_N2_KEEPWAVE false
#define DUEL_CHAR6_S1_N2_MOVEX 0
#define DUEL_CHAR6_S1_N2_MOVEY 0
#define DUEL_CHAR6_S1_N2_MOVEDUR 0


#define DUEL_CHAR6_S1_N3_OCCFRAME 17
#define DUEL_CHAR6_S1_N3_WAVEID -1
#define DUEL_CHAR6_S1_N3_WAVEPOSX 0
#define DUEL_CHAR6_S1_N3_WAVEPOSY 0
#define DUEL_CHAR6_S1_N3_DAMSCOPE 1
#define DUEL_CHAR6_S1_N3_KEEPWAVE false
#define DUEL_CHAR6_S1_N3_MOVEX 0
#define DUEL_CHAR6_S1_N3_MOVEY 0
#define DUEL_CHAR6_S1_N3_MOVEDUR 0


#define DUEL_CHAR7_S1_TIMES 1
#define DUEL_CHAR7_S1_TOALDAMSCOPE 2
#define DUEL_CHAR7_S1_HITS 1

#define DUEL_CHAR7_S1_N1_OCCFRAME 6
#define DUEL_CHAR7_S1_N1_WAVEID -1
#define DUEL_CHAR7_S1_N1_WAVEPOSX 0
#define DUEL_CHAR7_S1_N1_WAVEPOSY 0
#define DUEL_CHAR7_S1_N1_DAMSCOPE 1
#define DUEL_CHAR7_S1_N1_KEEPWAVE false
#define DUEL_CHAR7_S1_N1_MOVEX 0
#define DUEL_CHAR7_S1_N1_MOVEY 0
#define DUEL_CHAR7_S1_N1_MOVEDUR 0




#define DUEL_CHAR8_S1_TIMES 1
#define DUEL_CHAR8_S1_TOALDAMSCOPE 2
#define DUEL_CHAR8_S1_HITS 1

#define DUEL_CHAR8_S1_N1_OCCFRAME 14
#define DUEL_CHAR8_S1_N1_WAVEID -1
#define DUEL_CHAR8_S1_N1_WAVEPOSX 0
#define DUEL_CHAR8_S1_N1_WAVEPOSY 0
#define DUEL_CHAR8_S1_N1_DAMSCOPE 1
#define DUEL_CHAR8_S1_N1_KEEPWAVE false
#define DUEL_CHAR8_S1_N1_MOVEX 0
#define DUEL_CHAR8_S1_N1_MOVEY 0
#define DUEL_CHAR8_S1_N1_MOVEDUR 0


#define DUEL_CHAR9_S1_TIMES 1
#define DUEL_CHAR9_S1_TOALDAMSCOPE 2
#define DUEL_CHAR9_S1_HITS 1

#define DUEL_CHAR9_S1_N1_OCCFRAME 4
#define DUEL_CHAR9_S1_N1_WAVEID 18
#define DUEL_CHAR9_S1_N1_WAVEPOSX -50
#define DUEL_CHAR9_S1_N1_WAVEPOSY 0
#define DUEL_CHAR9_S1_N1_DAMSCOPE 1
#define DUEL_CHAR9_S1_N1_KEEPWAVE false
#define DUEL_CHAR9_S1_N1_MOVEX 0
#define DUEL_CHAR9_S1_N1_MOVEY 0
#define DUEL_CHAR9_S1_N1_MOVEDUR 0



#define DUEL_CHAR10_S1_TIMES 2
#define DUEL_CHAR10_S1_TOALDAMSCOPE 1
#define DUEL_CHAR10_S1_HITS 1

#define DUEL_CHAR10_S1_N1_OCCFRAME 0
#define DUEL_CHAR10_S1_N1_WAVEID 16
#define DUEL_CHAR10_S1_N1_WAVEPOSX 0
#define DUEL_CHAR10_S1_N1_WAVEPOSY 50
#define DUEL_CHAR10_S1_N1_DAMSCOPE -1
#define DUEL_CHAR10_S1_N1_KEEPWAVE false
#define DUEL_CHAR10_S1_N1_MOVEX 0
#define DUEL_CHAR10_S1_N1_MOVEY 0
#define DUEL_CHAR10_S1_N1_MOVEDUR 0

#define DUEL_CHAR10_S1_N2_OCCFRAME 8
#define DUEL_CHAR10_S1_N2_WAVEID 17
#define DUEL_CHAR10_S1_N2_WAVEPOSX 0
#define DUEL_CHAR10_S1_N2_WAVEPOSY 50
#define DUEL_CHAR10_S1_N2_DAMSCOPE -1
#define DUEL_CHAR10_S1_N2_KEEPWAVE true
#define DUEL_CHAR10_S1_N2_MOVEX 0
#define DUEL_CHAR10_S1_N2_MOVEY 0
#define DUEL_CHAR10_S1_N2_MOVEDUR 0


#define DUEL_CHAR11_S1_TIMES 1
#define DUEL_CHAR11_S1_TOALDAMSCOPE 2
#define DUEL_CHAR11_S1_HITS 1

#define DUEL_CHAR11_S1_N1_OCCFRAME 8
#define DUEL_CHAR11_S1_N1_WAVEID -1
#define DUEL_CHAR11_S1_N1_WAVEPOSX 0
#define DUEL_CHAR11_S1_N1_WAVEPOSY 0
#define DUEL_CHAR11_S1_N1_DAMSCOPE 1
#define DUEL_CHAR11_S1_N1_KEEPWAVE false
#define DUEL_CHAR11_S1_N1_MOVEX 0
#define DUEL_CHAR11_S1_N1_MOVEY 0
#define DUEL_CHAR11_S1_N1_MOVEDUR 0


#define DUEL_CHAR12_S1_TIMES 1
#define DUEL_CHAR12_S1_TOALDAMSCOPE 1
#define DUEL_CHAR12_S1_HITS 1

#define DUEL_CHAR12_S1_N1_OCCFRAME 9
#define DUEL_CHAR12_S1_N1_WAVEID 11
#define DUEL_CHAR12_S1_N1_WAVEPOSX 47  
#define DUEL_CHAR12_S1_N1_WAVEPOSY 90
#define DUEL_CHAR12_S1_N1_DAMSCOPE -1
#define DUEL_CHAR12_S1_N1_KEEPWAVE true
#define DUEL_CHAR12_S1_N1_MOVEX 0
#define DUEL_CHAR12_S1_N1_MOVEY 0
#define DUEL_CHAR12_S1_N1_MOVEDUR 0


#define DUEL_CHAR13_S1_TIMES 1
#define DUEL_CHAR13_S1_TOALDAMSCOPE 1
#define DUEL_CHAR13_S1_HITS 1

#define DUEL_CHAR13_S1_N1_OCCFRAME 7
#define DUEL_CHAR13_S1_N1_WAVEID -1
#define DUEL_CHAR13_S1_N1_WAVEPOSX 105
#define DUEL_CHAR13_S1_N1_WAVEPOSY 73
#define DUEL_CHAR13_S1_N1_DAMSCOPE 0
#define DUEL_CHAR13_S1_N1_KEEPWAVE false
#define DUEL_CHAR13_S1_N1_MOVEX 0
#define DUEL_CHAR13_S1_N1_MOVEY 0
#define DUEL_CHAR13_S1_N1_MOVEDUR 0




#define DUEL_CHAR14_S1_TIMES 1
#define DUEL_CHAR14_S1_TOALDAMSCOPE 2
#define DUEL_CHAR14_S1_HITS 1

#define DUEL_CHAR14_S1_N1_OCCFRAME 4
#define DUEL_CHAR14_S1_N1_WAVEID 8
#define DUEL_CHAR14_S1_N1_WAVEPOSX 105
#define DUEL_CHAR14_S1_N1_WAVEPOSY 73
#define DUEL_CHAR14_S1_N1_DAMSCOPE 1
#define DUEL_CHAR14_S1_N1_KEEPWAVE false
#define DUEL_CHAR14_S1_N1_MOVEX 0
#define DUEL_CHAR14_S1_N1_MOVEY 0
#define DUEL_CHAR14_S1_N1_MOVEDUR 0

#define DUEL_CHAR15_S1_TIMES 1
#define DUEL_CHAR15_S1_TOALDAMSCOPE 2
#define DUEL_CHAR15_S1_HITS 1

#define DUEL_CHAR15_S1_N1_OCCFRAME 5
#define DUEL_CHAR15_S1_N1_WAVEID 14
#define DUEL_CHAR15_S1_N1_WAVEPOSX 105
#define DUEL_CHAR15_S1_N1_WAVEPOSY 73
#define DUEL_CHAR15_S1_N1_DAMSCOPE 1
#define DUEL_CHAR15_S1_N1_KEEPWAVE false
#define DUEL_CHAR15_S1_N1_MOVEX 0
#define DUEL_CHAR15_S1_N1_MOVEY 0
#define DUEL_CHAR15_S1_N1_MOVEDUR 0


#define DUEL_CHAR16_S1_TIMES 2
#define DUEL_CHAR16_S1_TOALDAMSCOPE 3
#define DUEL_CHAR16_S1_HITS 2

#define DUEL_CHAR16_S1_N1_OCCFRAME 2
#define DUEL_CHAR16_S1_N1_WAVEID -1
#define DUEL_CHAR16_S1_N1_WAVEPOSX 0
#define DUEL_CHAR16_S1_N1_WAVEPOSY 0
#define DUEL_CHAR16_S1_N1_DAMSCOPE 0
#define DUEL_CHAR16_S1_N1_KEEPWAVE false
#define DUEL_CHAR16_S1_N1_MOVEX 0
#define DUEL_CHAR16_S1_N1_MOVEY 0
#define DUEL_CHAR16_S1_N1_MOVEDUR 0


#define DUEL_CHAR16_S1_N2_OCCFRAME 8
#define DUEL_CHAR16_S1_N2_WAVEID -1
#define DUEL_CHAR16_S1_N2_WAVEPOSX 0
#define DUEL_CHAR16_S1_N2_WAVEPOSY 0
#define DUEL_CHAR16_S1_N2_DAMSCOPE 1
#define DUEL_CHAR16_S1_N2_KEEPWAVE false
#define DUEL_CHAR16_S1_N2_MOVEX 0
#define DUEL_CHAR16_S1_N2_MOVEY 0
#define DUEL_CHAR16_S1_N2_MOVEDUR 0



#define DUEL_CHAR16_S2_TIMES 1
#define DUEL_CHAR16_S2_TOALDAMSCOPE 2
#define DUEL_CHAR16_S2_HITS 1

#define DUEL_CHAR16_S2_N1_OCCFRAME 37
#define DUEL_CHAR16_S2_N1_WAVEID -1
#define DUEL_CHAR16_S2_N1_WAVEPOSX 0	 
#define DUEL_CHAR16_S2_N1_WAVEPOSY 0
#define DUEL_CHAR16_S2_N1_DAMSCOPE 1 
#define DUEL_CHAR16_S2_N1_KEEPWAVE false
#define DUEL_CHAR16_S2_N1_MOVEX 0
#define DUEL_CHAR16_S2_N1_MOVEY 0
#define DUEL_CHAR16_S2_N1_MOVEDUR 0

#define DUEL_CHAR17_S1_TIMES 1
#define DUEL_CHAR17_S1_TOALDAMSCOPE 2
#define DUEL_CHAR17_S1_HITS 1

#define DUEL_CHAR17_S1_N1_OCCFRAME 5
#define DUEL_CHAR17_S1_N1_WAVEID -1
#define DUEL_CHAR17_S1_N1_WAVEPOSX 0
#define DUEL_CHAR17_S1_N1_WAVEPOSY 0
#define DUEL_CHAR17_S1_N1_DAMSCOPE 1
#define DUEL_CHAR17_S1_N1_KEEPWAVE false
#define DUEL_CHAR17_S1_N1_MOVEX 0
#define DUEL_CHAR17_S1_N1_MOVEY 0
#define DUEL_CHAR17_S1_N1_MOVEDUR 0


#define DUEL_CHAR18_S1_TIMES 1
#define DUEL_CHAR18_S1_TOALDAMSCOPE 2
#define DUEL_CHAR18_S1_HITS 1

#define DUEL_CHAR18_S1_N1_OCCFRAME 7
#define DUEL_CHAR18_S1_N1_WAVEID -1
#define DUEL_CHAR18_S1_N1_WAVEPOSX 0
#define DUEL_CHAR18_S1_N1_WAVEPOSY 0
#define DUEL_CHAR18_S1_N1_DAMSCOPE 1
#define DUEL_CHAR18_S1_N1_KEEPWAVE false
#define DUEL_CHAR18_S1_N1_MOVEX 0
#define DUEL_CHAR18_S1_N1_MOVEY 0
#define DUEL_CHAR18_S1_N1_MOVEDUR 0


#endif 
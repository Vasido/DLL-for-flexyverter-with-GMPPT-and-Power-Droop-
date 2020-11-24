#ifndef DEFINE_H
#define DEFINE_H

#define SIGN		(int)0x8000
#define SECTOR_0	2
#define SECTOR_1	0
#define SECTOR_2	4
#define SECTOR_3	5
#define SECTOR_4	13
#define SECTOR_5	15
#define SECTOR_6	11
#define SECTOR_7	10

#define MAX_16	(s32)0x00007fff
#define MIN_16	(s32)0xffff8000
#define MAX_32	(s32)0x7fffffff
#define MIN_32	(s32)0x80000000

#define HIGH_MODULATION_LIMIT	 1935
#define LOW_MODULATION_LIMIT	-1935

#define HIGH_CURRENT_LIMIT	(s16) 1580
#define LOW_CURRENT_LIMIT	(s16)-1580

#define f_dis 		15000		//Частота обработки
#define Ld 			5 >> 14//296e-6[H]		
#define Lq 			5 >> 15//147e-6[H]		
#define Rs 			26 >> 12//0.00634[Ohm]	
#define Vm			270
#define Umod_max	Vm / 2.0 * 1.5

// для моделирования:
#define Nmax 4500			 // период опорного сигнала

#endif
#include "Rectifier.h"

void RectifierSwitch(VARIABLE* src)
{
	
	src->rectifier.Control &= ~(ALL_SECTOR);
	// определяем полярности линейных напряжений
	src->rectifier.Control |= (u16)((unsigned int)(src->Uabc.A_B & SIGN) >> 16);
	src->rectifier.Control |= (u16)((unsigned int)(src->Uabc.B_C & SIGN) >> 17);
	src->rectifier.Control |= (u16)((unsigned int)(src->Uabc.C_A & SIGN) >> 18);
/*
	if(src->Uabc.A_B < 0)	src->rectifier.Control |= SECTOR_423;
	else					src->rectifier.Control &= ~SECTOR_423;

	if(src->Uabc.B_C < 0)	src->rectifier.Control |= SECTOR_456;
	else					src->rectifier.Control &= ~SECTOR_456;

	if(src->Uabc.C_A < 0)	src->rectifier.Control |= SECTOR_126;
	else					src->rectifier.Control &= ~SECTOR_126;
*/
	src->rectifier.Tgen++;

	switch(src->rectifier.Control & ALL_SECTOR)
	{
		case SECTOR_126: RectifierControl_S3(src); break;
		case SECTOR_456: RectifierControl_S1(src); break;
		case SECTOR_156: RectifierControl_S2(src); break;
		case SECTOR_423: RectifierControl_S5(src); break;
		case SECTOR_123: RectifierControl_S4(src); break;
		case SECTOR_453: RectifierControl_S6(src); break;
		default: break;
	}
	return;
}
//Sector 456 - 561
void RectifierControl_S1(VARIABLE* src)
{
	// если ранее был включен тиристор 4 => прямая последовательность
	if ((src->rectifier.Control & Gop_4) == Gop_4)
	{
		// разрешаем работу тиристора 6
		if ((src->rectifier.Control & Gop_6) != Gop_6)
		{
			// попали в ТЕК
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[1] = Max_Uop;
			src->rectifier.Control |= (Gop_4|Gop_5|Gop_6);
			src->rectifier.Control &= ~(Gop_1|Gop_2|Gop_3|TH_1_6|inverse_sequence);

			src->rectifier.k = 1.0 / src->rectifier.Tgen;
			src->rectifier.Tgen = 0;
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 1, -(src->Uabc.B_C), src->Uabc.C_A, TH_6, TH_5, TH_4);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	else
	{
		// иначе => обратная последовательность
		// разрешаем работу тиристора 5
		if ((src->rectifier.Control & Gop_5) != Gop_5)
		{
			// попали в ТЕК
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[1] = Max_Uop;
			src->rectifier.Control |= (Gop_5|Gop_6|Gop_1|inverse_sequence);
			src->rectifier.Control &= ~(Gop_2|Gop_3|Gop_4|TH_1_6);

			src->rectifier.k = 1.0 / src->rectifier.Tgen;
			src->rectifier.Tgen = 0;
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 1, -(src->Uabc.B_C), src->Uabc.A_B, TH_5, TH_6, TH_1);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	
	return;
}
//Sector 156 - 612
void RectifierControl_S2(VARIABLE* src)
{
	if ((src->rectifier.Control & Gop_5) == Gop_5)
	{
		if ((src->rectifier.Control & Gop_1) != Gop_1)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[0] = Max_Uop;
			src->rectifier.Control |= (Gop_1|Gop_5|Gop_6);
			src->rectifier.Control &= ~(Gop_2|Gop_3|Gop_4|TH_1_6|inverse_sequence);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 0, src->Uabc.A_B, -(src->Uabc.B_C), TH_1, TH_6, TH_5);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	else
	{
		if ((src->rectifier.Control & Gop_6) != Gop_6)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[2] = Max_Uop;
			src->rectifier.Control |= (Gop_1|Gop_2|Gop_6|inverse_sequence);
			src->rectifier.Control &= ~(Gop_3|Gop_4|Gop_5|TH_1_6);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 2, src->Uabc.A_B, -(src->Uabc.C_A), TH_6, TH_1, TH_2);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	return;
}
//Sector 126 - 123
void RectifierControl_S3(VARIABLE* src)
{
	if ((src->rectifier.Control & Gop_6) == Gop_6)
	{
		if ((src->rectifier.Control & Gop_2) != Gop_2)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[2] = Max_Uop;
			src->rectifier.Control |= (Gop_1|Gop_2|Gop_6);
			src->rectifier.Control &= ~(Gop_3|Gop_4|Gop_5|TH_1_6|inverse_sequence);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 2, -(src->Uabc.C_A), src->Uabc.A_B, TH_2, TH_1, TH_6);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	else
	{
		if ((src->rectifier.Control & Gop_1) != Gop_1)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[0] = Max_Uop;
			src->rectifier.Control |= (Gop_1|Gop_2|Gop_3|inverse_sequence);
			src->rectifier.Control &= ~(Gop_4|Gop_5|Gop_6|TH_1_6);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 0, -(src->Uabc.C_A), src->Uabc.B_C, TH_1, TH_2, TH_3);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	return;
}
// Sector 123 - 234
void RectifierControl_S4(VARIABLE* src)
{
	if ((src->rectifier.Control & Gop_1) == Gop_1)
	{
		if ((src->rectifier.Control & Gop_3) != Gop_3)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[1] = Max_Uop;
			src->rectifier.Control |= (Gop_1|Gop_2|Gop_3);
			src->rectifier.Control &= ~(Gop_4|Gop_5|Gop_6|TH_1_6|inverse_sequence);

			src->rectifier.k = 1.0 / src->rectifier.Tgen;
			src->rectifier.Tgen = 0;
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 1, src->Uabc.B_C, -(src->Uabc.C_A), TH_3, TH_2, TH_1);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_1) >> Gop_1_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	else
	{
		if ((src->rectifier.Control & Gop_2) != Gop_2)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[1] = Max_Uop;
			src->rectifier.Control |= (Gop_2|Gop_3|Gop_4|inverse_sequence);
			src->rectifier.Control &= ~(Gop_1|Gop_5|Gop_6|TH_1_6);

			src->rectifier.k = 1.0 / src->rectifier.Tgen;
			src->rectifier.Tgen = 0;
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 1, src->Uabc.B_C, -(src->Uabc.A_B), TH_2, TH_3, TH_4);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	return;
}
//Sector 423 - 345
void RectifierControl_S5(VARIABLE* src)
{
	if ((src->rectifier.Control & Gop_2) == Gop_2)
	{
		if ((src->rectifier.Control & Gop_4) != Gop_4)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[0] = Max_Uop;
			src->rectifier.Control |= (Gop_2|Gop_3|Gop_4);
			src->rectifier.Control &= ~(Gop_1|Gop_5|Gop_6|TH_1_6|inverse_sequence);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 0, -(src->Uabc.A_B), src->Uabc.B_C, TH_4, TH_3, TH_2);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_2) >> Gop_2_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	else
	{
		if ((src->rectifier.Control & Gop_3) != Gop_3)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[1] = 0;
			src->rectifier.Uop[2] = Max_Uop;
			src->rectifier.Control |= (Gop_4|Gop_5|Gop_3|inverse_sequence);
			src->rectifier.Control &= ~(Gop_1|Gop_2|Gop_6|TH_1_6);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 0, 2, -(src->Uabc.A_B), src->Uabc.C_A, TH_3, TH_4, TH_5);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	return;
}
//Sector 453 - 456
void RectifierControl_S6(VARIABLE* src)
{
	if ((src->rectifier.Control & Gop_3) == Gop_3)
	{
		if ((src->rectifier.Control & Gop_5) != Gop_5)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[2] = Max_Uop;
			src->rectifier.Control |= (Gop_4|Gop_5|Gop_3);
			src->rectifier.Control &= ~(Gop_1|Gop_2|Gop_6|TH_1_6|inverse_sequence);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 2, src->Uabc.C_A, -(src->Uabc.A_B), TH_5, TH_4, TH_3);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_3) >> Gop_3_OF)*src->rectifier.k;
	}
	else
	{
		if ((src->rectifier.Control & Gop_4) != Gop_4)
		{
			src->Control |= START_SECTOR;
			src->rectifier.Count[0] = 0;
			src->rectifier.Uop[0] = Max_Uop;
			src->rectifier.Control |= (Gop_4|Gop_5|Gop_6|inverse_sequence);
			src->rectifier.Control &= ~(Gop_1|Gop_2|Gop_3|TH_1_6);
		}
		if ((src->Control & START_RECTIFIER) == START_RECTIFIER)
		{
			Enable_Thyristor(src, 1, 0, src->Uabc.C_A, -(src->Uabc.B_C), TH_4, TH_5, TH_6);
		}
		src->rectifier.Uop[0] -= (u16)((src->rectifier.Control & Gop_4) >> Gop_4_OF)*src->rectifier.k;
		src->rectifier.Uop[1] -= (u16)((src->rectifier.Control & Gop_5) >> Gop_5_OF)*src->rectifier.k;
		src->rectifier.Uop[2] -= (u16)((src->rectifier.Control & Gop_6) >> Gop_6_OF)*src->rectifier.k;
	}
	return;
}

void Enable_Thyristor(VARIABLE* src, byte index, byte index_2, int Up_p_00, int Up_p_10, u16 Thyristor_0, u16 Thyristor_1, u16 Thyristor_2)
{
	// Thyristor_0 - включаемый в секторе тиристор (при альфа до 60 deg)
	// Thyristor_1 - предыдщий включённый тиристор (на него подаётся дублирующий импульс) (при альфа до 60 deg)
	// Thyristor_2 - 
	// для секторов S1, S3, S5:
		// Count[0]    - счётчик длительность импульса управления включаемого в секторе тиристора (при альфа до 60 deg)
		// Count[1]    - счётчик длительность импульса управления включаемого в секторе тиристора (при альфа от 60 deg до 120 deg)
	// для секторов S2, S4, S6:
		// Count[1]    - счётчик длительность импульса управления включаемого в секторе тиристора (при альфа до 60 deg)
		// Count[0]    - счётчик длительность импульса управления включаемого в секторе тиристора (при альфа от 60 deg до 120 deg)
	// т.о.:
	// Count[0] - чётчик длительность импульса управления тиристора катодной группы
	// Count[1] - чётчик длительность импульса управления тиристора анодной группы

	double Up_p_0 = (double)Up_p_00 * 0.00000011920928955078125;
	double Up_p_1 = (double)Up_p_10 * 0.00000011920928955078125;

	if(src->Ud < src->rectifier.Regul.pi_ref)
		src->Control |= ENABLE_PI_CONTROL;

///////////////////////////////////////при альфа до 60 deg///////////////////////////////////////////////////////////////////////////////////////////
	if (src->rectifier.Count[1 - index] >= MaxTimeEnabledThyristor)
	{
		// Если длительность импульса управления включаемого в секторе тиристора (при альфа до 60 deg) превышает уставку, то снимаем импульс
		// А также снимаем дублирующий импульс управления предыдущим тиристором
		src->rectifier.Control &= ~(Thyristor_0|Thyristor_1);
		return;
	}

	if ((src->rectifier.Control & Thyristor_0) == Thyristor_0)
	{
		// Если выставили импульс управления включаемым в секторе тиристором (при альфа до 60 deg), то инкремент счётчика
		src->rectifier.Count[1 - index]++;
		return;
	}

	if(src->rectifier.Um >= src->rectifier.Uop[index_2])
	{
		//src->Control |= ENABLE_PI_CONTROL;
		if((src->Ud <= Up_p_0) && (src->Ud < ud_max))
		{
			src->rectifier.Control |= (Thyristor_0|Thyristor_1);
			src->rectifier.Count[index] = 0;
			src->Control |= ENABLE_PI_CONTROL;
		}
		else 
		{
			//if(src->Ud > src->rectifier.Regul.pi_ref)
				src->Control &= ~(ENABLE_PI_CONTROL); //Заморозить ПИ-регулятор, т.к. размыкается контур
		}
		return;
	}
///////////////////////////////////////при альфа до 60 deg///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////при альфа от 60 deg до 120 deg//////////////////////////////////////////////////////////////////////////////////
	if (src->rectifier.Count[index] >= MaxTimeEnabledThyristor)
	{
		// Если длительность импульса управления включаемого в секторе тиристора (при альфа от 60 deg до 120 deg) превышает уставку, то снимаем импульс
		// А также снимаем дублирующий импульс управления предыдущим тиристором
		src->rectifier.Control &= ~(Thyristor_1|Thyristor_2);
	}

	if ((src->rectifier.Control & Thyristor_1) == Thyristor_1)
	{
		src->rectifier.Count[index]++;
		return;
	}

	if((src->rectifier.Um >= src->rectifier.Uop[(0x01 << index_2)&0x03]) && (src->rectifier.Count[index] == 0))
	{
		//src->Control |= ENABLE_PI_CONTROL;
		if((src->Ud <= Up_p_1) && (src->Ud < ud_max))
		{
			src->rectifier.Control |= (Thyristor_1|Thyristor_2);
			src->Control |= ENABLE_PI_CONTROL;
		}
		else
		{
			//if(src->Ud > src->rectifier.Regul.pi_ref)
				src->Control &= ~(ENABLE_PI_CONTROL); //Заморозить ПИ-регулятор, т.к. размыкается контур
		}
	}
///////////////////////////////////////при альфа от 60 deg до 120 deg//////////////////////////////////////////////////////////////////////////////////

	return;
}
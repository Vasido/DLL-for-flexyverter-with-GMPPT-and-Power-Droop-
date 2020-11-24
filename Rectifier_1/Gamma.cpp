
#include "All_Headers.h"
#include "Var_ref.h"

// ������� ������� ����� � �������:
void Gamma()
{
	//#define SDVIG 0
	s16 temp_s16;



	STR.Gamma.control = 0;
	STR.Gamma.control |= (u16)((u16)(STR.Gamma.alpha & SIGN) >> 12);							// ���������� ���������� �����
	STR.Gamma.control |= (u16)((u16)(STR.Gamma.betta & SIGN) >> 13);							// ���������� ���������� �����
	STR.Gamma.control |= (u16)((u16)((STR.Gamma.alpha - STR.Gamma.betta) & SIGN) >> 14);		// ���������� ����� � �����
	STR.Gamma.control |= (u16)((u16)((STR.Gamma.alpha - STR.Gamma.betta_minus) & SIGN) >> 15);	// ���������� ����� � -�����

	switch (STR.Gamma.control)
	{
		case SECTOR_0:
			STR.Gamma.gamma = gammaTable[STR.Gamma.alpha];
			//STR.temp1_double = 0;	/////�������/////
			break;
		case SECTOR_1:
			STR.Gamma.gamma = -gammaTable[STR.Gamma.betta] + 1*7200;
			//STR.temp1_double = 1;	/////�������/////
			break;
		case SECTOR_2:
			STR.Gamma.gamma = gammaTable[STR.Gamma.betta_minus] + 1*7200;
			//STR.temp1_double = 2;	/////�������/////
			break;
		case SECTOR_3:
			STR.Gamma.gamma = -gammaTable[STR.Gamma.alpha] + 2*7200;
			//STR.temp1_double = 3;	/////�������/////
			break;
		case SECTOR_4:
			STR.Gamma.gamma = gammaTable[-STR.Gamma.alpha] + 2*7200;
			//STR.temp1_double = 4;	/////�������/////
			break;
		case SECTOR_5:
			STR.Gamma.gamma = -gammaTable[-STR.Gamma.betta] + 3*7200;
			//STR.temp1_double = 5;	/////�������/////
			break;
		case SECTOR_6:
			STR.Gamma.gamma = gammaTable[STR.Gamma.betta] + 3*7200;
			//STR.temp1_double = 6;	/////�������/////
			break;
		case SECTOR_7:
			STR.Gamma.gamma = -gammaTable[-STR.Gamma.alpha] + 4*7200;
			//STR.temp1_double = 7;
			break;
		default:
			break;
	}

	STR.Gamma.gamma = STR.Gamma.gamma >> 4;
	//STR.Gamma.gamma = (s32)Filter((s16)(STR.Gamma.gamma), &STR.FILTER.FILTER[5]);


	//////////////SDVIG gamma////////////////////////////////////
	STR.Gamma.gamma = STR.Gamma.gamma;// +STR.SDVIG;
	if (STR.Gamma.gamma > 1799)
		STR.Gamma.gamma = STR.Gamma.gamma - 1800;
	/////////////////////////////////////////////////////////////

	if (STR.Gamma.gamma > 1799)
		STR.Gamma.gamma = 1800;
	if (STR.Gamma.gamma < 0)
		STR.Gamma.gamma = 0;

	// ������� ������� �� �����:
	temp_s16 = (s16)(STR.Gamma.gamma - STR.Gamma.gamma_old);	// ���� �����������
	//if ((temp_s16 < 1780.0) || (temp_s16 > 20.0))
	if (temp_s16 > (-100))
	{
		STR.Gamma.w = (temp_s16) << 2;
	}
	STR.Gamma.w = (s32)Filter((s32)(STR.Gamma.w), &STR.FILTER.FILTER[6]); // ���������� �������
	STR.Gamma.w = (STR.Gamma.w*49) >> 4;
	STR.Gamma.w2 = STR.Gamma.w;
	STR.Gamma.gamma_old = STR.Gamma.gamma;
}

// ������� ���������� ���������� �� sin � cos:
void SinCosPointer()
{
	s16 temp_s16;

	STR.Gamma.SIN_GAMMA.phA = sin_[STR.Gamma.gamma];

	temp_s16 = STR.Gamma.gamma + 1200;
	if (temp_s16 > 1799)
		temp_s16 = temp_s16 - 1800;
	STR.Gamma.SIN_GAMMA.phB = sin_[temp_s16];

	temp_s16 = STR.Gamma.gamma + 600;
	if (temp_s16 > 1799)
		temp_s16 = temp_s16 - 1800;
	STR.Gamma.SIN_GAMMA.phC = sin_[temp_s16];

	temp_s16 = STR.Gamma.gamma + 450;
	if (temp_s16 > 1799)
		temp_s16 = temp_s16 - 1800;
	STR.Gamma.COS_GAMMA.phA = sin_[temp_s16];

	temp_s16 = STR.Gamma.gamma + 1650;
	if (temp_s16 > 1799)
		temp_s16 = temp_s16 - 1800;
	STR.Gamma.COS_GAMMA.phB = sin_[temp_s16];

	temp_s16 = STR.Gamma.gamma + 1050;
	if (temp_s16 > 1799)
		temp_s16 = temp_s16 - 1800;
	STR.Gamma.COS_GAMMA.phC = sin_[temp_s16];
}
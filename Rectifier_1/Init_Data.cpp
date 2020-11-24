#include "All_Headers.h"
#include "Var_ref.h"

void Init_Data(STRUCT* STR, double *in, double *out, int counter)
{
	// уставки:
	/*
	STR->w_ref = (s16)in[0];
	STR->Id_ref = 0;

	STR->Gamma.gamma_old = 900;
	STR->Gamma.gamma = 900;
	STR->Gamma.SIN_GAMMA.phA = sin_[STR->Gamma.gamma + 51];
	STR->Gamma.SIN_GAMMA.phB = sin_[STR->Gamma.gamma + 187];
	STR->Gamma.SIN_GAMMA.phC = sin_[STR->Gamma.gamma + 119];
	STR->Gamma.COS_GAMMA.phA = sin_[STR->Gamma.gamma];
	STR->Gamma.COS_GAMMA.phA = sin_[STR->Gamma.gamma + 136];
	STR->Gamma.COS_GAMMA.phA = sin_[STR->Gamma.gamma + 68];

	STR->Interrupt_Number = 0;
	
	STR->Fabc.phA = STR->Fabc.phB = STR->Fabc.phC = 0;
	STR->Fdq.d = 0;
	STR->Fdq.q = 10000;
	
	STR->Ii_abc.phA = STR->Ii_abc.phB = STR->Ii_abc.phC = 0;
	STR->Ii_dq.d = STR->Ii_dq.q = 0;

	// Задаём нули:
	STR->OFFSET.Ii_abc.phA = STR->OFFSET.Ii_abc.phB = STR->OFFSET.Ii_abc.phC = 1861;
	STR->OFFSET.alpha = STR->OFFSET.betta = 2047;//1861;

	// Ii_abc:
	STR->FILTER.FILTER[0].OldValue = STR->FILTER.FILTER[1].OldValue = STR->FILTER.FILTER[2].OldValue = 0;
	STR->FILTER.FILTER[0].IntegralPortionK_1 = STR->FILTER.FILTER[1].IntegralPortionK_1 = STR->FILTER.FILTER[2].IntegralPortionK_1 = 0;
	STR->FILTER.FILTER[0].IntegralGain = STR->FILTER.FILTER[1].IntegralGain = STR->FILTER.FILTER[2].IntegralGain = 27451;		 //Частота среза 2000 Гц
	//частота среза считается вот так: 
	//f_среза = IntegralGain * PWM_FREQUENCY / ( DIVIDER * 2 * 3,1415 )
	//где PWM_FREQUENCY = 15k, DIVIDER = 32768

	// alpha, betta:
	STR->FILTER.FILTER[3].OldValue = STR->FILTER.FILTER[4].OldValue = 0;
	STR->FILTER.FILTER[3].IntegralPortionK_1 = STR->FILTER.FILTER[4].IntegralPortionK_1 = 0;
	STR->FILTER.FILTER[3].IntegralGain = STR->FILTER.FILTER[4].IntegralGain = 21961;		 //Частота среза 800 Гц

	// gamma:
	STR->FILTER.FILTER[5].OldValue = 0;
	STR->FILTER.FILTER[5].IntegralPortionK_1 = 0;
	STR->FILTER.FILTER[5].IntegralGain = 13725;		 //Частота среза 500 Гц
	
	// w:
	STR->FILTER.FILTER[6].OldValue = 0;
	STR->FILTER.FILTER[6].IntegralPortionK_1 = 0;
	STR->FILTER.FILTER[6].IntegralGain = 100;		 //Частота среза 20 Гц

	// параметры внешнего контура:
	STR->PI_w.Integral_Gain = (s32)in[3];
	STR->PI_w.Integral_Gain_Scale = (s32)in[4];
	STR->PI_w.Integral_H_Limit =  (s32)(MAX_32);//4000000;
	STR->PI_w.Integral_L_Limit = (s32)(MIN_32);//-4000000;
	STR->PI_w.Integral_Portion_Z = 0;
	STR->PI_w.Proportional_Gain = (s32)in[5];
	STR->PI_w.Proportional_Gain_Scale = (s32)in[6];
	STR->PI_w.Proportional_H_Limit = (s32)MAX_16;
	STR->PI_w.Proportional_L_Limit = (s32)MIN_16;
	STR->PI_w.Output_H_Limit =  700;
	STR->PI_w.Output_L_Limit = -700;

	// параметры внутреннего (токового) контура:
	STR->PI_Id.Integral_Gain = STR->PI_Iq.Integral_Gain = (s32)in[7];
	STR->PI_Id.Integral_Gain_Scale = STR->PI_Iq.Integral_Gain_Scale = (s32)in[8];
	STR->PI_Id.Integral_H_Limit = STR->PI_Iq.Integral_H_Limit = (s32)(MAX_32);// 4000000;//(s32)(MAX_32/10);
	STR->PI_Id.Integral_L_Limit = STR->PI_Iq.Integral_L_Limit = (s32)(MIN_32);//-4000000;//(s32)(MIN_32/10);
	STR->PI_Id.Integral_Portion_Z = STR->PI_Iq.Integral_Portion_Z = 0;
	STR->PI_Id.Proportional_Gain = STR->PI_Iq.Proportional_Gain = (s32)in[9];
	STR->PI_Id.Proportional_Gain_Scale = STR->PI_Iq.Proportional_Gain_Scale = (s32)in[10];
	STR->PI_Id.Proportional_H_Limit = STR->PI_Iq.Proportional_H_Limit = (s32)MAX_16;
	STR->PI_Id.Proportional_L_Limit = STR->PI_Iq.Proportional_L_Limit = (s32)MIN_16;
	STR->PI_Id.Output_H_Limit = (s32)MAX_32;
	STR->PI_Id.Output_L_Limit = (s32)MIN_32;
	STR->PI_Iq.Output_H_Limit = (s32)MAX_32;
	STR->PI_Iq.Output_L_Limit = (s32)MIN_32;

	STR->PI_d.Integral_Gain = (s32)in[11];
	STR->PI_d.Integral_Gain_Scale = (s32)in[12];
	STR->PI_d.Integral_H_Limit = (s32)(MAX_32);// 4000000;
	STR->PI_d.Integral_L_Limit = (s32)(MIN_32);//-4000000;
	STR->PI_d.Integral_Portion_Z = 0;
	STR->PI_d.Proportional_Gain = (s32)in[13];
	STR->PI_d.Proportional_Gain_Scale = (s32)in[14];
	STR->PI_d.Proportional_H_Limit = (s32)MAX_16;
	STR->PI_d.Proportional_L_Limit = (s32)MIN_16;
	STR->PI_d.Output_H_Limit = (s32)MAX_32;
	STR->PI_d.Output_L_Limit = (s32)MIN_32;

	STR->PI_q.Integral_Gain = (s32)in[15];
	STR->PI_q.Integral_Gain_Scale = (s32)in[16];
	STR->PI_q.Integral_H_Limit = (s32)(MAX_32);// 4000000;
	STR->PI_q.Integral_L_Limit = (s32)(MIN_32);//-4000000;
	STR->PI_q.Integral_Portion_Z = 0;
	STR->PI_q.Proportional_Gain = (s32)in[17];
	STR->PI_q.Proportional_Gain_Scale = (s32)in[18];
	STR->PI_q.Proportional_H_Limit = (s32)MAX_16;
	STR->PI_q.Proportional_L_Limit = (s32)MIN_16;
	STR->PI_q.Output_H_Limit = (s32)MAX_32;
	STR->PI_q.Output_L_Limit = (s32)MIN_32;

	STR->PI_w.Integral_Portion_Z  =  0;
	STR->PI_Id.Integral_Portion_Z =  0;
	STR->PI_Iq.Integral_Portion_Z =  0;
	//STR->PI_d.Integral_Portion_Z  =  -20000*0;// 2^30;////////////////////
	//STR->PI_q.Integral_Portion_Z  =  0*2^31;////////////////////

	STR->PI_w.Output  = 0;////////////////////
	STR->PI_Id.Output = 0;////////////////////
	STR->PI_Iq.Output = 0;////////////////////
	//STR->PI_d.Output  = -100*0;////////////////////
	//STR->PI_q.Output  = 22000*0;////////////////////
	STR->temp1_double = 0.0;////////////////
	*/

//	STR->Ld = 1;
//	STR->Lq = 1;
}
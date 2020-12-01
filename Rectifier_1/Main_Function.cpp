// Ļšīćšąģģą óļšąāėåķč˙ čķāåšņīšīģ ńņąšņøšą
// 17.04.15
#include "All_Headers.h"
#include "Var_def.h"

unsigned int T_d=50;//T_d=T_hrtm*t_d*f_s  where t_d=10 ns, f_s=95 kHz
float t_d=0.1;  // dead time, us

float Da = 0; //0.3;
float delta_Da = t_d * f_sw_M;
float Max_Da = 0.5 - delta_Da;
float shifted_Da = 0;
float Min_Da = 0.05;

float Da_temp_1 = 0; // 
float Da_temp_2 = 0; //
float Da_sqrt = 0; //
float Da_atan= 0; //


float V_in = 0;
float I_in = 0;
float V_out = 0;
float I_out = 0;


 float V_in_f=0;
 float I_in_f=0;
 float V_out_f=0;
 float I_out_f=0;

 float V_in_old = 0;

 //GMPPT
 float P_in=0;
 float P_out= 0;
 float P_out_old = 0;
 float GMPPs_P_out[10] = { 0 };
 float GMPPs_V_in[10] = { 0 };
 u16 GMPP_i = 0;
 ///
 ///LMPPT
 float V_in_ref_LPPT = 0;
 ////

 float P_lim = 0;

 float V_in_ref=0;

 float global_temp=0;

 int charging_duty_cycle_step=0;
 float charging_Da_buck = 0;

 float SQRT = 0;

 stPI_Params V_in_reg[8] =
 {//	Proportional_Gain,	Integral_Gain,	Integral_Portion_Z,	Integral_H_Limit,	Integral_L_Limit,	Output_H_Limit,		Output_L_Limit	flag_transition
	 {	4.471e-3,			7.305e-4,		0,					Max_Da,				0,					Max_Da,				0,				0},		//APWM_HBI_FBR
	 {	4.471e-3,			7.305e-4,		0,					Max_Da,				0,					Max_Da,				0,				0},		//BFBR_HBI_FBR
	 {	0,					0,				0,					Max_Da,				0,					Max_Da,				0,				0},		//BFBR_HBI_PSM_FBI
	 {	6.896e-3,			1.144e-3,		0,					Max_Da,				0,					Max_Da,				0,				0},		//PSM_FBI_FBR
	 {	2.057e-3,			5.289e-4,		0,					Max_Da,				0,					Max_Da,				0,				0},		//BFBR_FBI_FBR
	 {	0,					0,				0,					Max_Da,				0,					Max_Da,				0,				0},		//BFBR_FBR_PSM_VDR
	 {	5.893e-3,			1.911e-3,		0,					Max_Da,				0,					Max_Da,				0,				0},		//PSM_FBI_VDR
	 {	2.098e-3,			1.043e-3,		0,					Max_Da,				0,					Max_Da,				0,				0},		//SwBVDR_FBI_VDR
 };


 stPI_Params P_out_reg =
 {//	Proportional_Gain,	Integral_Gain,	Integral_Portion_Z,	Integral_H_Limit,	Integral_L_Limit,	Output_H_Limit,		Output_L_Limit	flag_transition
  		2e-3,				1.254e-4,		0,					Max_V_in,			Min_V_in,			Max_V_in,			Min_V_in,				0
 };


 u16 counter_Da_steps = 0;
 u16 counter_2=0;

u32 ADC1ConvertedValues[2];
u32 ADC2ConvertedValues[3];

u32 I_in_offset = 595;
u32 I_out_offset = 2070;

uint32_t TIM2_CNT = 0;
uint32_t HRTIM_CNT = 0;

HRTIM_TIMC HRTIM1_TIMC;
HRTIM_TIMD HRTIM1_TIMD;

HRTIM_TIMA HRTIM1_TIMA;
HRTIM_TIMB HRTIM1_TIMB;

HRTIM_Master HRTIM1_Master;
STRUCT STR;

/////////////////////////////



et_converter_mode eConverterMode = eAPWM_HBI_FBR;
states machine_state = Standby;
states prev_machine_state = Standby; //previously 
status machine_status = READY;
transition_callback transition_handler;

void __declspec(dllexport) simuser(double t, double delt, double* in, double* out)
{

	TIM2_CNT = (u32)in[0];	// Ņåźółåå ēķą÷åķčå Ņąéģåšą 1
	if (INIT < 1)
	{
		STR.V_out_fiter.IntegralGain = 0.0628; //f_filter 1 kHz f_interapt 100 kHz
		STR.V_out_fiter.IntegralPortionK_1 = 350;
		STR.V_out_fiter.OldValue = 350;

		STR.V_in_fiter.IntegralGain = 0.6283;//f_filter 10 kHz f_interapt 100 kHz
		STR.V_in_fiter.IntegralPortionK_1 = 0;
		STR.V_in_fiter.OldValue = 0;

		STR.I_in_fiter.IntegralGain = 0.0628; //f_filter 1 kHz f_interapt 100 kHz
		STR.I_in_fiter.IntegralPortionK_1 = 0;
		STR.I_in_fiter.OldValue = 0;

		STR.I_out_fiter.IntegralGain = 0.0628; //f_filter 1 kHz f_interapt 100 kHz
		STR.I_out_fiter.IntegralPortionK_1 = 0;
		STR.I_out_fiter.OldValue = 0;

		INIT++;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//--ōīšģčšīāąķčå äčńźšåņķīńņč: --------------------------------
	if ((TIM2_CNT > 50) && (flag_INT == 1))
	{
		flag_INT = 0; //ńįšīń ōėąćą ļšåšūāąķč˙
		TIM2_IRQHandler(in);	// Īįšąįīņźą ļšåšūāąķč˙ īņ Ņąéģåšą
	}
	if (TIM2_CNT < 50)
		flag_INT = 1;  //óńņąķīāźą ōėąćą ļšåšūāąķč˙
	//////////////////////////////////////////////////////////////////////////////////////////

	if (machine_status != FAULT)
	{
		if (I_in > 13)
		{
			machine_state = Stop_Reset;
			machine_status = FAULT;
		}
		else
		{
			//Input voltage protection	
			if ((V_in_f > Min_V_in_fault) && (V_in_f < Max_V_in))
			{

				//Power droop control
				if ((V_out_f > Min_V_out) && (V_out_f < Min_Normal_V_out))
				{
					machine_status = RUN;
					P_lim = (V_out_f - Min_V_out) * Raising_power_K;
				}
				else if ((V_out_f >= Min_V_out) && ((V_out_f <= Normal_V_out)))
				{
					machine_status = RUN;
					P_lim = Max_converter_power;
				}

				else if ((V_out_f > Normal_V_out) && ((V_out_f < Max_V_out)))
				{
					machine_status = RUN;
					P_lim = Max_converter_power - (V_out_f - Normal_V_out) * Falling_power_K;
				}

				//Output voltage protection	
				else
				{
					if (machine_status == RUN)
					{
						machine_state = Stop_Reset;
						machine_status = FAULT_V_out;
					}
				}

				if (((machine_state == GMPPT) || (machine_state == LMPPT)) && (P_out > P_lim))
				{
					V_in_ref += V_ref_step;
					GMPP_i = 0;
					P_out_reg.Integral_Portion_Z = V_in_ref;
					machine_state = Power_droop;

				}


			}
			else
			{
				if (machine_status == RUN)
				{
					machine_state = Stop_Reset;
					machine_status = FAULT_V_in;
				}
			}

		}
	}






	
	




	if ((in[8] > 30)&&(HRTIM_CNT==1))
	{
		HRTIM_CNT = 0;
		// Ōóķźöč˙ čģģčņąöčč šąįīņū ŲČĢ-ģīäóė˙
		out[0] = HRTIM1_TIMC.CMP1xR;
		out[1] = HRTIM1_TIMC.CMP2xR;

		//out[2] = HRTIM1_Master.MCMP1R;
		//out[3] = HRTIM1_Master.MCMP2R;

		out[2] = HRTIM1_TIMC.CMP3xR;
		out[3] = HRTIM1_TIMC.CMP4xR;

		out[4] = HRTIM1_TIMD.CMP1xR;
		out[5] = HRTIM1_TIMD.CMP2xR;

		out[6] = HRTIM1_TIMD.CMP3xR;
		out[7] = HRTIM1_TIMD.CMP4xR;

		out[8] = HRTIM1_TIMA.CMP1xR;
		out[9] = HRTIM1_TIMA.CMP2xR;

		out[10] = HRTIM1_TIMA.CMP3xR;
		out[11] = HRTIM1_TIMA.CMP4xR;

		out[12] = HRTIM1_TIMB.CMP1xR;
		out[13] = HRTIM1_TIMB.CMP2xR;

		out[14] = HRTIM1_TIMB.CMP3xR;
		out[15] = HRTIM1_TIMB.CMP4xR;
	}
	else if (in[8] < 30)
		HRTIM_CNT = 1;







	//out[15] = flag_INT;
	out[16] = machine_state;
	out[17] = machine_status;
	out[18] = V_in_ref;
	out[19] = P_out_old;
	out[20] = P_out;
	out[21] = P_lim;
	out[22] = Da;
	if (Da_sqrt < 0)
		Da_sqrt = 0; 
	out[23] = Da_sqrt;
	out[24] = Da_atan;
	
	/*
	out[9] = (double)STR.PI_Iq.Output;
	out[10] = (double)STR.w_ref;
	out[11] = (double)STR.Gamma.w;
	out[12] = (double)STR.Fdq.d;
	out[13] = (double)STR.Fdq.q;
	out[14] = (double)STR.Fabc.phA;
	out[15] = (double)STR.Ii_abc.phA;
	out[16] = (double)STR.Ii_dq.d;
	out[17] = (double)STR.Ii_dq.q;
	out[18] = (double)STR.temp1_double;
	out[19] = (double)STR.temp2_double;
	out[20] = (double)STR.SDVIG;
	out[21] = (double)STR.Gamma.SIN_GAMMA.phC;
	out[22] = (double)STR.  PI_w.Integral_Portion_Z;
	out[23] = (double)STR.PI_Id.Integral_Portion_Z;
	out[24] = (double)STR.PI_Iq.Integral_Portion_Z;
	*/
}


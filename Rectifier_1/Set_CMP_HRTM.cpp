
#include "All_Headers.h"
#include "Var_def.h"


void set_cmp_hrtm(double* in)
{
	static u16 charging_step_counter= 0;
	static uint32_t charging_Duty_cycle = 0;
	static float charging_Da_buck = 0.5;
	static float charging_Da_step_buck = 0;
	static float charging_Da_boost = 0;
	static float charging_Da_step_boost = 0;
	static int charging_duty_cycle_step=0;

	delta_Da = t_d * f_sw_M;
	Max_Da = 0.5 - delta_Da;
	T_d = (u32)(f_HRCK_M * t_d);
	Min_Da = K_Min_Da * t_d;
	/*if ((eConverterMode == eBFBR_HBI_FBR)|| (eConverterMode == eBFBR_FBI_FBR))
	{
			
			 if (Da > Max_Da)
				Da = Max_Da;
	*/
	//if (Da < Min_Da)
	//	Da = Min_Da;

//	if (shifted_Da > (Max_Da - Da))
//		shifted_Da = Max_Da - Da;
//	if (shifted_Da < 0.01)
//		shifted_Da = 0.01;
	switch (eConverterMode)
	{

	case eAPWM_HBI_FBR: //APWM  HBI-FBR
	
	// Input bridge
	//Switch S1
		HRTIM1_TIMC.CMP1xR = 0x0;
		HRTIM1_TIMC.CMP2xR = (u32)(Da * T_hrtm);
		//Switch S2
		HRTIM1_TIMC.CMP3xR = (u32)(Da * T_hrtm+ T_d);
		HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);
		//Switch S3
		HRTIM1_TIMD.CMP1xR = 0x0;
		HRTIM1_TIMD.CMP2xR = 0x0;
		//Switch S4
		HRTIM1_TIMD.CMP3xR = 0x0;
		HRTIM1_TIMD.CMP4xR = T_hrtm+ MinCmpVal;
		
		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = 0x0;
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = 0x0;
		HRTIM1_TIMB.CMP2xR = 0x0;
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = T_hrtm + MinCmpVal;
		HRTIM1_TIMB.CMP4xR = 0x0;
		
		
		if ((Da > 0.44))
		{
			eConverterMode = eBFBR_HBI_FBR;
			U_in_reg[eConverterMode].flag_transition = 0;
			U_in_reg[eConverterMode].Integral_Portion_Z = 0;

		}

		break;
		
	case eBFBR_HBI_FBR: //BFBR HBI-FBR
	
	// Input bridge
	//Switch S1
		HRTIM1_TIMC.CMP1xR = 0x0;
		HRTIM1_TIMC.CMP2xR = half_Thrtm- T_d;
		//Switch S2
		HRTIM1_TIMC.CMP3xR = half_Thrtm;
		HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;
		//Switch S3
		HRTIM1_TIMD.CMP1xR = 0x0;
		HRTIM1_TIMD.CMP2xR = 0x0;
		//Switch S4
		HRTIM1_TIMD.CMP3xR = 0x0;
		HRTIM1_TIMD.CMP4xR = T_hrtm+ MinCmpVal;
		
		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = 0x0;
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = (u32)(Da * T_hrtm);
		HRTIM1_TIMB.CMP2xR = (u32)(Da * T_hrtm+ half_Thrtm - T_d);
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
		HRTIM1_TIMB.CMP4xR = (u32)(Da * T_hrtm - T_d);
		if (HRTIM1_TIMB.CMP4xR > T_hrtm)
			HRTIM1_TIMB.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
		
		if (U_in_reg[eConverterMode].flag_transition == 1)
		{
			eConverterMode = eAPWM_HBI_FBR;
			U_in_reg[eConverterMode].Integral_Portion_Z = 0.435;
		}
		
		
		else if (U_in_ref < 0.12* U_out_f)
		{
			if (counter_Da_steps==0)
			{
				Da_temp_1 = U_in_ref * n * (2*Cr_fs * (U_in_ref * n - U_out_f) + I_out_f*0.5);
				Da_temp_2 = Da_temp_1 / (Da_temp_1 - P_out);
				Da_sqrt = Da_temp_2* Da_temp_2-1;
				counter_Da_steps++;
			}
			else if (counter_Da_steps==1)
			{
				Da_atan = in[6];
				counter_Da_steps++;
				
			}
			else
			{
			
					U_in_reg[ePSM_FBI_FBR].Integral_Portion_Z = wr_Ts * in[7];
					charging_Da_buck = 0.5;
					charging_Da_boost = Da;
					charging_Duty_cycle = 0;
					charging_Da_step_buck = (0.5 - (U_in_reg[ePSM_FBI_FBR].Integral_Portion_Z)) / Charging_N_C2;
					charging_Da_step_boost = charging_Da_boost / Charging_N_C2;
					charging_step_counter = 0;
					
					charging_duty_cycle_step = un_charging_duty_cycle_step_C2;
					counter_Da_steps = 0;
					eConverterMode = eBFBR_HBI_PSM_FBI;

			}
				
		}
		break;
	
	case eBFBR_HBI_PSM_FBI:

		//Charging of capacitor C2 
		if (charging_step_counter != Charging_N_C2)
		{
			charging_step_counter++;
			charging_Duty_cycle += charging_duty_cycle_step;

			charging_Da_buck -= charging_Da_step_buck;


			charging_Da_boost -= charging_Da_step_boost;
			if (charging_Da_boost < 0)
				charging_Da_boost = 0;
			//charging_Da_boost -= charging_Da_step_boost;
			//Switch S3
			HRTIM1_TIMD.CMP1xR = (u32)(charging_Da_buck * T_hrtm);
			HRTIM1_TIMD.CMP2xR = (u32)(charging_Da_buck * T_hrtm + charging_Duty_cycle - T_d);
			//Switch S4
			HRTIM1_TIMD.CMP3xR = (u32)(charging_Da_buck * T_hrtm + charging_Duty_cycle);
			HRTIM1_TIMD.CMP4xR = (u32)(charging_Da_buck * T_hrtm - T_d);
			if(HRTIM1_TIMD.CMP4xR> T_hrtm)
			HRTIM1_TIMD.CMP4xR = T_hrtm + MinCmpVal;

			U_in_reg[eConverterMode].Integral_Portion_Z = wr_Ts * in[7];


			//Switch Q3
			HRTIM1_TIMB.CMP1xR = HRTIM1_TIMC.CMP3xR;
			HRTIM1_TIMB.CMP2xR = (u32)(charging_Da_boost * T_hrtm + half_Thrtm - T_d);
			//Switch Q4
			HRTIM1_TIMB.CMP3xR = HRTIM1_TIMC.CMP1xR;
			HRTIM1_TIMB.CMP4xR = (u32)(charging_Da_boost * T_hrtm - T_d);
			if (HRTIM1_TIMB.CMP4xR > T_hrtm)
				HRTIM1_TIMB.CMP4xR = 0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
		}
		else if (U_in_reg[ePSM_FBI_FBR].flag_transition == 1)
			eConverterMode = eBFBR_HBI_FBR;

		else
		{
			eConverterMode = ePSM_FBI_FBR;
			U_in_reg[ePSM_FBI_FBR].flag_transition == 0;
		}
			
		break;



	case ePSM_FBI_FBR: //PSM FBI-FBR

		
		// Input bridge
		//Switch S1
		HRTIM1_TIMC.CMP1xR = 0x0;
		HRTIM1_TIMC.CMP2xR = (u32)(half_Thrtm - T_d);
		//Switch S2
		HRTIM1_TIMC.CMP3xR = half_Thrtm;
		HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);
		//Switch S3
		HRTIM1_TIMD.CMP1xR = (u32)(Da * T_hrtm);
		HRTIM1_TIMD.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
		//Switch S4
		HRTIM1_TIMD.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
		HRTIM1_TIMD.CMP4xR = (u32)(Da * T_hrtm - T_d);
		if (HRTIM1_TIMD.CMP4xR > T_hrtm)
			HRTIM1_TIMD.CMP4xR = 0x0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	
		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = 0x0;
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = 0x0;
		HRTIM1_TIMB.CMP2xR = 0x0;
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = 0;//T_hrtm + MinCmpVal;
		HRTIM1_TIMB.CMP4xR = 0x0;


		if (U_in_ref > 0.123 * U_out_f)
		{
			if (counter_Da_steps == 0)
			{
				Da_temp_1 = U_in_f * n * Cr_fs * U_in_f * n  + P_out;
				Da_temp_2 = Da_temp_1 / (Da_temp_1 - 2*P_out+ U_in_f * n* I_out_f);
				Da_sqrt = Da_temp_2 * Da_temp_2 - 1;
				counter_Da_steps++;
			}
			else if (counter_Da_steps == 1)
			{
				Da_atan = in[6];
				counter_Da_steps++;
			}
			else
			{
				U_in_reg[ePSM_FBI_FBR].flag_transition = 1;
				charging_Da_buck = Da;
				eConverterMode = eBFBR_HBI_PSM_FBI;
				U_in_reg[eBFBR_HBI_FBR].Integral_Portion_Z = wr_Ts * in[7];
				counter_Da_steps = 0;

				
				charging_Da_step_buck = -(0.5 - Da) / Charging_N_C2;
				charging_Duty_cycle = half_Thrtm;

				charging_duty_cycle_step = -un_charging_duty_cycle_step_C2;
						
				charging_Da_boost = 0;
				charging_Da_step_boost = -(U_in_reg[eBFBR_HBI_FBR].Integral_Portion_Z) / Charging_N_C2;
				charging_step_counter = 0;

			}
		}
		else if (Da > 0.44)
		{

			eConverterMode = eBFBR_FBI_FBR;
			U_in_reg[eConverterMode].flag_transition = 0;			
			U_in_reg[eConverterMode].Integral_Portion_Z = 0;
		
		}
		break;

	case eBFBR_FBI_FBR: //BFBR HBI-FBR

// Input bridge
//Switch S1
	//Switch S1
		HRTIM1_TIMC.CMP1xR = T_hrtm;
		HRTIM1_TIMC.CMP2xR = (u32)(half_Thrtm - T_d);
		//Switch S2
		HRTIM1_TIMC.CMP3xR = half_Thrtm;
		HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);
		//Switch S3
		HRTIM1_TIMD.CMP1xR = half_Thrtm;
		HRTIM1_TIMD.CMP2xR = (u32)(T_hrtm - T_d);
		//Switch S4
		HRTIM1_TIMD.CMP3xR = T_hrtm;
		HRTIM1_TIMD.CMP4xR = (u32)(half_Thrtm - T_d);

		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = 0x0;
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = (u32)(Da * T_hrtm);
		HRTIM1_TIMB.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
		HRTIM1_TIMB.CMP4xR = (u32)(Da * T_hrtm - T_d);
		if (HRTIM1_TIMB.CMP4xR> T_hrtm)
			HRTIM1_TIMB.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
		


		if (U_in_reg[eConverterMode].flag_transition == 1)
		{
			eConverterMode = ePSM_FBI_FBR;
			U_in_reg[eConverterMode].Integral_Portion_Z = 0.435;
		}


		else if (U_in_ref < 0.057 * U_out_f)
		{
			if (counter_Da_steps == 0)
			{
				Da_temp_1 = U_in_ref * n * ( Cr_fs * (2 * U_in_ref * n - U_out_f) + I_out_f );
				Da_temp_2 = Da_temp_1 / (Da_temp_1 - P_out);
				Da_sqrt = Da_temp_2 * Da_temp_2 - 1;
				counter_Da_steps++;
			}
			else if (counter_Da_steps == 1)
			{
				Da_atan = in[6];
				counter_Da_steps++;
			}
			else
			{


				U_in_reg[ePSM_FBI_VDR].Integral_Portion_Z = wr_Ts * in[7];
				charging_Da_buck = 0.5;
				charging_Da_boost = Da;
				charging_Duty_cycle = half_Thrtm;
				charging_Da_step_buck = (0.5 - (U_in_reg[ePSM_FBI_VDR].Integral_Portion_Z)) / Charging_N_C3;
				charging_Da_step_boost = charging_Da_boost / Charging_N_C3;
				charging_step_counter = 0;

				charging_duty_cycle_step = -un_charging_duty_cycle_step_C3;
				counter_Da_steps = 0;
				eConverterMode = eBFBR_FBR_PSM_VDR;

			}

		}
		break;

	case eBFBR_FBR_PSM_VDR:

		//Charging of capacitor C3 
		if (charging_step_counter != Charging_N_C3)
		{
			charging_step_counter++;
			charging_Duty_cycle += charging_duty_cycle_step;

			charging_Da_buck -= charging_Da_step_buck;


			charging_Da_boost -= charging_Da_step_boost;
			if (charging_Da_boost < 0)
				charging_Da_boost = 0;
			//charging_Da_boost -= charging_Da_step_boost;
			//Switch S3
			HRTIM1_TIMD.CMP1xR = (u32)(charging_Da_buck * T_hrtm);
			HRTIM1_TIMD.CMP2xR = (u32)(charging_Da_buck * T_hrtm + half_Thrtm - T_d);
			//Switch S4
			HRTIM1_TIMD.CMP3xR = (u32)(charging_Da_buck * T_hrtm + half_Thrtm);
			HRTIM1_TIMD.CMP4xR = (u32)(charging_Da_buck * T_hrtm - T_d);
			if (HRTIM1_TIMD.CMP4xR > T_hrtm)
				HRTIM1_TIMD.CMP4xR = T_hrtm + MinCmpVal;

			U_in_reg[eConverterMode].Integral_Portion_Z = wr_Ts * in[7];


			//Switch Q3
			HRTIM1_TIMB.CMP1xR = (u32)(charging_Da_boost * T_hrtm);
			HRTIM1_TIMB.CMP2xR = (u32)(charging_Da_boost * T_hrtm + charging_Duty_cycle - T_d);
			if (HRTIM1_TIMB.CMP2xR > T_hrtm)
				HRTIM1_TIMB.CMP2xR = 0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
			//Switch Q4
			HRTIM1_TIMB.CMP3xR = (u32)(charging_Da_boost * T_hrtm + charging_Duty_cycle);
			HRTIM1_TIMB.CMP4xR = (u32)(charging_Da_boost * T_hrtm - T_d);
			if (HRTIM1_TIMB.CMP4xR > T_hrtm)
				HRTIM1_TIMB.CMP4xR = 0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
		}
		else if (U_in_reg[ePSM_FBI_VDR].flag_transition == 1)
			eConverterMode = eBFBR_FBI_FBR;

		else
		{
			eConverterMode = ePSM_FBI_VDR;
			U_in_reg[ePSM_FBI_VDR].flag_transition == 0;
		}

		break;



	case ePSM_FBI_VDR: //PSM FBI-FBR

		// Input bridge
//Switch S1
		HRTIM1_TIMC.CMP1xR = 0x0;
		HRTIM1_TIMC.CMP2xR = (u32)(half_Thrtm - T_d);
		//Switch S2
		HRTIM1_TIMC.CMP3xR = half_Thrtm;
		HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);
		//Switch S3
		HRTIM1_TIMD.CMP1xR = (u32)(Da * T_hrtm);
		HRTIM1_TIMD.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
		//Switch S4
		HRTIM1_TIMD.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
		HRTIM1_TIMD.CMP4xR = (u32)(Da * T_hrtm - T_d);
		if (HRTIM1_TIMD.CMP4xR > T_hrtm)
			HRTIM1_TIMD.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11

		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = 0x0;
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = 0x0;
		HRTIM1_TIMB.CMP2xR = 0x0;
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = 0x0;
		HRTIM1_TIMB.CMP4xR = T_hrtm + MinCmpVal;



		if (U_in_ref > 0.059 * U_out_f)
		{
			if (counter_Da_steps == 0)
			{
				Da_temp_1 = 2*U_in_f * n * Cr_fs * U_in_f * n + 0.5 * P_out;
				Da_temp_2 = Da_temp_1 / (Da_temp_1 - P_out + U_in_f * n * I_out_f);
				Da_sqrt = Da_temp_2 * Da_temp_2 - 1;
				counter_Da_steps++;
			}
			else if (counter_Da_steps == 1)
			{
				Da_atan = in[6];
				counter_Da_steps++;
			}
			else
			{
				

				U_in_reg[ePSM_FBI_VDR].flag_transition = 1;
				charging_Da_buck = Da;
				eConverterMode = eBFBR_FBR_PSM_VDR;
				U_in_reg[eBFBR_FBI_FBR].Integral_Portion_Z = wr_Ts * in[7];
				counter_Da_steps = 0;


				charging_Da_step_buck = -(0.5 - Da) / Charging_N_C3;
				charging_Duty_cycle = 0;

				charging_duty_cycle_step = un_charging_duty_cycle_step_C3;

				charging_Da_boost = 0;
				charging_Da_step_boost = -(U_in_reg[eBFBR_FBI_FBR].Integral_Portion_Z) / Charging_N_C3;
				charging_step_counter = 0;


			}
		}
		else if (Da > 0.44)
		{
			eConverterMode = eSwBVDR_FBI_VDR;
			U_in_reg[eConverterMode].flag_transition = 0;

			U_in_reg[eConverterMode].Integral_Portion_Z = 0;

		}
		break;

	case eSwBVDR_FBI_VDR: //BFBR HBI-FBR

// Input bridge
//Switch S1
	//Switch S1
		HRTIM1_TIMC.CMP1xR = T_hrtm;
		HRTIM1_TIMC.CMP2xR = (u32)(half_Thrtm - T_d);
		//Switch S2
		HRTIM1_TIMC.CMP3xR = half_Thrtm;
		HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);
		//Switch S3
		HRTIM1_TIMD.CMP1xR = half_Thrtm;
		HRTIM1_TIMD.CMP2xR = (u32)(T_hrtm - T_d);
		//Switch S4
		HRTIM1_TIMD.CMP3xR = T_hrtm;
		HRTIM1_TIMD.CMP4xR = (u32)(half_Thrtm - T_d);

		// Output bridge
		// Passive rectifer
		//Switch Q1
		HRTIM1_TIMA.CMP1xR = 0x0;
		HRTIM1_TIMA.CMP2xR = 0x0;
		//Switch Q2
		HRTIM1_TIMA.CMP3xR = 0x0;
		HRTIM1_TIMA.CMP4xR = (u32)(Da * T_hrtm + half_Thrtm);
		//Switch Q3
		HRTIM1_TIMB.CMP1xR = 0x0;
		HRTIM1_TIMB.CMP2xR = 0x0;
		//Switch Q4
		HRTIM1_TIMB.CMP3xR = 0x0;
		HRTIM1_TIMB.CMP4xR = T_hrtm + MinCmpVal;


		if (U_in_reg[eConverterMode].flag_transition == 1)
		{
			eConverterMode = ePSM_FBI_VDR;
			U_in_reg[eConverterMode].Integral_Portion_Z = 0.435;
		}

		break;
	}

}
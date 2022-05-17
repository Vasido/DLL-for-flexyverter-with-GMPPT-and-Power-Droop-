
#include "All_Headers.h"
#include "Var_def.h"


void set_cmp_hrtm()
{
	static u16 charging_step_counter= 0;
	static uint32_t charging_Duty_cycle = half_Thrtm;
	//static float charging_Da_buck = 0.5;
	static float charging_Da_step_buck = 0;
	static float charging_Da_boost = 0;
	static float charging_Da_step_boost = 0;
	//static int charging_duty_cycle_step=0;

	static float Da_temp_1 = 0;
	static float Da_temp_2 = 0;

	T_d = (u32)(f_HRCK_M * t_d);
	Min_Da = K_Min_Da * t_d;

	switch (eConverterMode)
	{
		case eBuck_HBI_FBR: //APWM  HBI-FBR
		{
			// Input bridge
			//Switch S1
			HRTIM1_TIMC.CMP1xR = 0x0;
			HRTIM1_TIMC.CMP2xR = (u32)(Da * T_hrtm);
			//Switch S2
			HRTIM1_TIMC.CMP3xR = (u32)(Da * T_hrtm + T_d);
			HRTIM1_TIMC.CMP4xR = (u32)(T_hrtm - T_d);

			if ((Da > Max_Da_Buck- Da_Dead_zone))
			{
				eConverterMode = eBoost_HBI_FBR;
				V_in_reg[eConverterMode].flag_transition = 0;
				V_in_reg[eConverterMode].Integral_Portion_Z = 0;
				//Switch S1
				HRTIM1_TIMC.CMP1xR = 0x0;
				HRTIM1_TIMC.CMP2xR = half_Thrtm- T_d;
				//Switch S2
				HRTIM1_TIMC.CMP3xR = half_Thrtm;
				HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;
			}
		}
		break;
		
		case eBoost_HBI_FBR: //BFBR HBI-FBR
		{
			//Switch Q3
			HRTIM1_TIMB.CMP1xR = (u32)(Da * T_hrtm);
			HRTIM1_TIMB.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
			//Switch Q4
			HRTIM1_TIMB.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
			HRTIM1_TIMB.CMP4xR = (u32)(Da * T_hrtm - T_d);
			if (HRTIM1_TIMB.CMP4xR > T_hrtm)
				HRTIM1_TIMB.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11

			if (V_in_reg[eConverterMode].flag_transition == 1)
			{
				if (V_in_ref > G_trans_Buck_HBI_Boost_HBI * V_out_f+0.25)//0.163 * 350=57 V
				{
					eConverterMode = eBuck_HBI_FBR;
					V_in_reg[eConverterMode].Integral_Portion_Z = Max_Da_Buck - Da_Dead_zone- Da_hysteresis;

					//Switch Q3
					HRTIM1_TIMB.CMP1xR = 0x0;
					HRTIM1_TIMB.CMP2xR = 0x0;
					//Switch Q4
					HRTIM1_TIMB.CMP3xR = T_hrtm + MinCmpVal;
					HRTIM1_TIMB.CMP4xR = 0x0;
				}
				else
					V_in_reg[eConverterMode].flag_transition = 0;

			}
			else if (V_in_ref < G_trans_Boost_HBI_Buck_FBI * V_out_f-0.25)
			{
				Da_temp_1 = V_in_ref * n * (2 * Cr_fs * (V_in_ref * n - V_out_f) + I_out_f * 0.5);
				Da_temp_2 = (P_out * 1000) / (Da_temp_1);

				//counter_Da_steps++;
				V_in_reg[eConverterMode].flag_transition = 1;
				V_in_reg[eBuck_FBI_FBR].Integral_Portion_Z = arccos[(int)(1000 - Da_temp_2)];

				charging_Da_buck = 0.5;
				charging_Da_boost = Da;
				charging_Duty_cycle = 0;
				charging_Da_step_buck = (0.5 - (V_in_reg[eBuck_FBI_FBR].Integral_Portion_Z)) / Charging_N_C2;
				charging_Da_step_boost = charging_Da_boost / Charging_N_C2;
				charging_step_counter = 0;

				charging_duty_cycle_step = un_charging_duty_cycle_step_C2;
				//counter_Da_steps = 0;
				eConverterMode = eBoost_HBI_Buck_FBI;

				//Waiting for transion input and output voltage
				prev_machine_state = machine_state;
				machine_state = Transition_operation_mode;
			}
		}
		break;
	
		case eBoost_HBI_Buck_FBI:
		{
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
				if (HRTIM1_TIMD.CMP2xR > T_hrtm)
					HRTIM1_TIMD.CMP2xR = 0;

				//Switch S4
				HRTIM1_TIMD.CMP3xR = (u32)(charging_Da_buck * T_hrtm + charging_Duty_cycle);
				HRTIM1_TIMD.CMP4xR = (u32)(charging_Da_buck * T_hrtm - T_d);
				if (HRTIM1_TIMD.CMP4xR > T_hrtm)
					HRTIM1_TIMD.CMP4xR = T_hrtm - T_d;
				//Switch Q3
				HRTIM1_TIMB.CMP1xR = HRTIM1_TIMC.CMP3xR;
				HRTIM1_TIMB.CMP2xR = (u32)(charging_Da_boost * T_hrtm + half_Thrtm - T_d);
				//Switch Q4
				HRTIM1_TIMB.CMP3xR = HRTIM1_TIMC.CMP1xR;
				HRTIM1_TIMB.CMP4xR = (u32)(charging_Da_boost * T_hrtm - T_d);
				if (HRTIM1_TIMB.CMP4xR > T_hrtm)
					HRTIM1_TIMB.CMP4xR = 0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
			}
			else if (V_in_reg[eBuck_FBI_FBR].flag_transition == 1)
			{
				V_in_reg[eBuck_FBI_FBR].flag_transition = 0;
				eConverterMode = eBoost_HBI_FBR;
				//Switch S3
				HRTIM1_TIMD.CMP1xR = 0x0;
				HRTIM1_TIMD.CMP2xR = 0x0;
				//Switch S4
				HRTIM1_TIMD.CMP3xR = 0x0;
				HRTIM1_TIMD.CMP4xR = T_hrtm + MinCmpVal;
			}
			else
			{
				eConverterMode = eBuck_FBI_FBR;
				V_in_reg[eBuck_FBI_FBR].flag_transition = 0;
				//Switch Q3
				HRTIM1_TIMB.CMP1xR = 0x0;
				HRTIM1_TIMB.CMP2xR = 0x0;
				//Switch Q4
				HRTIM1_TIMB.CMP3xR = 0;//T_hrtm + MinCmpVal;
				HRTIM1_TIMB.CMP4xR = 0x0;
			}
		}
		break;

		case eBuck_FBI_FBR: //PSM FBI-FBR
		{
			//Switch S3
			HRTIM1_TIMD.CMP1xR = (u32)(Da * T_hrtm);
			HRTIM1_TIMD.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
			//Switch S4
			HRTIM1_TIMD.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
			HRTIM1_TIMD.CMP4xR = (u32)(Da * T_hrtm - T_d);
			if (HRTIM1_TIMD.CMP4xR > T_hrtm)
				HRTIM1_TIMD.CMP4xR = 0x0;///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
		
			if (V_in_ref > G_trans_Boost_HBI_Buck_FBI * V_out_f + 0.25)
			{
				Da_temp_1 = V_in_f * n * Cr_fs * V_in_f * n + P_out;
				Da_temp_2 = (2 * P_out - V_in_f * n * I_out_f) / (Da_temp_1);
				
				//counter_Da_steps++;
				V_in_reg[eBuck_FBI_FBR].flag_transition = 1;
				charging_Da_buck = Da;
				eConverterMode = eBoost_HBI_Buck_FBI;

				V_in_reg[eBoost_HBI_FBR].Integral_Portion_Z = arccos[(int)(1000 - Da_temp_2)];
				//counter_Da_steps = 0;

				charging_Da_step_buck = -(0.5 - Da) / Charging_N_C2;
				charging_Duty_cycle = half_Thrtm;
				charging_duty_cycle_step = -un_charging_duty_cycle_step_C2;

				charging_Da_boost = 0;
				charging_Da_step_boost = -(V_in_reg[eBoost_HBI_FBR].Integral_Portion_Z) / Charging_N_C2;
				charging_step_counter = 0;

				//Waiting for transion input and output voltage
				prev_machine_state = machine_state;
				machine_state = Transition_operation_mode;
			}
			else if (Da > Max_Da_Buck - Da_Dead_zone)
			{
				eConverterMode = eBoost_FBI_FBR;
				V_in_reg[eConverterMode].flag_transition = 0;
				V_in_reg[eConverterMode].Integral_Portion_Z = 0;

				// Input bridge
				//Switch S3
				HRTIM1_TIMD.CMP1xR = half_Thrtm;
				HRTIM1_TIMD.CMP2xR = (u32)(T_hrtm - T_d);
				//Switch S4
				HRTIM1_TIMD.CMP3xR = T_hrtm;
				HRTIM1_TIMD.CMP4xR = (u32)(half_Thrtm - T_d);
			}
		}
		break;

		case eBoost_FBI_FBR: //BFBR HBI-FBR
		{
			//Switch Q3
			HRTIM1_TIMB.CMP1xR = (u32)(Da * T_hrtm);
			HRTIM1_TIMB.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
			//Switch Q4
			HRTIM1_TIMB.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
			HRTIM1_TIMB.CMP4xR = (u32)(Da * T_hrtm - T_d);
			if (HRTIM1_TIMB.CMP4xR > T_hrtm)
				HRTIM1_TIMB.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11

			if (V_in_reg[eConverterMode].flag_transition == 1)
			{
				if (V_in_ref > G_trans_Buck_FBI_Boost_FBI * V_out_f+0.25)//0.077 * 350=27 V
				{
					eConverterMode = eBuck_FBI_FBR;
					V_in_reg[eConverterMode].Integral_Portion_Z = Max_Da_Buck - Da_Dead_zone - Da_hysteresis;
				}
			}
			else if (V_in_ref < G_trans_Boost_FBR_Buck_HBR * V_out_f-0.25)
			{
				Da_temp_1 = V_in_ref * n * (Cr_fs * (2 * V_in_ref * n - V_out_f) + I_out_f);
				Da_temp_2 = (1000 * P_out) / (Da_temp_1);
				if (Da_temp_2 > 1000)
					Da_temp_2 = 1000;
				
				//counter_Da_steps++;
				//counter_Da_steps++;

				V_in_reg[eConverterMode].flag_transition = 1;
				V_in_reg[eBuck_FBI_HBR].Integral_Portion_Z = arccos[(int)(1000 - Da_temp_2)];
				charging_Da_buck = 0.5;
				charging_Da_boost = Da;
				charging_Duty_cycle = half_Thrtm;
				charging_Da_step_buck = (0.5 - (V_in_reg[eBuck_FBI_HBR].Integral_Portion_Z)) / Charging_N_C3;
				charging_Da_step_boost = charging_Da_boost / Charging_N_C3;
				charging_step_counter = 0;

				charging_duty_cycle_step = -un_charging_duty_cycle_step_C3;
				//counter_Da_steps = 0;
				eConverterMode = eBoost_FBR_Buck_HBR;

				//Waiting for transion input and output voltage
				prev_machine_state = machine_state;
				machine_state = Transition_operation_mode;
			}
		}
		break;

		case eBoost_FBR_Buck_HBR:
		{
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
					HRTIM1_TIMD.CMP4xR = T_hrtm - T_d;

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
			else if (V_in_reg[eBuck_FBI_HBR].flag_transition == 1)
			{
				V_in_reg[eBuck_FBI_HBR].flag_transition = 0;
				eConverterMode = eBoost_FBI_FBR;
			}

			else if (V_in_reg[eBoost_FBI_FBR].flag_transition == 1)
			{
				eConverterMode = eBuck_FBI_HBR;
				V_in_reg[eBuck_FBI_HBR].flag_transition = 0;
				V_in_reg[eBoost_FBI_FBR].flag_transition = 0;
				//Switch Q3
				HRTIM1_TIMB.CMP1xR = 0x0;
				HRTIM1_TIMB.CMP2xR = 0x0;
				//Switch Q4
				HRTIM1_TIMB.CMP3xR = 0x0;
				HRTIM1_TIMB.CMP4xR = T_hrtm + MinCmpVal;
			}
	
		}
		break;

		case eBuck_FBI_HBR: //PSM FBI-FBR
		{
			// Input bridge
			//Switch S3
			HRTIM1_TIMD.CMP1xR = (u32)(Da * T_hrtm);
			HRTIM1_TIMD.CMP2xR = (u32)(Da * T_hrtm + half_Thrtm - T_d);
			//Switch S4
			HRTIM1_TIMD.CMP3xR = (u32)(Da * T_hrtm + half_Thrtm);
			HRTIM1_TIMD.CMP4xR = (u32)(Da * T_hrtm - T_d);
			if (HRTIM1_TIMD.CMP4xR > T_hrtm)
				HRTIM1_TIMD.CMP4xR = (u32)(T_hrtm - T_d);///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11

			if (V_in_ref > G_trans_Boost_FBR_Buck_HBR * V_out_f + 0.25)
			{
				Da_temp_1 = 2 * V_in_f * n * Cr_fs * V_in_f * n + 0.5 * P_out;
				Da_temp_2 = (P_out - V_in_f * n * I_out_f) / (Da_temp_1);
				
				//counter_Da_steps++;

				V_in_reg[eBuck_FBI_HBR].flag_transition = 1;
				charging_Da_buck = Da;
				eConverterMode = eBoost_FBR_Buck_HBR;
				V_in_reg[eBoost_FBI_FBR].Integral_Portion_Z = arccos[(int)(1000 - Da_temp_2)];
				//counter_Da_steps = 0;

				charging_Da_step_buck = -(0.5 - Da) / Charging_N_C3;
				charging_Duty_cycle = 0;

				charging_duty_cycle_step = un_charging_duty_cycle_step_C3;

				charging_Da_boost = 0;
				charging_Da_step_boost = -(V_in_reg[eBoost_FBI_FBR].Integral_Portion_Z) / Charging_N_C3;
				charging_step_counter = 0;

				//Waiting for transion input and output voltage
				prev_machine_state = machine_state;
				machine_state = Transition_operation_mode;

			}
			else if (Da > Max_Da_Buck - Da_Dead_zone)
			{
				eConverterMode = eBoost_FBI_HBR;
				V_in_reg[eConverterMode].flag_transition = 0;
				V_in_reg[eConverterMode].Integral_Portion_Z = 0;
				//Switch S3
				HRTIM1_TIMD.CMP1xR = half_Thrtm;
				HRTIM1_TIMD.CMP2xR = (u32)(T_hrtm - T_d);
				//Switch S4
				HRTIM1_TIMD.CMP3xR = T_hrtm;
				HRTIM1_TIMD.CMP4xR = (u32)(half_Thrtm - T_d);

			}
		}
		break;

		case eBoost_FBI_HBR: //BFBR HBI-FBR
		{
			// Output bridge
			// Passive rectifer
			//Switch Q1
			HRTIM1_TIMA.CMP1xR = 0x0;
			HRTIM1_TIMA.CMP2xR = 0x0;
			//Switch Q2
			HRTIM1_TIMA.CMP3xR = 0x0;
			HRTIM1_TIMA.CMP4xR = (u32)(Da * T_hrtm + half_Thrtm);
	
			if (V_in_reg[eConverterMode].flag_transition == 1)
			{
				if (V_in_ref > G_trans_Buck_HBR_Boost_HBR * V_out_f+0.25)//0.037 * 350=13 V
				{
					eConverterMode = eBuck_FBI_HBR;
					V_in_reg[eConverterMode].Integral_Portion_Z = Max_Da_Buck - Da_Dead_zone - Da_hysteresis;
					//Switch Q1
					HRTIM1_TIMA.CMP1xR = 0x0;
					HRTIM1_TIMA.CMP2xR = 0x0;
					//Switch Q2
					HRTIM1_TIMA.CMP3xR = 0x0;
					HRTIM1_TIMA.CMP4xR = 0x0;
				}
			}
		}
		break;
	}
}
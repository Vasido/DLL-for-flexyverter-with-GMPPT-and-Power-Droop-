
#include "All_Headers.h"
#include "Var_ref.h"

#include "main.h"
#include "Structs.h"

//status current_status = FAULT;
//enum signals getSignal(void); //function to calculate state machine control signals



//transition table
transition FSM_table[8] = {//[state][transient vector]

	   {fnc_standby},					//Standby
	   {fnc_start},						//Start		Waiting for Stabilization of input and output voltage filters
	   {fnc_gmppt},						//GMPPT		global maximum power point tracking
	   {fnc_lmppt},						//LMPPT		local maximum power point tracking
	   {fnc_power_droop},				//Power_droop based on output voltage
	   {fnc_transition_opeation_mode},	//Transition_operation_mode		stop all regulators in eBoost_HBI_Buck_FBI and eBoost_FBR_Buck_HBR modes
	   {fnc_stop_reset},				//Stop_Reset regulators and GMPP
	   {fnc_transit_to_LMPPT},			//Transit_to_LMPPT Transitiof from GMPPT to LPPPT
};

//Standby wait for normal input, output voltage and zero FAULT status
void fnc_standby(void)
{
	if (machine_status != FAULT)
	{
		if (machine_status == RUN)
		{
			machine_state = Start;
		}
	}

}

//Start		Waiting for Stabilization of input and output voltage filters then to GMPPT
void fnc_start(void)
{
	static u16 start_counter = 0;

	if (start_counter != 20)
	{
		start_counter++;
	}
	else if (((V_in_f - V_in_old) > 0.25) || ((V_in_f - V_in_old) < -0.25))
	{
		V_in_old= V_in_f;
		start_counter=0;
	}
	else
	{
		start_counter = 0;
		V_in_ref = V_in_f;
		machine_state = GMPPT;
		if (V_in_ref > (G_trans_Buck_HBI_Boost_HBI * V_out_f + 0.5)) //V_in_trans between Buck_HBI and Boost_HBI G_trans_Buck_HBI_Boost_HBI
		{
			eConverterMode = eBuck_HBI_FBR;
			//Switch S3
			HRTIM1_TIMD.CMP1xR = 0x0;
			HRTIM1_TIMD.CMP2xR = 0x0;
			//Switch S4
			HRTIM1_TIMD.CMP3xR = 0x0;
			HRTIM1_TIMD.CMP4xR = T_hrtm + MinCmpVal;
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
		}

		else if (V_in_ref > (G_trans_Boost_HBI_Buck_FBI * V_out_f - 0.5))//V_trans__Boost_HBI_Buck_FBI
		{
			//eConverterMode = BFBR_HBI_FBR;

			V_in_reg[eBuck_FBI_FBR].flag_transition = 1;
			eConverterMode = eBoost_HBI_Buck_FBI;
			// Input bridge
			//Switch S1
			HRTIM1_TIMC.CMP1xR = 0x0;
			HRTIM1_TIMC.CMP2xR = half_Thrtm - T_d;
			//Switch S2
			HRTIM1_TIMC.CMP3xR = half_Thrtm;
			HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;

			charging_duty_cycle_step = -un_charging_duty_cycle_step_C2;
			charging_Da_buck = 0;

			//Waiting for transion input and output voltage
			prev_machine_state = machine_state;
			machine_state = Transition_operation_mode;
		}

		else if (V_in_ref > (G_trans_Buck_FBI_Boost_FBI * V_out_f - 0.5))//V_trans_Buck_HBI_Boost_HBI
		{
			eConverterMode = eBuck_FBI_FBR;
			// Input bridge
			//Switch S1
			HRTIM1_TIMC.CMP1xR = 0x0;
			HRTIM1_TIMC.CMP2xR = half_Thrtm - T_d;
			//Switch S2
			HRTIM1_TIMC.CMP3xR = half_Thrtm;
			HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;

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
		}

		else if (V_in_ref > (G_trans_Boost_FBR_Buck_HBR * V_out_f + 0.5))
		{
			eConverterMode = eBoost_FBI_FBR;
			// Input bridge
			//Switch S1
			HRTIM1_TIMC.CMP1xR = 0x0;
			HRTIM1_TIMC.CMP2xR = half_Thrtm - T_d;
			//Switch S2
			HRTIM1_TIMC.CMP3xR = half_Thrtm;
			HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;
			// Input bridge
			//Switch S3
			HRTIM1_TIMD.CMP1xR = half_Thrtm;
			HRTIM1_TIMD.CMP2xR = (u32)(T_hrtm - T_d);
			//Switch S4
			HRTIM1_TIMD.CMP3xR = T_hrtm;
			HRTIM1_TIMD.CMP4xR = (u32)(half_Thrtm - T_d);
			//Switch Q1
			HRTIM1_TIMA.CMP1xR = 0x0;
			HRTIM1_TIMA.CMP2xR = 0x0;
			//Switch Q2
			HRTIM1_TIMA.CMP3xR = 0x0;
			HRTIM1_TIMA.CMP4xR = 0x0;
		}
		else if (V_in_ref > (G_trans_Buck_HBR_Boost_HBR * V_out_f - 0.5))
		{
			eConverterMode = eBuck_FBI_HBR;
			// Input bridge
			//Switch S1
			HRTIM1_TIMC.CMP1xR = 0x0;
			HRTIM1_TIMC.CMP2xR = half_Thrtm - T_d;
			//Switch S2
			HRTIM1_TIMC.CMP3xR = half_Thrtm;
			HRTIM1_TIMC.CMP4xR = T_hrtm - T_d;
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
		}
		else
		{
			//eConverterMode = eBoost_FBI_HBR;
			eConverterMode = eBoost_FBR_Buck_HBR;
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
			//Switch Q3
			HRTIM1_TIMB.CMP1xR = 0x0;
			HRTIM1_TIMB.CMP2xR = 0x0;
			//Switch Q4
			HRTIM1_TIMB.CMP3xR = 0x0;
			HRTIM1_TIMB.CMP4xR = T_hrtm + MinCmpVal;
			charging_duty_cycle_step = -un_charging_duty_cycle_step_C2;
			charging_Da_buck = 0.5;

			//Waiting for transion input and output voltage
			prev_machine_state = machine_state;
			machine_state = Transition_operation_mode;
		}
	}
}

//GMPPT		global maximum power point tracking
void fnc_gmppt(void)
{
	static u16 P_diff_sign = 0; // if (P_out-P_out_old)>0 then P_diff_sign=0
	static u16 GMPPT_counter = 0;
	static u16 i = 0;
	static u16 direction = 0;
	//static u16 GMPP_i = 0;
	//Power droop control
	
		if ((V_in_ref > Min_V_in) && (GMPP_i != 10)&&(direction==0))
		{
			//if (GMPPT_counter == 20)
			//{
			//	// if derivative of Power < 0 then P_diff_sign=1. 
			//	if (((P_out - P_out_old) < -0.2) && (P_diff_sign == 0))
			//	{
			//		P_diff_sign = 1;
			//		GMPPs_P_out[GMPP_i] = P_out_old;	//Save Global maximum power points
			//		GMPPs_V_in[GMPP_i] = V_in_ref+ V_ref_step_N;
			//		GMPP_i++;

			//	}
			//	// if derivative of Power > 0 then P_diff_sign=0. 
			//	if (((P_out - P_out_old) > 0.2) && (P_diff_sign == 1))
			//	{
			//		P_diff_sign = 0;
			//	}

			//	P_out_old = P_out;
			//	GMPPT_counter = 0;

			//}
			//else
			//	GMPPT_counter++;

			Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
			set_cmp_hrtm();

			V_in_ref -= V_ref_step;
		}
		else if ((V_in_ref <= Min_V_in) && (direction == 0))
		{
			direction = 1;
		}
		else
		{
			Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
			set_cmp_hrtm();

			V_in_ref += V_ref_step;
		}
		//else
		//{	///Finding out of Gloval Maximum power poit
		//	GMPP_i = 0;
		//	for (i = 1; i < 10; i++)
		//	{
		//		if (GMPPs_P_out[GMPP_i] < GMPPs_P_out[i])
		//			GMPP_i = i;
		//	}
		//	V_in_ref_LPPT = GMPPs_V_in[GMPP_i];
		//	GMPPs_P_out[0] = GMPPs_P_out[GMPP_i];
		//	GMPP_i = 0;

		//	i = 0;
		//	machine_state = Transit_to_LMPPT;

		//}
}

void fnc_transit_to_LMPPT(void)//Transitiof from GMPPT to LPPPT
{

	if (V_in_ref < V_in_ref_LPPT)
		V_in_ref += V_ref_step;
	else
	{
		machine_state = LMPPT;
		P_out_reg.Integral_Portion_Z = V_in_ref_LPPT;
	}
	Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
	set_cmp_hrtm();
}

//LMPPT		local maximum power point tracking
void fnc_lmppt(void)
{
	static u16 LMPPT_counter = 0;

	if (LMPPT_counter == 10)
	{
		V_in_ref = PI(&P_out_old, &P_out, &P_out_reg);
		Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
		set_cmp_hrtm();
		P_out_old = P_out;
		LMPPT_counter = 0;
	}
	else
		LMPPT_counter++;
}

//Power_droop based on output voltage. Funtion of power droop control is described in the main 
void fnc_power_droop(void)
{
	
	V_in_ref = PI(&P_out, &P_lim, &P_out_reg);
	Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
	set_cmp_hrtm();
	if (((P_out - P_lim) > 20) || ((P_out - P_lim) < -20))
	{
		machine_state = Stop_Reset;
		machine_status = READY;
	}
}

//Transition_operation_mode		stop all regulators in eBoost_HBI_Buck_FBI and eBoost_FBR_Buck_HBR modes
//Waiting for transition process of input and output voltage 
void fnc_transition_opeation_mode(void)
{
	static u16 transition_counter = 0;

	if (transition_counter != T_transition)
	{
		Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
		set_cmp_hrtm();
		transition_counter++;
	}
	else
	{
		transition_counter = 0;
		//Return to previous state
		machine_state = prev_machine_state;
	}
}

//Stop_Reset regulators and any state
void fnc_stop_reset(void)
{
	// Input bridge
	//Switch S1
	HRTIM1_TIMC.CMP1xR = 0x0;
	HRTIM1_TIMC.CMP2xR = 0x0;
	//Switch S2
	HRTIM1_TIMC.CMP3xR = 0x0;
	HRTIM1_TIMC.CMP4xR = 0x0;
	//Switch S3
	HRTIM1_TIMD.CMP1xR = 0x0;
	HRTIM1_TIMD.CMP2xR = 0x0;
	//Switch S4
	HRTIM1_TIMD.CMP3xR = T_hrtm + MinCmpVal;
	HRTIM1_TIMD.CMP4xR = 0x0;

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
	HRTIM1_TIMB.CMP4xR = 0x0;

	V_in_reg[eBuck_HBI_FBR].flag_transition = 0;
	V_in_reg[eBoost_HBI_FBR].flag_transition = 0;
	V_in_reg[eBuck_FBI_FBR].flag_transition = 0;
	V_in_reg[eBoost_FBI_FBR].flag_transition = 0;
	V_in_reg[eBuck_FBI_HBR].flag_transition = 0;
	V_in_reg[eBoost_FBI_HBR].flag_transition = 0;

	V_in_reg[eBuck_HBI_FBR].Integral_Portion_Z = 0;
	V_in_reg[eBoost_HBI_FBR].Integral_Portion_Z = 0;
	V_in_reg[eBuck_FBI_FBR].Integral_Portion_Z = 0;
	V_in_reg[eBoost_FBI_FBR].Integral_Portion_Z = 0;
	V_in_reg[eBuck_FBI_HBR].Integral_Portion_Z = 0;
	V_in_reg[eBoost_FBI_HBR].Integral_Portion_Z = 0;
	P_out_reg.Integral_Portion_Z = 0;
	machine_state = Standby;
}
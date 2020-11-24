
#include "All_Headers.h"
#include "Var_ref.h"



//status current_status = FAULT;
//enum signals getSignal(void); //function to calculate state machine control signals



//transition table
transition FSM_table[7] = {//[state][transient vector]

	   {fnc_standby},					//Standby
	   {fnc_start},						//Start		increment
	   {fnc_gmppt},						//GMPPT		global maximum power point tracking
	   {fnc_lmppt},						//LMPPT		local maximum power point tracking
	   {fnc_power_droop},				//Power_droop based on output voltage
	   {fnc_transition_opeation_mode},	//Transition_operation_mode		stop all regulators in eBFBR_HBI_PSM_FBI and eBFBR_FBR_PSM_VDR modes
	   {fnc_stop_reset},				//Stop_Reset regulators and GMPP
};





void fnc_standby(void)
{
	
}

void fnc_start(void)
{
	static u16 starting_counter = 0;

		V_in_ref=V_in_f;
		machine_state = GMPPT;
		machine_status = RUN;

}
void fnc_gmppt(void)
{

	Da = PI(&V_in_f, &V_in_ref, &V_in_reg[eConverterMode]);
	set_cmp_hrtm();


}
void fnc_lmppt(void)
{


}
void fnc_power_droop(void)
{


}

void fnc_transition_opeation_mode(void)
{


}

void fnc_stop_reset(void)
{


}
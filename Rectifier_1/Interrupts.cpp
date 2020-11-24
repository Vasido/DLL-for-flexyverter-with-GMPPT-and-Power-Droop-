
#include "All_Headers.h"
#include "Var_ref.h"

//static Filter_TypeDef Filt;

u32 temp;
double		temp_double;		// Вспомогательная переменная для моделирования

void TIM2_IRQHandler (double* in)
{
	
	V_in = (float)in[1];
	I_in = (float)in[2];
	V_out = (float)in[3];
	I_out = (float)in[4];
	
	Da_atan = (float)in[6];
	SQRT = (float)in[7];
	//V_in_ref= (float)in[5];


	

	V_in_f = Filter(&V_in, &STR.V_in_fiter);
	I_in_f = Filter(&I_in, &STR.I_in_fiter);
	V_out_f = Filter(&V_out, &STR.V_out_fiter);
	I_out_f = Filter(&I_out, &STR.I_out_fiter);
	P_out = V_out_f * I_out_f;
	

	transition_handler = FSM_table[machine_state].indicate_transition_handler;
	transition_handler();//transition specific action taken from the transaction table
}


// imulation of ADS operation
void ADC_refresh(double *input)
{
	// Иммитация работы АЦП:
	temp_double = (*input*4095.0)/3.3;						// 12-ти битное АЦП		
	
	if(temp_double > 4095.0) 
		temp_double = 4095.0;
	if(temp_double < 0.0) 
		temp_double = 0.0;

	//ADC->ADC1_RESULT = (((u32)temp_double) & 0x00000FFF);	//убираем дробную часть-имитация работы целочисленного процессора
}

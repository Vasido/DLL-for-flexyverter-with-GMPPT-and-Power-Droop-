
#include "All_Headers.h"
#include "Var_ref.h"

//static Filter_TypeDef Filt;

u32 temp;
double		temp_double;		// Вспомогательная переменная для моделирования

void TIM2_IRQHandler (double* in)
{

	static u16 counter = 0;
	
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
	P_out = V_in_f * I_in_f;
	
	Da= (float)in[9];
	if (counter < 100)
	{
		D_HPSM = 0.5;
			counter++;
	}
		
	else
	{

		V_lk_max = V_in * n + I_out_f * _4Cr_fs - V_out_f;
		I_lk_sw = V_lk_max * sin[(int)(Da * wr_Ts_1000_pi)]; ///switching resonant current 
		I_lk_int = V_lk_max * cos[(int)(Da * wr_Ts_1000_pi)];
		//D_rec = Da + (I_out_f - I_lk_int) / I_lk_sw;
		//if (D_rec < (Da + 0.001))
			//D_rec = Da + 0.001;
	//	D_HPSM = D_rec+0.05;
		temp_1 = (I_out_f - I_lk_int) / I_lk_sw;
		D_HPSM = Da + (I_out_f - I_lk_int) / I_lk_sw + 0.05;
		if (D_HPSM < (Da + 0.05))
			D_HPSM = Da + 0.05;
		if (D_HPSM > 0.49)
			D_HPSM = 0.49;
	}





	//transition_handler = FSM_table[machine_state].indicate_transition_handler;
	//transition_handler();//transition specific action taken from the transaction table
	set_cmp_hrtm();

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

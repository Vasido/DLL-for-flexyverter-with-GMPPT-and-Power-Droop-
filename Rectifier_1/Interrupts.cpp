
#include "All_Headers.h"
#include "Var_ref.h"

//static Filter_TypeDef Filt;

u32 temp;
double		temp_double;		// Вспомогательная переменная для моделирования

void TIM2_IRQHandler (double* in)
{
	
	U_in = (float)in[1];
	I_in = (float)in[2];
	U_out = (float)in[3];
	I_out = (float)in[4];
	
	U_in_ref= (float)in[5];


	

	U_in_f = Filter(&U_in, &STR.U_in_fiter);
	I_in_f = Filter(&I_in, &STR.I_in_fiter);
	U_out_f = Filter(&U_out, &STR.U_out_fiter);
	I_out_f = Filter(&I_out, &STR.I_out_fiter);
	P_out = U_out_f * I_out_f;
	
	Da = PI(&U_in_f, &U_in_ref, &U_in_reg[eConverterMode]);
	
	set_cmp_hrtm(in); 

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

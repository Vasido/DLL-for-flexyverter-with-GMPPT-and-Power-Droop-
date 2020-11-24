#include "All_Headers.h"
#include "Var_ref.h"

	// ПИ-регулятор
	float PI(float*Desired_Value, float*Measured_Value, stPI_Params* Params)
	{
		f32 Integral_Portion;
		f32 Proportional_Portion;
		f32 Output;
		f32 Input_Error;

		Input_Error = *Desired_Value - *Measured_Value;

		Proportional_Portion = (((Params->Proportional_Gain)* Input_Error) );// / f_dis;

	
  
		Integral_Portion = (Params->Integral_Gain)* Input_Error;
		Params->Integral_Portion_Z += Integral_Portion;

		if((Params->Integral_Portion_Z) > (Params->Integral_H_Limit))
			Params->Integral_Portion_Z = Params -> Integral_H_Limit;

		if ((Params->Integral_Portion_Z) < (Params->Integral_L_Limit))
		{
			Params->Integral_Portion_Z = Params->Integral_L_Limit;
			
		}
			
  
	
		Output = Proportional_Portion + Params->Integral_Portion_Z;

		if ((Output) > (Params->Output_H_Limit))
		{
			Output = Params->Output_H_Limit;


			// Ограничиваем интегральную часть для нормального возврата из ограничения: 
			Params->Integral_Portion_Z -= Integral_Portion;
		}
		else if ((Output) < (Params->Output_L_Limit))
		{
		Output = Params->Output_L_Limit;
			// Ограничиваем интегральную часть для нормального возврата из ограничения: 
			Params->Integral_Portion_Z -= Integral_Portion;
			Params->flag_transition = 1;
		}

		return Output;
	}

/*
// Интегратор
void Integral(s32 Desired_Value, PI_Params* Params)
{
	Params->Integral_Portion_Z += (Params->Integral_Gain)*Desired_Value;

	if((Params->Integral_Portion_Z) > (Params->Integral_H_Limit))
		(Params->Integral_Portion_Z) = (Params -> Integral_H_Limit);

	if((Params->Integral_Portion_Z) < (Params->Integral_L_Limit))
		(Params->Integral_Portion_Z) = (Params -> Integral_L_Limit);
  
	Params->Output = (Params->Integral_Portion_Z) >> (Params->Integral_Gain_Scale);
}



*/
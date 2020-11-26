#include "All_Headers.h"

float Filter(float *DesiredValue, Filter_params*F_P)
{
	
	float InputError;
  InputError = (*DesiredValue+ F_P->OldValue)/2 - (F_P ->IntegralPortionK_1);
  F_P -> IntegralPortionK_1 += (float)(InputError * (F_P -> IntegralGain));
  F_P -> OldValue = *DesiredValue;
  return (F_P ->IntegralPortionK_1);
 
}

	/*//////////////////////////////////////////
	Filt.Inp[0] = Filt.Inp[1] = Filt.Inp[2] = Filt.Inp[3] = Filt.Inp[4] = 0;
	Filt.K[0] = 32767;
	Filt.K[1] = 15040;
	Filt.K[2] = 15902;
	Filt.K[3] = 15040;
	Filt.K[4] = 32767;
	//////////////////////////////////////////

s32 Filter_FIR(Filter_TypeDef Filt_Inp, s32 input, u16 Number)
{
	Filt_Inp.Inp[0] = input;
	Filt_Inp.Out = 0;
	
	for (u16 i = 0; i < Number; i++)
	{
		Filt_Inp.Out += ((Filt_Inp.Inp[Number - i] * Filt_Inp.K[Number - i])>>15);
		Filt_Inp.Inp[Number - i] = Filt_Inp.Inp[Number - i - 1];
	}
	Filt_Inp.Out += ((Filt_Inp.Inp[0] * Filt_Inp.K[0])>>15);

	return Filt_Inp.Out;
}
*/
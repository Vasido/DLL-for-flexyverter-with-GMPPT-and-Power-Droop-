#ifndef STRUCTS_H
#define STRUCTS_H


//FILTER PARAMETERS
typedef struct {
	f32 OldValue;
	f32 IntegralGain;
	f32 IntegralPortionK_1;
} F_params;


//PI CONTROLLER PARAMETERS
typedef struct
{
   const float Proportional_Gain;
   //s32 Proportional_Gain_Scale;
   //s32 Proportional_H_Limit;
   //s32 Proportional_L_Limit;
   //float Proportional_Portion;
   const float Integral_Gain;
   //s32 Integral_Gain_Scale;	
   float Integral_Portion_Z;
   const float Integral_H_Limit;
   const float Integral_L_Limit;
   const float Output_H_Limit;
   const float Output_L_Limit;
   u16	flag_transition;
   //float Input_Error;
   //float Output;
} stPI_Params;

//MAIN STRUCT
typedef struct
{
	F_params		U_out_fiter;
	F_params		U_in_fiter;
	F_params		I_out_fiter;
	F_params		I_in_fiter;
	//stPI_Params		U_in_reg;

	//PI_Params		P_los;
	s16				w_ref;
	u16				Interrupt_Number;

} STRUCT;


typedef struct
{
	u32 CMP1xR=0;
	u32 CMP2xR=0;
	u32 CMP3xR=0;
	u32 CMP4xR=0;

} HRTIM_TIMC;

typedef struct
{
	u32 CMP1xR=0;
	u32 CMP2xR=0;
	u32 CMP3xR=0;
	u32 CMP4xR=0;

} HRTIM_TIMD;

typedef struct
{
	u32 CMP1xR=0;
	u32 CMP2xR=0;
	u32 CMP3xR=0;
	u32 CMP4xR=0;

} HRTIM_TIMA;

typedef struct
{
	u32 CMP1xR=0;
	u32 CMP2xR=0;
	u32 CMP3xR=0;
	u32 CMP4xR=0;

} HRTIM_TIMB;

typedef struct
{
	u32 MCMP1R=0;
	u32 MCMP2R=0;


} HRTIM_Master;



#endif
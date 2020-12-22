#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

//FILTER PARAMETERS
typedef struct {
	f32 OldValue;
	f32 IntegralGain;
	f32 IntegralPortionK_1;
} Filter_params;


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
	Filter_params		V_out_fiter;
	Filter_params		V_in_fiter;
	Filter_params		I_out_fiter;
	Filter_params		I_in_fiter;
	//stPI_Params		V_in_reg;

	//PI_Params		P_los;
	
	u16				Interrupt_Number;

} STRUCT;


typedef struct
{
	u32 CMP1xR;
	u32 CMP2xR;
	u32 CMP3xR;
	u32 CMP4xR;

} HRTIM_TIMC;

typedef struct
{
	u32 CMP1xR;
	u32 CMP2xR;
	u32 CMP3xR;
	u32 CMP4xR;

} HRTIM_TIMD;

typedef struct
{
	u32 CMP1xR;
	u32 CMP2xR;
	u32 CMP3xR;
	u32 CMP4xR;

} HRTIM_TIMA;

typedef struct
{
	u32 CMP1xR;
	u32 CMP2xR;
	u32 CMP3xR;
	u32 CMP4xR;

} HRTIM_TIMB;

typedef struct
{
	u32 MCMP1R;
	u32 MCMP2R;


} HRTIM_Master;

typedef void (*transition_callback)(void); //callback prototype
//transition structure prototype
struct transition
{

	transition_callback indicate_transition_handler;
	//status current_status;
};




#ifdef __cplusplus
}
#endif



#endif
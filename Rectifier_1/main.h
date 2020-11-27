/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "type_def.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

 enum et_converter_mode
{
	
	eAPWM_HBI_FBR,
	eBFBR_HBI_FBR,
    eBFBR_HBI_PSM_FBI,
	ePSM_FBI_FBR ,
    eBFBR_FBI_FBR,
    eBFBR_FBR_PSM_VDR,
	ePSM_FBI_VDR,
    eSwBVDR_FBI_VDR

} ;


//state machine states enumerator
 enum states
{
    Standby,
    Start,
    GMPPT,
    LMPPT,
    Power_droop,
    Transition_operation_mode,
    Stop_Reset

};

//state machine control signal states enumerator i.e. state vectors
 enum status
{
    FAULT, //High input current 
    READY,//waiting for normal operation input  and outpup volatge 10<Vin<60, 320<Vout<380
    RUN,   //	GMPPT, 	LMPPT, Power_limitation
    FAULT_V_in,
    FAULT_V_out
};





/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/


/* USER CODE BEGIN EFP */

// definition declaration
extern float t_d; // dead time, us
extern unsigned int T_d; //T_d=T_hrtm*t_d*f_s  where t_d=10 ns, f_s=95 kHz

extern float Da; //0.3;
extern float delta_Da;
extern float Max_Da;
extern float shifted_Da;
extern float Min_Da;

extern float Da_temp_1; // 
extern float Da_temp_2; // 
extern float Da_sqrt; //
extern float Da_atan; //



extern et_converter_mode eConverterMode;

extern float V_in;
extern float I_in;
extern float V_out;
extern float I_out;

extern float V_in_f;
extern float I_in_f;
extern float V_out_f;
extern float I_out_f;

extern float V_in_old;

/// GMPPT
extern float P_in;
extern float P_out;
extern float P_out_old;
extern float GMPPs_P_out[10];
extern float GMPPs_V_in[10];
/// 


extern float P_lim;

extern float V_in_ref;

extern float global_temp;

extern float SQRT;

extern u32 ADC1ConvertedValues[2];
extern u32 ADC2ConvertedValues[3];

extern u32 I_in_offset;
extern u32 I_out_offset;

extern stPI_Params V_in_reg[8];
extern u16 counter_2;
extern u16 counter_Da_steps;

extern int charging_duty_cycle_step;
extern float charging_Da_buck;

extern states machine_state;
extern states prev_machine_state;
extern status machine_status;

extern transition_callback transition_handler;
extern transition FSM_table[7];


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */



#define f_HRCK		(float)(576e6)	//HRTIM input clock taken from CubeMx, Hz
#define f_HRCK_M	(float)(576)	//HRTIM input clock taken from CubeMx, MHz
#define f_sw		(float)(100e3)	//switching frequency of transistors, Hz
#define f_sw_M		(float)(f_sw/1000000)	//switching frequency of transistors, MHz
#define T_hrtm 		(uint32_t)(f_HRCK/f_sw) //Period of HRTIM
#define half_Thrtm ((uint32_t)(T_hrtm)>>1) // Half of T_hrtm

#define MinCmpVal 	(uint32_t)(50)				//hardware minimal compare value for the HRTIM frequency=567 Mhz



#define K_Min_Da      (float)((f_HRCK_M*f_sw)/f_HRCK)

#define f_interupt	(uint32_t)(100000)	//Hz

#define U_in_adc_scale	(float)(0.0209) 	//
#define I_in_adc_scale	(float)(0.00337)		//1/296
#define V_out_adc_scale	(float)(0.209)	//615/100
#define I_out_adc_scale	(float)(0.001)		//

#define f_filter_cutoff_V_in	(uint32_t)(10000)	//Hz
#define f_filter_cutoff_V_out	(uint32_t)(1000)		//Hz
#define f_filter_cutoff_I_in	(uint32_t)(1000)		//Hz
#define f_filter_cutoff_I_out	(uint32_t)(1000)		//Hz

#define Cr_fs	(float)(2.34e-3) /// Cr*fs 
#define wr_Ts	(float)(0.15) /// 1/(wr*Ts) 


#define n	12 /// turns ratio of tranformer

//Charging of series capasitors
#define Charging_N_C2	(int)(19) // amout of time period for carging C2
#define Charging_N_C3	(int)(6) //amout of time period for carging C3
#define un_charging_duty_cycle_step_C2	(int)(half_Thrtm/Charging_N_C2) //
#define un_charging_duty_cycle_step_C3	(int)(half_Thrtm/Charging_N_C3) //

//Power droop control
#define Max_converter_power	(float)(300) //W
#define Min_V_out	        (float)(320) //V
#define Min_Normal_V_out	(float)(325) //V
#define Normal_V_out	    (float)(350) //V
#define Max_V_out	        (float)(380) //V
#define Raising_power_K      (float)(Max_converter_power/(Min_Normal_V_out-Min_V_out)) //W/V
#define Falling_power_K      (float)(Max_converter_power/(Max_V_out-Normal_V_out)) //W/V

#define Max_V_in	(float)(75) //W
#define Min_V_in    (float)(10) //V
#define Min_V_in_fault    (float)(8) //V

//fnc Start
#define T_start	(u16)(1+(f_interupt*5)/(2*3.141592*f_filter_cutoff_V_out)) //

//fnc GMPPT
#define V_ref_freq	(float)(100) //Hz
#define V_ref_step	(float)((V_ref_freq*2*3.151592)/(f_interupt)) //


//fnc fnc_transition_opeation_mode
#define T_transition    (u16)(Charging_N_C2*4) //

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

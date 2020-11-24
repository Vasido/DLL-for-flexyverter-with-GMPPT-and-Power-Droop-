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

typedef enum
{
	
	eAPWM_HBI_FBR       =   0,
	eBFBR_HBI_FBR       =   1,
    eBFBR_HBI_PSM_FBI   =   2,
	ePSM_FBI_FBR        =   3,
    eBFBR_FBI_FBR       =   4,
    eBFBR_FBR_PSM_VDR   =   5,
	ePSM_FBI_VDR        =   6,
    eSwBVDR_FBI_VDR     =   7,

} et_converter_mode;
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

extern float U_in;
extern float I_in;
extern float U_out;
extern float I_out;

extern float U_in_f;
extern float I_in_f;
extern float U_out_f;
extern float I_out_f;

extern float P_in;
extern float P_out;
extern float P_los;
extern float P_los_old;

extern float U_in_ref;

extern stPI_Params U_in_reg[8];


extern u32 ADC1ConvertedValues[2];
extern u32 ADC2ConvertedValues[3];

extern u32 I_in_offset;
extern u32 I_out_offset;

extern u16 counter_2;
extern u16 counter_Da_steps;




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

#define U_in_adc_scale	(float)(0.0209) 	//
#define I_in_adc_scale	(float)(0.00337)		//1/296
#define U_out_adc_scale	(float)(0.209)	//615/100
#define I_out_adc_scale	(float)(0.001)		//



#define Cr_fs	(float)(2.34e-3) /// Cr*fs 
#define wr_Ts	(float)(0.15) /// 1/(wr*Ts) 


#define n	12 /// turns ratio of tranformer

//Charging of series capasitors
#define Charging_N_C2	(int)(18) //
#define Charging_N_C3	(int)(6) //
#define un_charging_duty_cycle_step_C2	(int)(half_Thrtm/Charging_N_C2) //
#define un_charging_duty_cycle_step_C3	(int)(half_Thrtm/Charging_N_C3) //



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

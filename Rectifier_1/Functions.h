#ifndef FUNCTIONS_H
#define FUNCTIONS_H


void TIM2_IRQHandler(double*);
float Filter(float* , F_params*);
float PI(float*, float*, stPI_Params*);
void set_cmp_hrtm();



//transitions handlers
void fnc_standby(void); //stand by function and waiting normal input and outpu voltage
void fnc_start(void); //stand by function and waiting normal input and outpu voltage
void fnc_gmppt(void);
void fnc_lmppt(void);
void fnc_power_droop(void);
void fnc_transition_opeation_mode(void);
void fnc_stop_reset(void);



#endif
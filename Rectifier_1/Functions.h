#ifndef FUNCTIONS_H
#define FUNCTIONS_H


void TIM2_IRQHandler(double*);
float Filter(float* , F_params*);
float PI(float*, float*, stPI_Params*);
void set_cmp_hrtm(double*);
#endif
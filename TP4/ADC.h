#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>


void ADC_Init();
void ADC_StartConvertion();

void ADC_Polling_Enable();
void ADC_Polling_Disable();



#endif
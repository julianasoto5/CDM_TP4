#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_Init();
void ADC_StartConvertion();

void ADC_Interrupt_Enable();
void ADC_Interrupt_Disable();



#endif
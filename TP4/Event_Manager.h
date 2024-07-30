#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_
#include <avr/io.h>
#include <stdio.h>

#include "PWM_RGB/PWM_RGB.h"
#include "UART/UART.h"
#include "ADC/ADC.h"


void EVENT_MANAGER_ShowWelcome();
void EVENT_MANAGER_Update();
	
#endif
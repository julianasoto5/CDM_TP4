/*
 * TP4.c
 *
 * Created: 7/15/2024 4:25:55 PM
 * Author : julis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM.h"
#include "ADC.h"
#include "lib_delay.h"
#include "Event_Manager.h"
uint8_t i = 0;
int main(void){
	cli();
	UART_Init(0X67);
	PWM_Init();
	PWM_START;
	ADC_Init();
	sei();

	EVENT_MANAGER_ShowWelcome();
	while(1){
		EVENT_MANAGER_Update();
	}
}
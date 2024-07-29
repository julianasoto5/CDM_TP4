#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM_RGB/PWM_RGB.h"
#include "ADC/ADC.h"
#include "lib_delay.h"
#include "Event_Manager.h"

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
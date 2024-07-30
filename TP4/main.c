#include "main.h"

int main(void){
	PWM_START;
	
	cli();
	
	UART_Init(0X67);
	ADC_Init();
	PWM_Init();
	
	sei();

	EVENT_MANAGER_ShowWelcome();
	
	while(1){
		EVENT_MANAGER_Update();
	}
}
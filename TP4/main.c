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
	DDRD = (1<<PORTD6);
	ADC_Init();
	sei();

	EVENT_MANAGER_ShowMsg(BIENVENIDA);
	EVENT_MANAGER_ShowMsg(MENU);
	while(1){
		
	}
}/*









int main(void)
{	cli();
	
	UART_Init(0X67);
	
	
	PWM_START;
	//OC0A esta en PORTD6, no PORTB5
	DDRD |= (1<<PORTD6);
	PWM_Init();
	sei();
	
	EVENT_MANAGER_ShowWelcome();
	EVENT_MANAGER_ShowMenu();
	
	ADC_Init();
	int i;
	while(1){i++; }
   }


*/
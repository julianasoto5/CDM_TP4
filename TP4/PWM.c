#include "PWM.h"
#define PORT_RED PORTB5
#define PORT_GREEN PORTB2
#define PORT_BLUE PORTB1

#define PWM_PERIOD 20
#define PWM_DELTA 5


#define PWM_OFF_RED PORTB &=~(1<<PORT_RED)
#define PWM_OFF_GREEN PORTB &=~(1<<PORT_GREEN)
#define PWM_OFF_BLUE PORTB &=~(1<<PORT_BLUE)

#define PWM_ON_RED PORTB |= (1<<PORT_RED)
#define PWM_ON_GREEN PORTB |=(1<<PORT_GREEN)
#define PWM_ON_BLUE PORTB |= (1<<PORT_BLUE)

#define PWM_START DDRB |= ((1<<PORT_RED) | (1<<PORT_GREEN) | (1<<PORT_BLUE))

#define T 0.02
#define PREESCALER 256
#define GET_OCR1B ((F_CPU*T/PREESCALER)-1)  //Formula para el OCR1A


volatile static int cont = 19;

//50Hz --> T = 0.02s --> PWM_Update cada 1ms
void PWM_Init(){
	//Modo 5 Fast PWM, 8-bit
	TCCR1A = (1<<WGM10); // 0x02	
	TCCR1B = ((1<<WGM12)|(1<<CS12)); // Modo5 - Preescaler 256 //0x0C
	OCR1B = GET_OCR1B;
	
	TIMSK1 = (1 << OCIE1B); 
	
	//TIMER0 CTC --> T=0.001 --> PREES = 64 --> OCR0A = 249
	TCCR0A = (1<<WGM01);
	TCCR0B = ((1<<CS01) | (1<<CS00));
	OCR0A = 249;
	
	TIMSK0 = (1<<OCIE0A);
	
	
	
}

void PWM_Update(){
	static uint16_t PWM_position = 0;
	if (++PWM_position >= PWM_PERIOD){
		PWM_position = 0;
	}
	if (PWM_position < PWM_DELTA){
		PWM_ON_BLUE;
		PWM_ON_GREEN;
		PWM_ON_RED;
	} else {
		PWM_OFF_BLUE; 
		PWM_OFF_GREEN;
		PWM_OFF_RED;
	}
}

ISR (TIMER0_COMPA_vect){
	PWM_Update();
}


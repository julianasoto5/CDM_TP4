#include "TIMER.h"
#define T 2
#define PREESCALER 1024
//Al final si se puede hacer de 2 segundos jaja -> T = 2 y PREESCALER = 1024 --> OCR1A = 31249 que si entra en 16 bits.

#define GET_OCR1A ((F_CPU*T/PREESCALER)-1)  //Formula para el OCR1A

void TIMER_Init() {		  //Configuracion del Timer de 2s
	TCCR1B |= (1 << WGM12);               // Configura el modo 3 Fast PWM (Pulse Width Modulation)
	TCCR1B |= ((1 << CS12) | (1 << CS10));// Configura el preescalador a 1024
	OCR1A = GET_OCR1A;                    // Establece el valor de comparación
	TIMER_Enable();
}

void TIMER_Enable(){
	TIMSK1 |= (1 << OCIE1A);            // Habilita la interrupción de comparación A	
	TCNT1 = 0;							//Reiniciar contador
}

void TIMER_Disable(){
	TIMSK1 &= ~(1 << OCIE1A);              // Deshabilita la interrupción de comparación A
}

#include "PWM.h"
#include "UART/serialPort.h"
/*

1ra conclusion: Me voy a hacer la boluda y usar el PORTD6 en vez del
PORTB5 asi ya tengo la señal generada y no tengo que hacer una 
conversion rara (que dsps hay que hacerla pero primero que funcione).

2da conclusion: No puede tener frecuencia 50Hz con resolucion
de 8 bits y con un clock de 16MHz porque el preescalador necesario 
seria 1250. Con el mas cercano, N = 1024, la frecuencia generada
es de 61.03Hz (15625/256 Hz).

*/



/*Necesitamos generar 3 señales, pero los timer solo tienen 2 unidades de 
  comparación de salida cada uno. Hay que usar 2 timers. Usamos las dos
  unidades del timer 1 y una unidad del timer 0.
  
  Configuracion del TIMER0:
  - Modo de operación del Timer 0: Fast PWM, modo 3 --> 8bits de resolucion 
  - Modo de Comparacion de salida: non-inverting mode --> hay que activar las 2 unidades
  - Preescalador N = 1024 --> frecuencia de 61.03Hz aproximadamente (15625/256)
  - Habilitacion de interrupcion. ANALIZAR PORQUE NO SE QUE HABILITAR 
  
  Configuracion de TIMER1:
  -  Modo de operacion: Fast PWM, modo 5 --> 8bits de resolucion
  -  Modo de Comparacion de salida: nnon-inverting mode --> solo unidad A
  -  Preescalador N = 1024 --> frecuencia de 61.03Hz aproximadamente
  -	 Habilitacion de interrupcion
*/
void Change_Blue();
void Change_Green();
void Change_Red();


static uint8_t colors_RGB[3];


void PWM_Init(){
	
	//Inicialización TIMER0
	TCCR0A = ((1<<WGM01) | (1<<WGM00)); //Modo 3
	TCCR0A |= ((1<<COM0A1) | (1<<COM0A0)); //No invertido
	TCCR0B = ((1<<CS02) | (1<<CS00)); //Preescalador
	//TIMSK0 = (1<<TOIE0);
	
	//Inicialización TIMER1
	TCCR1A = (1<<WGM10); //Modo 5
	TCCR1B = (1<<WGM12);
	TCCR1A |= ((1<<COM1A1) | (1<<COM1B1) | (1<<COM1A0) | (1<<COM1B0));//No invertido
	TCCR1B |= ((1<<CS12) | (1<<CS10));
	//TIMSK1 = (1<<TOIE1);
	
	OCR0A = 255;
	OCR1B = 255;
	OCR1A = 255;
	
	//reinicio contadores porque me pinta :D
	TCNT0 = 0;
	TCNT1 = 0;
}

void PWM_Change_DC_RGB(rgb color, int16_t new_value){
	colors_RGB[color] = new_value/1024;
	SerialPort_Send_uint8_t(colors_RGB[color]);
	switch (color){
		case RED: Change_Red(); break;
		case GREEN: Change_Green(); break;
		case BLUE: Change_Blue(); break;
		case INIT: break;
	}
}


void Change_Red(){ //GUARDA ACA Q TOCA EL ROJO --> PD6=>PB5
	OCR0A = colors_RGB[RED];
}

void Change_Blue(){ 
	OCR1A = colors_RGB[BLUE];
}

void Change_Green(){ 
	OCR1B = colors_RGB[GREEN];
}

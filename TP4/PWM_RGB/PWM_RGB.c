#include "PWM_RGB.h"
/*
1ra conclusion: No puede tener frecuencia de 50Hz con resolucion
de 8 bits y con un clock de 16MHz porque el preescalador necesario 
seria 1250. Con el mas cercano, N = 1024, la frecuencia generada
es de 61.03Hz (15625/256 Hz).

2da Conclusion: La señal PWM del PB5 será generada por software
porque no tiene un OCnx en su terminal (como si lo tienen el PB1
y el PB2, con las salidas OC1A y OC1B respectivamente)
*/



/*Necesitamos generar 3 señales, pero los timer solo tienen 2 unidades de 
  comparación de salida cada uno. Hay que usar 2 timers. Usamos las dos
  unidades del timer 1 y una unidad del timer 0.
  
  Configuracion del TIMER0:
  - Modo de operación del Timer 0: Fast PWM, modo 3 --> 8bits de resolucion 
  - Modo de Comparacion de salida: inverting mode -->  solo unidad A
  - Preescalador N = 1024 --> frecuencia de 61.03Hz aproximadamente (15625/256)
  - Habilitacion de interrupcion
  
  Configuracion de TIMER1:
  -  Modo de operacion: Fast PWM, modo 5 --> 8bits de resolucion
  -  Modo de Comparacion de salida: inverting mode --> ambas unidades
  -  Preescalador N = 1024 --> frecuencia de 61.03Hz aproximadamente
 
*/
void Update_Blue(void);
void Update_Green(void);
void Update_Red(void);

void (*Update[])(void) = {Update_Red, Update_Green, Update_Blue};



static uint8_t colors_RGB[3];



void PWM_Init(){
	
	//Inicialización TIMER0
	TCCR0A = ((1<<WGM01) | (1<<WGM00)); //Modo 3
	TCCR0A |= ((1<<COM0A1) | (1<<COM0A0)); //Invertido
	TCCR0B = ((1<<CS02) | (1<<CS00)); //Preescalador
	TIMSK0 = (1<<TOIE0) | (1<<OCIE0A);
	
	//Inicialización TIMER1
	TCCR1A = (1<<WGM10); //Modo 5
	TCCR1B = (1<<WGM12);
	TCCR1A |= ((1<<COM1A1) | (1<<COM1B1) | (1<<COM1A0) | (1<<COM1B0));//Invertido
	TCCR1B |= ((1<<CS12) | (1<<CS10));
	
	OCR0A = 0;
	OCR1B = 0;
	OCR1A = 0;
	
	//reinicio contadores porque me pinta :D
	TCNT0 = 0;
	TCNT1 = 0;
}

void PWM_Change_DC_RGB(rgb color, int8_t new_value){
	colors_RGB[color] = new_value;
	(*Update[color])();
}


void Update_Red(){
	OCR0A = colors_RGB[RED];
	if(OCR0A == 255) TIMSK0 &= ~(1<<OCIE0A);
	else if (!OCR0A){
		TIMSK0 &= ~(1<<TOIE0);
	} else TIMSK0 |= (1<<TOIE0) | (1<<OCIE0A);
	
}

void Update_Blue(){ 
	OCR1A = colors_RGB[BLUE];
}

void Update_Green(){ 
	OCR1B = colors_RGB[GREEN];
}

uint8_t* PWM_GetRGB(){
	return colors_RGB;	
}



#include <avr/io.h>
void ADC_Init(){
	//DDRC &= ~(1<<channel); //INPUT
	
	ADMUX = (1<<REFS0); //Vref = VCC
	ADMUX |= (1<<ADLAR); //Left justified 
	ADMUX |= 0x03; //ADC3
	
	ADCSRA = (1<<ADEN); //Habilitacion del ADC
	
	ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //Preescalador N = 128

	DIDR0 = (1<<ADC3D); //para reducir el consumo de energia en el buffer
		
}

void ADC_StartConvertion(){
	ADCSRA |= (1<<ADSC); //activo conversion. Cuando termine se va a bajar el nivel automaticamente, lo que desencadena la interrupcion
}

void ADC_Polling_Enable(){
	ADCSRA |= (1<<ADIE); //Habilitacion de interrupcion de conversion completa
}

void ADC_Polling_Disable(){
	ADCSRA &= ~(1<<ADIE); //Deshabilitacion de interrupcion de conversion completa
}









#include <avr/io.h>
void ADC_Init(){
	
	ADMUX = (1<<REFS0); //Vref = VCC
	ADMUX |= (1<<ADLAR); //Left justified 
	ADMUX |= ((1<<MUX1)|(1<<MUX0)); //ADC3
	
	ADCSRA = (1<<ADEN); //Habilitacion del ADC
	ADCSRA |= (1<<ADIE); //Habilitacion de interrupcion de conversion completa
	ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //Preescalador N = 128

	DIDR0 = (1<<ADC3D); //para reducir el consumo de energia en el buffer
		
}

void ADC_StartConvertion(){
	ADCSRA |= (1<<ADSC); //activo conversion. Cuando termine se va a bajar el nivel automaticamente, lo que desencadena la interrupcion
}









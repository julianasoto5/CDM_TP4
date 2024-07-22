
#include "UART.h"

static volatile char BUFFER_TX [BUFFER_SIZE];
static volatile uint8_t index = 0;
static volatile uint8_t TX_enabled = 0; //desactivado

void UART_Init(uint16_t ubrr_value) {
	
	// Configuración de baud rate
	UBRR0H = (uint8_t)(ubrr_value >> 8); // Parte alta del UBRR
	
	UBRR0L = (uint8_t)ubrr_value;   
	     
	// Configuración: 8 bits de datos, sin paridad, 1 bit de parada (stop bit)
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	
	SerialPort_RX_Enable();		// Activo el Receptor del Puerto Serie
	SerialPort_TX_Enable();		// Activo el Transmisor del Puerto Serie
	TX_enabled=1;
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion
	// Habilitar transmisión y recepción
	//UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	BUFFER_TX[0] = '/0';
}

void UART_TX_Disable(){
	UCSR0B &=~(1<<TXEN0);
	TX_enabled = 0;
}

void UART_TX_Enable(){
	SerialPort_TX_Enable();
	TX_enabled = 1;
}

void UART_transmit_string(char* str) {
	uint8_t i=0;
	BUFFER_TX[i] = str[i++];
	SerialPort_TX_Interrupt_Enable();
	while(str[i] && i < BUFFER_SIZE){
		BUFFER_TX[i] = str[i];
		i++;
	}
	 BUFFER_TX[i] = '\0';
}

uint8_t UART_Transmition_Status(){
	return TX_enabled;
}

char UART_Get_Char_From_Buffer(){
	return BUFFER_TX[index++];
}

void UART_Reset_Index(){
	index = 0;
}
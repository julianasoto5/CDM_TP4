
#include "UART.h"


static volatile char BUFFER_TX [BUFFER_SIZE];
static volatile uint8_t index_w = 0;
static volatile uint8_t index_r = 0;
static volatile uint8_t TX_enabled = 0; //desactivado
static volatile uint8_t RX_enabled = 0;
static volatile uint8_t cantL = 0;
static uint8_t current_tx_status = 0; //capaz tdv lo necesito porque en send_string tarda un poquito en cargarse el BUFFER_TX


void UART_Init(uint16_t ubrr_value) {
	
	// Configuración de baud rate
	UBRR0H = (uint8_t)(ubrr_value >> 8); // Parte alta del UBRR
	
	UBRR0L = (uint8_t)ubrr_value;
	
	// Configuración: 8 bits de datos, sin paridad, 1 bit de parada (stop bit)
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	
	UART_RX_Enable();		// Activo el Receptor del Puerto Serie
	UART_TX_Enable();		// Activo el Transmisor del Puerto Serie

	BUFFER_TX[0] = '\0';
}


void UART_TX_Disable(){ 
	UCSR0B &=~(1<<TXEN0);
	TX_enabled = 0;
}

void UART_TX_Enable(){
	UCSR0B |= (1<<TXEN0);
	TX_enabled = 1;
}

void UART_TX_Interrupt_Enable(){
	UCSR0B |= (1<<UDRIE0);
}
void UART_TX_Interrupt_Disable(void){
	UCSR0B &= ~(1<<UDRIE0);
}

void UART_RX_Enable(){
	UCSR0B |= (1<<RXEN0);
	RX_enabled = 1;
}

void UART_RX_Disable(){
	UCSR0B &= ~(1<<RXEN0);
	RX_enabled = 0;
}

void UART_RX_Interrupt_Enable(){
	UCSR0B |= (1<<RXCIE0);
}

void UART_RX_Interrupt_Disable(){ 
	UCSR0B &= ~(1<<RXCIE0);
}


uint8_t UART_TX_Status(){
	return TX_enabled;
}

uint8_t UART_Reception_Status(){  
	return RX_enabled;
}
	
uint8_t UART_Write_Char(char c){
	if (index_w<BUFFER_SIZE){
		BUFFER_TX[index_w++]=c;
		return 1;
	}return 0;
}

void UART_Send_String(char* str) { //solo llena el buffer
	uint8_t i=0;
	
	while (BUFFER_TX[0]); //espera a q este disponible el buffer
	current_tx_status = 1;
	cantL = strlen(str);
	UART_TX_Interrupt_Enable();
	
	while (str[i]){
		if (UART_Write_Char(str[i])){
			i++;
		}
	}
	BUFFER_TX[i] = '\0';
	
}



void UART_Send_Data(char data){
	UDR0 = data;
}

char UART_Receive_Data(){
	return UDR0;
}

char UART_Get_Char_From_Buffer(){
	if ((index_r<index_w) && (BUFFER_TX[index_r]))
		return BUFFER_TX[index_r++];
	else return 0;
}

void UART_Reset_TX(){
	index_r = 0;
	index_w = 0;
	BUFFER_TX[0]='\0';
	cantL=0;
	current_tx_status = 0;
	UART_TX_Interrupt_Disable();
	
}

uint8_t UART_Current_TX_Status(){ //devuelve True si sigue transmitiendo
	return (current_tx_status) && (index_r != cantL); //se quiere transmitir algo y/o se esta transmitiendo
}


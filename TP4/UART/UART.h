#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "../lib_delay.h"

#define BUFFER_SIZE 300


void UART_Init(uint16_t ubrr_value);
void UART_TX_Disable(void);
void UART_TX_Enable(void);
void UART_TX_Interrupt_Enable(void);
void UART_TX_Interrupt_Disable(void);
void UART_RX_Enable(void);
void UART_RX_Disable(void);
void UART_RX_Interrupt_Enable(void);
void UART_RX_Interrupt_Disable(void);

uint8_t UART_Transmition_Status();
uint8_t UART_Reception_Status();
uint8_t UART_Current_TX_Status();


void UART_Send_String(char*);
void UART_Send_Data(char);

char UART_Receive_Data();
char UART_Get_Char_From_Buffer();
void UART_Reset_TX();


#endif
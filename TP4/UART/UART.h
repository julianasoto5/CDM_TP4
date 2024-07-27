#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "serialPort.h"
#define BUFFER_SIZE 300


void UART_Init(uint16_t ubrr_value);
void UART_TX_Disable(void);
void UART_TX_Enable(void);
void UART_Transmit_String(char* str);
uint8_t UART_Transmition_Status();
char UART_Get_Char_From_Buffer();
void UART_Reset_Index();

#endif
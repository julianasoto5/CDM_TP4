#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_
#include <avr/io.h>
#include "PWM.h"
#include "UART/UART.h"
#include "ADC.h"
typedef enum{BIENVENIDA, MENU} public_msg_type;


void EVENT_MANAGER_ShowMsg(public_msg_type);
	
#endif
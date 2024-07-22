/*
 * TP4.c
 *
 * Created: 7/15/2024 4:25:55 PM
 * Author : julis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM.h"

int main(void)
{
	cli();
	PWM_Init();
	sei();
    while (1) 
    {
    }
}



//Event Manager
#include "Event_Manager.h"
#include <util/delay.h>
#define ENTER '\r'
#define CONVERT_NUM_CHAR_TO_INT(x) x-'0'


char* public_msg[2] = {"\r\nBienvenidx al Controlador del LED RGB",
"\r\nSeleccione una letra para modificar un color:\r\nR (Red/Rojo)\r\nG (Green/Verde)\r\nB (Blue/Azul)\r\n"};



typedef enum{BRILLO, EXITO} private_msg_type;
char* private_msg[2] = {"\r\nPara modificar el brillo del color seleccionado, modifique el potenciometro y luego presione cualquier tecla: ",
						"\r\nModificacion realizada con exito.\r\n"};

typedef enum {INVALID, OUT_OF_RANGE} error_type;
char* error_msg[2] = {"El dato ingresado es invalido. Intentelo de nuevo.\n", "El numero ingresado esta fuera de rango. Intentelo de nuevo.\n"};
	

volatile char data;
volatile char c;
volatile int8_t num = 0;

uint8_t select_color = 1;
rgb current_color=INIT;
void EVENT_MANAGER_ShowMsg(public_msg_type msg){
	//UART_Transmit_String(BIENVENIDA);
	SerialPort_Send_String(public_msg[msg]);
}

void showMsg(private_msg_type msg){
	SerialPort_Send_String(private_msg[msg]);
}

void showError(error_type error){
	//UART_Transmit_String(error_msg[error]);
	SerialPort_Send_String(error_msg[error]);
}

void Reception_Detected(){ //tres opciones. Esta esperando un RGB, un numero o un S/N
	
	c = UDR0;
	SerialPort_Send_Data(c);
		if (select_color){ //esta esperando un RGB
			switch (c){
				case 'R': current_color = RED; select_color = 0; showMsg(BRILLO); break;
				case 'G': current_color = GREEN; select_color = 0; showMsg(BRILLO); break;
				case 'B': current_color = BLUE; select_color = 0; showMsg(BRILLO); break;
				default: current_color = INIT;
						 showError(INVALID);
						 _delay_ms(10);
						 EVENT_MANAGER_ShowMsg(MENU);
			}
		
		}else {//esta esperando una tecla para el uso del potenciometro
			ADC_StartConvertion();
			
			//Probar con la interrupcion cuando todo funcione
			while((ADCSRA&(1<<ADIF))==0); //wait for end of conversion
			ADCSRA |= (1<<ADIF); //clear the ADIF flag
			//SerialPort_send_int16_t(ADC);		
			PWM_Change_DC_RGB(current_color,2*ADCH);
			select_color = 1;
			showMsg(EXITO);
			EVENT_MANAGER_ShowMsg(MENU);
		}
			
}



void Transmition_Allowed(){
	
	//UART_Reset_Index(); //static uint8_t i=0;
	data = UART_Get_Char_From_Buffer();
	if (data){
		SerialPort_Send_Data(data); _delay_ms(1);
	}
	else{
		//Desactivo las interrupciones de transmision
		UART_Reset_Index();
		SerialPort_TX_Interrupt_Disable();
	}
}


ISR (ADC_vect){ //ADC Convertion Complete
	
	PWM_Change_DC_RGB(current_color,ADCH);
	_delay_ms(50);
	select_color = 1;
	showMsg(EXITO);
	EVENT_MANAGER_ShowMsg(MENU);
	
}



ISR(USART_RX_vect){ //se detecto una recepcion
	Reception_Detected();
}
/*

ISR(USART_UDRE_vect){
	//Flag_Transmition_Allowed = 1;
	Transmition_Allowed();
}*/

/*ISR(TIMER0_OVF_vect){
	
}

ISR(TIMER1_OVF_vect){
	
}*/

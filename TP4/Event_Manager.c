
//Event Manager
#include "Event_Manager.h"
#include <util/delay.h>
#define ENTER '\r'
#define CONVERT_CHAR_TO_INT(x) x-'0'

char* BIENVENIDA = "Bienvenidx al Controlador del LED RGB";
char* MENU = "\r\nSeleccione una letra para modificar un color:\r\nR (Red/Rojo)\r\nG (Green/Verde)\r\nB (Blue/Azul)\r\n";
char* INGRESO_NUM = "\r\nIngrese un numero entre 0 y 255: ";
char* BRILLO = "\r\nSi desea modificar el brillo del color seleccionado, modifique el potenciometro y luego presione la tecla 'B', caso contrario presione cualquier otra tecla: ";
char* EXITO = "\r\nModificacion realizada con exito.\r\n";

typedef enum {INVALID, OUT_OF_RANGE} error_type;
char* error_msg[2] = {"El dato ingresado es invalido. Intentelo de nuevo.\n", "El numero ingresado esta fuera de rango. Intentelo de nuevo.\n"};
	

volatile char data;
volatile char c;
volatile int8_t num = 0;

uint8_t new_color_value;
uint8_t change_color = 0, select_color = 1;
rgb current_color=INIT;
void EVENT_MANAGER_ShowWelcome(){
	//UART_Transmit_String(BIENVENIDA);
	SerialPort_Send_String(BIENVENIDA);
}

void EVENT_MANAGER_ShowMenu(){
	SerialPort_Send_String(MENU);
	//UART_Transmit_String(MENU);
}

void showNumRange(){
	//UART_Transmit_String(INGRESO_NUM);
	SerialPort_Send_String(INGRESO_NUM);
}

void showSuccesMsg(){
	SerialPort_Send_String(EXITO);
}
void showBrightnessQuestion(){
	SerialPort_Send_String(BRILLO);
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
				case 'R': current_color = RED; select_color = 0; showNumRange(); change_color = 1; break;
				case 'G': current_color = GREEN; select_color = 0; showNumRange(); change_color = 1; break;
				case 'B': current_color = BLUE; select_color = 0; showNumRange(); change_color = 1; break;
				default: current_color = INIT;
						 showError(INVALID);
						 _delay_ms(10);
						 EVENT_MANAGER_ShowMenu();
			}
		
		}
		else if (change_color){ //esta esperando un numero
			static unsigned int cantNum = 0;
				if(c !='#'){ //escribio un numero
					num =num*10+CONVERT_CHAR_TO_INT(c);	
					cantNum++;
				}else{ 
					cantNum=0;
					if ((num >= 0) | (num<256)){
						change_color = 0;
						new_color_value = num;
						showBrightnessQuestion();
					}
					else {
						showError(OUT_OF_RANGE);
						showNumRange();
					}
					num=0;
					
				}
			}else {//esta esperando una B para el uso del potenciometro
				if (c == 'B'){ //quiere modificar el brillo, manda señal de inicio de conversion
					ADC_StartConvertion();
					while((ADCSRA&(1<<ADIF))==0); //wait for end of conversion
					ADCSRA |= (1<<ADIF); //clear the ADIF flag
						
						PWM_Change_DC_RGB_Brightness(current_color,new_color_value,ADCH);
						_delay_ms(100);
						select_color = 1;
						EVENT_MANAGER_ShowMenu();
					
				}else { //no quiere modificar el brillo
					PWM_Change_DC_RGB(current_color,new_color_value);
					select_color = 1;
					showSuccesMsg();
					EVENT_MANAGER_ShowMenu();
				}
		
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
	
	PWM_Change_DC_RGB_Brightness(current_color,new_color_value,ADCH);
	_delay_ms(500);
	select_color = 1;
	showSuccesMsg();
	EVENT_MANAGER_ShowMenu();
	
}



ISR(USART_RX_vect){ //se detecto una recepcion
	Reception_Detected();
}
/*

ISR(USART_UDRE_vect){
	//Flag_Transmition_Allowed = 1;
	Transmition_Allowed();
}*/

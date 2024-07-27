
//Event Manager
#include "Event_Manager.h"
#include <util/delay.h>
#include <stdio.h>
#define ENTER '\r'
#define CONVERT_NUM_CHAR_TO_INT(x) x-'0'

volatile unsigned char Flag_OVF = 0;
volatile unsigned char Flag_COMPA = 0;
volatile unsigned char Flag_Next = 0;
volatile unsigned char Flag_RX = 0;
volatile unsigned char Flag_TX = 0;
volatile unsigned char Flag_ADC = 0;
void End_Convertion_ADC();


char* BIENVENIDA = {"\r\nBienvenidx al Controlador del LED RGB"};

typedef enum{MENU, BRILLO, EXITO} private_msg_type;
char private_msg[3][255] = {"\r\nSeleccione una letra para modificar un color:\r\nR (Red/Rojo)\r\nG (Green/Verde)\r\nB (Blue/Azul)\r\n",
						"\r\nPara modificar el brillo del color seleccionado, modifique el potenciometro y luego presione cualquier tecla: ",
						"\r\nModificacion realizada con exito."};

typedef enum {INVALID} error_type;
char* error_msg = {"\r\nEl dato ingresado es invalido. Intentelo de nuevo."};
const char* color_code = "\r\nEl color mostrado tiene el codigo RGB #%02X%02X%02X";	

volatile char data;
volatile char c;


volatile uint8_t select_color = 1;
rgb current_color=INIT;
void EVENT_MANAGER_ShowWelcome(){
	UART_Send_String(BIENVENIDA);
	UART_Send_String(private_msg[MENU]);
	UART_RX_Interrupt_Enable(); //habilito recepciones
}

void showMsg(private_msg_type msg){
	UART_Send_String(private_msg[msg]);
}

void showError(error_type error){
	UART_Send_String(error_msg);
}

void Reception_Detected(){ //tres opciones. Esta esperando un RGB, un numero o un S/N
	
	c = UART_Receive_Data();
	UART_Send_Data(c);
	UART_RX_Interrupt_Disable();
		if (select_color){ //esta esperando un RGB
			switch (c){
				case 'R': current_color = RED; select_color = 0; showMsg(BRILLO); break;
				case 'G': current_color = GREEN; select_color = 0; showMsg(BRILLO); break;
				case 'B': current_color = BLUE; select_color = 0; showMsg(BRILLO); break;
				default: current_color = INIT;
						 showError(INVALID);
						 Flag_Next = 1;
			}
			UART_RX_Interrupt_Enable();
		}else {//esta esperando una tecla para el uso del potenciometro
			ADC_StartConvertion();
			//ADC = 0
			//Next = 0
			//RX = 0
			//TX = 0
			//UART_RX_Disable();
		}
		
			
}



void Transmition_Allowed(){
	if (!UART_Current_TX_Status()){
			//Desactivo las interrupciones de transmision
			UART_Reset_Index();
			UART_TX_Interrupt_Disable();
	}else{

	data = UART_Get_Char_From_Buffer();
	if (data){
		UART_Send_Data(data); 
	}
	}
}

void End_Convertion_ADC(){
	PWM_Change_DC_RGB(current_color,ADCH);
	UART_RX_Interrupt_Disable();
	showMsg(EXITO);
	
/*	me lo ignora el forro jajajajja
	uint8_t* color = PWM_GetRGB();
	char* aux = "";
	scanf(aux,color_code,color[RED],color[GREEN],color[BLUE]);
	UART_Send_String(aux);
*/
	select_color = 1;		
	Flag_Next = 1;
}

void EVENT_MANAGER_Update(){
	
	if (Flag_COMPA){
		PWM_OFF_RED;
		Flag_COMPA = 0;
	}
	if (Flag_OVF){
		PWM_ON_RED;
		Flag_OVF = 0;
	}
	
	if (Flag_RX){
		Reception_Detected();
		Flag_RX = 0;
	}
	
	if (Flag_ADC){
		End_Convertion_ADC();
		Flag_ADC = 0;
	}
	if (Flag_Next){
		showMsg(MENU);
		UART_RX_Interrupt_Enable();
		Flag_Next = 0;
	}
	
}


ISR(TIMER0_COMPA_vect){
	Flag_COMPA = 1;
}
ISR(TIMER0_OVF_vect){
	Flag_OVF = 1;
}




ISR (ADC_vect){ //ADC Convertion Complete
	Flag_ADC = 1;
}



ISR(USART_RX_vect){ //se detecto una recepcion
	Flag_RX = 1;
}

ISR(USART_UDRE_vect){
	Transmition_Allowed();
}



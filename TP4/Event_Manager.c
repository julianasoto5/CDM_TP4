
//Event Manager
#include "Event_Manager.h"

#define ENTER '\r\n'

//Declaracion de Flags del sistema
volatile unsigned char Flag_OVF = 0;
volatile unsigned char Flag_COMPA = 0;
volatile unsigned char Flag_Next = 1;
volatile unsigned char Flag_RX = 0;
volatile unsigned char Flag_TX = 0;
volatile unsigned char Flag_ADC = 0;
//---------------------------------

//Definicion del tipo de dato de mensaje
typedef enum{MENU, BRILLO, EXITO, ERROR} private_msg_type;
	
char* BIENVENIDA = {"\r\nBienvenidx al Controlador del LED RGB"};

	
char* private_msg[] = {"\r\nSeleccione una letra para modificar un color:\r\nR (Red/Rojo)\r\nG (Green/Verde)\r\nB (Blue/Azul)\r\n",
						"\r\nModifique el potenciometro para variar el color. \r\nPresione cualquier tecla para finalizar la operación: ",
						"\r\nModificacion realizada con exito.",
						"\r\nEl dato ingresado es invalido. Intentelo de nuevo."
						};

const char* color_code = "\r\nEl color mostrado tiene el codigo RGB #%02X%02X%02X\r\n";	

//Prototipo de funciones privadas
void End_Convertion_ADC();
void showMsg(private_msg_type msg);
void Reception_Detected();
void Transmition_Allowed();
//---------------------------------




volatile uint8_t select_color = 1;
static rgb current_color;

void EVENT_MANAGER_ShowWelcome(){
	UART_Send_String(BIENVENIDA);
}

void showMsg(private_msg_type msg){
	UART_Send_String(private_msg[msg]);
}


void Reception_Detected(){ //tres opciones. Esta esperando un RGB, un numero o un S/N
	static volatile char c;

	c = UART_Receive_Data();
	UART_Send_Data(c);
	
		if (select_color){ //esta esperando un RGB
			switch (c){
				case 'R': current_color = RED; 
						  select_color = 0; 
						  break;
				case 'G': current_color = GREEN; 
						  select_color = 0; ;
						  break;
				case 'B': current_color = BLUE; 
						  select_color = 0; 
						  break;
				default:  showMsg(ERROR);
						  Flag_Next = 1;
			}
			if (!select_color){ //se ingreso R_G_B --> mensaje del potenciometro e inicio de ADC
				showMsg(BRILLO);
				UART_RX_Interrupt_Enable();
				ADC_Interrupt_Enable();
				ADC_StartConvertion();
			}
			
			
		}else { //finalizo la operacion del potenciometro --> desactiva ADC y manda mensaje final
			UART_RX_Interrupt_Disable();
			ADC_Interrupt_Disable();
			End_Convertion_ADC();
		}
		
			
}



void Transmition_Allowed(){
	static volatile char data;
	if (!UART_Current_TX_Status()){
			//Desactivo las interrupciones de transmision
			UART_Reset_TX();
	}else{

		data = UART_Get_Char_From_Buffer();
		if (data){
			UART_Send_Data(data);
		}
	}
}

void End_Convertion_ADC(){
	static char aux[200];
	showMsg(EXITO);
	
	uint8_t* color = PWM_GetRGB();
	sprintf(aux,color_code,color[RED],color[GREEN],color[BLUE]);
	UART_Send_String(aux);

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
		PWM_Change_DC_RGB(current_color, ADCH);
		ADC_StartConvertion();
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



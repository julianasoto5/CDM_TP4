
//Event Manager
#include "Event_Manager.h"
#include <util/delay.h>
#define ENTER '\r'
#define CONVERT_NUM_CHAR_TO_INT(x) x-'0'

volatile unsigned char Flag_OVF = 0;
volatile unsigned char Flag_COMPA = 0;
volatile unsigned char Flag_Next = 0;
volatile unsigned char Flag_RX = 0;
volatile unsigned char Flag_TX = 0;



char* BIENVENIDA = {"\r\nBienvenidx al Controlador del LED RGB"};

typedef enum{MENU, BRILLO, EXITO} private_msg_type;
char* private_msg[3] = {"\r\nSeleccione una letra para modificar un color:\r\nR (Red/Rojo)\r\nG (Green/Verde)\r\nB (Blue/Azul)\r\n",
						"\r\nPara modificar el brillo del color seleccionado, modifique el potenciometro y luego presione cualquier tecla: ",
						"\r\nModificacion realizada con exito.\r\n"};

typedef enum {INVALID, OUT_OF_RANGE} error_type;
char* error_msg[2] = {"El dato ingresado es invalido. Intentelo de nuevo.\n", "El numero ingresado esta fuera de rango. Intentelo de nuevo.\n"};
	

volatile char data;
volatile char c;


volatile uint8_t select_color = 1;
rgb current_color=INIT;
void EVENT_MANAGER_ShowWelcome(){
	UART_Send_String(BIENVENIDA);
	UART_Send_String(private_msg[MENU]);
}

void showMsg(private_msg_type msg){
	UART_Send_String(private_msg[msg]);
}

void showError(error_type error){
	UART_Send_String(error_msg[error]);
}

void Reception_Detected(){ //tres opciones. Esta esperando un RGB, un numero o un S/N
	
	c = UART_Receive_Data();
	UART_Send_Data(c);
		if (select_color){ //esta esperando un RGB
			switch (c){
				case 'R': current_color = RED; select_color = 0; showMsg(BRILLO); break;
				case 'G': current_color = GREEN; select_color = 0; showMsg(BRILLO); break;
				case 'B': current_color = BLUE; select_color = 0; showMsg(BRILLO); break;
				default: current_color = INIT;
						 showError(INVALID);
						 _delay_ms(10);
						 showMsg(MENU);
			}
		
		}else {//esta esperando una tecla para el uso del potenciometro
			ADC_StartConvertion();
			//Probar con la interrupcion cuando todo funcione
			while((ADCSRA&(1<<ADIF))==0); //wait for end of conversion
			ADCSRA |= (1<<ADIF); //clear the ADIF flag
			_delay_ms(50);
			PWM_Change_DC_RGB(current_color,ADCH);
			select_color = 1;
			showMsg(EXITO);
			Flag_Next = 1;
			
		}
			
}



void Transmition_Allowed(){
	if (!UART_Current_TX_Status()){
			//Desactivo las interrupciones de transmision
			UART_Reset_Index();
			UART_TX_Interrupt_Disable();
	}else{
	//UART_Reset_Index(); //static uint8_t i=0;
	data = UART_Get_Char_From_Buffer();
	if (data){
		UART_Send_Data(data); 
		//_delay_ms(1);
	}
	}
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
	
	
	if (Flag_Next){
		showMsg(MENU);
		Flag_Next = 0;
	}
	
}


ISR(TIMER0_COMPA_vect){
	Flag_COMPA = 1;
}
ISR(TIMER0_OVF_vect){
	Flag_OVF = 1;
}




/*
ISR (ADC_vect){ //ADC Convertion Complete
	
	PWM_Change_DC_RGB(current_color,ADCH);
	_delay_ms(50);
	select_color = 1;
	showMsg(EXITO);
	showMsg(MENU);
	
}*/



ISR(USART_RX_vect){ //se detecto una recepcion
	Flag_RX = 1;
}

ISR(USART_UDRE_vect){
	Transmition_Allowed();
}



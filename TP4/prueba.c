//MENU

#define MODIFY_RED 'R'
#define MODIFY_GREEN 'G'
#define MODIFY_BLUE 'B'

typedef enum {R = MODIFY_RED,G = MODIFY_GREEN,B = MODIFY_BLUE} rgb;



void decode(){
	rgb c = UART_Get_Char_From_Buffer();

	
}


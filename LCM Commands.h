#include "tm4c123gh6pm.h"
#include "delay functions.h"

// commands define
#define clear_display 0x01 // clears display
#define return_home 0x02 // sets cursor to place (0, 0)
#define entry_mode_set 0x06 // increment cursor; shift display off
#define display_control 0x0E //diplay on; cursor on; cursor blink off
#define cursor_shift 0x10 // 
#define function_set 0x38 //8-bit mode; 2-line; 5x8 font

void enable_toggle(){
	GPIO_PORTE_DATA_R &= ~0x01;
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTE_DATA_R &= ~0x01;
}

void send_instruction(unsigned char instruction){
	GPIO_PORTE_DATA_R = 0x00;
	GPIO_PORTB_DATA_R = instruction;
	enable_toggle();
}

void send_data(){
	GPIO_PORTE_DATA_R = 0x02;
}

void print_char(char letter){
	send_data();
	GPIO_PORTB_DATA_R = letter;
	enable_toggle();
	delay_us(1);
}

void lcm_print(char text[16]){
	int i;
	for (i = 0; i < 16; i++){
		print_char(text[i]);
	}
}

void LCM_Init(){
	send_instruction(clear_display);
	send_instruction(function_set);
	send_instruction(display_control);
	send_instruction(entry_mode_set);
}

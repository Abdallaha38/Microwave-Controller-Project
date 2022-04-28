#include "tm4c123gh6pm.h"
#include "delay functions.h"

// commands define
#define write_instruction 0x00
#define write_data 0x04
#define read_flag 0x02
#define read_data 0x06
#define clear_display 0x01 // clears display
#define return_home 0x02 // sets cursor to place (0, 0)
#define entry_mode_1 0x06 // increment cursor; shift display off
#define entry_mode_2 0x07 // increment cursor; shift display on
#define display_control_1 0x0C //diplay on; cursor off; cursor blink off
#define display_control_2 0x0D //diplay on; cursor off; cursor blink on
#define display_control_3 0x0E //diplay on; cursor on; cursor blink off
#define display_control_4 0x0F //diplay on; cursor on; cursor blink on
#define cursor_shift 0x10 // 
#define function_set 0x38 //8-bit mode; 2-line; 5x8 font

void enable_toggle(){
	GPIO_PORTE_DATA_R &= ~0x01;
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTE_DATA_R &= ~0x01;
}

void lcm_instruction(unsigned char instruction){
	GPIO_PORTE_DATA_R = write_instruction;
	GPIO_PORTB_DATA_R = instruction;
	enable_toggle();
	if ( instruction == clear_display)
		delay_us(1550);
	else if ( instruction == return_home)
		delay_us(1550);
	else
		delay_us(37);
}

void lcm_print_char(char letter){
	GPIO_PORTE_DATA_R = write_data;
	GPIO_PORTB_DATA_R = letter;
	enable_toggle();
	//delay_us(37);
	delay_ms(200);
}

void lcm_print_string(char text[16]){
	int i;
	for (i = 0; i < 16; i++){
		lcm_print_char(text[i]);
	}
}

void lcm_cgram_address(unsigned char address){
	unsigned char temp = 0x40 | address;
	lcm_instruction(temp);
}

void lcm_ddram_address(unsigned char address){
	unsigned char temp = 0x80 | address;
	lcm_instruction(temp);
}

void lcm_write_cgram(unsigned char address, unsigned char data){
	lcm_cgram_address(address);
	lcm_print_char(data);
}

void lcm_write_ddram(unsigned char address, unsigned char data){
	lcm_ddram_address(address);
	lcm_print_char(data);
}

unsigned char lcm_read_cgram(unsigned char address){
	lcm_cgram_address(address);
	GPIO_PORTE_DATA_R = read_data;
	delay_us(37);
	return GPIO_PORTB_DATA_R;
}

unsigned char lcm_read_ddram(unsigned char address){
	lcm_ddram_address(address);
	GPIO_PORTE_DATA_R = read_data;
	delay_us(37);
	return GPIO_PORTB_DATA_R;
}

void lcm_newline(){
	lcm_ddram_address(0x40);
}

void LCM_Init(){
	lcm_instruction(clear_display);
	lcm_instruction(function_set);
	lcm_instruction(display_control_3);
	lcm_instruction(entry_mode_1);
}

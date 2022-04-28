#include "tm4c123gh6pm.h"
#include "delay functions.h"
#include "string.h"

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

const unsigned char line0[40] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x04, 0x25, 0x26, 0x27};
const unsigned char line1[40] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67};

void PORTB_Init(){
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_RCGCGPIO_R & 0x02) == 0);
	GPIO_PORTB_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTB_CR_R = 0xFF;
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_AFSEL_R = 0x00;
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTB_PCTL_R = 0x00000000;
	GPIO_PORTB_DIR_R = 0xFF;
}

void PORTE_Init(){
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_RCGCGPIO_R & 0x10) == 0);
	GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTE_CR_R = 0x07;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_DEN_R = 0x07;
	GPIO_PORTE_PCTL_R = 0x00000000;
	GPIO_PORTE_DIR_R = 0x07;
}

void enable_toggle(){
	GPIO_PORTE_DATA_R &= ~0x01;
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTE_DATA_R &= ~0x01;
}

void lcm_instruction(unsigned char instruction){
	GPIO_PORTE_DATA_R = write_instruction;
	GPIO_PORTB_DATA_R = instruction;
	enable_toggle();
	if ( instruction == clear_display || instruction == return_home)
		delay_us(1550);
	/*else if ( instruction == return_home)
		delay_us(1550);*/
	else
		delay_us(37);
}

void lcm_print_char(char letter, int interval){
	GPIO_PORTE_DATA_R = write_data;
	GPIO_PORTB_DATA_R = letter;
	enable_toggle();
	//delay_us(37);
	delay_ms(interval);
}

void lcm_print_string(char text[16]){
	int i;
	lcm_instruction(entry_mode_1);
	for (i = 0; i < strlen(text); i++){
		lcm_print_char(text[i], 250);
	}
}

void lcm_print(char text[40]){
	int i;
	int length = strlen(text);
	
	if (length < 17){
		lcm_instruction(entry_mode_1);
		for (i = 0; i < length; i++){
			lcm_print_char(text[i], 250);
			}
	}
	
	else if (length > 16 && length < 41){
		lcm_instruction(entry_mode_1);
		for (i = 0; i < 15; i++){
			lcm_print_char(text[i], 500);
		}
		lcm_instruction(entry_mode_2);
		for (i = 15; i < length; i++){
			lcm_print_char(text[i], 500);
		}
	}
	
	else if (length > 40)
		lcm_print_string("Text too long.");
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
	lcm_print_char(data, 1);
}

void lcm_write_ddram(unsigned char address, unsigned char data){
	lcm_ddram_address(address);
	lcm_print_char(data, 1);
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

void lcm_move(int line, int place){
	if (line == 0)
		lcm_ddram_address(line0[place]);
	else if (line == 1)
		lcm_ddram_address(line1[place]);
}

void lcm_newline(){
	lcm_instruction(return_home);
	lcm_move(1, 0);
}

void lcm_Init(){
	PORTB_Init();
	PORTE_Init();
	lcm_instruction(clear_display);
	lcm_instruction(function_set);
	lcm_instruction(display_control_3);
	lcm_instruction(entry_mode_1);
}

// necessary includes
#include "tm4c123gh6pm.h"
#include "delay functions.h"
#include "string.h"
#include "stdio.h"

// control signals define
#define write_instruction 0x00
#define write_data 0x04
#define read_flag 0x02
#define read_data 0x06

// instructions define
#define clear_display 0x01 // clears display
#define return_home 0x02 // sets cursor to place (0, 0)
#define entry_mode_1 0x06 // cursor from left to right; auto shift display off
#define entry_mode_2 0x07 // cursor from left to right; autoshift display on
#define entry_mode_3 0x04 // cursor from right to left; shift display off
#define entry_mode_4 0x05 // cursor from right to left; shift display on
#define display_control_1 0x0C //diplay on; cursor off; cursor blink off
#define display_control_2 0x0D //diplay on; cursor off; cursor blink on
#define display_control_3 0x0E //diplay on; cursor on; cursor blink off
#define display_control_4 0x0F //diplay on; cursor on; cursor blink on
#define cursor_shift_right 0x14 //do not shift display; shift cursor to the left 
#define cursor_shift_left 0x10 //do not shift display; shift cursor to the right
#define display_shift_right 0x1C //shift display to the right; cursor moves accordingly
#define display_shift_left 0x18 //shift display to the left; cursor moves accordingly
#define function_set 0x38 //8-bit mode; 2-line; 5x8 font

// location addresses
const unsigned char line0[40] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x04, 0x25, 0x26, 0x27};
const unsigned char line1[40] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67};

//Ports B, E initiation
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

// a function to toggle enable pin high then low again
void enable_toggle(){
	GPIO_PORTE_DATA_R &= ~0x01;
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTE_DATA_R &= ~0x01;
}

void lcm_control(unsigned char control_signal){
	GPIO_PORTE_DATA_R &= ~0x06;
	GPIO_PORTE_DATA_R |= control_signal;
}

unsigned char lcm_read_address(){
	lcm_control(read_flag);
	return GPIO_PORTB_DATA_R & 0x7F;
}

unsigned char lcm_busy_flag(){
	lcm_control(read_flag);
	return ((GPIO_PORTB_DATA_R & 0x10) >> 7);
}

int lcm_getline(){
	unsigned char temp1 = lcm_read_address();
	int temp2;
	
	if (temp1 < 0x27)
		temp2 = 0;
	else if (temp1 > 0x40)
		temp2 = 1;
	
	return temp2;	
}

int lcm_getlocation(){
	
	unsigned char address = lcm_read_address();
	int temp = lcm_getline();
	int i;
	
	if (temp == 0){
		for (i = 0; i < 40; i++){
			if (line0[i] == address)
				break;
		}
	}
	else {
		for (i = 0; i < 40; i++){
			if (line1[i] == address)
				break;
		}
	}
	
	return i;	
}

unsigned char lcm_get_ddram_address(int line, int place){
	unsigned char temp;
	int i;
	if (line == 0){
		for(i = 0; i < 40; i++){
			if(i == place){
				temp = line0[i];
				break;
			}
		}
	}
	else if (line == 1){
		for(i = 0; i < 40; i++){
			if(i == place){
				temp = line1[i];
				break;
			}
		}
	}
	
	return temp;
}

// a function to pass instructions to LCM
void lcm_instruction(unsigned char instruction){
	lcm_control(write_instruction);
	GPIO_PORTB_DATA_R = instruction;
	enable_toggle();
	if ( instruction == clear_display || instruction == return_home)
		delay_us(1530);
	else
		delay_us(39);
}

void lcm_shift_control(unsigned char shift_type, int times){
	int i;
	lcm_control(write_instruction);
	for(i = 0; i < times; i++){
		lcm_instruction(shift_type);
	}
}

// a function that prints a single char to LCM
void lcm_print_char(char letter, int interval){
	GPIO_PORTE_DATA_R = write_data;
	GPIO_PORTB_DATA_R = letter;
	enable_toggle();
	if (interval == 0)
		delay_us(43);
	else
	 delay_ms(interval);
}

// a function to print a text of less than of equal 16 characters on LCM
void lcm_print_string(char text[16]){
	int i;
	lcm_instruction(entry_mode_1);
	for (i = 0; i < strlen(text); i++){
		lcm_print_char(text[i], 0);
	}
}

void lcm_print_int(int x){
	char temp[16];
	sprintf(temp, "%d", x);
	lcm_print_string(temp);
}

// a function to print a text of less that or equal 40 characters on LCM
void lcm_print(char text[40]){
	int i;
	int length = strlen(text);
	
	if (length < 17){
		lcm_instruction(entry_mode_1);
		for (i = 0; i < length; i++){
			lcm_print_char(text[i], 1);
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
		lcm_print_string("E: Text too long");
}

// a function that sets an address to CGRAM
void lcm_cgram_address(unsigned char address){
	unsigned char temp = 0x40 | address;
	lcm_instruction(temp);
}

// a function that sets an address to DDRAM
void lcm_ddram_address(unsigned char address){
	unsigned char temp = 0x80 | address;
	lcm_instruction(temp);
}

/*// a function that writes data to CGRAM
void lcm_write_cgram(unsigned char address, unsigned char data){
	lcm_cgram_address(address);
	lcm_print_char(data, 1);
}

// a function that reads data from CGRAM
unsigned char lcm_read_cgram(unsigned char address){
	lcm_cgram_address(address);
	GPIO_PORTE_DATA_R = read_data;
	delay_us(43);
	return GPIO_PORTB_DATA_R;
}*/

// a function that moves cursor to a specific place in screen
void lcm_movecursor(int line, int place){
	if (line == 0)
		lcm_ddram_address(line0[place]);
	else if (line == 1)
		lcm_ddram_address(line1[place]);
}

void lcm_move_display(int line, int place){
	int temp = lcm_get_ddram_address(line, place) - lcm_read_address();
	if (temp < 0){
		lcm_shift_control(display_shift_left, -temp);
	}
	else if (temp > 0){
		lcm_shift_control(display_shift_right, temp);
	}
}

// a function that reads data from DDRAM
unsigned char lcm_read(int row, int column){
	lcm_movecursor(row, column);
	lcm_control(read_data);
	delay_us(43);
	return GPIO_PORTB_DATA_R;
}

// a function that moves cursor to the beginning of the second line
void lcm_newline(){
	int i = 0;
	lcm_movecursor(1, 0);
	while(i < 40){
		lcm_print_char(' ', 0);
		i++;
	}
	lcm_instruction(return_home);
	lcm_movecursor(1, 0);
}

void print_time_char(char minutes[2], char seconds[2]){
	lcm_print(minutes);
	lcm_print(":");
	lcm_print(seconds);
}

void print_time(int minute, int second){
	char temp_s[2], temp_m[2];
	
	if(minute == 0){
		temp_m[0] = '0';
		temp_m[1] = '0';
	}
	else if (minute < 10){
		sprintf(temp_m, "%d", minute);
		temp_m[1] = temp_m[0];
		temp_m[0] = '0';
	}
	else
		sprintf(temp_m, "%d", minute);
	
	if(second == 0){
		temp_s[0] = '0';
		temp_s[1] = '0';
	}
	else if (second < 10){
		sprintf(temp_s, "%d", second);
		temp_s[1] = temp_s[0];
		temp_s[0] = '0';
	}
	else
		sprintf(temp_s, "%d", second);
	
	print_time_char(temp_m, temp_s);
}

void print_delay(int minutes, int seconds){
	int i, j = seconds, k;
	lcm_newline();
	lcm_print("Time left: ");
	for(i = minutes; i >= 0; i--){
		for(j; j >= 0; j--){
			lcm_movecursor(1, 11);
			lcm_print("     ");
			lcm_movecursor(1, 11);
			print_time(i, j);
			delay_s(1);
		}
		j = 59;
	}
	for(k = 0; k < 3; k++){ 
		lcm_newline();
		delay_ms(500);
		lcm_print("Finished !!");
		delay_ms(500);
	}
}

// LCM initiation function
void lcm_Init(){
	PORTB_Init();
	PORTE_Init();
	lcm_instruction(clear_display);
	lcm_instruction(function_set);
	lcm_instruction(display_control_3);
	lcm_instruction(entry_mode_1);
}

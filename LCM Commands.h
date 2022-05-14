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
#define display_control_1 0x0C // diplay on; cursor off; cursor blink off
#define display_control_2 0x0D // diplay on; cursor off; cursor blink on
#define display_control_3 0x0E // diplay on; cursor on; cursor blink off
#define display_control_4 0x0F // diplay on; cursor on; cursor blink on
#define display_off 0x08 // display off; data are still stored
#define display_on 0x0C // display back on; stored data are printed
#define cursor_shift_right 0x14 // do not shift display; shift cursor to the left 
#define cursor_shift_left 0x10 // do not shift display; shift cursor to the right
#define display_shift_right 0x1C // shift display to the right; cursor moves accordingly
#define display_shift_left 0x18 // shift display to the left; cursor moves accordingly
#define function_set 0x38 // 8-bit mode; 2-line; 5x8 font

// location addresses
const unsigned char line0[40] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x04, 0x25, 0x26, 0x27};
const unsigned char line1[40] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67};

// Port B initiation
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

// Port E initiation
void PORTE_Init(){
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_RCGCGPIO_R & 0x10) == 0);
	GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTE_CR_R = 0x1F;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_DEN_R = 0x1F;
	GPIO_PORTE_PCTL_R = 0x00000000;
	GPIO_PORTE_DIR_R = 0x17;
}

// A funtion to toggle enable pin high then low again
void enable_toggle(){
	GPIO_PORTE_DATA_R &= ~0x01;
	GPIO_PORTE_DATA_R |= 0x01;
	GPIO_PORTE_DATA_R &= ~0x01;
}

// A funtion to set control signals
void lcm_control(unsigned char control_signal){
	GPIO_PORTE_DATA_R &= ~0x06;
	GPIO_PORTE_DATA_R |= control_signal;
}

// A funtion that reads the address of the current cell
unsigned char lcm_read_address(){
	char x;
	GPIO_PORTB_DIR_R = 0x00;
	lcm_control(read_flag);
	enable_toggle();
	x = GPIO_PORTB_DATA_R;
	GPIO_PORTB_DIR_R = 0xFF;
	return (x & 0x7F);
}

// A funtion that returns busy flag; high = LCM is in operation please wait
unsigned char lcm_busy_flag(){
	char x;
	GPIO_PORTB_DIR_R = 0x00;
	lcm_control(read_flag);
	enable_toggle();
	x = GPIO_PORTB_DATA_R;
	GPIO_PORTB_DIR_R = 0xFF;
	return ((x & 0x10) >> 7);
}

// A funtion to pass instructions to LCM
void lcm_instruction(unsigned char instruction){
	lcm_control(write_instruction);
	GPIO_PORTB_DATA_R = instruction;
	enable_toggle();
	if ( instruction == clear_display || instruction == return_home)
		delay_us(1530);
	else
		delay_us(39);
}

// A funtion that controls LCM cursor and display shift
void lcm_shift_control(unsigned char shift_type, int times){
	int i;
	lcm_control(write_instruction);
	for(i = 0; i < times; i++){
		lcm_instruction(shift_type);
	}
}

// A funtion that prints a single char to LCM
void lcm_print_char(char letter, int interval){
	lcm_control(write_data);
	GPIO_PORTB_DATA_R = letter;
	enable_toggle();
	if (interval == 0)
		delay_us(43);
	else
	 delay_ms(interval);
}

// A funtion to print a text of less than of equal 16 characters on LCM
void lcm_print_string(char text[16]){
	int i;
	lcm_instruction(entry_mode_1);
	for (i = 0; i < strlen(text); i++){
		lcm_print_char(text[i], 0);
	}
}

// A funtion that directly prints an integer on LCM
void lcm_print_int(int x){
	char temp[16];
	sprintf(temp, "%d", x);
	lcm_print_string(temp);
}

// A funtion to print a text of less that or equal 40 characters on LCM
// in case of more than 16 characters the display shifts with text
void lcm_print(char text[40]){
	int i;
	int length = strlen(text);
	
	if (length < 17){
		lcm_instruction(entry_mode_1);
		for (i = 0; i < length; i++){
			lcm_print_char(text[i], 0);
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

// A funtion that sets an address to DDRAM
// DDRAM = Display Data RAM
void lcm_ddram_address(unsigned char address){
	unsigned char temp = 0x80 | address;
	lcm_instruction(temp);
}

// A funtion that converts a location on LCM to its DDRAM address
unsigned char lcm_LoctationToAddress(int row, int column){
	unsigned char x;
	if (row == 0)
		x = line0[column];
	else if (row == 1)
		x = line1[column];
	
	return x;	
}

// A funtion that moves cursor to a specific place in screen
void lcm_movecursor(int line, int place){
	if (line == 0)
		lcm_ddram_address(line0[place]);
	else if (line == 1)
		lcm_ddram_address(line1[place]);
}

// A funtion that returns what line the cursor is on
int lcm_cursor_line(){
	unsigned char x = lcm_read_address();
	int line;
	
	if (x < 0x28){
		line = 0;
	}
	else if (x > 0x3F){
		line = 1;
	}
	
	return line;
}

// A funtion that returns what place the cursor is on
int lcm_cursor_place(){
	unsigned char x = lcm_read_address();
	int i, place;
	
	if (x < 0x28){
		for(i = 0; i < 40; i++){
			if (line0[i] == x){
				place = i;
				break;
			}
		}
	}
	else if (x > 0x3F){
		for(i = 0; i < 40; i++){
			if (line1[i] == x){
				place = i;
				break;
			}
		}
	}
	
	return place;
}

// A funtion to moves display to specific column on LCM
// use lcm_movecursor to move the cursor as well
void lcm_movedisplay(int place){
	lcm_instruction(return_home);
	if (place < 0)
		lcm_shift_control(display_shift_right, -place);
	else
		lcm_shift_control(display_shift_left, place);
}

// A funtion that reads a character from a location on display
char lcm_read(int row, int column){
	char x;
	lcm_movecursor(row, column);
	GPIO_PORTB_DIR_R = 0x00;
	lcm_control(read_data);
	enable_toggle();
	x = GPIO_PORTB_DATA_R;
	delay_us(43);
	GPIO_PORTB_DIR_R = 0xFF;
	return (x & 0x7F);
}

// A funtion that moves cursor to the beginning of the first line
void lcm_firstline() {
	int i = 0;
	lcm_movecursor(0, 0);
	while (i < 40) {
		lcm_print_char(' ', 0);
		i++;
	}
	lcm_instruction(return_home);
}

// A funtion that moves cursor to the beginning of the second line
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

// A funtion that prints chars in time format
void print_time_char(char minutes[2], char seconds[2]){
	lcm_print(minutes);
	lcm_print(":");
	lcm_print(seconds);
}

// A funtion that prints integers in time format
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
	
	lcm_print(temp_m);
	lcm_print(":");
	lcm_print(temp_s);
}

// A funtion that prints a countdown to 0 from assigned time
void print_delay(int minutes, int seconds){
	int i, j = seconds, k;
	lcm_newline();
	lcm_print("Time left: ");
	for(i = minutes; i >= 0; i--){
		for(; j >= 0; j--){
			lcm_movecursor(1, 11);
			print_time(i, j);
			delay_s(1);
		}
		j = 59;
	}
	
	lcm_newline();
	lcm_print("Finished !!");
	for(k = 0; k < 3; k++){
		GPIO_PORTF_DATA_R &= ~0x0E;
		GPIO_PORTE_DATA_R &= ~0x10;
		lcm_instruction(display_off);
		delay_ms(500);
		GPIO_PORTF_DATA_R |= 0x0E;
		GPIO_PORTE_DATA_R |= 0x10;
		lcm_instruction(display_on); 
		delay_ms(500);
	}
	GPIO_PORTF_DATA_R &= ~0x0E;
	GPIO_PORTE_DATA_R &= ~0x10;
}

// LCM initiation function
void lcm_Init(){
	PORTB_Init();
	PORTE_Init();
	lcm_instruction(clear_display);
	lcm_instruction(function_set);
	lcm_instruction(display_control_1);
	lcm_instruction(entry_mode_1);
}


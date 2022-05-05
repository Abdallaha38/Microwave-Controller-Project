#include "tm4c123gh6pm.h"
#include "keypad.h"
#include "LCM Commands.h"
#include "interrupts.h"

char t[5] = { '0', '0', ':', '0', '0' };  //temporary array to store and show remaining time
int minutes, seconds, i;

void doorLockActive(int x) {         //function to activiate door-lock interrupt
	if (!x)
		GPIO_PORTD_IM_R &= ~0x80;
	else if (x)
		GPIO_PORTD_IM_R |= 0x80;
}

void pauseActive(int x) {            //function to (pause door-lock activiation) i.e. interruption
	if (!x)
		GPIO_PORTF_IM_R &= ~0x10;
	else if (x) {
		GPIO_PORTF_ICR_R |= 0x10;
		GPIO_PORTF_IM_R |= 0x10;
	}
}

void choose_meal() {       //main function to choose a meal

	char m;    //character variable to check letter entered by user
	int i, z;     //integer variables to hold the time taken by microwave to finish
	short count;    //variable to be used in checking number of kilos entered by user

	pauseActive(0);
	doorLockActive(0);
	lcm_instruction(clear_display);
	lcm_print("WELCOME !!");
	delay_s(2);
Begin:        //start the program
	lcm_movecursor(0, 0);  //1st line in LCD
	lcm_print("Choose a meal");
	lcm_movecursor(1, 0);    //2nd line in LCD
	lcm_print("from (A-B-C-D)");
	m = keypad_clicked();     //waiting for letter entered by user
	if (m != 'A' && m != 'B' && m != 'C' && m != 'D') {
		lcm_instruction(clear_display);
		lcm_print("Err");     //error message if user hasn't choosen a correct letter
		delay_s(1);
		goto Begin;         //branch to Begin lable to start from beggining
	}
	lcm_instruction(clear_display);
	pauseActive(1);

	switch (m) {

	case 'A':
		lcm_print_string("Popcorn");
		minutes = 1;
		seconds = 0;
		lcm_newline();
		lcm_print("start?");
		while (GPIO_PORTF_DATA_R & 0x01);    //SW-2 shouled be ON (PUSHED) to start the microwave
		break;

	case 'B':
		lcm_print_string("Beef weight?");
		delay_ms(100);
		m = keypad_clicked();
		for (count = 1; count < 10; count++)    //Checking for valid kilos entered (1-9)
		{
			lcm_instruction(clear_display);
			if ((m - '0') == count)       
				break;
			else if (count == 9)
			{
				lcm_print_string("Err");    //error message if user hasn't choosen a correct number for kilos
				delay_s(1);
				lcm_instruction(clear_display);
				lcm_print_string("Beef weight?");
				m = keypad_clicked();
				count = 0;
			}
		}
		lcm_print_char(m, 0);
		lcm_print(" kg");
		delay_s(2);
		lcm_instruction(clear_display);
		lcm_print("start?");
		i = m - '0';
		z = 30 * i;   //it takes 0.5 minute(30 sec.) for whole kilo
		//lcm_print("Time remaining = z in minutes & seconds");
		minutes = z / 60;
		seconds = z % 60;
		while (GPIO_PORTF_DATA_R & 0x01);   //SW-2 shouled be ON (PUSHED) to start the microwave
		break;

	case 'C':
		lcm_print_string("Chicken weight?");
		delay_ms(100);
		m = keypad_clicked();
		for (count = 1; count < 10; count++)     //Checking for valid kilos entered (1-9)
		{
			lcm_instruction(clear_display);
			if ((m - '0') == count)
				break;
			else if (count == 9)
			{
				lcm_print_string("Err");     //error message if user hasn't choosen a correct number for kilos
				delay_s(1);
				lcm_instruction(clear_display);
				lcm_print_string("Chicken weight?");
				m = keypad_clicked();
				count = 0;
			}
		}
		lcm_print_char(m, 0);
		lcm_print(" kg");
		delay_s(2);
		lcm_instruction(clear_display);
		lcm_print("start?");
		i = m - '0';
		z = 12 * i;         //it takes 0.2 minute(12 sec.) for whole kilo
	        //lcm_print("Time remaining = z in minutes & seconds");
		minutes = z / 60;
		seconds = z % 60;
		while (GPIO_PORTF_DATA_R & 0x01);     //SW-2 shouled be ON (PUSHED) to start the microwave
		break;

	case 'D':
		lcm_print_string("Cooking Time?");
		t[0] = '0';
		t[1] = '0';
		t[3] = '0';
		t[4] = '0';
		delay_ms(100);
		while (1) { 
			m = keypad_clicked();
			if (m == '*')    //indicate that user has finished entering time
				break;
			else if (m == 'A' || m == 'B' || m == 'C' || m == 'D' || m == '#') {   //if user by false entered letter.. go back to choose a meal
				lcm_instruction(clear_display);
				lcm_print("Err");
				delay_s(1);
				lcm_instruction(return_home);    //return to wait for user entering numbers
			}
			else {
				t[0] = t[1];
				t[1] = t[3];
				t[3] = t[4];
				t[4] = m;
			}
			delay_ms(250);
			lcm_movecursor(0, 0);
			lcm_print("'*' to confirm");
			lcm_movecursor(1, 0);
			lcm_print(t);
			seconds = 10 * (t[3] - '0') + (t[4] - '0');
			minutes = 10 * (t[0] - '0') + (t[1] - '0');
		}
		lcm_firstline();
		lcm_print("start?");
		while (GPIO_PORTF_DATA_R & 0x01);    //SW-2 shouled be ON (PUSHED) to start the microwave
		break;
	}
}

void start() {    //starting the microwave system(look door, show time remaining and turn buzzer with leds after finishing) 
	doorLockActive(1);
	lcm_instruction(clear_display);
	GPIO_PORTF_DATA_R |= 0x0E;
	print_delay(((minutes > 30) ? 30 : minutes), ((minutes >= 30) ? 0 : seconds));
	GPIO_PORTE_DIR_R |= 0x08;
	GPIO_PORTE_DATA_R &= ~0x08;
}

GPIOD_Handler() {    //handle door interrupts
	lcm_movedisplay(20);
	lcm_movecursor(0, 20);
	lcm_print("CLOSE THE DOOR!!");
	while (GPIO_PORTD_MIS_R == 0x80) {
		lcm_instruction(display_off);
		GPIO_PORTF_DATA_R |= 0x0E;
		delay_ms(500);
		lcm_instruction(display_on);
		GPIO_PORTF_DATA_R &= ~0x0E;
		delay_ms(500);
	}
	lcm_instruction(return_home);
}

GPIOF_Handler() {     //handle leds interrupts
	delay_ms(500);
	while (1) {
		GPIO_PORTF_DATA_R ^= 0x0E;
		delay_ms(300);
		if ((GPIO_PORTF_DATA_R & 0x01) == 0) {
			GPIO_PORTF_ICR_R |= 0x10;
			GPIO_PORTF_DATA_R |= 0x0E;
			break;
		}
		else if ((GPIO_PORTF_DATA_R & 0x10) == 0) {
			GPIO_PORTE_DIR_R |= 0x08;
			GPIO_PORTE_DATA_R &= ~0x08;
			break;
		}
	}
}

int main() {

	systick_Init();
	lcm_Init();
	keypad_Init();
	interrupt_Init();
	choose_meal();
	start();
}

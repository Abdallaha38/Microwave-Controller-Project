#include "tm4c123gh6pm.h"
#include "keypad.h"
#include "LCM Commands.h"
#include "interrupts.h"

char t[5] = { '0', '0', ':', '0', '0' };
int minutes, seconds, i;

void interrupt_rdy(int x) {
	if (!x) {
		GPIO_PORTD_IM_R &= ~0x80;
		//GPIO_PORTD_IM_R &= ~0x10;
	}
	else if (x) {
		GPIO_PORTD_IM_R |= 0x80;
		//GPIO_PORTD_IM_R |= 0x10;
	}
}

void choose_meal() {

	char m;
	int i, z;
	char kilos[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	short count;

	interrupt_rdy(0);
	lcm_instruction(clear_display);
	lcm_print("Welcome");
	delay_s(3);
	lcm_movecursor(0, 0);
	lcm_print("Choose a program");
	lcm_movecursor(1, 0);
	lcm_print("from (A-B-C-D)");
	m = keypad_clicked();
	lcm_instruction(clear_display);

	switch (m) {

	case 'A':
		lcm_print_string("Popcorn");
		interrupt_rdy(1);
		minutes = 1;
		seconds = 0;
		lcm_newline();
		lcm_print("start?");
		while (GPIO_PORTF_DATA_R & 0x01);
		break;

	case 'B':
		lcm_print_string("Beef weight?");
		delay_ms(100);
		m = keypad_clicked();
		//Checking for valid kilos entered (1-9)
		for (count = 0; count < 9; count++)
		{
			lcm_instruction(clear_display);
			if (m == kilos[count])
				break;
			else if (count == 8)
			{
				lcm_print_string("Err");
				delay_s(2);
				lcm_instruction(clear_display);
				lcm_print_string("Beef weight?");
				m = keypad_clicked();
				count = 0;
			}
		}
		lcm_print_char(m, 0);
		delay_s(2);
		interrupt_rdy(1);
		lcm_instruction(clear_display);
		lcm_print("start?");
		i = m - '0';
		z = 30 * i;
		//lcm_print("Time remaining = z seconds");
		minutes = z / 60;
		seconds = z % 60;
		while (GPIO_PORTF_DATA_R & 0x01);
		break;

	case 'C':
		lcm_print_string("Chicken weight?");
		delay_ms(100);
		m = keypad_clicked();
		//Checking for valid kilos entered (1-9)
		for (count = 0; count < 9; count++)
		{
			lcm_instruction(clear_display);
			if (m == kilos[count])
				break;
			else if (count == 8)
			{
				lcm_print_string("Err");
				delay_s(2);
				lcm_instruction(clear_display);
				lcm_print_string("Chicken weight?");
				m = keypad_clicked();
				count = 0;
			}
		}
		lcm_print_char(m, 0);
		delay_s(2);
		interrupt_rdy(1);
		lcm_instruction(clear_display);
		lcm_print("start?");
		i = m - '0';
		z = 12 * i;
		minutes = z / 60;
		seconds = z % 60;
		while (GPIO_PORTF_DATA_R & 0x01);
		break;

	case 'D':
		lcm_print_string("Cooking Time?");
		interrupt_rdy(1);
		t[0] = '0';
		t[1] = '0';
		t[3] = '0';
		t[4] = '0';
		delay_ms(100);
		while (1) {
			m = keypad_clicked();
			if (m == '*')
				break;
			else {
				t[0] = t[1];
				t[1] = t[3];
				t[3] = t[4];
				t[4] = m;
			}
			delay_ms(250);
			lcm_movecursor(1, 0);
			lcm_print(t);
			seconds = 10 * (t[3] - '0') + (t[4] - '0');
			minutes = 10 * (t[0] - '0') + (t[1] - '0');
		}
		lcm_firstline();
		lcm_print("start?");
		while (GPIO_PORTF_DATA_R & 0x01);
		break;
	}
}

void start() {
	lcm_instruction(clear_display);
	GPIO_PORTF_DATA_R |= 0x0E;
	print_delay(((minutes > 30) ? 30 : minutes), ((minutes >= 30) ? 0 : seconds));
	GPIO_PORTE_DIR_R |= 0x08;
	GPIO_PORTE_DATA_R &= ~0x08;
}

GPIOD_Handler() {
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

GPIOF_Handler() {
	delay_ms(500);
	while (1) {
		GPIO_PORTF_DATA_R &= ~0x0E;
		delay_ms(500);
		GPIO_PORTF_DATA_R |= 0x0E;
		delay_ms(500);
		if ((GPIO_PORTF_DATA_R & 0x01) == 0) {
			GPIO_PORTF_ICR_R |= 0x10;
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

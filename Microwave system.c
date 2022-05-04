#include "tm4c123gh6pm.h"
#include "keypad.h"
#include "LCM Commands.h"
#include "interrupts.h"

char t[5] = { '0', '0', ':', '0', '0' };
int minutes, seconds, i;

void doorLockActive(int x) {
	if (!x)
		GPIO_PORTD_IM_R &= ~0x80;
	else if (x)
		GPIO_PORTD_IM_R |= 0x80;
}

void pauseActive(int x) {
	if (!x)
		GPIO_PORTF_IM_R &= ~0x10;
	else if (x)
		GPIO_PORTF_IM_R |= 0x10;
}

void choose_meal() {

	char m;
	int i, z;
	short count;

	pauseActive(0);
	doorLockActive(0);
	lcm_instruction(clear_display);
	lcm_print("Welcome");
	delay_s(2);
Begin:
	lcm_movecursor(0, 0);
	lcm_print("Choose a program");
	lcm_movecursor(1, 0);
	lcm_print("from (A-B-C-D)");
	m = keypad_clicked();
	if (m != 'A' && m != 'B' && m != 'C' && m != 'D') {
		lcm_instruction(clear_display);
		lcm_print("Err");
		delay_s(1);
		goto Begin;
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
		while (GPIO_PORTF_DATA_R & 0x01);
		break;

	case 'B':
		lcm_print_string("Beef weight?");
		delay_ms(100);
		m = keypad_clicked();
		//Checking for valid kilos entered (1-9)
		for (count = 1; count < 10; count++)
		{
			lcm_instruction(clear_display);
			if ((m - '0') == count)
				break;
			else if (count == 9)
			{
				lcm_print_string("Err");
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
		for (count = 1; count < 10; count++)
		{
			lcm_instruction(clear_display);
			if ((m - '0') == count)
				break;
			else if (count == 9)
			{
				lcm_print_string("Err");
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
		z = 12 * i;
		minutes = z / 60;
		seconds = z % 60;
		while (GPIO_PORTF_DATA_R & 0x01);
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
			if (m == '*')
				break;
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
		while (GPIO_PORTF_DATA_R & 0x01);
		break;
	}
}

void start() {
	doorLockActive(1);
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

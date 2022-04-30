#include "tm4c123gh6pm.h"
#include "LCM commands.h"
#include "keypad.h"
#include "ctype.h"



char t[5] = {'0', '0', ':', '0', '0'};
int minutes, seconds, i;

void timer(){
	seconds = 10 * (t[3] - '0') + (t[4] - '0');
	minutes = 10 * (t[0] - '0') + (t[1] - '0');
	print_delay(minutes, seconds);
}

void check_start(char x){
	if(x == '*')
		timer();
	else{
		t[0] = t[1];
		t[1] = t[3];
		t[3] = t[4];
		t[4] = x;
	}
}

void cookingTimer(){
	delay_ms(100);
	for(i = 0; i < 3; i++){
		check_start(keypad_clicked());
		lcm_instruction(clear_display);
		lcm_print_string(t);
		lcm_movecursor(0, 4);
		delay_ms(250);
	}
	check_start(keypad_clicked());
	lcm_instruction(clear_display);
	lcm_print_string(t);
	while(keypad_clicked() != '*');
	timer();
}

void choose_meal(){
	
	char m;
	int i, z;
	char kilos[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	short count;
 
	//lcm_print("Enter letter (A-B-C-D) to choose a meal");
	//lcm_instruction(clear_display);
	m = keypad_clicked();
	/*if(!isalpha(x)){
		lcm_print_string("Invalid input !");
		lcm_instruction(clear_display); 
	}*/
	
	switch(m){
		
		case 'A':
			lcm_print_string("Popcorn");
			print_delay(1, 0); 
			break;
		
		case 'B':
			lcm_print_string("Beef weight?");
			delay_ms(100);
			m = keypad_clicked();
			//Checking for valid kilos entered (1-9)
			for(count = 0; count < 9; count++)
				{
					lcm_instruction(clear_display);
					if(m == kilos[count])
						break;
					else if(count == 8)
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
			lcm_instruction(clear_display); 
			i = m - '0';
			z = 30 * i;
			//lcm_print("Time remaining = z seconds");
			print_delay((z / 60), (z % 60));
			break;
   
		case 'C' :
			lcm_print_string("Chicken weight?");
			delay_ms(100);
			m = keypad_clicked();
			//Checking for valid kilos entered (1-9)
			for(count = 0; count< 9; count++)
				{
					lcm_instruction(clear_display);
					if(m == kilos[count])
						break;
					else if(count == 8)
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
			lcm_instruction(clear_display); 
			i = m - '0';
			z = 12 * i;
			//lcm_print("Time remaining = z seconds");
			print_delay((z / 60), (z % 60));
			break;	
		
		case 'D' :
			lcm_print_string("Cooking Time?");
			cookingTimer();
			break;
	}
}

int main(){
	
	systick_Init();
	lcm_Init();
	keypad_Init();
	
	while(1){
		choose_meal();
	}
	
}

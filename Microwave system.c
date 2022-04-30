#include "tm4c123gh6pm.h"
#include "LCM commands.h"
#include "string.h"
#include "stdio.h"
#include "keypad.h"
#include "ctype.h"

void choose_meal(){
	
	char x, m;
	char temp[4];
	int i, y, z, o;
	char kilos[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	short count;
 
	lcm_print("Enter letter(A-B-C-D) to choose a meal");
	lcm_instruction(clear_display);
	x = keypad_clicked();
	if(!isalpha(x)){
		lcm_print_string("Invalid input !");
		lcm_instruction(clear_display); 
	}
	
	switch(x){
		
		case 'A':
			lcm_print_string("Popcorn");
			print_delay(0, 60); 
			break;
		
		case 'B':
			lcm_print_string("Beef weight?");
			lcm_instruction(clear_display); 
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
			print_delay(0,z);
			break;
   
		case 'C' :
			lcm_print_string("Chicken weight?");
			lcm_instruction(clear_display);
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
			print_delay(0,z);
			break;	
		
		case 'D' :
			lcm_print_string("Cooking Time?");
		
		//Printing time with no conditions neither a way for counting down, yet :)
			m = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_string("00:0");
			lcm_print_char(m, 0);
			temp[0] = m;
		
			m = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_string("00:");
			lcm_print_char(temp[0], 0);
			lcm_print_char(m, 0);
			temp[1] = m;
		
			m = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_string("0");
			lcm_print_char(temp[0], 0);
			lcm_print_string(":");
			lcm_print_char(temp[1], 0);
			lcm_print_char(m, 0);
			temp[2] = m;
			
			m = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_char(temp[0], 0);
			lcm_print_char(temp[1], 0);
			lcm_print_string(":");
			lcm_print_char(temp[2], 0);
			lcm_print_char(m, 0);
			temp[3] = m;
			/*
			lcm_instruction(clear_display);
			print_delay();
			lcm_instruction(clear_display);
			*/
			break;
	}
  
}

int main(){
	
	while(1){
		choose_meal();
	}
	
}

#include "tm4c123gh6pm.h"
#include "LCM commands.h"
#include "string.h"
#include "stdio.h"
#include "keypad.h"
#include "ctype.h"

void choose_meal(){
	
	char x ;
	int i,y,z,o;
 
 
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
			i = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_int(i);
			delay_s(2);
			z = 30*i;
			lcm_print("Time remaining = z seconds");
			print_delay(0,z);
			break;
   
		case 'C' :
			lcm_print_string("Chicken weight?");
			lcm_instruction(clear_display);
			i = keypad_clicked();
			lcm_instruction(clear_display);
			lcm_print_int(i);
			delay_s(2);
			z = 12*i;
			lcm_print("Time remaining = z seconds");
			print_delay(0,z);
			break;	
		
		case 'D' :
			lcm_print_string("Cooking Time?");
			lcm_instruction(clear_display);
			lcm_print("You must use numbers (1-30) as 00:00");
			lcm_instruction(clear_display);
			keypad_clicked(i,y,z,o);
			lcm_instruction(clear_display);
			print_delay(o,z ,y,i);
			lcm_instruction(clear_display);
			break;
	}
  
}

int main(){
	
	while(1){
		choose_meal();
	}
	
}

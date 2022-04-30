#include "delay functions.h"
#include "tm4c123gh6pm.h"

//Keypad intializaton
void keypad_Init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x05; //Enable clock in PORTC and PORTA
  while ((SYSCTL_RCGCGPIO_R & 0x05)==0) //Wait to be set
	
 //PORTC digital output for rows
 GPIO_PORTC_CR_R |= 0xF0; //Allow settings for all pins of PORTC
 GPIO_PORTC_AMSEL_R &= ~0xF0; //Disable analog functionality
 GPIO_PORTC_PCTL_R &= ~0xFFFF0000; //Enable digital functionality
 GPIO_PORTC_AFSEL_R &= ~0xF0; //Disable alternate function select
 GPIO_PORTC_DIR_R |= 0xF0 ; //PD0-PD3 rows and set as digital output pins
 GPIO_PORTC_DEN_R |= 0xF0; //Set PORTC as digital pins
 GPIO_PORTC_DATA_R &= ~0xF0; //Clear data pins intially
	
 //PORTA digital input for columns
 GPIO_PORTA_CR_R |= 0xF0; //Allow settings for all pins of PORTC
 GPIO_PORTA_AMSEL_R &= ~0xF0; //Disable analog functionality
 GPIO_PORTA_PCTL_R &= ~0xFFFF0000; //Enable digital functionality
 GPIO_PORTA_AFSEL_R &= ~0xF0;
 GPIO_PORTA_DIR_R &= ~0xF0 ; //PC4-PC7 columns and set as digital output pins
 GPIO_PORTA_DEN_R |= 0xF0; //Set PORTA as digital pins
 GPIO_PORTA_PDR_R = 0xF0;
 GPIO_PORTA_DATA_R &= ~0xF0; //Clear data pins intially
	
}



void delay(int time){
 int i, j;
 for (i = 0; i < 1; i++){
  for (j = 0; j < time; j++);
 }
}

//Deifinig characters of the keypad
char keys[4][4] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

//Getting clicked key
char keypad_clicked()
{
  while(1)
  {
  short i, j;
    for(i = 0; i < 4; i++) //Scan columns 
    { 
   GPIO_PORTC_DIR_R = (1U << (i + 4));
      GPIO_PORTC_DATA_R = (1U << (i + 4));
   delay(50);
      for(j = 0; j < 4; j++) //Scan rows
      {
        if((GPIO_PORTA_DATA_R & 0xF0) == (1U << (j+4)))
          return keys[i][j];
      }
  delay(500); //Debounce delay
    }
  }
}
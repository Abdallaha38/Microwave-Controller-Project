#include "delay_func.c"
//#include "tm4c123gh6pm.h"

//Keypad intializaton
void keypad_Init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x06;      			  //Enable clock in PORTD and PORTC
  while ((SYSCTL_RCGCGPIO_R & 0x06)==0)	  //Wait to be set
	
	//PortD digital output for rows
	GPIO_PORTD_CR_R |= 0x1F;								//Allow settings for all pins of PORTD
	GPIO_PORTD_AMSEL_R &= ~0x1F;						//Disable analog functionality
	GPIO_PORTD_PCTL_R &= ~0x000FFFFF;				//Enable digital functionality
	GPIO_PORTD_AFSEL_R &= ~0x1F;						//Disable alternate function select
	GPIO_PORTD_DIR_R |= 0x0F	;							//PD0-PD3 rows and set as digital output pins
	GPIO_PORTD_DEN_R |= 0x1F;								//Set PORTD as digital pins
	GPIO_PORTD_DATA_R &= ~0x1F;							//Clear data pins intially
	
	//PortC digital input for columns
	GPIO_PORTC_DIR_R &= ~0xF0	;							//PC4-PC7 rows and set as digital output pins
	GPIO_PORTC_DEN_R |= 0xF0;								//Set PORTC as digital pins
	GPIO_PORTC_PUR_R |= 0xF0;								//Enable pull up resistor on PORTC
	GPIO_PORTC_DATA_R &= ~0x1F;							//Clear data pins intially
	
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
    for(short i = 0; i < 4; i++)    //Scan columns 
    {
      GPIO_PORTD_DATA_R = (1U << i);
      for(short j = 0; j < 4; j++)  //Scan rows
      {
        if((GPIO_PORTC_DATA_R & 0xF0) == (1U << j+4))
          return keys[j][i];
      }
			delay_us(5);									//Debounce delay
    }
  }
}



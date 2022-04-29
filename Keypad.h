#include "tm4c123gh6pm.h"

//Keypad intializaton
void keypad_Init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x09;      			  //Enable clock in PORTD and PORTA
  while ((SYSCTL_RCGCGPIO_R & 0x09)==0)	  //Wait to be set
	
	//PortD digital output for rows
	GPIO_PORTD_CR_R |= 0x0F;								//Allow settings for all pins of PORTD
	GPIO_PORTD_AMSEL_R &= ~0x0F;						//Disable analog functionality
	GPIO_PORTD_PCTL_R &= ~0x0000FFFF;				//Enable digital functionality
	GPIO_PORTD_AFSEL_R &= ~0x0F;						//Disable alternate function select
	GPIO_PORTD_DIR_R |= 0x0F	;							//PD0-PD3 rows and set as digital output pins
	GPIO_PORTD_DEN_R |= 0x0F;								//Set PORTD as digital pins
	GPIO_PORTD_DATA_R &= ~0x0F;							//Clear data pins intially
	
	//PortA digital input for columns
	GPIO_PORTA_CR_R |= 0xF0;								//Allow settings for all pins of PORTA
	GPIO_PORTA_AMSEL_R &= ~0xF0;						//Disable analog functionality
	GPIO_PORTA_PCTL_R &= ~0xFFFF0000;				//Enable digital functionality
	GPIO_PORTA_AFSEL_R &= ~0xF0;						//Disable alternate function select
	GPIO_PORTA_DIR_R &= ~0xF0	;							//PA4-PA7 columns and set as digital input pins
	GPIO_PORTA_DEN_R |= 0xF0;								//Set PORTA as digital pins
	GPIO_PORTA_DATA_R &= ~0xF0;							//Clear data pins intially
	
}

// Function that makes 1 micro seconed delay
void systick_1us(){
   NVIC_ST_RELOAD_R = 16 - 1;    //frequency is 16MGHz
	 NVIC_ST_CURRENT_R = 0;
	 while (( NVIC_ST_CTRL_R & 0x00010000) == 0){}
}

// Function that makes multiple of micro seconed delay
void delay_uus(int time){  // the argument is no. of micro-seconds i.e delay(5) it will generate a 5-micro-sec delay
   int i;
	 for(i=0;i< time; i++){
	 systick_wait_1us();
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
    for(short i = 0; i < 4; i++)    //Scan columns 
    {
      GPIO_PORTD_DATA_R = (1U << i);
			delay_uus(5);									//Debounce delay

      for(short j = 0; j < 4; j++)  //Scan rows
      {
        if((GPIO_PORTA_DATA_R & 0xF0) == (1U << (j+4)))
          return keys[j][i];
      }
    }
  }
}



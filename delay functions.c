#include "tm4c123gh6pm.h"
#include "Io.h"
#include "stdlib.h"
#include "stdio.h"
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
	
// Function that makes 1 micro seconed delay
void systick_wait_1us(){
   NVIC_ST_RELOAD_R = 80 - 1;
	 NVIC_ST_CURRENT_R = 0;
	 while (( NVIC_ST_CTRL_R & 0x00010000) == 0){}
}

// Function that makes 1 ms delay
void systick_wait_1ms(){
   NVIC_ST_RELOAD_R = 80000 - 1;
	 NVIC_ST_CURRENT_R = 0;
	 while (( NVIC_ST_CTRL_R & 0x00010000) == 0){}
}

// Function that makes 1 sec or multiple of it  delay
void delay_s(int time){  // the argument is no. of seconds i.e delay(5) it will generate a 5-sec delay
	int i;
	int delay = time * 1000;
	 for(i = 0 ; i < delay ; i++){
	       systick_wait_1ms();
	 }	 
}

// Function that makes 1 min or multiple of it  delay
void delay_m(int time){    // the argument is no. of min i.e delay(5) it will generate a 5-min delay
   int i;
	 int delay = time * 60;
	 for(i = 0 ; i < delay ; i++){
	       delay_s(1);
	 }	 
}
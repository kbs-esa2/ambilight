#include <stdio.h>

#define switches 	(volatile int *) 	0x00021000
#define leds 		(int *) 			0x00021010
#define SDRAM		(volatile int *)	0x08000000

void main(){
	while(1){
		int command = (*switches & 0x030000)>>16;
		int value = (*switches & 0xFFFF);
		if(command&1){
			*SDRAM = value;
		}
		else if(command&3){
			*leds = *SDRAM;
		}
		
	}
 }

#include <stdio.h>

#define switches 	(volatile int *) 	0x00021030
#define keys 	(volatile int *) 	0x00021020
#define leds 		(int *) 			0x00021040

#define SDRAM		(volatile int *)	0x08000000
#define DMAINCONTROL		(volatile int *)	0x00021060
#define DMAOUTCONTROL		(volatile int *)	0x00021070
#define PIXELBUFFER		(volatile int *)	0x00200000

#define STREAMCONTROL0 (volatile int *)	0x00021010
#define STREAMCONTROL1 (volatile int *)	0x00021000

void main(){
	*leds = 0x00000000;
	while(1){

		int command = (*switches & 0x030000)>>16;
		int value = (*switches & 0xFFFF);
		
		if((*switches>>0)&1){
			*leds |= (1<<0);
			*STREAMCONTROL0 = 1;
		}
		else{
			*leds &= ~(1<<0);
			*STREAMCONTROL0 = 0;
		}
		if((*switches>>1)&1){
			*leds |= (1<<1);
			*STREAMCONTROL1 = 1;
		}
		else{
			*leds &= ~(1<<1);
			*STREAMCONTROL1 = 0;
		}
		
		
	}
 }

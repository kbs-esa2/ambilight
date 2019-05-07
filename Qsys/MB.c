#include <stdio.h>

#define switches 	(volatile int *) 	0x00021030
#define keys 	(volatile int *) 	0x00021020
#define leds 		(int *) 			0x00021040
#define PLLSlave		(volatile int *)	0x00021050

#define niosMemorySlave		(volatile int *)	0x18000000
#define DMAINCONTROL		(volatile int *)	0x00021060
#define DMAOUTCONTROL		(volatile int *)	0x00021070
#define sramBuffer		(volatile int *)	0x00000000
#define sdramBuffer		(volatile int *)	0x08000000

#define AVConfigSlave		(volatile int *)	0x00021080
#define JtagSlave		(volatile int *)	0x00021090

#define bufferBypassSplit (volatile int *)	0x00021010
#define bufferBypassMerge (volatile int *)	0x00021000
#define SourceSelect (volatile int *)	0x000210a0

void setBufferInputAdress(unsigned int adress);
void setBufferOutputAdress(unsigned int adress);

void setInput(int input);
void setBufferBypass(int input);

void main(){
	*leds = 0x00000000;
	setBufferInputAdress(0x08000000);
	setBufferOutputAdress(0x08100000);
	while(1){

		int command = (*switches & 0x030000)>>16;
		int value = (*switches & 0xFFFF);
		
		if((*switches>>0)&1){
			*leds |= (1<<0);
			setInput(1);
		}
		else{
			*leds &= ~(1<<0);
			setInput(0);
		}
		if((*switches>>1)&1){
			*leds |= (1<<1);
			setBufferBypass(1);
		}
		else{
			*leds &= ~(1<<1);
			setBufferBypass(0);
		}
		if((*switches>>2)&1){
			setBufferInputAdress(0x08000000);
			
		}
		if((*switches>>3)&1){
			setBufferInputAdress(0x08100000);
		}
		if((*switches>>4)&1){
			setBufferOutputAdress(0x08000000);
			
		}
		if((*switches>>5)&1){
			setBufferOutputAdress(0x08100000);
		}
		
		
	}
 }

 void setBufferInputAdress(unsigned int adress){
*(DMAINCONTROL+4) = adress;
*DMAINCONTROL = 0;
*(DMAINCONTROL+4) = adress;
 }

 void setBufferOutputAdress(unsigned int adress){
	 *(DMAOUTCONTROL+4) = adress;
*DMAOUTCONTROL = 0;
*(DMAOUTCONTROL+4) = adress;
 }

 void setInput(int input){
	 if (input == 1) {
		 *SourceSelect = 1;
	 }
	 else{
*SourceSelect = 0;
	 }
	 
 }

 void setBufferBypass(int input){
	 if (input == 1) {
		 *bufferBypassSplit = 1;
		 *bufferBypassMerge = 1;
	 }
	 else{
*bufferBypassSplit = 0;
*bufferBypassMerge = 0;
	 }
	 
 }

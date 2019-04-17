#define switches 	(volatile int *) 	0x00021000
#define leds 		(int *) 		0x00021010

void main(){
	while(1){
		*leds = *switches;
	}
 }

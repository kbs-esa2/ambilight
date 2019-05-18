#include <stdio.h>
#include <stdlib.h>

#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (int *)0x08221020
#define PLLSlave (volatile int *)0x00021050

#define niosMemorySlave (volatile int *)0x18000000
#define DMAINCONTROL (volatile int *)0x00021060
#define DMAOUTCONTROL (volatile int *)0x00021070
#define decoderBuffer (volatile char *)0x08000000
#define overlayBuffer (volatile long *)0x04000000
#define ledsBuffer (volatile char *)0x04050000

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090

#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240

void main()
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			*(ledsBuffer + (i * 4) + j) = 0x00000000;
		}
	}

	while (1)
	{

		for (int i = 0; i < 19; i++)
		{
			for (int k = 0; k < 19; k++)
			{
				for (int j = 0; j < 4; j++)
				{
					*(ledsBuffer + (k * 4) + j) = 0x00000000;
				}
			}
			*(ledsBuffer + (i * 4)) = 255;
			*(ledsBuffer + (i * 4) + 1) = 20;
			printf("sending leds");
		}
	}
}

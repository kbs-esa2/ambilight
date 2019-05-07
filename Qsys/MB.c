#include <stdio.h>
#include <stdlib.h>

#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (int *)0x08221020
#define PLLSlave (volatile int *)0x00021050

#define niosMemorySlave (volatile int *)0x18000000
#define DMAINCONTROL (volatile int *)0x00021060
#define DMAOUTCONTROL (volatile int *)0x00021070
#define backBuffer (volatile unsigned char *)0x08040000
#define frontBuffer (volatile unsigned char *)0x08000000

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090

#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240


void main()
{
	*leds = 0x00000000;
	while (1)
	{
		int frameSize = FRAMEHEIGHT*FRAMEWIDTH;

		if ((*switches >> 0) & 1)
		{
			*leds |= (1 << 0);
			for (int i = 0; i < 40; i++)
			{
				*(frontBuffer+i) = 255;
				*(frontBuffer+i*3+1) = 0;
				*(frontBuffer+i*3+2) = 0;
			}
		}
		else
		{
			*leds &= ~(1 << 0);
		}
		if ((*switches >> 1) & 1)
		{
			*leds |= (1 << 1);
			for (int i = 0; i < 40; i++)
			{
				*(frontBuffer+i) = 0;
				*(frontBuffer+i*3+1) = 255;
				*(frontBuffer+i*3+2) = 0;
			}
		}
		else
		{
			*leds &= ~(1 << 1);
		}

		if ((*switches >> 2) & 1)
		{
			*leds |= (1 << 2);
			for (int i = 0; i < 40; i++)
			{
				*(backBuffer+i) = 255;
				*(backBuffer+i+frameSize) = 0;
				*(backBuffer+i+frameSize*2) = 0;
			}
		}
		else
		{
			*leds &= ~(1 << 2);
		}

		if ((*switches >> 3) & 1)
		{
			*leds |= (1 << 3);
			for (int i = 0; i < 40; i++)
			{
				*(backBuffer+i) = 0;
				*(backBuffer+i+frameSize) = 255;
				*(backBuffer+i+frameSize*2) = 0;
			}
		}
		else
		{
			*leds &= ~(1 << 3);
		}
		
	}
}



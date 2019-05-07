#include <stdio.h>
#include <stdlib.h>

#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (int *)0x08221020
#define PLLSlave (volatile int *)0x00021050

#define niosMemorySlave (volatile int *)0x18000000
#define DMAINCONTROL (volatile int *)0x00021060
#define DMAOUTCONTROL (volatile int *)0x00021070
#define decoderBuffer (volatile short *)0x08000000
#define overlayBuffer (volatile long *)0x04000000

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090

#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240

void setPixel(unsigned int X, unsigned int Y, unsigned short R, unsigned short G, unsigned short B, unsigned short A);
void fillSquare(unsigned short R, unsigned short G, unsigned short B, unsigned short A, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
void fillBlack();
void fillClear();
void getPixelColor(unsigned short* out,unsigned int X,unsigned int Y);

unsigned short pixel[3];

void main()
{
	*leds = 0x00000000;
	fillClear();

	while (1)
	{
		int frameSize = FRAMEHEIGHT * FRAMEWIDTH;

		if ((*switches >> 0) & 1)
		{
			*leds |= (1 << 0);
			fillBlack();
		}
		else
		{
			*leds &= ~(1 << 0);
		}
		if ((*switches >> 1) & 1)
		{
			*leds |= (1 << 1);
			fillClear();
		}
		else
		{
			*leds &= ~(1 << 1);
		}

		if ((*switches >> 2) & 1)
		{
			*leds |= (1 << 2);
			for (unsigned int i = 0; i < 20; i++)
			{
				setPixel(i, 20, 0, 255, 0, 255);
			}
		}
		else
		{
			*leds &= ~(1 << 2);
		}

		if ((*switches >> 3) & 1)
		{
			*leds |= (1 << 3);
			for (unsigned int i = 0; i < 20; i++)
			{
				setPixel(i, 0, 0, 0, 255, 255);
			}
		}
		else
		{
			*leds &= ~(1 << 3);
		}

		if ((*switches >> 4) & 1)
		{
			*leds |= (1 << 4);
			for (unsigned int i = 0; i < 20; i++)
			{
				setPixel(i, 255, 255, 0, 0, 255);
			}
		}
		else
		{
			*leds &= ~(1 << 4);
		}
		unsigned int targetX = 160;
		unsigned int targetY = 120;
		setPixel(targetX-1,targetY-1,0,255,0,255);
		setPixel(targetX-1,targetY+1,0,255,0,255);
		setPixel(targetX+1,targetY-1,0,255,0,255);
		setPixel(targetX+1,targetY+1,0,255,0,255);
		getPixelColor(pixel,targetX,targetY);
		fillSquare(pixel[0], pixel[1], pixel[2], 255, 20, 20, 0, 0);
	}
}

void setPixel(unsigned int X, unsigned int Y, unsigned short R, unsigned short G, unsigned short B, unsigned short A)
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int offset = 0;
	unsigned long pixelData = 0;
	//clip coordinate values
	if (X < 0)
		x = 0;
	else if (X > (FRAMEWIDTH - 1))
		x = FRAMEWIDTH - 1;
	else
		x = X;

	if (Y < 0)
		y = 0;
	else if (Y > (FRAMEHEIGHT - 1))
		y = FRAMEHEIGHT - 1;
	else
		y = Y;

	offset = (y * FRAMEWIDTH) + x;

	pixelData |= B & 0x000000FF;
	pixelData |= (G & 0x000000FF) << 8;
	pixelData |= (R & 0x000000FF) << 16;
	pixelData |= (A & 0x000000FF) << 24;

	//write pixeldata to buffer
	*(overlayBuffer + offset) = pixelData;
}

void fillBlack()
{
	for (unsigned int j = 0; j < FRAMEHEIGHT; j++)
	{
		for (unsigned int i = 0; i < FRAMEWIDTH; i++)
		{
			setPixel(i, j, 0, 0, 0, 255);
		}
	}
}

void fillClear()
{
	for (unsigned int j = 0; j < FRAMEHEIGHT; j++)
	{
		for (unsigned int i = 0; i < FRAMEWIDTH; i++)
		{
			setPixel(i, j, 0, 0, 0, 0);
		}
	}
}

void fillSquare(unsigned short R, unsigned short G, unsigned short B, unsigned short A, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			setPixel(x + i, y + j, R, G, B, A);
		}
	}
}

void getPixelColor(unsigned short* out,unsigned int X,unsigned int Y){
	unsigned short R = 0;
	unsigned short G = 0;
	unsigned short B = 0;
	unsigned int offset = (Y*FRAMEWIDTH+X)*3;

	R = *(decoderBuffer+offset);
	G = *(decoderBuffer+offset+1);
	B = *(decoderBuffer+offset+2);

	out[0] = R;
	out[1] = G;
	out[2] = B;

}

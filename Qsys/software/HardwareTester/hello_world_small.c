/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "sys/alt_stdio.h"

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

#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240

#define LedsTopBot 30
#define LedsLeftRight 21

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090

#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240

void setPixel(unsigned int X, unsigned int Y, unsigned short R, unsigned short G, unsigned short B, unsigned short A);
void fillSquare(unsigned short R, unsigned short G, unsigned short B, unsigned short A, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
void fillBlack();
void fillClear();
void getPixelColor(unsigned short* out,unsigned int X,unsigned int Y);
void drawSquare(unsigned int x, unsigned int y);

unsigned short pixel[3];

int main()
{
	*leds = 0x00000000;
	fillClear();
	alt_putstr("Hello from Nios II!\n");
	while (1)
	{
		for(unsigned int xcoordinate = 0; xcoordinate <= FRAMEWIDTH; xcoordinate = xcoordinate + (FRAMEWIDTH/LedsTopBot))
		      drawSquare(xcoordinate, 0);
		      printf("for1\n");
		      for(unsigned int xcoordinate = 0; xcoordinate <= FRAMEWIDTH; xcoordinate = xcoordinate + (FRAMEWIDTH/LedsTopBot))
		      drawSquare(xcoordinate, 240 -(FRAMEHEIGHT/LedsLeftRight));
		      printf("for2\n");
		      for(unsigned int ycoordinate = 0; ycoordinate <= FRAMEHEIGHT; ycoordinate = ycoordinate + (FRAMEHEIGHT/LedsLeftRight))
		      drawSquare(0, ycoordinate);
		      printf("for3\n");
		      for(unsigned int ycoordinate = 0; ycoordinate <= FRAMEHEIGHT; ycoordinate = ycoordinate + (FRAMEHEIGHT/LedsLeftRight))
		      drawSquare(320 - (FRAMEWIDTH/LedsTopBot), ycoordinate);
		      printf("for4\n");
		      printf("drawoverlay\n");

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
	return 0;
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

void drawSquare(unsigned int x, unsigned int y) {   //x & y are coordinates
  for(unsigned int i; i <= (FRAMEHEIGHT/LedsLeftRight); i++)
  setPixel(x, y+i, 247, 234, 51, 255);
  for(unsigned int i; i <= (FRAMEHEIGHT/LedsLeftRight); i++)
  setPixel(x+(FRAMEWIDTH/LedsTopBot), y+i, 247, 234, 51, 255);
  for(unsigned int i; i <= (FRAMEWIDTH/LedsTopBot); i++)
  setPixel(x+i, y, 247, 234, 51, 255);
  for(unsigned int i; i <= (FRAMEWIDTH/LedsTopBot); i++)
  setPixel(x+i, y+(FRAMEHEIGHT/LedsLeftRight), 247, 234, 51, 255);
}

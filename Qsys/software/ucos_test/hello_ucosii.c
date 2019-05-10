/* UCOSII for NIOS, KBSESA2, Ambilight-waproject */
#include <stdio.h>
#include "includes.h"

/* Define adresses & frame dimensions */
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

//!!!1!!1!!! Define voor test
#define targetX 160
#define targetY 120

/* Define Task stacksize */
#define   TASK_STACKSIZE       2048
OS_STK    TaskCounter_stk[TASK_STACKSIZE];
OS_STK    TaskFillSquare_stk[TASK_STACKSIZE];
OS_STK    TaskGetColor_stk[TASK_STACKSIZE];

/* Define Task priority */
#define TaskCounter_PRIORITY      1
#define TaskFillSquare_PRIORITY      2
#define TaskGetColor_PRIORITY   3

/* Functions */
void fillSquare(unsigned char R, unsigned char G, unsigned char B, unsigned char A, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
void getPixelColor(unsigned short* out,unsigned int X,unsigned int Y);
void setPixel(unsigned int X, unsigned int Y, unsigned char R, unsigned char G, unsigned char B, unsigned char A);

/* Tasks */
void TaskCounter(void* pdata) {
  int counter = 0;
  while (1) { 
    printf("Counter: %d\n", counter);
    counter++;
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

void TaskFillSquare(void* pdata) {
  while (1) { 
    printf("FillSquareTask\n");
    OSTimeDlyHMSM(0, 0, 2, 0);
  }
}

void TaskGetColor(void* pdata) {
  while (1) { 
    printf("Get Color Task\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}

/* Main function, creates tasks and starts the scheduler */
int main(void) {
  
  OSTaskCreateExt(TaskCounter,
                  NULL,
                  (void *)&TaskCounter_stk[TASK_STACKSIZE-1],
                  TaskCounter_PRIORITY,
                  TaskCounter_PRIORITY,
                  TaskCounter_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(TaskFillSquare,
                  NULL,
                  (void *)&TaskFillSquare_stk[TASK_STACKSIZE-1],
                  TaskFillSquare_PRIORITY,
                  TaskFillSquare_PRIORITY,
                  TaskFillSquare_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(TaskGetColor,
                  NULL,
                  (void *)&TaskGetColor_stk[TASK_STACKSIZE-1],
                  TaskGetColor_PRIORITY,
                  TaskGetColor_PRIORITY,
                  TaskGetColor_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}

void fillSquare(unsigned char R, unsigned char G, unsigned char B, unsigned char A, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			setPixel(x + i, y + j, R, G, B, A);
		}
	}
}

void getPixelColor(unsigned short* out,unsigned int X,unsigned int Y) {
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned int offset = (Y*FRAMEWIDTH+X)*3;

	R = *(decoderBuffer+offset);
	G = *(decoderBuffer+offset+1);
	B = *(decoderBuffer+offset+2);

	out[0] = R;
	out[1] = G;
	out[2] = B;
}

void setPixel(unsigned int X, unsigned int Y, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
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
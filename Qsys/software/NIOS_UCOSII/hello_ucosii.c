/* UCOSII for NIOS, KBSESA2, Ambilight-waproject */
#include <stdio.h>
#include <math.h>
#include "includes.h"

/* Define adresses & frame dimensions */
#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (int *)0x08221020
#define PLLSlave (volatile int *)0x00021050

#define niosMemorySlave (volatile int *)0x18000000
#define DMAINCONTROL (volatile int *)0x00021060
#define DMAOUTCONTROL (volatile int *)0x00021070
#define decoderBuffer (volatile char *)0x08000000
#define overlayBuffer (volatile long *)0x04000000

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090


//!!!1!!1!!! Define voor test
#define targetX frameWidth/2
#define targetY frameHeight/2

/* Dimensions */
#define LedsTop 31        //Amount of leds on the top side of the screen
#define LedsBot 24        //Amount of leds on the bottom side of the screen
#define LedsLeft 20       //Amount of leds on the left side of the screen
#define LedsRight 20      //Amount of leds on the left right side of the screen


/* Define Task stacksize */
#define   TASK_STACKSIZE       4096
OS_STK    TaskCounter_stk[TASK_STACKSIZE];
OS_STK    TaskFillSquare_stk[TASK_STACKSIZE];
OS_STK    TaskGetColor_stk[TASK_STACKSIZE];
OS_STK    TaskSetOverlay_stk[TASK_STACKSIZE];

/* Define Task priority */
#define TaskCounter_PRIORITY      4
#define TaskFillSquare_PRIORITY   3
#define TaskGetColor_PRIORITY     2
#define TaskSetOverlay_PRIORITY   1

/* Functions */
void calibrate();
void drawOverlay(unsigned char transparency);
void drawSquare(unsigned int x, unsigned int y, unsigned char ledsX, unsigned char ledsY, unsigned char transparency);
void fillClear();
void fillSquare(unsigned char R, unsigned char G, unsigned char B, unsigned char A, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
void getPixelColor(unsigned char* out,unsigned int X,unsigned int Y);
void setPixel(unsigned int X, unsigned int Y, unsigned char R, unsigned char G, unsigned char B, unsigned char A);


/* Global variables*/
unsigned char pixel[3];

//Sizes of blocks, 
unsigned int cornerWidthTop = 0;
unsigned int cornerWidthBot = 0;
unsigned int cornerHeight = 0;
unsigned int frameBlockTopWidth = 0;
unsigned int frameBlockLeftRightHeight = 0;
unsigned int frameBlockBotWidth = 0;



//Size of frame and empty black screen on the monitor, adjusted through calibration
unsigned int frameWidth = 320;
unsigned int frameHeight = 240;
unsigned int emptyHeightTop = 0;
unsigned int emptyHeightBot = 0;
unsigned int emptyWidthLeft = 0;
unsigned int emptyWidthRight = 0;

/* Tasks */
void TaskCounter(void* pdata) {
  int counter = 0;
  while (1) { 
    //printf("Counter: %d\n", counter);
    counter++;
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

void TaskFillSquare(void* pdata) {
  while (1) {
    fillSquare(pixel[0], pixel[1], pixel[2], 255, 9, 9, 1, 1);
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
}

void TaskGetColor(void* pdata) {
  while (1) {
    setPixel(targetX-1,targetY-1,0,255,0,255);
    setPixel(targetX-1,targetY+1,0,255,0,255);
    setPixel(targetX+1,targetY-1,0,255,0,255);
    setPixel(targetX+1,targetY+1,0,255,0,255);
    getPixelColor(pixel,targetX,targetY); 
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
}

void TaskSetOverlay(void* pdata) {
  unsigned char overlayStatus = 0;
  while(1) {
    if ((*switches >> 0) & 1) {   
      if (!overlayStatus) {
        *leds |= (1 << 0);
        drawOverlay(255);
        overlayStatus = 1;
      }
		} else {
      if (overlayStatus) {
        *leds &= ~(1 << 0);  
        drawOverlay(0);
        overlayStatus = 0;
      } 
		}
    OSTimeDlyHMSM(0,0,0,300);
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

  OSTaskCreateExt(TaskSetOverlay,
                  NULL,
                  (void *)&TaskSetOverlay_stk[TASK_STACKSIZE-1],
                  TaskSetOverlay_PRIORITY,
                  TaskSetOverlay_PRIORITY,
                  TaskSetOverlay_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  fillClear();
  calibrate();
  OSStart();
  return 0;
}

/* Calibrates width and height of the screen and the squares */
void calibrate() {
  /*--- Calibrate frame dimensions ---*/
  unsigned char emptyFrameColor[3];
  unsigned int i = 0;

  //Set dimensions to standard max
  frameHeight = 240;
  frameWidth = 320;

  //Check sizes of all 4 sides
  while(!emptyHeightTop) {
    getPixelColor(emptyFrameColor, (frameWidth/2), i);
    if(emptyFrameColor[0] < 100 && emptyFrameColor[1] < 100 && emptyFrameColor[2] < 100) i++;
    else emptyHeightTop = i;
  }
  i = 1;
  while(!emptyHeightBot) {
    getPixelColor(emptyFrameColor, (frameWidth/2), 240 - i);
    if(emptyFrameColor[0] < 100 && emptyFrameColor[1] < 100 && emptyFrameColor[2] < 100) i++;
    else emptyHeightBot = i;
  }
  i = 0;
  while(!emptyWidthLeft) {
    getPixelColor(emptyFrameColor, i, (frameHeight/2));
    if(emptyFrameColor[0] < 100 && emptyFrameColor[1] < 100 && emptyFrameColor[2] < 100) i++;
    else emptyWidthLeft = i;
  }
  i = 0;
  while(!emptyWidthRight) {
    getPixelColor(emptyFrameColor, frameWidth - i, (frameHeight/2));
    if(emptyFrameColor[0] < 100 && emptyFrameColor[1] < 100 && emptyFrameColor[2] < 100) i++;
    else emptyWidthRight = i;
  }

  //Calculate the dimensions of the frame
  frameWidth = frameWidth - emptyWidthLeft - emptyWidthRight;
  frameHeight = frameHeight - emptyHeightTop - emptyHeightBot;

  /*--- Calibrate sizes of overlay blocks ---*/
  

}

/* Draw the overlay witch DrawSquare, transparency set to 255 or 0, for on or off */
void drawOverlay(unsigned char transparency) {



  // HIERBOVEN NOG 4 VIERKANTJES IN ALLE HOEKEN!
  for(unsigned int x = 0; x < frameWidth; x += frameWidth/LedsTop) drawSquare(emptyWidthLeft + x, emptyHeightTop, LedsTop, LedsLeft, transparency);
  printf("1\n");
  for(unsigned int x = 0 + (frameWidth/LedsBot); x < (frameWidth - frameWidth/LedsBot); x += frameWidth/LedsBot) drawSquare(x + emptyWidthLeft, (emptyHeightTop + frameHeight - squareHeight), LedsBot, LedsLeft, transparency);
  printf("2\n");
  for(unsigned int y = 0; y < frameHeight; y += (frameHeight/LedsLeft)) drawSquare(emptyWidthLeft, emptyHeightTop + y, LedsTop, LedsLeft, transparency);
  printf("3\n");
  for(unsigned int y = 0; y <  frameHeight; y += (frameHeight/LedsRight)) drawSquare((emptyWidthLeft + frameWidth - squareWidth), emptyHeightTop + y, LedsBot, LedsRight, transparency);
  printf("4\n");
}

/* Draw one square, representing the area from where data is received */
void drawSquare(unsigned int x, unsigned int y, unsigned char ledsX, unsigned char ledsY, unsigned char transparency) {   //x & y are coordinates
  for(int i = 0; i <= frameWidth/ledsX; i++) setPixel(x+i, y, 244, 238, 66, transparency);
  for(int i = 0; i <= frameWidth/ledsX; i++) setPixel(x+i, y+squareHeight, 244, 238, 66, transparency);
  for(int i = 0; i <= frameHeight/ledsY; i++) setPixel(x, y+i, 244, 238, 66, transparency);
  for(int i = 0; i <= frameHeight/ledsY; i++) setPixel(x+squareWidth, y+i, 244, 238, 66, transparency);
}

/* Clears the screen of (noise)pixels */
void fillClear() {
  for (unsigned int i = 0; i <= frameHeight; i++) {
    for (unsigned int j = 0; j <= frameWidth; j++) {
      setPixel(j , i, 0, 0, 0, 0);
    }
  }
}

/* Fills a square with RGB values taken from an array */
void fillSquare(unsigned char R, unsigned char G, unsigned char B, unsigned char A, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			setPixel(x + i, y + j, R, G, B, A);
		}
	}
}

/* get color of a pixel, put RGB values in array */
void getPixelColor(unsigned char* out, unsigned int X, unsigned int Y) {
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned int offset = (Y*frameWidth+X)*4;

	R = *(decoderBuffer+offset);
	G = *(decoderBuffer+offset+1);
	B = *(decoderBuffer+offset+2);

	out[0] = B;
	out[1] = G;
	out[2] = R;
}

/* Draw one pixel */
void setPixel(unsigned int X, unsigned int Y, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int offset = 0;
	unsigned long pixelData = 0;
	//clip coordinate values
	if (X < 0) x = 0;
	else if (X > (320 - 1)) x = 320 - 1;
	else x = X;

	if (Y < 0) y = 0;
	else if (Y > (240 - 1)) y = 240 - 1;
	else y = Y;

	offset = (y * 320) + x;

  //Write to pixelData
	pixelData |= B & 0x000000FF;
	pixelData |= (G & 0x000000FF) << 8;
	pixelData |= (R & 0x000000FF) << 16;
	pixelData |= (A & 0x000000FF) << 24;

	//write pixelData to buffer
	*(overlayBuffer + offset) = pixelData;
}

/*

int cornerWidthTop;
int cornerWidthBot;
int cornerHeight;
int frameBlockTopWidth;
int frameBlockLeftRightHeight;
int frameBlockBotWidth;

//breedte bovenste hoeken.
int temp = FRAMEWIDTH / ledsTop;
temp = FRAMEWIDTH - (temp * 2);
frameBlockTopWidth = temp % ledsTop;
cornerWidthTop = (FRAMEWIDTH - ((ledsTop - 2) * frameBlockTopWidth)) / 2;

//hoogte alle hoeken
temp = FRAMEHEIGHT / ledsLeftRight;
temp = FRAMEHEIGHT - (temp * 2);
frameBlockLeftRightHeight = temp % ledsLeftRight;
cornerHeight = (FRAMEHEIGHT - ((ledsLeftRight - 2) * frameBlockLeftRightHeight)) / 2 ;

//breedte onderste hoeken
temp = FRAMEWIDTH / ledsBot;
temp = FRAMEWIDTH - (temp * 2);
frameBlockBotWidth = temp % ledsTop;
cornerWidthBot = (FRAMEWIDTH - ((ledsBot - 2) * frameBlockBotWidth)) / 2;
/* UCOSII for NIOS, KBSESA2, Ambilight-waproject */
#include <stdio.h>
#include "includes.h"

/* Type definitions */
typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} color;

typedef struct {
  unsigned int id;
  unsigned int X;
  unsigned int Y;
  unsigned int Width;
  unsigned int Height;
} block;

typedef struct {
  unsigned int X;
  unsigned int Y;
  unsigned int Width;
  unsigned int Height;
  unsigned int size;
  unsigned int numLeds;
  block pixels[50];
} edge;

/* Define adresses & frame dimensions */
#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (int *)0x08221020
#define PLLSlave (volatile int *)0x00021050

#define niosMemorySlave (volatile int *)0x18000000
#define DMAINCONTROL (volatile int *)0x00021060
#define DMAOUTCONTROL (volatile int *)0x00021070
#define decoderBuffer (color *)0x08000000
#define overlayBuffer (color *)0x04000000
#define ledBuffer (color *)0x04050000

#define AVConfigSlave (volatile int *)0x00021080
#define JtagSlave (volatile int *)0x00021090


//!!!1!!1!!! Define voor test
#define targetX frameWidth/2
#define targetY frameHeight/2

/* Dimensions */
#define FRAMEWIDTH 320
#define FRAMEHEIGHT 240
#define LEDSTOP 31        //Amount of leds on the top side of the screen
#define LEDSBOTTOM 24        //Amount of leds on the bottom side of the screen
#define LEDSLEFT 20       //Amount of leds on the left side of the screen
#define LEDSRIGHT 20      //Amount of leds on the left right side of the screen


/* Define Task stacksize */
#define   TASK_STACKSIZE       4096
OS_STK    TaskCounter_stk[TASK_STACKSIZE];
OS_STK    TaskFillSquare_stk[TASK_STACKSIZE];
OS_STK    TaskGetColor_stk[TASK_STACKSIZE];
OS_STK    TaskSetOverlay_stk[TASK_STACKSIZE];
OS_STK    TaskLedRotate_stk[TASK_STACKSIZE];

/* Define Task priority */
#define TaskCounter_PRIORITY      2
#define TaskFillSquare_PRIORITY   2
#define TaskGetColor_PRIORITY     2
#define TaskSetOverlay_PRIORITY   2
#define TaskLedRotate_PRIORITY    1




/* Functions */
void fillClear();
void fillSquare(color pixel, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
color getPixelColor(unsigned int X, unsigned int Y);
void setPixel(unsigned int X, unsigned int Y, color pixel);
void setLed(unsigned char index, color pixel);
void drawTaskBar();
color colorFromHex(unsigned int in);
void drawBlockBorder(block b,color c);
void calculateEdgeBlocks(block frame);

/* Global variables*/
color readPixel = {0,0,0,0};
edge topEdge;
edge bottomEdge;
edge leftEdge;
edge rightEdge;
block inputframe = {0,0,0,320,238};

/* Global colors */
color borderColor = {0,255,255,255};

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
    readPixel.alpha = 255;
    fillSquare(readPixel, 9, 9, 1, 1);
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
}

void TaskGetColor(void* pdata) {
  
  while (1) {
    color targetColor = {0,255,0,255};
    setPixel(160-1,120-1,targetColor);
    setPixel(160-1,120+1,targetColor);
    setPixel(160+1,120-1,targetColor);
    setPixel(160+1,120+1,targetColor);
    readPixel = getPixelColor(160,120);
    color c = {0,255,0,255};
    setLed(0,c );
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
}

void TaskSetOverlay(void* pdata) {
  unsigned char overlayStatus = 0;
  while(1) {
    if ((*switches >> 0) & 1) {   
      if (!overlayStatus) {
        *leds |= (1 << 0);
        overlayStatus = 1;
      }
		} else {
      if (overlayStatus) {
        *leds &= ~(1 << 0);
        overlayStatus = 0;
      } 
		}
    OSTimeDlyHMSM(0,0,0,300);
  }
}

void TaskLedRotate(void* pdata){
  color off = {0,0,0,0};
  color on = {255,255,255,255};
  while (1)
  {
    
    for (unsigned int i = 0; i < 19; i++)
    {
      for (unsigned int j = 0; j < 19; j++)
      {
        
        setLed(j,off);
      }
      setLed(i,on);
      OSTimeDlyHMSM(0,0,0,200);
    }
    
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

  OSTaskCreateExt(TaskLedRotate,
                  NULL,
                  (void *)&TaskLedRotate_stk[TASK_STACKSIZE-1],
                  TaskLedRotate_PRIORITY,
                  TaskLedRotate_PRIORITY,
                  TaskLedRotate_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  fillClear();
  //drawTaskBar();
  topEdge.size = 10;
  leftEdge.size = 10;
  bottomEdge.size = 10;
  rightEdge.size = 10;
  drawBlockBorder(inputframe,borderColor);
  calculateEdgeBlocks(inputframe);
  for (unsigned char i = 0; i < topEdge.numLeds; i++)
  {
      drawBlockBorder(topEdge.pixels[i],colorFromHex(0xff0000ff));
  }
  for (unsigned char i = 0; i < leftEdge.numLeds; i++)
  {
      drawBlockBorder(leftEdge.pixels[i],colorFromHex(0x00ff00ff));
  }

  //OSStart();
  return 0;
}


/* Clears the screen of (noise)pixels */
void fillClear() {
  color clear = {0,0,0,0};
  for (unsigned int i = 0; i <= frameHeight; i++) {
    for (unsigned int j = 0; j <= frameWidth; j++) {
      setPixel(j , i, clear);
    }
  }
  for (unsigned int j = 0; j <= 19; j++) {
      color redish = {255,20,0,0};
      setLed(j,redish);
    }
}

/* Fills a square with RGB values taken from an array */
void fillSquare(color pixel, unsigned int width, unsigned int height, unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			setPixel(x + i, y + j, pixel);
		}
	}
}

/* get color of a pixel, put RGB values in array */
color getPixelColor(unsigned int X, unsigned int Y) {
	unsigned int offset = (Y*frameWidth+X);
  return *(decoderBuffer+offset);
}

/* Draw one pixel */
void setPixel(unsigned int X, unsigned int Y, color pixel) {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int offset = 0;
	//clip coordinate values
	if (X < 0) return;
	else if (X >= 320) return;
	else x = X;

	if (Y < 0) return;
	else if (Y >= 240) return;
	else y = Y;

  //calculate buffer offset
	offset = (y * 320) + x;

  
	//write pixelData to buffer
	*(overlayBuffer + offset) = pixel;
}

/* Change the color of a led */
void setLed(unsigned char index, color pixel){
 *(ledBuffer + (index)) = pixel;
}

/* draw taskbar at top of screen */
void drawTaskBar(){
  color background = colorFromHex(0x8f8f8faf);
  fillSquare(background,frameWidth,30,0,0);
}

//get an byte array of type color from 32 bit integer
color colorFromHex(unsigned int in){
  color out = {0,0,0,0};
  out.red |= (in>>24) & 0xFF;
  out.green |= (in>>16) & 0xFF;
  out.blue |= (in>>8) & 0xFF;
  out.alpha |= (in>>0) & 0xFF;
  return out;
}

void drawBlockBorder(block b,color c){
  //draw top row
  for (int i = 0; i < b.Width; i++)
  {
    setPixel((b.X + i), b.Y, c);
  }
  //draw bottom row
  for (int i = 0; i < b.Width; i++)
  {
    setPixel((b.X + i), (b.Y + b.Height - 1), c);
  }
  //draw left row
  for (int i = 0; i < b.Height; i++)
  {
    setPixel(b.X, (b.Y + i), c);
  }
  //draw right row
  for (int i = 0; i < b.Height; i++)
  {
    setPixel((b.X + b.Width  - 1), (b.Y + i), c);
  }
  
}

/* calculate all the edge blocks sizes */
//corner pixels (like top left) are included in side edges
//left and right pixel index is from top to bottom 
void calculateEdgeBlocks(block frame){
  //calculate top edge
  int topBlockSize = (frame.Width/(LEDSTOP + 2));
  int leftBlockSize = (frame.Height/LEDSLEFT);
  int bottomBlockSize = (frame.Width/(LEDSBOTTOM + 2));
  int rightBlockSize = (frame.Height/LEDSRIGHT);
  topEdge.X = frame.X;
  topEdge.Y = frame.Y;
  topEdge.Width = frame.Width;
  topEdge.Height = topEdge.size;
  topEdge.numLeds = LEDSTOP;
  for (unsigned char i = 0; i < topEdge.numLeds; i++)
  {
    topEdge.pixels[i].id = LEDSLEFT + i + 1;
    topEdge.pixels[i].X = (frame.Width - topBlockSize * LEDSTOP)/2 + (topBlockSize * i);
    topEdge.pixels[i].Y = topEdge.Y;
    topEdge.pixels[i].Width = topBlockSize;
    topEdge.pixels[i].Height = topEdge.size;
  }
  printf("top block width: %d\n",topBlockSize);
  printf("number of blocks: %d\n",LEDSTOP);
printf("left block height: %d\n",leftBlockSize);
  printf("number of blocks: %d\n",LEDSLEFT);
  printf("bottom block width: %d\n",bottomBlockSize);
  printf("number of blocks: %d\n",LEDSBOTTOM);
  printf("right block height: %d\n",rightBlockSize);
  printf("number of blocks: %d\n",LEDSRIGHT);
  //calculate left edge  
  leftEdge.X = 0;
  leftEdge.Y = 0;
  leftEdge.Width = leftEdge.size;
  leftEdge.Height = frame.Height;
  leftEdge.numLeds = LEDSLEFT;
  for (unsigned char i = 0; i < leftEdge.numLeds; i++)
  {
    leftEdge.pixels[i].id = LEDSLEFT - i;
    leftEdge.pixels[i].X = leftEdge.X;
    if (i == 0)
    {
      leftEdge.pixels[i].Y = 0;
      leftEdge.pixels[i].Width = (frame.Width - (topBlockSize * LEDSTOP))/2;
      leftEdge.pixels[i].Height = (leftEdge.Height - leftBlockSize * (LEDSLEFT - 2))/2;
    }
    else if (i == (LEDSLEFT - 1))
    {
      leftEdge.pixels[i].Y = leftEdge.pixels[0].Height + (leftBlockSize * (i - 1));
      leftEdge.pixels[i].Width = (frame.Width - (bottomBlockSize * LEDSBOTTOM))/2;
      leftEdge.pixels[i].Height = frame.Height - (leftBlockSize * (LEDSLEFT - 2)) - leftEdge.pixels[0].Height;
    }    
    else{
      leftEdge.pixels[i].Y = leftEdge.pixels[0].Height + (leftBlockSize * (i - 1));
    leftEdge.pixels[i].Width = leftEdge.size;
    leftEdge.pixels[i].Height = leftBlockSize;
    }    
    
  }
  printf("bottomleft height: %d width: %d\n",leftEdge.pixels[19].Height,leftEdge.pixels[19].Width);
  //calculate bottom edge
  bottomEdge.X = frame.Width - (bottomBlockSize * LEDSBOTTOM);
  bottomEdge.Y = frame.Height - bottomEdge.size;
  bottomEdge.Width = frame.Width - (bottomBlockSize * LEDSBOTTOM);
  bottomEdge.Height = bottomEdge.size;
  bottomEdge.numLeds = LEDSTOP;
  for (unsigned char i = 0; i < bottomEdge.numLeds; i++)
  {
    bottomEdge.pixels[i].id = LEDSLEFT + i + 1;
    bottomEdge.pixels[i].X = (frame.Width - topBlockSize * LEDSTOP)/2 + (topBlockSize * i);
    bottomEdge.pixels[i].Y = bottomEdge.Y;
    bottomEdge.pixels[i].Width = topBlockSize;
    bottomEdge.pixels[i].Height = bottomEdge.size;
  }
  
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
*/

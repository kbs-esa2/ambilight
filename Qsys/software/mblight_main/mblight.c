/* UCOSII for NIOS, KBSESA2, Ambilight-project */
#include <stdio.h>
#include "includes.h"

/* Type definitions */
typedef unsigned char byte;

typedef struct {
  byte red;
  byte green;
  byte blue;
  byte alpha;
} color;

typedef struct {
  unsigned int id;
  unsigned int X;
  unsigned int Y;
  unsigned int Width;
  unsigned int Height;
  color average;
} block;

typedef struct {
  unsigned int X;
  unsigned int Y;
  unsigned int Width;
  unsigned int Height;
  unsigned int size;
  unsigned int numLeds;
  block frameBlock[50];
} edge;

/* Define adresses & frame dimensions */
#define switches (volatile int *)0x08221010
#define keys (volatile int *)0x08221000
#define leds (volatile int *)0x08221020
#define PLLSlave (volatile int *)0x08221030
#define ucos_timer (volatile int *)0x08200020

#define niosMemorySlave (volatile int *)0x18000000
#define callibrationframe (block *)0x040B0000
#define DMAINCONTROL (volatile int *)0x08221040
#define DMAOUTCONTROL (volatile int *)0x08221050
#define decoderBuffer (color *)0x08000000
#define overlayBuffer (color *)0x04000000
#define ledBuffer (color *)0x04050000
#define brightnessBuffer (volatile int *)0x08200040

#define AVConfigSlave (volatile int *)0x08221060
#define JtagSlave (volatile int *)0x08221070

/* Dimensions */
#define FRAMEWIDTH 320    
#define FRAMEHEIGHT 240
#define LEDSTOP 31    //Amount of leds on the top side of the screen
#define LEDSBOTTOM 24 //Amount of leds on the bottom side of the screen
#define LEDSLEFT 20   //Amount of leds on the left side of the screen
#define LEDSRIGHT 20  //Amount of leds on the left right side of the screen

/* Define Task stacksize */
#define TASK_STACKSIZE 4096
OS_STK TaskGetColor_stk[TASK_STACKSIZE];
OS_STK TaskSetOverlay_stk[TASK_STACKSIZE];
OS_STK TaskWriteLed_stk[TASK_STACKSIZE];

/* Define Task priority */
#define TaskGetColor_PRIORITY 1
#define TaskSetOverlay_PRIORITY 3
#define TaskWriteLed_PRIORITY 2

/* Functions */
void calibrate();
void fillClear();
void fillSquare(color pixel, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void drawBlockBorder(block b, color c);
void calculateEdgeBlocks(block frame);

color getPixelColor(unsigned int X, unsigned int Y);
void setPixel(unsigned int X, unsigned int Y, color pixel);
void setLed(byte index, color pixel);
color colorFromHex(unsigned int in);
byte getPixelLuminance(color p);

void getAverages();
void averageToLeds();
void averageToBlocks();

/* Global variables*/
color readPixel = {0, 0, 0, 0};
edge topEdge;
edge bottomEdge;
edge leftEdge;
edge rightEdge;
block inputframe;

/* Global colors */
color borderColor = {0, 255, 255, 255};

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
void TaskGetColor(void *pdata) {
  color testc2 = {0, 0, 255, 255};
  while (1) {

    /*  TEST */
    color testc = {0, 255, 0, 200};
    byte testx = 160;
    byte testy = 120;

    setPixel(testx-1, testy, testc);
    setPixel(testx+1, testy, testc);
    setPixel(testx, testy-1, testc);
    setPixel(testx, testy+1, testc);
    testc2 = getPixelColor(testx, testy);
   for (byte i = 0; i < 96; i++) setLed(i, testc2);
//    /* END TEST */

    //getAverages();
    
    OSTimeDlyHMSM(0, 0, 0, 200);
  }
}

void TaskWriteLed(void *pdata) {
  while (1) {
    //averageToLeds();
    //printf("TaskWriteLed!\n");
    OSTimeDlyHMSM(0, 0, 2, 100);
  }
}

void TaskSetOverlay(void *pdata) {
  byte overlayStatus = 0;
  while (1) {
    if ((*switches >> 0) & 1) {
      if (!overlayStatus) { //Draw overlay with different colors on each side
        *leds |= (1 << 0);
        for (byte i = 0; i < topEdge.numLeds; i++) drawBlockBorder(topEdge.frameBlock[i], colorFromHex(0xff0000ff));
        for (byte i = 0; i < leftEdge.numLeds; i++) drawBlockBorder(leftEdge.frameBlock[i], colorFromHex(0x00ff00ff));
        for (byte i = 0; i < bottomEdge.numLeds; i++) drawBlockBorder(bottomEdge.frameBlock[i], colorFromHex(0x0000ffff));
        for (byte i = 0; i < rightEdge.numLeds; i++) drawBlockBorder(rightEdge.frameBlock[i], colorFromHex(0xffff00ff));
        overlayStatus = 1;
      }
    } else {
      if (overlayStatus) {  //Draw overlay with transparent pixels
        *leds &= ~(1 << 0);
        for (byte i = 0; i < topEdge.numLeds; i++) drawBlockBorder(topEdge.frameBlock[i], colorFromHex(0x00000000));
        for (byte i = 0; i < leftEdge.numLeds; i++) drawBlockBorder(leftEdge.frameBlock[i], colorFromHex(0x00000000));
        for (byte i = 0; i < bottomEdge.numLeds; i++) drawBlockBorder(bottomEdge.frameBlock[i], colorFromHex(0x00000000));
        for (byte i = 0; i < rightEdge.numLeds; i++) drawBlockBorder(rightEdge.frameBlock[i], colorFromHex(0x00000000));
        overlayStatus = 0;
      }
    }
    OSTimeDlyHMSM(0, 0, 0, 300);
  }
}


/* Main function, creates tasks and starts the scheduler */
int main(void) {
	color test = {255,0,0,255};
	for (byte i = 0; i < 95; i++) setLed(i, test);

  OSTaskCreateExt(TaskGetColor,
                  NULL,
                  (void *)&TaskGetColor_stk[TASK_STACKSIZE - 1],
                  TaskGetColor_PRIORITY,
                  TaskGetColor_PRIORITY,
                  TaskGetColor_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(TaskWriteLed,
                  NULL,
                  (void *)&TaskWriteLed_stk[TASK_STACKSIZE - 1],
                  TaskWriteLed_PRIORITY,
                  TaskWriteLed_PRIORITY,
                  TaskWriteLed_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(TaskSetOverlay,
                  NULL,
                  (void *)&TaskSetOverlay_stk[TASK_STACKSIZE - 1],
                  TaskSetOverlay_PRIORITY,
                  TaskSetOverlay_PRIORITY,
                  TaskSetOverlay_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  fillClear();

  // If switch 1 is set high, calibrate
  if ((*switches >> 0) & 1) {   
	  calibrate();
      *callibrationframe = inputframe;
  } else {
      inputframe = *callibrationframe;
  }

  topEdge.size = 15;
  leftEdge.size = 15;
  bottomEdge.size = 15;
  rightEdge.size = 15;

  calculateEdgeBlocks(inputframe);

  OSStart();
  return 0;
}

/* Clears the screen of (noise)frameBlock */
void fillClear() {
  color clear = {0, 0, 0, 0};
  for (unsigned int i = 0; i <= frameHeight; i++) {
    for (unsigned int j = 0; j <= frameWidth; j++) {
      setPixel(j, i, clear);
    }
  }
}

/* Get color of a pixel, put RGB values in array */
color getPixelColor(unsigned int X, unsigned int Y) {
  color input;
  color output = {0,0,0,0};
  unsigned int offset = (Y * frameWidth + X);
  input = *(decoderBuffer + offset);
  output.red = input.blue;
  output.green = input.green;
  output.blue = input.red;
  return output;
}

/* Draw one pixel */
void setPixel(unsigned int X, unsigned int Y, color pixel) {
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int offset = 0;
  color pixelData;
  // Clip coordinate values
  if (X < 0)
    return;
  else if (X >= 320)
    return;
  else
    x = X;

  if (Y < 0)
    return;
  else if (Y >= 240)
    return;
  else
    y = Y;

  // Calculate buffer offset
  offset = (y * 320) + x;

  // Write pixelData to buffer
  pixelData.alpha = pixel.alpha;
  pixelData.red = pixel.blue;
  pixelData.green = pixel.green;
  pixelData.blue = pixel.red;
  *(overlayBuffer + offset) = pixelData;
}

/* Change the color of a led */
void setLed(byte index, color pixel) {
  *(ledBuffer + (index)) = pixel;
}

// Get a byte array of type color from 32 bit integer
color colorFromHex(unsigned int in) {
  color out = {0, 0, 0, 0};
  out.red |= (in >> 24) & 0xFF;
  out.green |= (in >> 16) & 0xFF;
  out.blue |= (in >> 8) & 0xFF;
  out.alpha |= (in >> 0) & 0xFF;
  return out;
}

void drawBlockBorder(block b, color c) {
  // Draw top row
  for (int i = 0; i < b.Width; i++) setPixel((b.X + i), b.Y, c);
  // Draw bottom row
  for (int i = 0; i < b.Width; i++) setPixel((b.X + i), (b.Y + b.Height - 1), c);
  // Draw left row
  for (int i = 0; i < b.Height; i++) setPixel(b.X, (b.Y + i), c);
  // Draw right row
  for (int i = 0; i < b.Height; i++) setPixel((b.X + b.Width - 1), (b.Y + i), c);
}

/* Calculate all the edge blocks sizes */
// Corner frameBlock (for example, top left) are included in side edges
// Left and right pixel index is from top to bottom
void calculateEdgeBlocks(block frame) {
  int topBlockSize = (frame.Width / (LEDSTOP + 2));
  int leftBlockSize = (frame.Height / LEDSLEFT);
  int bottomBlockSize = (frame.Width / (LEDSBOTTOM + 2));
  int rightBlockSize = (frame.Height / LEDSRIGHT);

  // Calculate top edge
  topEdge.X = frame.X;
  topEdge.Y = frame.Y;
  topEdge.Width = frame.Width;
  topEdge.Height = topEdge.size;
  topEdge.numLeds = LEDSTOP;
  for (byte i = 0; i < topEdge.numLeds; i++) {
    topEdge.frameBlock[i].id = LEDSLEFT + i + 1;
    topEdge.frameBlock[i].X = frame.X + (frame.Width - topBlockSize * LEDSTOP) / 2 + (topBlockSize * i);
    topEdge.frameBlock[i].Y = topEdge.Y;
    topEdge.frameBlock[i].Width = topBlockSize;
    topEdge.frameBlock[i].Height = topEdge.size;
  }

  // Calculate left edge
  leftEdge.X = frame.X;
  leftEdge.Y = frame.Y;
  leftEdge.Width = leftEdge.size;
  leftEdge.Height = frame.Height;
  leftEdge.numLeds = LEDSLEFT;
  for (byte i = 0; i < leftEdge.numLeds; i++) {
    leftEdge.frameBlock[i].id = LEDSLEFT - i;
    leftEdge.frameBlock[i].X = leftEdge.X;
    if (i == 0) {
      leftEdge.frameBlock[i].Y = frame.Y;
      leftEdge.frameBlock[i].Width = (frame.Width - (topBlockSize * LEDSTOP)) / 2;
      leftEdge.frameBlock[i].Height = (leftEdge.Height - leftBlockSize * (LEDSLEFT - 2)) / 2;
    } else if (i == (LEDSLEFT - 1)) {
      leftEdge.frameBlock[i].Y = frame.Y + leftEdge.frameBlock[0].Height + (leftBlockSize * (i - 1));
      leftEdge.frameBlock[i].Width = (frame.Width - (bottomBlockSize * LEDSBOTTOM)) / 2;
      leftEdge.frameBlock[i].Height = frame.Height - (leftBlockSize * (LEDSLEFT - 2)) - leftEdge.frameBlock[0].Height;
    } else {
      leftEdge.frameBlock[i].Y = frame.Y + leftEdge.frameBlock[0].Height + (leftBlockSize * (i - 1));
      leftEdge.frameBlock[i].Width = leftEdge.size;
      leftEdge.frameBlock[i].Height = leftBlockSize;
    }
  }

  // Calculate bottom edge
  bottomEdge.X = frame.X + frame.Width - (bottomBlockSize * LEDSBOTTOM);
  bottomEdge.Y = frame.Y + frame.Height - bottomEdge.size;
  bottomEdge.Width = frame.Width - (bottomBlockSize * LEDSBOTTOM);
  bottomEdge.Height = bottomEdge.size;
  bottomEdge.numLeds = LEDSBOTTOM;
  for (byte i = 0; i < bottomEdge.numLeds; i++) {
    bottomEdge.frameBlock[i].id = (LEDSTOP + LEDSBOTTOM + LEDSLEFT + LEDSRIGHT) - i + 1;
    bottomEdge.frameBlock[i].X = frame.X + (frame.Width - bottomBlockSize * LEDSBOTTOM) / 2 + (bottomBlockSize * i);
    bottomEdge.frameBlock[i].Y = bottomEdge.Y;
    bottomEdge.frameBlock[i].Width = bottomBlockSize;
    bottomEdge.frameBlock[i].Height = bottomEdge.size;
  }

  // Calculate right edge
  rightEdge.X = frame.X + frame.Width - rightEdge.size;
  rightEdge.Y = frame.Y;
  rightEdge.Width = rightEdge.size;
  rightEdge.Height = frame.Height;
  rightEdge.numLeds = LEDSRIGHT;
  for (byte i = 0; i < rightEdge.numLeds; i++) {
    rightEdge.frameBlock[i].id = LEDSLEFT + LEDSTOP + i;
    if (i == 0) {
      rightEdge.frameBlock[i].X = frame.X + leftEdge.frameBlock[0].Width + LEDSTOP * topBlockSize;
      rightEdge.frameBlock[i].Y = rightEdge.Y;
      rightEdge.frameBlock[i].Width = frame.Width - leftEdge.frameBlock[0].Width - LEDSTOP * topBlockSize;
      rightEdge.frameBlock[i].Height = (rightEdge.Height - rightBlockSize * (LEDSRIGHT - 2)) / 2;
    } else if (i == (LEDSRIGHT - 1)) {
      rightEdge.frameBlock[i].X = frame.X + leftEdge.frameBlock[LEDSLEFT - 1].Width + LEDSBOTTOM * bottomBlockSize;
      rightEdge.frameBlock[i].Y = frame.Y + rightEdge.frameBlock[0].Height + (rightBlockSize * (i - 1));
      rightEdge.frameBlock[i].Width = frame.Width - leftEdge.frameBlock[LEDSLEFT - 1].Width - bottomBlockSize * LEDSBOTTOM;
      rightEdge.frameBlock[i].Height = frame.Height - (rightBlockSize * (LEDSRIGHT - 2)) - rightEdge.frameBlock[0].Height;
    } else {
      rightEdge.frameBlock[i].X = rightEdge.X;
      rightEdge.frameBlock[i].Y = frame.Y + rightEdge.frameBlock[0].Height + (rightBlockSize * (i - 1));
      rightEdge.frameBlock[i].Width = rightEdge.size;
      rightEdge.frameBlock[i].Height = rightBlockSize;
    }
  }
}

/* Calibrate the screen to create new dimensions */
void calibrate() {
  unsigned int atTop = 0;
  unsigned int atBottom = FRAMEHEIGHT - 1;
  unsigned int atLeft = 0;
  unsigned int atRight = FRAMEWIDTH - 1;

  // While color is black, look for color
  // Increment variables to calculate new dimension
  while (getPixelLuminance(getPixelColor(FRAMEWIDTH / 2, atTop)) <= 50) atTop++;
  while (getPixelLuminance(getPixelColor(FRAMEWIDTH / 2, atBottom)) <= 50) atBottom--;
  while (getPixelLuminance(getPixelColor(atLeft, FRAMEHEIGHT / 2)) <= 50) atLeft++;
  while (getPixelLuminance(getPixelColor(atRight, FRAMEHEIGHT / 2)) <= 50) atRight--;
   
  // Set new dimensions with variables
  inputframe.X = atLeft;
  inputframe.Y = atTop;
  inputframe.Width = atRight - atLeft + 1;
  inputframe.Height = atBottom - atTop + 1;
}

byte getPixelLuminance(color p) {
  unsigned int total = p.red + p.green + p.blue;
  return (total / 3);
}

/* Calculate the avarage colors from each frame block */
void getAverages() {
  unsigned long totalR;
  unsigned long totalG;
  unsigned long totalB;
  color temp;

  // Sum of the colors of the pixels per frameblock on the left side
	for(unsigned int i = 0; i < leftEdge.numLeds; i++){	// i = frameblock num.
		for(unsigned int j = 0; j < leftEdge.frameBlock[i].Height; j++){ // j = y value
			for(unsigned int k = 0; k < leftEdge.frameBlock[i].Width; k++){ // k = x value
				temp = getPixelColor(leftEdge.frameBlock[i].X + j,leftEdge.frameBlock[i].Y + k);
				totalR += temp.red;
				totalG += temp.green;
				totalB += temp.blue;
			}
		}

    // Colors are written to variable struct
        leftEdge.frameBlock[i].average.red = totalR / (leftEdge.frameBlock[i].Height * leftEdge.frameBlock[i].Width);
        leftEdge.frameBlock[i].average.green = totalG / (leftEdge.frameBlock[i].Height * leftEdge.frameBlock[i].Width);
        leftEdge.frameBlock[i].average.blue = totalB/ (leftEdge.frameBlock[i].Height * leftEdge.frameBlock[i].Width);

    // Reset local variables to zero
		totalR = 0;
		totalG = 0;
		totalB = 0;
  }

   // Sum of the colors of the pixels per frameblock on the top side
	for(unsigned int i = 0; i < topEdge.numLeds; i++){	// i = frameblock num.
		for(unsigned int j = 0; j < topEdge.frameBlock[i].Height; j++){ // j = y value
			for(unsigned int k = 0; k < topEdge.frameBlock[i].Width; k++){ // k = x value
				temp = getPixelColor(topEdge.frameBlock[i].X + j,topEdge.frameBlock[i].Y + k);
				totalR += temp.red;
				totalG += temp.green;
				totalB += temp.blue;
			}
		}

    // Colors are written to variable struct
        topEdge.frameBlock[i].average.red = totalR/ (topEdge.frameBlock[i].Height * topEdge.frameBlock[i].Width);
        topEdge.frameBlock[i].average.green = totalG / (topEdge.frameBlock[i].Height * topEdge.frameBlock[i].Width);
        topEdge.frameBlock[i].average.blue = totalB / (topEdge.frameBlock[i].Height * topEdge.frameBlock[i].Width);

    // Reset local variables to zero
    totalR = 0;
    totalG = 0;
    totalB = 0;
  }

  // Sum of the colors of the pixels per frameblock on the right side
	for(unsigned int i = 0; i < rightEdge.numLeds; i++){  // i = frameblock num.
		for(unsigned int j = 0; j < rightEdge.frameBlock[i].Height; j++){ //j = y value
			for(unsigned int k = 0; k < rightEdge.frameBlock[i].Width; k++){ //k = x value
				temp = getPixelColor(rightEdge.frameBlock[i].X + j,rightEdge.frameBlock[i].Y + k);
				totalR += temp.red;
				totalG += temp.green;
				totalB += temp.blue;
			}
		}

    // Colors are written to variable struct
        rightEdge.frameBlock[i].average.red = totalR / (rightEdge.frameBlock[i].Height * rightEdge.frameBlock[i].Width);
        rightEdge.frameBlock[i].average.green = totalG / (rightEdge.frameBlock[i].Height * rightEdge.frameBlock[i].Width);
        rightEdge.frameBlock[i].average.blue = totalB / (rightEdge.frameBlock[i].Height * rightEdge.frameBlock[i].Width);

    // Reset local variables to zero
		totalR = 0;
		totalG = 0;
		totalB = 0;
	}

  // Sum of the colors of the pixels per frameblock on the bottom side
	for(unsigned int i = 0; i < bottomEdge.numLeds; i++){ // i = frameblock num.
		for(unsigned int j = 0; j < bottomEdge.frameBlock[i].Height; j++){ //j = y value
			for(unsigned int k = 0; k < bottomEdge.frameBlock[i].Width; k++){ //k = x value
				temp = getPixelColor(bottomEdge.frameBlock[i].X + j,bottomEdge.frameBlock[i].Y + k);
				totalR += temp.red;
				totalG += temp.green;
				totalB += temp.blue;
			}
		}

    // Colors are written to variable struct
        bottomEdge.frameBlock[i].average.red = totalR / (bottomEdge.frameBlock[i].Height * bottomEdge.frameBlock[i].Width);
        bottomEdge.frameBlock[i].average.green = totalG / (bottomEdge.frameBlock[i].Height * bottomEdge.frameBlock[i].Width);
        bottomEdge.frameBlock[i].average.blue = totalB / (bottomEdge.frameBlock[i].Height * bottomEdge.frameBlock[i].Width);

    // Reset local variables to zero
    totalR = 0;
    totalG = 0;
    totalB = 0;
  }
}

/* Writes the average colors to the leds */
void averageToLeds(){
    for(unsigned int i = 0 ; i < leftEdge.numLeds; i++ ){
        leftEdge.frameBlock[i].average.red = (leftEdge.frameBlock[i].average.red * (*(brightnessBuffer) + 1)) / 16;
        leftEdge.frameBlock[i].average.green = (leftEdge.frameBlock[i].average.green * (*(brightnessBuffer) + 1)) / 16;
        leftEdge.frameBlock[i].average.blue = (leftEdge.frameBlock[i].average.blue * (*(brightnessBuffer) + 1)) / 16;
        setLed( leftEdge.frameBlock[i].id , leftEdge.frameBlock[i].average );
    }
    for(unsigned int i = 0 ; i < topEdge.numLeds; i++ ){
        topEdge.frameBlock[i].average.red = (topEdge.frameBlock[i].average.red * (*(brightnessBuffer) + 1)) / 16;
        topEdge.frameBlock[i].average.green = (topEdge.frameBlock[i].average.green * (*(brightnessBuffer) + 1)) / 16;
        topEdge.frameBlock[i].average.blue = (topEdge.frameBlock[i].average.blue * (*(brightnessBuffer) + 1)) / 16;
        setLed( topEdge.frameBlock[i].id , topEdge.frameBlock[i].average );
    }
    for(unsigned int i = 0 ; i < rightEdge.numLeds; i++ ){
        rightEdge.frameBlock[i].average.red = (rightEdge.frameBlock[i].average.red * (*(brightnessBuffer) + 1)) / 16;
        rightEdge.frameBlock[i].average.green = (rightEdge.frameBlock[i].average.green * (*(brightnessBuffer) + 1)) / 16;
        rightEdge.frameBlock[i].average.blue = (rightEdge.frameBlock[i].average.blue * (*(brightnessBuffer) + 1)) / 16;
        setLed( rightEdge.frameBlock[i].id , rightEdge.frameBlock[i].average );
    }
    for(unsigned int i = 0 ; i < bottomEdge.numLeds; i++ ){
        bottomEdge.frameBlock[i].average.red = (bottomEdge.frameBlock[i].average.red * (*(brightnessBuffer) + 1)) / 16;
        bottomEdge.frameBlock[i].average.green = (bottomEdge.frameBlock[i].average.green * (*(brightnessBuffer) + 1)) / 16;
        bottomEdge.frameBlock[i].average.blue = (bottomEdge.frameBlock[i].average.blue * (*(brightnessBuffer) + 1)) / 16;
        setLed( bottomEdge.frameBlock[i].id , bottomEdge.frameBlock[i].average );
    }}

/* Writes the average colors to the overlay */
void averageToBlocks(){
	for(unsigned int i = 0 ; i < leftEdge.numLeds; i++ ) {
		leftEdge.frameBlock[i].average.alpha = 255;
		fillSquare(leftEdge.frameBlock[i].average , leftEdge.frameBlock[i].X  + 1, leftEdge.frameBlock[i].Y + 1, leftEdge.frameBlock[i].Width - 2, leftEdge.frameBlock[i].Height - 2);
	}
	for(unsigned int i = 0 ; i < topEdge.numLeds; i++ ){
		topEdge.frameBlock[i].average.alpha = 255;
		fillSquare(topEdge.frameBlock[i].average , topEdge.frameBlock[i].X + 1, topEdge.frameBlock[i].Y + 1, topEdge.frameBlock[i].Width - 2, topEdge.frameBlock[i].Height - 2);
	}
	for(unsigned int i = 0 ; i < rightEdge.numLeds; i++ ){
		rightEdge.frameBlock[i].average.alpha = 255;
		fillSquare(rightEdge.frameBlock[i].average , rightEdge.frameBlock[i].X + 1, rightEdge.frameBlock[i].Y + 1, rightEdge.frameBlock[i].Width - 2, rightEdge.frameBlock[i].Height - 2);
	}
	for(unsigned int i = 0 ; i < bottomEdge.numLeds; i++ ){
		bottomEdge.frameBlock[i].average.alpha = 255;
		fillSquare(bottomEdge.frameBlock[i].average , bottomEdge.frameBlock[i].X + 1, bottomEdge.frameBlock[i].Y + 1, bottomEdge.frameBlock[i].Width - 2, bottomEdge.frameBlock[i].Height - 2);
	}
}

/* Fills a sqaure with a specific color */
void fillSquare(color pixel, unsigned int x, unsigned int y, unsigned int width, unsigned int height) { // x & y are coordinates
  for (unsigned int i = 0; i < width; i++) {
    for (unsigned int j = 0; j < height; j++) {
      setPixel(x + i, y + j, pixel);
    }
  }
}

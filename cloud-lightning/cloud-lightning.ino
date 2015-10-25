/*
    Multicolored Plasma for the Arduino Micro-Controller and NeoPixel Shield
    Copyright (C) 2013 John Ericksen
    https://github.com/johncarl81/neopixelplasma/blob/master/neopixelplasma.ino
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
*/

#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
int NUM_LEDS = 2;
int LED_PIN = 4;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Convenient 2D point structure
struct Point {
  float x;
  float y;
};

float colorStretch = 0.11;    // Higher numbers will produce tighter color bands. I like 0.11

// Simple moving average
float yValues[] = {
  0,
  0.707,
  1,
  0.9,
  0.707,
  0.71,
  0.74,
  1.2,
  1.5,
  1,
  0,
  0.3,
  0.35,
  0.4,
  1,
  0.707,
  1
};

float simple_moving_average_previous = 0;
float simple_moving_average_current;

int delayTimes[] = {
  1,
  3,
  5,
  5,
  10,
  15,
  30,
  50,
  10,
  15,
  20,
  25,
  25,
  25,
  30,
  50
};

#define numDelayOptions (sizeof(delayTimes)/sizeof(int)) //array size  

int NUM_Y_VALUES = 17;

int len = 5;
float theta_1 = 0.0;
float theta_2 = 0.0;
float theta_Increment = 50; // Controls the speed of the moving points. Higher == faster. I like 0.03
float t_Increment = 0.03;
float t = 0.0;

int currentPixel = 0;

#define NUM_ROSES 3
float k[NUM_ROSES];
void setup() {

  // set up serial port
  Serial.begin(9600);
  
  // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int currentDataPoint = 0;
void loop() {
  if (random(10) == 3) {
    for (int i = 0; i < 10; i++) {
      lightningStrike(0);
    }
  }
  turnAllPixelsOff();

  if (random(10) == 3) {
    for(int i = 0; i < 10; i++) {
      lightningStrike(1);
    }
  }

  turnAllPixelsOff();
  delay(1000);

  
  
}

void turnAllPixelsOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void lightningStrike(int pixel) {
  Serial.print("currentDataPoint: ");
  Serial.println(currentDataPoint);
  float brightness = simple_moving_average(currentDataPoint, (currentDataPoint+1)%NUM_Y_VALUES);

  Serial.print("Brightness: ");
  Serial.println(brightness);
  float scaledWhite = abs(brightness*500);
  Serial.print("scaledWhite: ");
  Serial.println(scaledWhite);
  Serial.println(" ");
  
  strip.setPixelColor(pixel, strip.Color(scaledWhite, scaledWhite, scaledWhite));
  strip.show();
  delay(delayTimes[random(numDelayOptions)]);
  currentDataPoint++;
  currentDataPoint = currentDataPoint%NUM_Y_VALUES;
}

// https://en.wikipedia.org/wiki/Moving_average#Simple_moving_average
float simple_moving_average(uint32_t startingValue, uint32_t endingValue) {
  simple_moving_average_current = simple_moving_average_previous + 
                                  (yValues[startingValue])/NUM_Y_VALUES - 
                                  (yValues[endingValue])/NUM_Y_VALUES;

  simple_moving_average_previous = simple_moving_average_current;
  Serial.print("yValue[currentDataPoint]: ");
  Serial.println(yValues[currentDataPoint]);

  Serial.print("yValues[starting]: ");
  Serial.println(yValues[startingValue]);
  Serial.print("yValues[starting] / num: ");
  Serial.println((yValues[startingValue]) / NUM_Y_VALUES);

  Serial.print("yValues[end]: ");
  Serial.println(yValues[endingValue]);
  Serial.print("yValues[end] / num: ");
  Serial.println((yValues[endingValue]) / NUM_Y_VALUES);

  return simple_moving_average_current;
}

void lightning() {
  t += t_Increment;
  
  // Based on the harmonic movement of a double pendulum: https://en.wikipedia.org/wiki/Double_pendulum
  Point rose1 = { (cos(1.000*t)*cos(t)*4.5), (cos(1.310*t)*sin(t)*4.0) };
  Point rose2 = { (cos(1.770*t)*cos(t)*4.5), (cos(2.865*t)*sin(t)*4.0) };
  Point rose3 = { (cos(0.250*t)*cos(t)*4.5), (cos(0.750*t)*sin(t)*4.0) };
//  Point lissajous = { (A*sin(a*t + sigma)), (B*sin(b*t)) };
  //  x=A\sin(at+\delta),\quad y=B\sin(bt), 
  
  
  byte row, col;
  
  // For each row...
  for( row=0; row<5; row++ ) {
    float row_f = float(row);  // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.
    
    // For each column...
    for( col=0; col<8; col++ ) {
      float col_f = float(col);  // Optimization.
      
      // Calculate the distance between this LED, and rose1.
      Point dist1 = { col_f - rose1.x, row_f - rose1.y };  // The vector
      float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y );
      
      // Calculate the distance between this LED, and rose2.
      Point dist2 = { col_f - rose2.x, row_f - rose2.y };  // The vector
      float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y );
      
      // Calculate the distance between this LED and rose3.
      Point dist3 = { col_f - rose3.x, row_f - rose3.y };
      float distance3 = sqrt( dist3.x*dist3.x + dist3.y*dist3.y );
      
      // Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
      // You can use a cos() for slightly different shading, or experiment with other functions. Go crazy!
      float color_1 = distance1;  // range: 0.0...1.0
      float color_2 = distance2;
      float color_3 = distance3;
      float color_4 = (sin( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;
      
      // Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
      color_1 *= color_1 * color_4;
      color_2 *= color_2 * color_4;
      color_3 *= color_3 * color_4;
      color_4 *= color_4;
 
      // Scale the color up to 0..7 . Max brightness is 7.
      // calming Seaside
      //strip.setPixelColor(col + (8 * row), strip.Color(color_1, color_2, color_3));
      // burning christmas tree
      //strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_3, color_4));
      // perfect fairies
      //strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_3, color_1));
      // lightning???
      //currentPixel = col+(8*row);
      //cycle_color_fade(color_2, color_3, color_1, 0, 0, 0);
      int scaledWhite = color_1*4;
      if (scaledWhite < 10) { scaledWhite = 0; }
      strip.setPixelColor(col+(8*row), strip.Color(scaledWhite, scaledWhite, scaledWhite));
      //strip.setPixelColor(col + (8 * row), strip.Color(color_3, color_2, color_1));
      //strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_4, color_3));
    }
  }
  strip.setBrightness(75);
  strip.show();
}

// Passing in 0 for the "end" values means the LEDs will fade
// to off. You can change these to other color values
// to fade between two colors.
void cycle_color_fade(int rStart, int gStart, int bStart, int rEnd, int gEnd, int bEnd) {
  int Rstart = rStart;
  int Gstart = gStart;
  int Bstart = bStart;

  int Rend = rEnd;
  int Gend = gEnd;
  int Bend = bEnd;

  // larger values of 'n' will give a smoother/slower transition.
  int n = 100;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j<strip.numPixels(); j++) {
      float Rnew = Rstart + (Rend - Rstart) * i / n;
      float Gnew = Gstart + (Gend - Gstart) * i / n;
      float Bnew = Bstart + (Bend - Bstart) * i / n;
      strip.setPixelColor(j, strip.Color(Rnew, Gnew, Bnew));
    }
    strip.show();
    delay(10);
  }
} 



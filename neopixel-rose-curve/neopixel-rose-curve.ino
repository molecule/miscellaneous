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
int NUM_LEDS = 18;
int LED_PIN = 4;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Convenient 2D point structure
struct Point {
  float x;
  float y;
};

float colorStretch = 0.11;    // Higher numbers will produce tighter color bands. I like 0.11 .

int len = 5;
float theta_1 = 0.0;
float theta_2 = 0.0;
float theta_Increment = 0.03; // Controls the speed of the moving points. Higher == faster. I like 0.03
float t_Increment = 0.03;
float t = 0.0;

#define NUM_ROSES 3
float k[NUM_ROSES];
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// This function is called every frame.
void loop() {

  t += t_Increment;
  
  // Based on the harmonic movement of a double pendulum: https://en.wikipedia.org/wiki/Double_pendulum
  Point rose1 = { (cos(1.000*t)*cos(t)*4.5), (cos(1.310*t)*sin(t)*4.0) };
  Point rose2 = { (cos(1.770*t)*cos(t)*4.5), (cos(2.865*t)*sin(t)*4.0) };
  Point rose3 = { (cos(0.250*t)*cos(t)*4.5), (cos(0.750*t)*sin(t)*4.0) };
  
  
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
      strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_3, color_1));
      //strip.setPixelColor(col + (8 * row), strip.Color(color_3, color_2, color_1));
      //strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_4, color_3));
    }
  }
  strip.setBrightness(75);
  strip.show();
}



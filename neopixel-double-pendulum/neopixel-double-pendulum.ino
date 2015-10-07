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

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// This function is called every frame.
void loop() {
  theta_1 += theta_Increment;
  theta_2 += theta_Increment;
  
  // Based on the harmonic movement of a double pendulum: https://en.wikipedia.org/wiki/Double_pendulum
  Point pendulum_1 = { (len/2*sin(theta_1)), (-len/2*cos(theta_1)) };
  Point pendulum_2 = { (len*(sin(theta_1) + .5*sin(theta_2))), (-len*(cos(theta_1) + .5*cos(theta_2))) };
  
  byte row, col;
  
  // For each row...
  for( row=0; row<5; row++ ) {
    float row_f = float(row);  // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.
    
    // For each column...
    for( col=0; col<8; col++ ) {
      float col_f = float(col);  // Optimization.
      
      // Calculate the distance between this LED, and p1.
      Point dist1 = { col_f - pendulum_1.x, row_f - pendulum_1.y };  // The vector from p1 to this LED.
      float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y );
      
      // Calculate the distance between this LED, and p2.
      Point dist2 = { col_f - pendulum_2.x, row_f - pendulum_2.y };  // The vector from p2 to this LED.
      float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y );
      
      // Calculate the distance between this pendulum_1 and pendulum_2.
      Point dist3 = { pendulum_1.x - pendulum_2.x, pendulum_1.x - pendulum_2.y };
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
      //strip.setPixelColor(col + (8 * row), strip.Color(color_2, color_4, color_3));
    }
  }
  strip.setBrightness(75);
  strip.show();
}



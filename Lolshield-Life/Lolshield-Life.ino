/*
 Conway's "Life"
 
 Writen for the LoL Shield, designed by Jimmie Rodgers:
 http://jimmieprodgers.com/kits/lolshield/
 
 This needs the Charliplexing library, which you can get at the
 LoL Shield project page: http://code.google.com/p/lolshield/
 
 Created by Jimmie Rodgers on 12/30/2009.
 Adapted from: http://www.arduino.cc/playground/Main/DirectDriveLEDMatrix
 
 History:
    December 30, 2009 - V1.0 first version written at 26C3/Berlin
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Version 3 General Public
  License as published by the Free Software Foundation; 
  or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//******** Neopixel ***********//
#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
#define NEOPIXEL_PIN 4
#define NUM_LEDS 18
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t bright_blue   = strip.Color(10, 24, 50);

#define DELAY 150             //Sets the time each generation is shown
#define RESEEDRATE 100        //Sets the rate the world is re-seeded
#define SIZEX 1    //Sets the X axis size
#define SIZEY NUM_LEDS    //Sets the Y axis size
byte world[2][SIZEX][SIZEY];  //Creates a double buffer world
const int density = 50;       //Sets density % during seeding
int geck = 0;                 //Counter for re-seeding

void setup() {
  // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  randomSeed(analogRead(5));
  seedWorld();
}

void loop() {
  static byte boring = 0;
  byte changed = 0;

  // Birth and death cycle 
  for (byte x = 0; x < SIZEX; x++) { 
    for (byte y = 0; y < SIZEY; y++) {
      // Default is for cell to stay the same
      byte alive = world[0][x][y];
      if (alive > 0) {
        strip.setPixelColor(y+x*SIZEY, bright_blue);
      } else {
        strip.setPixelColor(y+x*SIZEY, 0);
      }

      byte count = neighbours(x, y); 
      if (count == 3 && !alive) {
        // A new cell is born
        alive = 1; 
        ++changed;
      } else if ((count < 2 || count > 3) && alive) {
        // Cell dies
        alive = 0;
        ++changed;
      }
      world[1][x][y] = alive;
    }
  }
  
  // Copy next generation into place
  for (byte x = 0; x < SIZEX; x++) { 
    for (byte y = 0; y < SIZEY; y++) {
      world[0][x][y] = world[1][x][y];
    }
  }

  if (changed)
    boring = 0;
  else
    ++boring;

  //Counts and then checks for re-seeding
  //Otherwise the display will die out at some point
  if (boring >= 5 || ++geck >= RESEEDRATE) {
    geck = 0;
    seedWorld();
  }

  strip.show();
  delay(DELAY);
}

//Re-seeds based off of RESEEDRATE
void seedWorld(){
  for (byte i = 0; i < SIZEX; i++) {
    for (byte j = 0; j < SIZEY; j++) {
      if (random(100) < density)
        world[0][i][j] = 1;
      else
        world[0][i][j] = 0;
    } 
  }
}

//Runs the rule checks, including screen wrap
byte neighbours(byte x, byte y) {
  return world[0][(x + 1) % SIZEX][y] + 
    world[0][x][(y + 1) % SIZEY] + 
    world[0][(x + SIZEX - 1) % SIZEX][y] + 
    world[0][x][(y + SIZEY - 1) % SIZEY] + 
    world[0][(x + 1) % SIZEX][(y + 1) % SIZEY] + 
    world[0][(x + SIZEX - 1) % SIZEX][(y + 1) % SIZEY] + 
    world[0][(x + SIZEX - 1) % SIZEX][(y + SIZEY - 1) % SIZEY] + 
    world[0][(x + 1) % SIZEX][(y + SIZEY - 1) % SIZEY];
}

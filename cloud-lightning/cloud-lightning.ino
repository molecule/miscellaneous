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
int NUM_LEDS = 4;
int LED_PIN = 4;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Simple moving average
float yValues[] = {
  0,
  7,
  10,
  9,
  7.1,
  7.5,
  7.4,
  12,
  15,
  10,
  0,
  3,
  3.5,
  4,
  1,
  7,
  1
};

float simple_moving_average_previous = 0;
float simple_moving_average_current;

float random_moving_average_previous = 0;
float random_moving_average_current;
 
int NUM_Y_VALUES = 17;

float (*functionPtrs[10])(); //the array of function pointers
int NUM_FUNCTIONS = 2;

void setup() {

  // set up serial port
  Serial.begin(9600);
  
  // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // initializes the array of function pointers.
  functionPtrs[0] = simple_moving_average;
  functionPtrs[1] = random_moving_average;
}

int currentDataPoint = 0;
int chance = 5;
String TRIGGER_LIGHTNING_STORM = String("LIGHTNING");
String SUNNY_DAY = String("SUNNY");
String CLOUD_DAY = String("CLOUDY");

enum State: byte {
  SUNNY, CLOUDY, STORMY
};
State state = SUNNY; 
int startingColors[] = {5, 1, 0};
int endingColors[] = {44, 25, 1};
int RED_INDEX = 0;
int GREEN_INDEX = 1;
int BLUE_INDEX = 2;
void loop() {

  //if (readFromBluetooth() == TRIGGER_LIGHTNING_STORM) {
  if (random(chance) == 3) {
    int led = random(NUM_LEDS);
    for (int i = 0; i < 10; i++) {
      lightningStrike(random(NUM_LEDS));
    }
    chance = 10;
  } else {
    chance = 5;
  }
  turnAllPixelsOff();
  delay(1000);
}

/**
 * Read the data from the BLE, breaking on '\n' and '\r' characters.
 */
String readFromBluetooth() {
  String readString = "";
  
    while (Serial.available()) {
    delay(10);  //small delay to allow input buffer to fill

    char c = Serial.read(); //gets one byte from serial buffer
    if (c == '\n' || c == '\r') {
      break;
    }
    readString += c;
  } 
  return readString;
}

void turnAllPixelsOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void lightningStrike(int pixel) {
  float brightness = callFunction(random(NUM_FUNCTIONS));
  float scaledWhite = abs(brightness*500);
  
  strip.setPixelColor(pixel, strip.Color(scaledWhite, scaledWhite, scaledWhite));
  strip.show();
  delay(random(1, 30));
  currentDataPoint++;
  currentDataPoint = currentDataPoint%NUM_Y_VALUES;
}

float callFunction(int index) {
  return (*functionPtrs[index])(); //calls the function at the index of `index` in the array
}

// https://en.wikipedia.org/wiki/Moving_average#Simple_moving_average
float simple_moving_average() {
  uint32_t startingValue = currentDataPoint;
  uint32_t endingValue = (currentDataPoint+1)%NUM_Y_VALUES;
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


float random_moving_average() {
  float firstValue = random(1, 10);
  float secondValue = random(1, 10);
  Serial.print("first: ");
  Serial.println(firstValue);
  Serial.print("first/NUM_Y_VALUES");
  Serial.println(firstValue/NUM_Y_VALUES);
  random_moving_average_current = random_moving_average_previous +
                                  firstValue/NUM_Y_VALUES -
                                  secondValue/NUM_Y_VALUES;
  random_moving_average_previous = random_moving_average_current;

  return random_moving_average_current;
}

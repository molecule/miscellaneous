/*
  Example for Charliplexing library
  
  Alex Wenger <a.wenger@gmx.de> http://arduinobuch.wordpress.com/
  History:
    30/Dez/09 - V0.0 wrote the first version at 26C3/Berlin
*/

#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 18
#define NEOPIXEL_PIN 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t red           = strip.Color(246, 0, 0);

struct point {
  uint8_t xp;        // Point Position in X direction (multplied by 16)
  uint8_t x_speed;   // Speed
  uint8_t flag;  
} points[9];

void setup()                    // run once, when the sketch starts
{
  // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  for(uint8_t i = 0; i < 9; i++)
  {
    points[i].xp = 0;
    points[i].x_speed = random(1, 16);
    points[i].flag = 1;
  }
}

uint8_t heart_p[] = {
  4,5,
  3,4,
  2,4,
  5,4,
  6,4,
  7,5,
  1,5,
  7,6,
  1,6,
  6,7,
  2,7,
  5,8,
  3,8,
  4,9,
};

void heart()
{
  for(uint8_t y = 0; y < 9; y++)
    for(uint8_t x = 3; x < 11; x++)
    {
      strip.setPixelColor(x, 0);
      strip.setPixelColor(y, 0);    
    }
  for(uint8_t i = 0; i < 14; i++)
  {
    strip.setPixelColor(heart_p[i*2+1], red);
    strip.setPixelColor(heart_p[i*2], red);    
  }
}

uint8_t heart_flag;

void loop()                     // run over and over again
{
  for(uint8_t i = 0; i < 9; i++)
  {
    points[i].xp += points[i].x_speed;
    if (points[i].xp >= 14*16) 
    {
      points[i].x_speed = random(1, 16);
      points[i].xp = 0;
      points[i].flag ^= 1;
    }
    strip.setPixelColor(points[i].xp/16, points[i].flag);
    strip.setPixelColor(i, points[i].flag);    
  }
  
  heart_flag++;
  if (heart_flag < 20) {
    heart();
  }
  
  delay(40);
}

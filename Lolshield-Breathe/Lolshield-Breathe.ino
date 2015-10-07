// note attempt to port unsuccessful as of yet.
// can't figure out what Clear is supposed to do...

/*
 Super-simple LoL Shield "breathe" fading test
 Written by Thilo Fromm <kontakt@thilo-fromm.de>.
 Writen for the LoL Shield, designed by Jimmie Rodgers:
 http://jimmieprodgers.com/kits/lolshield/
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

uint32_t blue          = strip.Color(0, 0, 255);

#define SHADES 8

const unsigned int inhale_time_ms  = 500;
const unsigned int hold_breath_ms  = 600;
const unsigned int exhale_time_ms  = 800;
const unsigned int pause_breath_ms = 2000;

void setup()                    // run once, when the sketch starts
{
    // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()                     // run over and over again
{ 
    // inhale
    for (int8_t i=0; i <= SHADES-1; i++) {
        uint8_t sleep =   inhale_time_ms / SHADES 
                        + ( SHADES / 2 - i ) * ( inhale_time_ms / (SHADES * 6) );
                        
        //LedSign::Clear(i);
        uint32_t color = strip.Color(0, 0, i);
        setAll(color);
        
        //LedSign::Flip(true);
        strip.show();
        delay( sleep );
    }
    delay( hold_breath_ms );

    // exhale
    for (int8_t i=SHADES-1; i >= 0; i--) {
        uint8_t sleep =   exhale_time_ms / SHADES 
                        + ( SHADES / 2 - i ) * ( inhale_time_ms / (SHADES * 6) );
        //LedSign::Clear(i);
        uint32_t color = strip.Color(0, 0, i);
        setAll(color);
        
        //LedSign::Flip(true);
        strip.show();
        delay( sleep );
    }

    // pause
    delay( pause_breath_ms );
}

void setAll(uint32_t color) {
  for (int j = 0; j < NUM_LEDS; j++) {
    strip.setPixelColor(j, blue);
  }
}


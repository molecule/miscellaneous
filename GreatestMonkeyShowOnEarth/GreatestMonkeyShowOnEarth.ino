//******** Buzzer ***********//
#include <toneAC.h>

const int duration = 452;

//******** IR Receive **********//
#include <IRremote.h>

// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRrx_PIN_VECTOR PIND
#define IRrx 5

#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements

// we will store up to 50 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;

//********* IR Send *********//
const uint32_t IR_REMOTE_OK_MUTE = 0x20DF906F;

//******** Neopixel *********** //
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NEOPIXEL_PIN 11
#define NUM_LEDS 6
int delayVal = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t red           = strip.Color(24, 0, 0);

void setup() {

  //Serial.begin(9600);
  //Serial.println("Greatest Monkey Show on Earth");
  
  // Set up on-board LED
  pinMode(13, OUTPUT);
  pinMode(IRrx, INPUT);

  // Neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

/**
 * The loop that listens for IR.
 * 
 * Based on Trinket/Gemma compatible IR read sketch
 * This sketch/program uses a PNA4602 or TSOP38238 to
 * read an IR code and perform a function.  
 * Based on Adafruit tutorial http://learn.adafruit.com/ir-sensor/using-an-ir-sensor
 * and http://learn.adafruit.com/trinket-gemma-mini-theramin-music-maker
*/
void loop() {
  irCode=listenForIR(); // Wait for an IR Code

  // Process the pulses to get a single number representing code
  for (int i = 0; i < 34; i++) {
    irCode=irCode<<1;
    if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
      irCode|=0; 
    } else {
      irCode|=1;
    }
  }

  //Serial.println("irCode");
  //Serial.println(irCode, HEX);

  if (irCode == IR_REMOTE_OK_MUTE) {
    terribleNoise();
  }
  
  memset(pulses, 0, sizeof(pulses)); 
}

static void terribleNoise() {
  digitalWrite(13, HIGH);
  chasePersist(strip.numPixels(), red);
  for (int i = 0; i < 400; i ++) {
    for (unsigned long freq = 1500; freq <= 1550; freq += 1) {  
      if (freq %2 == 0) { 
        toneAC(freq); 
      }
      else { 
        toneAC(freq-3); 
      }
      delay(.01);     // Wait ms so you can hear it.
    }
  }
  toneAC(0); // Turn off toneAC, can also use noToneAC().
  digitalWrite(13, LOW);
  chasePersist(strip.numPixels(), 0);
}

static void chasePersist(int numPixels, uint32_t c) {
 for(int i=0;i<numPixels;i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, c); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
  } 
}


/** 
  *  From the Raw IR decoder sketch!
  *  This sketch/program uses the Arduno and a PNA4602 to
  *  decode IR received. This can be used to make a IR receiver
  *  (by looking for a particular code)
  *  or transmitter (by pulsing an IR LED at ~38KHz for the
  *  durations detected
  *  Code is public domain, check out www.ladyada.net and adafruit.com
  *  for more tutorials!
*/
uint16_t listenForIR() {  // IR receive code
   currentpulse = 0;
   while (1) {
     unsigned int highpulse, lowpulse;  // temporary storage timing
     highpulse = lowpulse = 0; // start out with no pulse length 
    
     while (IRrx_PIN_VECTOR & _BV(IRrx)) { // got a high pulse
        highpulse++; 
        delayMicroseconds(RESOLUTION);
        if (((highpulse >= MAXPULSE) && (currentpulse != 0)) || currentpulse == NUMPULSES) {
          return currentpulse; 
        }
     }
     pulses[currentpulse][0] = highpulse;
    
     while (! (IRrx_PIN_VECTOR & _BV(IRrx))) { // got a low pulse
        lowpulse++; 
        delayMicroseconds(RESOLUTION);
        if (((lowpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
          return currentpulse; 
        }
     }
     pulses[currentpulse][1] = lowpulse;
     currentpulse++;
   }
}

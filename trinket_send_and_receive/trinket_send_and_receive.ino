/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */
 
 // Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.
 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 3
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);
 
#include <PinChangeInt.h>
//#include <PinChangeInterrupt.h>
//#include <SoftwareSerial.h>       // use if you do not wish to use the lightweight library 
//#include <SendOnlySoftwareSerial.h>  // See http://forum.arduino.cc/index.php?topic=112013.0
 
//SoftwareSerial Serial(1,0);      // Receive, Transmit
//SendOnlySoftwareSerial Serial(4);   // Transmit serial on Trinket/Gemma pin GPIO #4

const int buttonPin = 8;
//#include <IRremote.h>
#include <IRLib.h>

// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRpin_PIN PINB // ATTiny85 had Port B pins
//#define IRpin_PIN PIND // Uno?
#define IRpin 2

#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;

int led = 13; // blink built in LED

#define SEND_ONES 1 
#define SEND_TWOS 2
int role = SEND_ONES;
int expecting;
int sending;
int allOnes = 0x11111111;
int allTwos = 0x22222222;
int buttonVal = LOW;

IRsend irsend;

void setup()
{
  if (SEND_ONES == role) {
    expecting = allTwos;
    sending = allOnes;
  } else {
    expecting = allOnes;
    sending = allTwos; 
  }
  Serial.begin(9600);
  Serial.println("trinket_send_and_receive");
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  PCintPort::attachInterrupt(buttonPin, button_press, RISING);  // detection type CHANGE, FALLING, or RISING
  pinMode(IRpin, INPUT);   // Listen to IR receiver on Trinket/Gemma pin D2
}

void button_press() {
  Serial.print("sending: ");
  Serial.println(sending);
  //irsend.sendNEC(sending, 32); // 38kHz
  irsend.send(NEC, sending, 32); 
  digitalWrite(led, HIGH);
  delay(40);
  digitalWrite(led, LOW);
  delay(3000);
}

void loop() {
    Serial.println("listening...");
    uint16_t numpulse=listenForIR(); // Wait for an IR Code

    // Process the pulses to get a single number representing code
    for (int i = 0; i < 34; i++) {
      irCode=irCode<<1;
      if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
        irCode|=0; 
      } else {
        irCode|=1;
      }
    }
    
    printcode();
    
    if (irCode == expecting) {
      Serial.println("recv 0x11111111");
      digitalWrite(led, HIGH);
      delay(40);
      digitalWrite(led, LOW);
    } else {
      digitalWrite(led, LOW);
    }
  delay(500);
}
/*
void setupPixels() {
  #ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
}

void blinkPixels(int numPixels, uint32_t c, int wait) {
   for(int i=0; i<numPixels; i++) {
    pixels.setPixelColor(i, c); 
   }
   pixels.show();
   delay(wait);
   for(int i=0; i<numPixels; i++) {
    pixels.setPixelColor(i, 0); 
   }
}
*/

void printcode(void) {
  Serial.print("0x");
  Serial.println(irCode, HEX); // print all 32 bits in hex
}

uint16_t listenForIR() {  // IR receive code
  currentpulse = 0;
  while (1) {
   unsigned int highpulse, lowpulse;  // temporary storage timing
   highpulse = lowpulse = 0; // start out with no pulse length 
  
   while (IRpin_PIN & _BV(IRpin)) { // got a high pulse
      highpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse; 
      }
   }
   pulses[currentpulse][0] = highpulse;

   while (! (IRpin_PIN & _BV(IRpin))) { // got a low pulse
      //Serial.println("low pulse");
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

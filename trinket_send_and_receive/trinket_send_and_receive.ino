/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRpin_PIN PINB // ATTiny85 had Port B pins
#define IRpin 2

#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;

int led = 1; // blink 'digital' pin 1 - AKA the built in red LED
int button = 0;

#define SEND_ONES 1 
#define SEND_TWOS 2
int role = SEND_ONES;
int expecting;
int sending;
int allOnes = 0x11111111;
int allTwos = 0x22222222;
int buttonVal;

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
  //Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(IRpin, INPUT);   // Listen to IR receiver on Trinket/Gemma pin D2
}

void loop() {
  buttonVal = digitalRead(button);
  if (buttonVal == HIGH) {
    irsend.sendNEC(sending, 32); // 38
    digitalWrite(led, HIGH);
    delay(40);
    digitalWrite(led, LOW);
    delay(3000);
  } else {
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
  
    if (irCode == expecting) {
      //Serial.println("recv 0x11111111");
      digitalWrite(led, HIGH);
      delay(40);
      digitalWrite(led, LOW);
    } else {
      digitalWrite(led, LOW);
    }
  }
}

uint16_t listenForIR() {  // IR receive code
  currentpulse = 0;
  while (1) {
   unsigned int highpulse, lowpulse;  // temporary storage timing
   highpulse = lowpulse = 0; // start out with no pulse length 
  
   while (IRpin_PIN & _BV(IRpin)) { // got a high pulse
      buttonVal = digitalRead(button);
      if (HIGH == buttonVal) {
        return 0; 
      } 
      highpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse; 
      }
   }
   pulses[currentpulse][0] = highpulse;

   while (! (IRpin_PIN & _BV(IRpin))) { // got a low pulse
     buttonVal = digitalRead(button);
     if (HIGH == buttonVal) {
        return 0; 
      }
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

/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.3   January 2014
 * Copyright 2014 by Chris Young http://cyborg5.com
 * Based on original example sketch for IRremote library 
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://www.righto.com/
 */
//#include <IRLib.h>
int led = 13;
#include <IRremote.h>

//IRsend My_Sender;
IRsend irsend;

void setup()
{
  //Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
    
    //My_Sender.send(SONY,0xa8bca, 20);
    irsend.sendNEC(0x55555555, 32);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
}


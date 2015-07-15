#include <PinChangeInterrupt.h>

// #install <SoftwareSerial.h>       // use if you do not wish to use the lightweight library 
#include <SendOnlySoftwareSerial.h>  // See http://forum.arduino.cc/index.php?topic=112013.0
 
// SoftwareSerial Serial(1,0);      // Receive, Transmit (Receive not used)
SendOnlySoftwareSerial Serial(4);   // Transmit serial on Trinket/Gemma pin GPIO #0/D0

const int buttonPin = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  attachPcInterrupt(buttonPin, button_press, RISING);  // detection type CHANGE, FALLING, or RISING
}

void loop()
{
    Serial.println("listening...");
    // waiting for IR receive code

    // do your stuff
    delay(500);
}

void button_press()
{
   Serial.println("sending IR code");
}

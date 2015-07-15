#include <PinChangeInt.h>
#include <IRLib.h>

int led = 13; // blink built in LED
const int buttonPin = 8;
int buttonVal = LOW;

IRsend irsend;
int sending = 0x55555555;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT);
  PCintPort::attachInterrupt(buttonPin, button_press, RISING);  // detection type CHANGE, FALLING, or RISING
}

void loop() {
  // put your main code here, to run repeatedly
 
}


void button_press() { 
  irsend.send(NEC, sending, 32); 
}


/* Trinket/Gemma compatible IR read sketch
This sketch/program uses an Adafruit Trinket or Gemma
ATTiny85 based mini microcontroller and a PNA4602 or TSOP38238 to
read an IR code and perform a function.  
Based on Adafruit tutorial http://learn.adafruit.com/ir-sensor/using-an-ir-sensor
and http://learn.adafruit.com/trinket-gemma-mini-theramin-music-maker
*/
 
// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
//#define IRpin_PIN  PINB // ATTiny85 had Port B pins // Trinket
#define IRpin_PIN PIND // Uno?
#define IRpin      2    // IR sensor - TSOP38238 on Pin GPIO #2 / D2
 
int led = 13; // blink 'digital' pin 1 - AKA the built-in red LED

#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    50  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements
 
// we will store up to 50 pulse pairs (this is -a lot-, reduce if needed)
uint16_t pulses[50][2];   // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;
 
void setup() {
  Serial.begin(9600);
  Serial.println("trinket_ir_receive");
  pinMode(IRpin, INPUT);       // Listen to IR receiver on Trinket/Gemma pin D2
  pinMode(led, OUTPUT); // Output tones on Trinket/Gemma pin D1
}
 
void loop() {
  
  irCode=listenForIR(); // Wait for an IR Code
  
  // Process the pulses to get our code
  for (int i = 0; i < 34; i++) {
    irCode=irCode<<1;
    if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
      irCode|=0; 
    } else {
      irCode|=1;
    }
  }
  Serial.println("irCode");
  Serial.println(irCode, HEX);
  if (irCode==0xffffffff) {
    Serial.println("success");
  }

} // end loop
 

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

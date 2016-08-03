
/**
 * The loop that runs when we're in "Networking" mode.
 * 
 * Based on Trinket/Gemma compatible IR read sketch
 * This sketch/program uses an Adafruit Trinket or Gemma
 * ATTiny85 based mini microcontroller and a PNA4602 or TSOP38238 to
 * read an IR code and perform a function.  
 * Based on Adafruit tutorial http://learn.adafruit.com/ir-sensor/using-an-ir-sensor
 * and http://learn.adafruit.com/trinket-gemma-mini-theramin-music-maker
*/
void ir_loop() {
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

  //Serial.println(irCode, HEX);
  
  //Check received IR code for comm header
  uint32_t irCopy = (irCode >> 20) << 12;
  
  if (irCode == IR_REMOTE_PLAY && PUZZLE == 1) {
    qcom_blue_flash();
    delay(delayVal);
    qcom_blue_flash();
    //chase();
  } else if (irCode = IR_REMOTE_REWIND && PUZZLE == 2) {
    chasePersist(strip.numPixels(), deep_purple);
    chase();
  } else if (irCode != 0) {
    chasePersist(strip.numPixels(), yellowOrange);
    chase();
  }
  memset(pulses, 0, sizeof(pulses));
  
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
  while (true) {
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

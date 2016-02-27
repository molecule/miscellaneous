

int calibrate = 0;
int lowThreshold = 0;
int highThreshold = 13;


int InputPin = A0; // analog pin 0 is the input pin  
int val = 0; // variable to store the value read 
int redEyes = 8; // turns on the red LED for glowing eyes effect.

void setup() { 
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(redEyes, OUTPUT);
} 

void loop() { 
  // first minute is calibration.
  
  if(calibrate) {
    calibration();
  }
  
  digitalWrite(redEyes, LOW);
  val = analogRead(InputPin); // read the input pin 0 to 1023 
  //Serial.println("high: ");
  //Serial.println(highThreshold);
  //Serial.println(" low: ");
  //Serial.println(lowThreshold);
  //Serial.println(" val: ");
  Serial.println( val);
  if (val >= highThreshold) // if sensor value is above threshold set output HIGH 
  { 
    //digitalWrite(redEyes, HIGH);
    //delay(1000);
  } 
   
}

void calibration(){
  
    for(int i = 0; i < 10; i++){
      lowThreshold += analogRead(InputPin);
      Serial.println(lowThreshold);
    }
    lowThreshold = lowThreshold/10;
    highThreshold = lowThreshold + 10;
    Serial.println(lowThreshold);
    Serial.println(highThreshold);
    calibrate = 0;
  } 

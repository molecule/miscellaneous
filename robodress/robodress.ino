#include <Servo.h>

int calibrate = 0;
int lowThreshold = 0;
int highThreshold = 22;

Servo myServo;
int servoPin = 5;
int pos = 0;


int infraredInputPin = A0; // analog pin 0 is the input pin  
int val = 0; // variable to store the value read 
int redEyes = 8; // turns on the red LED for glowing eyes effect.

void setup() { 
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(redEyes, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0);
} 

void loop() { 
  // first minute is calibration. 
  if(calibrate) {
    calibration();
  }
  
  digitalWrite(redEyes, LOW);
  val = analogRead(infraredInputPin); // read the input pin 0 to 1023 
  //Serial.println("high: ");
  //Serial.println(highThreshold);
  //Serial.println(" low: ");
  //Serial.println(lowThreshold);
  //Serial.println(" val: ");
  Serial.println( val);
  if (val >= highThreshold) // if sensor value is above threshold set output HIGH 
  { 
    myServo.write(75);
    //digitalWrite(redEyes, HIGH);
    delay(1000);
    myServo.write(0);
  } 
   
}

void calibration(){
  
    for(int i = 0; i < 10; i++){
      lowThreshold += analogRead(infraredInputPin);
      Serial.println(lowThreshold);
    }
    lowThreshold = lowThreshold/10;
    highThreshold = lowThreshold + 10;
    Serial.println(lowThreshold);
    Serial.println(highThreshold);
    calibrate = 0;
  } 

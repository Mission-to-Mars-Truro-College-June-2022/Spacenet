#include <NewPing.h>

int rm_enabled = 9;//ENB
int lm_enabled = 10;//ENA
int rm_reverse = 8; //IN3
int lm_forwards = 11;//IN2
int rm_forwards = 7; //IN4
int lm_reverse = 12; //IN1

const int RIGHT_FEEDBACK = 2;
const int LEFT_FEEDBACK = 3;

volatile int leftcounter = 0;
volatile int rightcounter = 0;

int sensorPin = A2;    // select the input pin for the potentiometer
int sensorPin2 = A6;
int sensorPin3 = A7;
int sensorValueR = 0;
int sensorValueM = 0;
int sensorValueL = 0; // variable to store the value coming from the sensor

int fullRotationTime = 3000;
int brightestTime = 0;
int brightestLight = 1000;

int floor_dist = 0;

#define TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200);
  
  pinMode(rm_enabled, OUTPUT);
  pinMode(lm_enabled, OUTPUT);
  
  pinMode(rm_reverse, OUTPUT);
  pinMode(lm_forwards, OUTPUT);

  pinMode(rm_forwards, OUTPUT);
  pinMode(lm_reverse, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_FEEDBACK), LeftMotorISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_FEEDBACK), RightMotorISR, RISING);

  sonar.ping_cm(); 
  
  
  set_speed(255, -255); // Spin

  while(true){
    sensorValueR = analogRead(sensorPin);
    sensorValueM = analogRead(sensorPin2);
    sensorValueL = analogRead(sensorPin3);
    
    int sensorAverage = ((sensorValueR + sensorValueM + sensorValueL) / 3);
  
    if (brightestLight > sensorAverage) { // Record brighest time and intcity
      brightestLight = sensorAverage;
      brightestTime = millis();
      Serial.print("1: ");
      Serial.println(sensorValueR);
      Serial.print("2: ");
      Serial.println(sensorValueM);
      Serial.print("3: ");
      Serial.println(sensorValueL);
      Serial.print("Brightest: ");
      Serial.println(brightestLight);
      Serial.print("Brightest Time: ");
      Serial.println(brightestTime);
      Serial.println("-");
    }
  
    if (millis() >= fullRotationTime){ // Once rotation is complete
      // Turn until it is pointing at the brightest soruce   
      set_speed(0,0);
      delay(500);
      turn_right(brightestTime); 
      // Go towards light source
      set_speed(255,255);
      break;
    }
  }
  
}

void turn_right(int D) {
  set_speed(255, -255);
  delay(D);
  set_speed(0, 0);
}

void turn_left(int D) {
  set_speed(-255, 255);
  delay(D);
  set_speed(0, 0);
}

void set_speed(int l_speed, int r_speed){
  if(l_speed < 0) {
    analogWrite(lm_enabled, -l_speed);
    digitalWrite(lm_forwards, LOW);
    digitalWrite(lm_reverse, HIGH);
  } else {
    analogWrite(lm_enabled, l_speed);
    digitalWrite(lm_reverse, LOW);
    digitalWrite(lm_forwards, HIGH);
  }

  if(r_speed < 0) {
    analogWrite(rm_enabled, -r_speed);
    digitalWrite(rm_forwards, LOW);
    digitalWrite(rm_reverse, HIGH);
  } else {
    analogWrite(rm_enabled, r_speed);
    digitalWrite(rm_reverse, LOW);
    digitalWrite(rm_forwards, HIGH);
  }
}

int get_dist() {
  return (rightcounter + leftcounter) / 2;
}

void forward(){
  const int turn_offset = 25; // Counters the turn
  const int speed_offset = 15; // Speed change
  if(leftcounter == rightcounter){
      set_speed(255 - speed_offset,210 - turn_offset - speed_offset);
    }
    if(leftcounter > rightcounter){
      set_speed(255 - speed_offset ,255 - turn_offset - speed_offset);    
    }
    if(leftcounter < rightcounter){
      set_speed(255 - turn_offset - speed_offset,255  - speed_offset);
    }
}



void loop() {
    sensorValueR = analogRead(sensorPin);
    sensorValueM = analogRead(sensorPin2);
    sensorValueL = analogRead(sensorPin3);

    int lightVarience = 170;

    floor_dist = sonar.ping_cm();

    if (floor_dist < 17){

      if (sensorValueR > sensorValueL + lightVarience){ // Brighter on left
        set_speed(-255,255); // turn Left
      }
       else if (sensorValueL > sensorValueR + lightVarience){ // Brighter on right
        set_speed(255,-255); // turn right
      } else { // Else fowards
        set_speed(255, 255);
      }
      
    } else {
      set_speed(-255, -210);
      delay(500);
      set_speed(0,0);
      exit(0);
    }

}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

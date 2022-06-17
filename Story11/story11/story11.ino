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

#define TRIGGER_PIN  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
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
  delay(50);
  
  
}

void turn_90_right(int D) {
  set_speed(255, -255);
  delay(D);
  set_speed(0, 0);
}

void turn_90_left(int D) {
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


void loop() {
  const int turn_offset = 20;

  if(leftcounter== rightcounter){
    set_speed(255,210 - turn_offset);
  }
  if(leftcounter > rightcounter){
    set_speed(255,230 - turn_offset);    
  }
  if(leftcounter < rightcounter){
    set_speed(230,255 - turn_offset);
  }

  delay(50);

  int dist = sonar.ping_cm();

  if(dist < 20 && dist > 5) {
    set_speed(0, 0);
    turn_90_left(750);
  }
}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

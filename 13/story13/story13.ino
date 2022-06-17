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

#define TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int sensorPin = A2;    // select the input pin for the potentiometer
int sensorPin2 = A6;
int sensorPin3 = A7;
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0; // variable to store the value coming from the sensor
int bestLight = 435;

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
  const int turn_offset = 0;
  const int speed_offset = 15;
  const int acceptable_light_variance = 20;
  sensorValue1 = analogRead(sensorPin);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);

  set_speed(255, -255);


  int delay_ticks = 0;
  
  if((sensorValue1 < 435) && (sensorValue2 < 435) && (sensorValue3 < 435)) { // If all sensers are less than 
    int avgLight = (sensorValue1 + sensorValue2 + sensorValue3) / 3;

    //If a lightsource has already been detected then wait until we try and find a new one
    if(delay_ticks > 0) {
      delay_ticks--;
    } else { // A light source hasnt relecently been detcted
      if(avgLight < bestLight) { // if avg lights is brighter than the brightest light, delay for some time
        bestLight = avgLight;
        delay_ticks = 75;
        
      } else if(avgLight >= bestLight - acceptable_light_variance && avgLight <= bestLight + acceptable_light_variance)  // If light is the average +- variance then drive towards it
        if(leftcounter== rightcounter){
          set_speed(255 - speed_offset,210 - turn_offset - speed_offset);
        }
        if(leftcounter > rightcounter){
          set_speed(255 - speed_offset ,230 - turn_offset - speed_offset);    
        }
        if(leftcounter < rightcounter){
          set_speed(230 - speed_offset,255 - turn_offset - speed_offset);
        }

        // Drive for time then stop
        delay(500);
        set_speed(0,0);
        exit(0);
        
      }
      
    }
    
    
  }
}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

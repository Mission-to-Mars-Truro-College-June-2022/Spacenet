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
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0; // variable to store the value coming from the sensor

int fullRotationTime = 3150;
int brightestTime = 0;
int brightestLight = 1000;

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



void loop() {
  const int turn_offset = 25; // Counters the turn
  const int speed_offset = 15; // Speed change
  sensorValue1 = analogRead(sensorPin);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue3 = analogRead(sensorPin3);

  


  set_speed(255, -255); // Spin

  int sensorAverage = ((sensorValue1 + sensorValue2 + sensorValue3) / 3);

  if (brightestLight > sensorAverage) { // Record brighest time and intcity
    brightestLight = sensorAverage;
    brightestTime = millis();
    Serial.print("1: ");
    Serial.println(sensorValue1);
    Serial.print("2: ");
    Serial.println(sensorValue2);
    Serial.print("3: ");
    Serial.println(sensorValue3);
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
    set_speed(0,0);

    // Drive towards the source
    leftcounter = 0;
    rightcounter = 0;
    
    if(leftcounter == rightcounter){
      set_speed(255 - speed_offset,210 - turn_offset - speed_offset);
    }
    if(leftcounter > rightcounter){
      set_speed(255 - speed_offset ,255 - turn_offset - speed_offset);    
    }
    if(leftcounter < rightcounter){
      set_speed(255 - turn_offset - speed_offset,255  - speed_offset);
    }

    delay(1000);
    set_speed(0, 0); 
    exit(0);
  }
    

}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

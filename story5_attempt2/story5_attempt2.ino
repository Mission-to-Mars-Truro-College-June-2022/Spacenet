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

void set_speed(int l_speed, int r_speed){
  analogWrite(lm_enabled, l_speed);
  analogWrite(rm_enabled, r_speed);
  digitalWrite(lm_forwards, HIGH);
  digitalWrite(rm_forwards, HIGH);
  delay(250);
}

void loop() {
  if(leftcounter== rightcounter){
    set_speed(255,210);
  }
  if(leftcounter > rightcounter){
    set_speed(255,230);    
  }
  if(leftcounter < rightcounter){
    set_speed(230,255);
  }

  
  if (millis() >= 10000){
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

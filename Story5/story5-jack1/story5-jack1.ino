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

int l_speed = 255;
int r_speed = 255;


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

  enabled(true);

 
}

void forward(bool input) { //fowards on/off
  if (input == true){
   digitalWrite(lm_forwards, HIGH);
   digitalWrite(rm_forwards, HIGH);
  }else{
  digitalWrite(lm_forwards, LOW);
  digitalWrite(rm_forwards, LOW);
  }
}

void enabled(bool input) { //enables both motors at high or low
  if (input == true){
   digitalWrite(lm_enabled, HIGH);
   digitalWrite(rm_enabled, HIGH);
  }else{
  digitalWrite(lm_enabled, LOW);
  digitalWrite(rm_enabled, LOW);
  }
}

void reverse(bool input) { //reverse on/off
  if (input == true){
   digitalWrite(lm_reverse, HIGH);
   digitalWrite(rm_reverse, HIGH);
  }else{
  digitalWrite(lm_reverse, LOW);
  digitalWrite(rm_reverse, LOW);
  }
}

void set_speed(int lm_speed, int rm_speed){
  analogWrite(lm_enabled, lm_speed);
  analogWrite(rm_enabled, rm_speed);
}

void go() {  //the logic for the given problem/story
  forward(true);

   if(leftcounter - 5 > rightcounter && leftcounter + 5 < rightcounter) {
     set_speed(255,130);
     digitalWrite(LED_BUILTIN, LOW); 
   }
   else if(leftcounter > rightcounter) { // Turning right, so turn left
     set_speed(235, 155); // Turn Left
     Serial.println("Left turn");
     digitalWrite(LED_BUILTIN, HIGH);
   } else if (rightcounter > leftcounter) { // Turning Left, so turn right
      set_speed(255, 50); // Turn Right
      Serial.println("Right turn");
     digitalWrite(LED_BUILTIN, HIGH);
  }
  
}

void debugCounters() //outputs the numbers stored under each counter
{
  Serial.print("Left ctr: ");
  Serial.print(leftcounter);
  
  Serial.print(" - Right ctr: ");
  Serial.println(rightcounter);

  delay(50);
}

void turn(int D,int l_speed,int r_speed){
  analogWrite(rm_enabled, r_speed);
  analogWrite(lm_enabled, l_speed);
  forward(true);
  delay(D);
  enabled(true);
  
}

void loop() {
  debugCounters();

  go();
  if(millis() >= 10000) 
  {
    Serial.println("Stopped");
    forward(false);
    exit(0);
  }
  
}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

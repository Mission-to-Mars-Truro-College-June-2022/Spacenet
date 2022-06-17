const int LEFT_FEEDBACK = 2;
const int RIGHT_FEEDBACK = 3;

volatile int leftcounter = 0;
volatile int rightcounter = 0;

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(LEFT_FEEDBACK), LeftMotorISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_FEEDBACK), RightMotorISR, RISING);
  
}

void loop() {
  Serial.print("Left Counter ");
  Serial.println(leftcounter);

  Serial.print("Right Counter ");
  Serial.println(rightcounter);

  delay(250);
}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

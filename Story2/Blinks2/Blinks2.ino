int rm_enabled = 9;//ENB
int lm_enabled = 10;//ENA
int rm_reverse = 8; //IN3
int lm_forwards = 11;//IN2
int rm_forwards = 7; //IN4
int lm_reverse = 12; //IN1

void setup() {
  pinMode(rm_enabled, OUTPUT);
  pinMode(lm_enabled, OUTPUT);
  
  pinMode(rm_reverse, OUTPUT);
  pinMode(lm_forwards, OUTPUT);

  pinMode(rm_forwards, OUTPUT);
  pinMode(lm_reverse, OUTPUT);

 
}

void forward(bool input) {
  if (input == true){
   digitalWrite(lm_forwards, HIGH);
   digitalWrite(rm_forwards, HIGH);
  }else{
  digitalWrite(lm_forwards, LOW);
  digitalWrite(rm_forwards, LOW);
  }
}

void enabled(bool input) {
  if (input == true){
   digitalWrite(lm_enabled, HIGH);
   digitalWrite(rm_enabled, HIGH);
  }else{
  digitalWrite(lm_enabled, LOW);
  digitalWrite(rm_enabled, LOW);
  }
}

void reverse(bool input) {
  if (input == true){
   digitalWrite(lm_reverse, HIGH);
   digitalWrite(rm_reverse, HIGH);
  }else{
  digitalWrite(lm_reverse, LOW);
  digitalWrite(rm_reverse, LOW);
  }
}

void go(){
  enabled(true);
  forward(true);
  delay(2000);
  forward(false);
  delay(1000);
  reverse(true);
  delay(2000);
  reverse(false);
  enabled(false);
  delay(1000);
}

void loop() {
   go();
}

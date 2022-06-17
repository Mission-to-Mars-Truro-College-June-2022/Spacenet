int rm_enabled = 9;//ENB
int lm_enabled = 10;//ENA
int rm_reverse = 8; //IN3
int lm_forwards = 11;//IN2
int rm_forwards = 7; //IN4
int lm_reverse = 12; //IN1


int sensorPin = A2;    // select the input pin for the LDR
int sensorPin2 = A6;
int sensorPin3 = A7;
int sensorValueR = 0;
int sensorValueM = 0;
int sensorValueL = 0; // variable to store the value coming from the sensor

int flashCounter = 0; //number of flashes
uint64_t flashTimer = 0; //timer to keep track of time in relation to flashes
bool flash = false; //
bool flashOff = false; //
const int flashTolerance = 800; //the value that represents how much flashing the rover can take 
const int flashDuration = 1500; //how long after a flash to wait to see if another flash appears

void setup() {
  Serial.begin(115200);
  
  pinMode(rm_enabled, OUTPUT);
  pinMode(lm_enabled, OUTPUT);
  
  pinMode(rm_reverse, OUTPUT);
  pinMode(lm_forwards, OUTPUT);

  pinMode(rm_forwards, OUTPUT);
  pinMode(lm_reverse, OUTPUT);

}

void loop() {
  sensorValueR = analogRead(sensorPin);
  sensorValueM = analogRead(sensorPin2);
  sensorValueL = analogRead(sensorPin3);

  /*
  Serial.print("Right: ");
  Serial.println(sensorValueR);
  Serial.print("Middle: ");
  Serial.println(sensorValueM);
  Serial.print("Left: ");
  Serial.println(sensorValueL);
  delay(50);
  Serial.println("----------------------");
  */

  if (sensorValueM < flashTolerance && !flash){ //if the light is being shone and it is a new flash
    set_speed(0,0);
    flashTimer = millis();
    flashCounter++;
    flash = true; //the flashing is true
    Serial.println(sensorValueM);
    Serial.println(flashCounter);
    flashOff = true;
  }else if(sensorValueM > flashTolerance){ //if the flash has ended
    if (flashOff){
      flashTimer = millis();
      flashOff = false;
    }
    flash = false;
  } 
  if (flashCounter > 0 && millis() > flashTimer + flashDuration && !flash){ //if all the flashes are done
    
    switch(flashCounter){
      case 2: // 2 flashes = forwards
        set_speed(255, 255);
        Serial.println("forwards");
        break;

      
      case 3: // 3 = turn left
        set_speed(-150, 150);
        Serial.println("left");
        break;

      case 4: // 4 = turn right
        set_speed(150, -150);
        Serial.println("right");
        break;

      case 5: // 5 = reverse
        set_speed(-255, -255);
        Serial.println("reverse");
        break;

      case 6: // 6 = DANCe
        dance();
        break;
      
       default: // any other result = stop
        set_speed(0,0);
        Serial.println("cease");
        break;
    }
    Serial.print(flashCounter);
    Serial.println("   -   FLASH... WHOA");
    flashCounter = 0;
  }
}

void dance(){
  for (int i = 0; i < 25; i++){
    set_speed(255, -255);
    delay(200);
    set_speed(-255, 255);
    delay(200);
  }
  turn_left(3000);
  turn_right(3000);
  set_speed(255,255);
  delay(1000);
  set_speed(-255,-255);
  delay(1000);
  set_speed(0,0);
}

void dance2(){
  set_speed(255, 255);
  delay(250);
  set_speed(-255,-255);
  delay(500);
  set_speed(255,255);
  delay(250);
  set_speed(255,-255);
  delay(1000);
  set_speed(255, 0);
  delay(250);
  set_speed(-255, 0);
  delay(500);
  set_speed(255, 0);
  delay(250);
  set_speed(0, 255);
  delay(250);
  set_speed(0, -255);
  delay(500);
  set_speed(0, 255);
  delay(250);
  set_speed(255, -255);
  delay(1000);
  set_speed(255,255);
  delay(500);
  set_speed (0,0);
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

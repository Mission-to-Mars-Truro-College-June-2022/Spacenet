#include <NewPing.h>

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
const int flashTolerance = 200; //the value that represents how much flashing the rover can take.(MIDDLE SENSOR)
const int turnFlashTolerence = 100; // the value that represents how much flashing the rover can take.(L + R SENSOR)
const int flashDuration = 1500; //how long after a flash to wait to see if another flash appears
int dance3_counter = 0;

int floor_dist = 0;
int forward_dist = 0;

// Downwards Sonar
#define TRIGGER_PIN_DOWN   13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_DOWN     17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_DOWN 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Forwards Sonar
#define TRIGGER_PIN_FORWARD  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_FOWARD     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_FOWARD 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar_forward(TRIGGER_PIN_FORWARD, ECHO_PIN_FOWARD, MAX_DISTANCE_FOWARD); // NewPing setup of pins and maximum distance.
NewPing sonar_down(TRIGGER_PIN_DOWN, ECHO_PIN_DOWN, MAX_DISTANCE_DOWN); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200);
  
  pinMode(rm_enabled, OUTPUT);
  pinMode(lm_enabled, OUTPUT);
  
  pinMode(rm_reverse, OUTPUT);
  pinMode(lm_forwards, OUTPUT);

  pinMode(rm_forwards, OUTPUT);
  pinMode(lm_reverse, OUTPUT);

  sonar_down.ping_cm(); 

}

void follow_mode() {
  
  const int lightVarience = 170;
  uint64_t lastLight = 0;
  bool stopped = false;
  //while light is found

  Serial.println("In follow mode...");
  
  while(true) {
    sensorValueR = analogRead(sensorPin);
    sensorValueM = analogRead(sensorPin2);
    sensorValueL = analogRead(sensorPin3);

    // Get distance to floor and forward
    floor_dist = sonar_down.ping_cm();
    forward_dist = sonar_forward.ping_cm();
    Serial.print("Floor Dist:");
    Serial.println(floor_dist);
    Serial.print("Forward Dist");
    Serial.println(forward_dist);
    Serial.println("-");

    if (floor_dist > 17){ // flor is moer than 17cm away
      set_speed(-255,-255);
      delay(250);
      set_speed(0,0);
    }

    if(sensorValueM > 260 && sensorValueR > 260 && sensorValueL > 260) {
      // if sensors lost the required light level

      Serial.println("Light lost");
      if(!stopped) {
        set_speed(0, 0);
        lastLight = millis();
        stopped = true;
      } else {
        if (millis() > lastLight + 3000) {
          wiggle();
          break;
        }
      }
    } else { // Sensors have found required light level
      Serial.println("Following light...");
      stopped = false;
      if (sensorValueR > sensorValueL + lightVarience){ // Brighter on left
        set_speed(-255,255); // turn Left
      }
       else if (sensorValueL > sensorValueR + lightVarience){ // Brighter on right
        set_speed(255,-255); // turn right
      } else { // Else fowards
        set_speed(255, 255);
      }
    }
  }
  
  //follow
}

void loop() {
  getLDRValues(); // Read values from lihgt sensors
  // Sensor readout
  Serial.print("Right: ");
  Serial.println(sensorValueR);
  Serial.print("Middle: ");
  Serial.println(sensorValueM);
  Serial.print("Left: ");
  Serial.println(sensorValueL);
  delay(50);
  Serial.println("----------------------");

  
    // Get distance to floor and forward
    floor_dist = sonar_down.ping_cm();
    forward_dist = sonar_forward.ping_cm();
    Serial.print("Floor Dist:");
    Serial.println(floor_dist);
    Serial.print("Forward Dist");
    Serial.println(forward_dist);
    Serial.println("-");

    if (floor_dist > 17){ // FLoor distance check, if distance is more than 17cm away from sensor stop
      set_speed(-255,-255);
      delay(250);
      set_speed(0,0);
    }
  

  if (sensorValueM < flashTolerance && !flash){ //if the light is being shone and it is a new flash
    set_speed(0,0);
    flashTimer = millis();
    flashCounter++;
    flash = true; //the flashing is true
    Serial.println(sensorValueM);
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
        
      case 3: // turning mode
        turning_mode();
        Serial.println("Turning Mode");
        break;
        
      case 4: // 4 = reverse
        set_speed(-255, -255);
        Serial.println("reverse");
        break;

      case 5: // 5 = follow
        follow_mode();
        Serial.println("follow");
        break;

      case 6: // 6 = DANCe
        dance();
        Serial.println("dance 1");
        break;

      case 7: // 7 = DANCe
        dance2();
        Serial.println("dance 2");
        break;

       case 8: // 8 = DANCe
        dance3();
        Serial.println("dance 3");
        break;

       case 9: // 8 = DANCe
        dance4();
        Serial.println("dance 4");
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

void turning_mode(){
  Serial.println("ENTERING TURNING");
  digitalWrite(LED_BUILTIN, HIGH); // Show turnng mode is on
  wiggle();
  
  while(true){
    getLDRValues();
    Serial.print("Right: ");
    Serial.println(sensorValueR);
    Serial.print("Middle: ");
    Serial.println(sensorValueM);
    Serial.print("Left: ");
    Serial.println(sensorValueL);
    Serial.println("----------------------");
    
    if (sensorValueL <= turnFlashTolerence && sensorValueR > turnFlashTolerence){ // If left ldr is receiveing more light
      set_speed(-100, 100); 
      Serial.println("LEFT");
      break;
    } else if (sensorValueL > turnFlashTolerence && sensorValueR <= turnFlashTolerence){ // Right ldr is recieving more light
      set_speed(100, -100);
      Serial.println("RIGHT");
      break;
    }
  }
  Serial.println("EXITED TURNING");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void wiggle(){
  set_speed(-255, 255);
  delay(50);
  set_speed(0,0);
  delay(50);
  set_speed(255, -255);
  delay(50);
  set_speed(0,0);
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
  for (int i=0; i<2; i++){
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
  }
  set_speed(0,0);
}

void dance3(){
  dance3_counter+=1;
  /*
  for (int i=0; i<256; i+=5){
    if (i > 155){
      set_speed(i-200, i); //maxes at 100, 255
      delay(50);
    }else{
      set_speed(0, i);
      delay(50);
    }
    */
   for(int counter=0; counter<3; counter++){
    /*
     for (int i=0; i<156; i+=5){
      set_speed(100+i, 255-i);
      delay(50);
   }
     for (int i=0; i<101; i+=5){
     set_speed(255-i, 155+i);
     delay(50);
   }
   */
   for (int i=0; i<256; i+=5){
    set_speed(i,155-i);
    delay(50);
  }
  if (counter % 2 == 0){
    set_speed(255,-255);
    delay(2750);
  }
  for (int i=0; i<256; i+=5){
    set_speed(155-i,i);
    delay(50);
  }
  if (counter % 2 != 0){
    set_speed(-255,255);
    delay(2750);
  }
  }
  set_speed(0,0);
}

void dance4(){
  for(int i=0; i=510; i+=5){
    set_speed(255-i,255);
    delay(50);
    set_speed(-255+i,-255);
   delay(50);
    set_speed(255,255-i);
    delay(50);
   set_speed(-255,-255+i);
   delay(50);
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
    Serial.println(l_speed);
  } else {
    analogWrite(lm_enabled, l_speed);
    digitalWrite(lm_reverse, LOW);
    digitalWrite(lm_forwards, HIGH);
    Serial.println(l_speed);
  }

  if(r_speed < 0) {
    analogWrite(rm_enabled, -r_speed);
    digitalWrite(rm_forwards, LOW);
    digitalWrite(rm_reverse, HIGH);
    Serial.println(r_speed);
  } else {
    analogWrite(rm_enabled, r_speed);
    digitalWrite(rm_reverse, LOW);
    digitalWrite(rm_forwards, HIGH);
    Serial.println(r_speed);
  }
}

void getLDRValues(){
  sensorValueR = analogRead(sensorPin);
  sensorValueM = analogRead(sensorPin2);
  sensorValueL = analogRead(sensorPin3);
}

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


  const int turn_offset = 20;
  int turn;

  for(turn=0; turn<8; turn++) {
      Serial.println(turn);
    //move
    uint64_t start_millis = millis();


    while (millis() < start_millis + 1250)  {
      if(leftcounter== rightcounter){
        set_speed(255,210 - turn_offset);
      }
      if(leftcounter > rightcounter){
        set_speed(255,230 - turn_offset);    
      }
      if(leftcounter < rightcounter){
        set_speed(230,255 - turn_offset);
      }
    }

    delay(300);
    
    //turn
    if (turn < 3){
       turn_90_right(725);
    } else if (turn == 7){
      break;
    } else if (turn > 3) {
      turn_90_left(700);
     
    }
  }
  for(turn=0; turn<8; turn++) {
    //move
    uint64_t start_millis = millis();
    
    while (millis() < start_millis + 1250)  {
      if(leftcounter== rightcounter){
        set_speed(-255,-210 + turn_offset);
      }
      if(leftcounter > rightcounter){
        set_speed(-255,-230 + turn_offset);    
      }
      if(leftcounter < rightcounter){
        set_speed(-230,-255 + turn_offset);
      }
      
    }

    delay(300);
    
    //turn
    if (turn < 3){
       turn_90_right(800);
    } else if (turn == 7){

      break;
    } else if (turn > 3) {
      turn_90_left(620);
     
    }
  }

  set_speed(0,0);
  digitalWrite(rm_forwards, LOW);
  digitalWrite(lm_forwards, LOW);
  digitalWrite(rm_reverse, LOW);
  digitalWrite(lm_reverse, LOW);
  digitalWrite(rm_enabled, LOW);
  digitalWrite(lm_enabled, LOW);
  exit(0);
  
  
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


  set_speed(0,0);
  exit(0);
  
  /*  
  if(leftcounter== rightcounter){
    set_speed(255,210);
  }
  if(leftcounter > rightcounter){
    set_speed(255,230);    
  }
  if(leftcounter < rightcounter){
    set_speed(230,255);
  }


//  int avg_dist = (rightcounter + leftcounter) / 2;
//  
//  //distance check (360 = 1m)
//  if(avg_dist >= 720 + 150) {
//    set_speed(0,0);
//    exit(0);
//  }
  
  */
}

void LeftMotorISR() {
  leftcounter++;
}

void RightMotorISR(){
  rightcounter++;
}

#include <ArduinoUnit.h>

bool is_a_leap_year(int year){
 
  return (((year % 4) == 0) && (year % 100 != 0)) or (year % 400 == 0);
}

test(a_typical_common_year_returns_false){
  assertFalse(is_a_leap_year(2001));
}
test(a_typical_leap_year_returns_true){
  assertTrue(is_a_leap_year(1996));
}
test(an_atypical_common_year_returns_false){
  assertFalse(is_a_leap_year(1900));
}
test(an_atypical_leap_year_returns_true){
  assertTrue(is_a_leap_year(2000));
}



void setup() {
  Serial.begin(115200);
}

void loop() {
  Test::run();
}

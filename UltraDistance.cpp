#include "Arduino.h"
#include "UltraDistance.h"

UltraDistance::UltraDistance(int pin_echo, int pin_trig){  

  //Pins setup
  pinMode(pin_trig, OUTPUT);
  _pin_trig = pin_trig;
  pinMode(pin_echo, INPUT);
  _pin_echo = pin_echo;

}

long UltraDistance::read(){  
  
  //Distance calculation
  long duration, distance;
  digitalWrite(_pin_trig, LOW);  
  delayMicroseconds(2); 
  digitalWrite(_pin_trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(_pin_trig, LOW);
  duration = pulseIn(_pin_echo, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}

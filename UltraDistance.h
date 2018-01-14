#ifndef UltraDistance_h
#define UltraDistance_h

#include "Arduino.h"
#include "Arduino.h"



class UltraDistance{

	public:
		UltraDistance(int pin_echo, int pin_trig);
		long read();
  	private:
		int _pin_echo;
		int _pin_trig;

};

#endif
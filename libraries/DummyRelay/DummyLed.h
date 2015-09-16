#ifndef LED_H
#define LED_H

class LED {

	public: 
	  	LED();
	    LED(int outPin);
	    void ToggleLED();
	    void TurnOn();
	    void TurnOff();
	private:
	   	void setup();
		int outPin;
		int ledState;

};

#endif
 
 
#include "Button.h"
#include <Arduino.h>



Button::Button(int arduinoPin)
	: buttonPin (arduinoPin)
{
	pinMode(buttonPin, INPUT_PULLUP);
}


void Button::isButtonPressed()
{
	if (digitalRead(buttonPin) == PRESSED) //button is pressed
	{
		if (buttonActive == false) //if it is first cycle with pressed button
			buttonActive = true, buttonTimer = millis(); 


		buttonPressDuration = millis() - buttonTimer;

		if ((buttonPressDuration > longPressThreshold) && (longPressActive == false))
		{
			longPressActive = true;
			Serial.println("Long press true");

		}
			 
	}
	else
	{
		if (shortPressActive == true)
		{
			shortPressActive = false;
			Serial.println("Short press false");
		}
			 

		if (buttonActive==true)
		{

			if (longPressActive == true)
			{
				longPressActive = false;
				Serial.println("Long press false");
			}
				 
			else
			{
				if (buttonPressDuration > debounceThreshold)
				{
					shortPressActive = true;
					Serial.println("Short press true");
				}
					 
			}

			buttonActive = false;// reset the button active status
		}
	}
	
}
bool Button::isShortPress()
{
	return risingEdge(shortPressActive, oldSignalShort);
}

bool Button::isLongPress()
{
	return risingEdge(longPressActive, oldSignalLong);
}

bool Button::risingEdge(bool signal,bool &oldSignal)
{
	bool edge = signal && !oldSignal;
	oldSignal = signal;

	return edge;
}




void Button::executeIfPressed(void (*func)(void),const char * longOrShort)   //const char * longOrShort
{
	

	if (longOrShort == "short" && isShortPress())
		 func();
	else if (longOrShort == "long" && isLongPress())
		func();
}





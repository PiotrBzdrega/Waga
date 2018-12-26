#include "Button.h"
#include <Arduino.h>



Button::Button(int arduinoPin)
	: pin (arduinoPin)
{
	pinMode(pin, INPUT_PULLUP);
}




bool Button::isButtonPressed()
{
	isShortPressed = 0; //ustawiam zmienne aby by³y pozytywne prez jeden cykl.
	isLongPressed = 0;

	currentState = digitalRead(pin);

	if (currentState != prevState)
	{
		delay(debonuce);
		currentState = digitalRead(pin);

		if (currentState==PRESSED)
		{
			counter = millis();
		}

		if (currentState == NOT_PRESSED)
		{
			unsigned long currentMillis = millis();

			if (currentMillis-counter>=longPress)
			{
				isLongPressed = 1;
			}
			else if (currentMillis - counter >= shortPress)
			{
				isShortPressed = 1;
			}
		}
		prevState = currentState;
	}
}


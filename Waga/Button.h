#pragma once

#define PRESSED false
#define NOT_PRESSED true



class Button
{
public:
	Button(int);

	bool isLongPressed;
	bool isShortPressed;
	bool isButtonPressed();
private:
	int pin;
	const int debonuce = 10;
	const unsigned long shortPress = 100;
	const unsigned long  longPress = 1000;
	unsigned long counter = 0;
	bool prevState = NOT_PRESSED;
	bool currentState;
};


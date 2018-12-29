#pragma once

#define PRESSED LOW
#define NOT_PRESSED HIGH



class Button
{
public:
	Button(int);
	void isButtonPressed();
	
	void executeIfPressed(void(*func)(void), const char *); //execute indicated function if short/long pressed button

private:
	bool isLongPress();
	bool isShortPress();
	bool risingEdge(bool,bool&); //detect risignal edge of signal, base on oldSignal 
	int buttonPin; //digital hardware pin 

	const unsigned long longPressThreshold = 5000; // the threshold (in milliseconds) before a long press is detected
	const unsigned long debounceThreshold = 30; // the threshold (in milliseconds) for a button press to be confirmed (i.e. not "noise")

	unsigned long buttonTimer = 0; // stores the time that the button was pressed (relative to boot time)
	unsigned long buttonPressDuration = 0; // stores the duration (in milliseconds) that the button was pressed/held down for


	bool buttonActive = false; // indicates if the button is active/pressed
	bool longPressActive = false; // indicate if the button has been long-pressed
	bool oldSignalLong = false; //oldsignal of rising edge after long-pressed
	bool shortPressActive = false; // indicate if the button has been short-pressed
	bool oldSignalShort = false; //oldsignal of rising edge after short-pressed
	
};


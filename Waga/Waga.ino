/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "Button.h"


//hx711
#define calibration_factor 652.0
#define DOUT  5
#define CLK  6

//lcd
#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7


// --------CONSTANTS LED---------------
const int interval = 1000; 
const int ledPin = 13;
const int buttonPin = 2;
// --------CONSTANTS ANIMATION---------------
const int intervalAnimation = 1000; // number of milisecs when the LED is off
//------------ VARIABLES ANIMATION---------------------
unsigned long previousAnimationMillis = 0;
int animationCharIndex = 1;
bool direction = true;
//------------ VARIABLES BUTTON---------------------
bool oldImpulse = 0;
//------------ VARIABLES LED---------------------
byte targetLedState = HIGH; // this variable is necesary to help arduino monitor the status of LED
							//the initial condition (t=0) can be either HIGH or LOW
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousLedMillis = 0;   // will store last time the LED was updated


byte customChar1[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};
byte customChar2[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111
};
byte customChar3[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
byte customChar4[8] = {
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
byte customChar5[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

void updateTargetLED();
void updateAnimation();
bool risingEdge(bool);

HX711 scale(DOUT, CLK);

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

Button button(2);

void setup() {
	Serial.begin(9600);

	//pinMode(buttonPin, INPUT_PULLUP);


	pinMode(ledPin, OUTPUT);

	lcd.begin(16, 2); //  <<----- My LCD was 16x2

	// Switch on the backlight
	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH);

	/*lcd.home();*/
	lcd.setCursor(1, 0);

	lcd.print(F("Waga"));
	
	lcd.createChar(1, customChar1);
	lcd.createChar(2, customChar2);
	lcd.createChar(3, customChar3);
	lcd.createChar(4, customChar4);
	lcd.createChar(5, customChar5);


	
	scale.set_scale(calibration_factor);
	scale.tare();
}



void loop() 
{
	char buffer[10];
	char c[10];
	lcd.setCursor(0, 1);
	float x = scale.get_units(2); //wydrukowanie wagi, (częstotliwość drukowania)
	dtostrf(x, 5, 1, c);
	sprintf(buffer, "%s", c);
	lcd.print(buffer);

	lcd.setCursor(6, 1);
	lcd.print(F("g"));

	//isLongOrShortState(digitalRead(buttonPin));

	button.isButtonPressed();
	
	if (risingEdge(button.isLongPressed))
	{
		targetLedState = !targetLedState;
		digitalWrite(ledPin, targetLedState);
	}
	////////////////////////////////////////////////////Przekazać wskaźnik na fukcję do risingEdge, żeby nie sprawdzać warunków w loop'ie 
	if (risingEdge(button.isShortPressed))
	{
		scale.tare();
	}
	

	currentMillis = millis();   // capture the latest value of millis()
							  

	updateAnimation();
}




bool risingEdge(bool pin)
{
	bool edge = pin && !oldImpulse;
	oldImpulse = pin;

	return edge;

	/*if (edge == 1)
	{
		scale.tare();
	}*/
}
void updateAnimation()
{
	if ((unsigned long)currentMillis - previousAnimationMillis >= intervalAnimation)
	{
		//pętla w tą i z powrotem
		if (direction == true)
		{
			if (animationCharIndex == 5)
			{
				animationCharIndex = 4;
				direction = false;
			}
			else
			{
				animationCharIndex++;
			}
		}
		else
		{
			if (animationCharIndex == 1)
			{
				animationCharIndex = 2;
				direction = true;
			}
			else
			{
				animationCharIndex--;
			}
		}


		lcd.setCursor(15, 1);
		lcd.write(animationCharIndex);

		previousAnimationMillis += intervalAnimation;

	}
}
void updateTargetLED()
{
	if ((unsigned long)currentMillis - previousLedMillis >= interval)
	{

		targetLedState = !targetLedState;
		digitalWrite(ledPin, targetLedState);

		previousLedMillis += interval;

	}
}

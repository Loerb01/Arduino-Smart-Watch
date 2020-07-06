#include <RTClib.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define x_axis 0
#define y_axis 1
#define sw_pin 8

RTC_DS3231 rtc;

//Define custom symbol for °
byte degree[8] = {
	B01100,
	B10010,
	B10010,
	B01100,
	B00000,
	B00000,
	B00000,
};

void setup() {
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.createChar(0, degree);
	rtc.begin();//Who needs error checking anyways?
}

void loop() {
	writeMainScreen();
	delay(500);
}

//1 = up
//2 = down
//3 = left
//4 = right
//0 = select
int get_input() {
	if(sw_pin == LOW){
		return 0;
	}
	
	if(analogRead(y_axis) <= 260){
		return 1;
	}else if(analogRead(y_axis) >= 760){
		return 2;
	}
	
	if(analogRead(x_axis) <= 260){
		return 3;
	}else if(analogRead(x_axis) >= 760){
		return 4;
	}
	return 5;
}

void writeMainScreen() {
	lcd.clear();
	DateTime now = rtc.now();
	lcd.setCursor(0, 0);
	
	//Because multiple calls to toString() doesn't work
	{
		lcd.print(now.hour());
		lcd.print(":");
		lcd.print(now.minute());
		lcd.print(now.toString("AP"));
		lcd.print(" ");
		lcd.print(now.month());
		lcd.print("/");
		lcd.print(now.day());
		lcd.print("/");
		lcd.print(now.year());
		lcd.setCursor(0, 1);
		lcd.print(now.second());
	}
	
	//Add Whitespace between seconds and temperature
	if(now.second() > 9){
		lcd.print("        ");
	}else {
		lcd.print("         ");
	}
	lcd.print(rtc.getTemperature());
	lcd.write(byte(0));
}
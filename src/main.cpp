#include <Arduino.h>
#include <RTClib.h>
#include <OneButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

#define Taste_A 2
#define Taste_B 3

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long laufzeit = 0;
byte screen_number = 1;

byte alarm_hour = 8;
byte alarm_minute = 45;
boolean alarm_active = false;


OneButton buttonA(Taste_A, true);
OneButton buttonB(Taste_B, true);

void clickeditA() {
  switch (screen_number) {
    case 1:
      screen_number = 2;
      break;
    case 2:
      screen_number = 1;
      break;
    case 3:
      alarm_hour++;
      if (alarm_hour > 23) alarm_hour = 0;
      break;
    case 4:
      alarm_minute++;
      if (alarm_minute > 59) alarm_minute = 0;
      break;
  }
}

void clickeditB() {
  if (screen_number >= 2) screen_number++;
  if (screen_number > 4) screen_number = 2;
}

void setup() {
  pinMode(Taste_A, INPUT_PULLUP); //Button A
  pinMode(Taste_B, INPUT_PULLUP); //Button B

  buttonA.attachClick(clickeditA);
  buttonB.attachClick(clickeditB);

  Serial.begin(9600);
  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Could not find RTC");
    while (1);
  }

  Serial.println("RTC found and connected!");

  if (! rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC time and date set!");
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  buttonA.tick();
  buttonB.tick();
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setCursor(0,0);

  switch (screen_number) {
    case 1:
      display.println("Current Time & Date");
      display.print(now.year(), DEC);
      display.print('/');
      display.print(now.month(), DEC);
      display.print('/');
      display.println(now.day(), DEC);
      display.println(daysOfTheWeek[now.dayOfTheWeek()]);
      if (now.hour()<10) display.print("0");
      display.print(now.hour(), DEC);
      display.print(':');
      if (now.minute()<10) display.print("0");
      display.print(now.minute(), DEC);
      display.print(':');
      if (now.second()<10) display.print("0");
      display.print(now.second(), DEC);
      display.println();
      break;
    case 2:
      display.println("Alarm Time");
      if (alarm_hour<10) display.print("0");
      display.print(alarm_hour, DEC);
      display.print(':');
      if (alarm_minute<10) display.print("0");
      display.println(alarm_minute, DEC);
      break;
    case 3:
      display.println("Set Alarm Hour");
      display.println("To set press Button A");
      display.print("Hour: ");
      if (millis() % 1500 < 1000) {
        if (alarm_hour<10) display.print("0");
        display.println(alarm_hour);
      } else
        display.println("__");
      break;
    case 4:
      display.println("Set Alarm Minute");
      display.println("To set press Button A");
      display.print("Minute: ");
      if (millis() % 1500 < 1000) {
        if (alarm_minute<10) display.print("0");
        display.println(alarm_minute);
      } else
        display.println("__");
      break;
  }

  display.print("Screen: ");
  display.println(screen_number);

  if (millis() - 250 > laufzeit) {
    display.display();
    laufzeit = millis();
  }

}

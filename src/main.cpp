#include <Arduino.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  if (! rtc.begin()) {
    Serial.println("Could not find RTC");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  //nothing to do here
}

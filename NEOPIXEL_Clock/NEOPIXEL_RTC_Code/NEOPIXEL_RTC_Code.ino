#include <DS1302.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
const int NUM_LEDS = 24; // your Neopixel ring LED count
const int BRIGHTNESS_POT_PIN = A0; // pin where pot is connected

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// DS1302 RTC
const int RST_PIN = 9;
const int DAT_PIN = 10;
const int CLK_PIN = 11;
DS1302 rtc(RST_PIN, DAT_PIN, CLK_PIN);

String serialBuffer = "";

void readSerialTime() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      serialBuffer.trim();

      // HH:MM:SS
      if (serialBuffer.length() == 8 && serialBuffer.charAt(2) == ':' && serialBuffer.charAt(5) == ':') {
        int hour = serialBuffer.substring(0, 2).toInt();
        int min  = serialBuffer.substring(3, 5).toInt();
        int sec  = serialBuffer.substring(6, 8).toInt();

        if (hour >= 0 && hour < 24 && min >= 0 && min < 60 && sec >= 0 && sec < 60) {
          rtc.time(Time(2024, 6, 28, hour, min, sec, Time::kFriday));
          Serial.println("Time set to " + serialBuffer);
        } else {
          Serial.println("Invalid time values!");
        }
      } else {
        Serial.println("Please use HH:MM:SS format");
      }

      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }
}

void showClock(Time t) {
  int hourLED   = t.hr % NUM_LEDS;
  int minuteLED = (t.min * NUM_LEDS) / 60;
  int secondLED = (t.sec * NUM_LEDS) / 60;

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t r = 0, g = 0, b = 0;

    int potValue = analogRead(BRIGHTNESS_POT_PIN);

    if (i == hourLED)   r = map(potValue, 0, 1023, 0, 255);
    if (i == minuteLED) g = map(potValue, 0, 1023, 0, 255);
    if (i == secondLED) b = map(potValue, 0, 1023, 0, 255);

    strip.setPixelColor(i, r, g, b);
  }

  strip.show();
}

void setup() {
  Serial.begin(115200);
  rtc.halt(false);
  rtc.writeProtect(false);

  strip.begin();
  strip.show();
}

void loop() {
  readSerialTime();

  Time t = rtc.time();
  showClock(t);
}

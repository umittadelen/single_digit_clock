#include <DS1302.h>

const int RST_PIN = 9;
const int DAT_PIN = 10;
const int CLK_PIN = 11;
DS1302 rtc(RST_PIN, DAT_PIN, CLK_PIN);

const int segmentPins[7] = {3, 2, 6, 7, 8, 5, 4};

const byte digitSegments[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
};

void displayDigit(int digit) {
  for (int i = 0; i < 7; i++)
    digitalWrite(segmentPins[i], digitSegments[digit][i] ? HIGH : LOW);
}

void clearDisplay() {
  for (int i = 0; i < 7; i++)
    digitalWrite(segmentPins[i], LOW);
}

String serialBuffer = "";

void readSerialTime() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      serialBuffer.trim();
      if (serialBuffer.length() == 5 && serialBuffer.charAt(2) == ':') {
        int hour = serialBuffer.substring(0, 2).toInt();
        int min  = serialBuffer.substring(3, 5).toInt();
        if (hour >= 0 && hour < 24 && min >= 0 && min < 60) {
          rtc.time(Time(2024, 6, 28, hour, min, 0, Time::kFriday));
          Serial.println("Time set to " + serialBuffer);
        } else {
          Serial.println("Invalid time!");
        }
      } else {
        Serial.println("Please use HH:MM format");
      }
      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }
}

// Simple Display state
unsigned long lastUpdate = 0;
int currentDigit = 0;
int digits[4] = {0, 0, 0, 0};
bool displayOn = false;

void setup() {
  for (int i = 0; i < 7; i++)
    pinMode(segmentPins[i], OUTPUT);
  Serial.begin(115200);

  rtc.halt(false);
  rtc.writeProtect(false);
}

void loop() {
  readSerialTime();

  Time t = rtc.time();
  digits[0] = t.hr / 10;
  digits[1] = t.hr % 10;
  digits[2] = t.min / 10;
  digits[3] = t.min % 10;

  unsigned long now = millis();

  // First digit gets extra delay before showing
  int delayBetween = (currentDigit == 0 && !displayOn) ? 1000 : 500;

  if (now - lastUpdate >= delayBetween) {
    if (displayOn) {
      clearDisplay();
      displayOn = false;
      lastUpdate = now;
    } else {
      displayDigit(digits[currentDigit]);
      displayOn = true;
      lastUpdate = now;
      currentDigit = (currentDigit + 1) % 4;
    }
  }
}

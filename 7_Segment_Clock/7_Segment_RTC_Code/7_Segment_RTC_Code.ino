#include <DS1302.h>

const int RST_PIN = 9;
const int DAT_PIN = 10;
const int CLK_PIN = 11;
DS1302 rtc(RST_PIN, DAT_PIN, CLK_PIN);

const int segmentPins[7] = {3, 2, 6, 7, 8, 5, 4};
bool reverse_lights = true;  // <- Set this to true to reverse HIGH/LOW | some segments have VCC as common, so LOW means ON

const byte digitSegments[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
};

void setSegment(int pin, bool on) {
  digitalWrite(pin, (reverse_lights ? !on : on) ? HIGH : LOW);
}

void displayDigit(int digit) {
  for (int i = 0; i < 7; i++)
    setSegment(segmentPins[i], digitSegments[digit][i]);
}

void clearDisplay() {
  for (int i = 0; i < 7; i++)
    setSegment(segmentPins[i], false);
}

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

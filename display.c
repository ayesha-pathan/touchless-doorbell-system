#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PROX_PIN    2     // IR sensor digital output to D2
#define BUZZER_PIN  8

// If your sensor is active-LOW (most FC-51/KY-032), leave this true.
// If yours outputs HIGH when object is near, set this to false.
bool SENSOR_ACTIVE_LOW = true;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ring only once per approach within this window (ms)
const unsigned long COOLDOWN_MS = 3000;
unsigned long lastRing = 0;

void setup() {
  pinMode(PROX_PIN, INPUT);  // many modules already have on-board pull-ups
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Touchless Door");
  lcd.setCursor(0,1); lcd.print("Bell (Nano+IR)");
  delay(1200);
  lcd.clear();
}

bool visitorDetected() {
  int raw = digitalRead(PROX_PIN);
  return SENSOR_ACTIVE_LOW ? (raw == LOW) : (raw == HIGH);
}

void ringBellOnce() {
  lcd.setCursor(0,0); lcd.print("Visitor Detected");
  lcd.setCursor(0,1); lcd.print("Proximity Alert  ");

  // 1s tone; works with active or passive buzzers
  tone(BUZZER_PIN, 2000, 1000);
  delay(1100);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  if (visitorDetected() && (now - lastRing >= COOLDOWN_MS)) {
    lastRing = now;
    ringBellOnce();
  }

  // small poll delay
  delay(50);
}
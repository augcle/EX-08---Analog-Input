#include <Arduino.h>

const uint8_t pinR = 9;
const uint8_t pinG = 10;
const uint8_t pinB = 11;

const uint8_t potPin = A0;

// Uno default analog reference is ~5V (can vary slightly depending on USB power)
const float VREF = 5.0;

// Potentiometer is wired to 3.3V and GND in this exercise
const float MAX_INPUT_VOLT = 3.3;

void setRGB(int r, int g, int b) {
  analogWrite(pinR, constrain(r, 0, 255));
  analogWrite(pinG, constrain(g, 0, 255));
  analogWrite(pinB, constrain(b, 0, 255));
}

void setup() {
  Serial.begin(115200);

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  setRGB(0, 0, 0);
}

void loop() {
  int adc = analogRead(potPin);              // 0..1023
  float volt = adc * (VREF / 1023.0);        // 0..~5V

  // print with 3 decimals
  Serial.print("ADC=");
  Serial.print(adc);
  Serial.print("  V=");
  Serial.println(volt, 3);

  // Normalize 0..3.3V -> 0..1
  float t = volt / MAX_INPUT_VOLT;
  t = constrain(t, 0.0, 1.0);

  // 8e: 0V purple (255,0,255) -> max red (255,0,0)
  int r = 255;
  int g = 0;
  int b = (int)(255.0 * (1.0 - t));

  setRGB(r, g, b);

  delay(20); // smooth updates
}


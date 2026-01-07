/**
 * @file Ex8_Analog.ino
 * @author
 * @date 2026-01-07
 * @brief Exercise 8: Read analog voltage from a potentiometer and fade an RGB LED.
 *
 * @details
 * This sketch reads an analog value from pin A0 using the Arduino Uno ADC (0..1023).
 * The ADC value is converted to voltage using a reference voltage (VREF).
 * The measured voltage is then mapped to a color fade:
 * - 0.0 V   -> Purple (R=255, G=0, B=255)
 * - 5.0 V+  -> Red    (R=255, G=0, B=0)
 *
 * The RGB LED is controlled using PWM via analogWrite() on pins 9, 10, and 11.
 *
 * @hardware
 * - Potentiometer:
 *   - Middle pin (wiper) -> A0
 *   - Outer pins -> 5.0V and GND
 * - RGB LED (common cathode / common ground):
 *   - Longest leg -> GND
 *   - R, G, B legs -> PWM pins 9, 10, 11 through resistors
 *
 * @note
 * VREF is set to 5.0 V (Uno default analog reference).
 */

#include <Arduino.h>

/**
 * @name RGB PWM pins
 * @brief PWM output pins used to drive the RGB LED channels.
 * @{
 */
const uint8_t pinR = 9;   ///< PWM pin for Red channel
const uint8_t pinG = 10;  ///< PWM pin for Green channel
const uint8_t pinB = 11;  ///< PWM pin for Blue channel
/** @} */

/**
 * @brief Analog input pin connected to the potentiometer wiper.
 */
const uint8_t potPin = A0;

/**
 * @brief ADC reference voltage in volts.
 * @details
 * Default Uno analog reference is ~5V (can vary slightly depending on USB power).
 */
const float VREF = 5.0;

/**
 * @brief Maximum expected input voltage from the potentiometer in volts.
 * @details
 * In this exercise the potentiometer is wired between 5.0V and GND, so the wiper
 * voltage range is 0..5.0V.
 */
const float MAX_INPUT_VOLT = 5.0;

/**
 * @brief Write PWM values to the RGB LED.
 *
 * @details
 * The input values are constrained to the valid PWM range (0..255) before being
 * written with analogWrite(). For a common-cathode RGB LED, higher values mean
 * brighter light for that color channel.
 *
 * @param r Red channel intensity (0..255)
 * @param g Green channel intensity (0..255)
 * @param b Blue channel intensity (0..255)
 */
void setRGB(int r, int g, int b) {
  analogWrite(pinR, constrain(r, 0, 255));
  analogWrite(pinG, constrain(g, 0, 255));
  analogWrite(pinB, constrain(b, 0, 255));
}

/**
 * @brief Arduino setup function.
 *
 * @details
 * Initializes serial communication and configures RGB pins as outputs.
 * Also turns the RGB LED off initially.
 */
void setup() {
  Serial.begin(115200);

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  setRGB(0, 0, 0);
}

/**
 * @brief Arduino main loop.
 *
 * @details
 * 1) Reads the ADC value from the potentiometer (0..1023).
 * 2) Converts it into volts using VREF.
 * 3) Prints the ADC value and voltage to Serial with 3 decimals.
 * 4) Normalizes the voltage from 0..5.0V into a factor t in range 0..1.
 * 5) Fades the RGB LED from purple to red by decreasing the blue channel.
 */
void loop() {
  int adc = analogRead(potPin);              // 0..1023
  float volt = adc * (VREF / 1023.0);        // 0..~VREF

  // Print with 3 decimals
  Serial.print("ADC=");
  Serial.print(adc);
  Serial.print("  V=");
  Serial.println(volt, 3);

  // Normalize 0..5.0V -> 0..1 and clamp
  float t = volt / MAX_INPUT_VOLT;
  t = constrain(t, 0.0, 1.0);

  // 0V purple (255,0,255) -> 5.0V red (255,0,0)
  int r = 255;
  int g = 0;
  int b = (int)(255.0 * (1.0 - t));

  setRGB(r, g, b);

  delay(20); // smooth updates
}

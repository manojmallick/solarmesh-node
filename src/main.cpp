#include <Arduino.h>

/**
 * SolarMesh Node — solar-powered Meshtastic infrastructure node (MVP).
 * Production: charge controller telemetry + deep sleep duty cycle + LoRa beacon.
 */

constexpr int LED_PIN = 2;
constexpr float FULL_BATTERY_V = 4.15f;

float batteryV = 3.95f;
uint32_t sleepSec = 30;
uint32_t beacons = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(300);
  Serial.println(F("SolarMesh Node — install-and-forget mesh infra (sim)"));
}

void loop() {
  // Solar charge / drain model
  float solar = (float)random(0, 100) / 100.0f;
  batteryV += (solar > 0.4f ? 0.01f : -0.005f);
  if (batteryV > FULL_BATTERY_V) batteryV = FULL_BATTERY_V;
  if (batteryV < 3.3f) batteryV = 3.3f;

  // Adaptive sleep: lower battery → longer sleep
  sleepSec = batteryV < 3.6f ? 120 : 30;
  beacons++;

  digitalWrite(LED_PIN, HIGH);
  Serial.printf(
      "{\"beacon\":%lu,\"battery_v\":%.2f,\"solar\":%.2f,\"sleep_s\":%lu}\n",
      (unsigned long)beacons,
      batteryV,
      solar,
      (unsigned long)sleepSec);
  delay(100);
  digitalWrite(LED_PIN, LOW);

  // Deep sleep stand-in
  delay(sleepSec * 10); // scaled down for serial demo (real: esp_deep_sleep)
}

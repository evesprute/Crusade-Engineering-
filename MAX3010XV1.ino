#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

uint32_t irBuffer[100];
uint32_t redBuffer[100];

int32_t bufferLength = 100;
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

unsigned long lastIRPrint = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 SDA, SCL

  if (!particleSensor.begin(Wire)) {
    Serial.println("MAX30102 not found. Check wiring.");
    while (1);
  }
  byte ledBrightness = 40;
  byte sampleAverage = 8;
  byte ledMode = 2;
  byte sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 16384;

  particleSensor.setup(
    ledBrightness,
    sampleAverage,
    ledMode,
    sampleRate,
    pulseWidth,
    adcRange
  );

  Serial.println("Place finger on sensor and keep still.");
}

void loop() {
  for (byte i = 0; i < bufferLength; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();

    if (millis() - lastIRPrint > 2000) {
      lastIRPrint = millis();
      Serial.print("IR=");
      Serial.println(irBuffer[i]);
    }
  }

  maxim_heart_rate_and_oxygen_saturation(
    irBuffer,
    bufferLength,
    redBuffer,
    &spo2,
    &validSPO2,
    &heartRate,
    &validHeartRate
  );

  Serial.print("Heart Rate: ");
  if (validHeartRate) {
    Serial.print(heartRate);
    Serial.print(" BPM");
  } else {
    Serial.print("Invalid");
  }

  Serial.print(" | SpO2: ");
  if (validSPO2) {
    Serial.print(spo2);
    Serial.println("%");
  } else {
    Serial.println("Invalid");
  }

  delay(1000);
}

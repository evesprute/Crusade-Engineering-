const int ECG_PIN = 34;   // AD8232 OUT
const int LO_PLUS = 25;   // AD8232 LO+
const int LO_MINUS = 26;  // AD8232 LO-

void setup() {
  Serial.begin(115200);

  pinMode(ECG_PIN, INPUT);
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);

  Serial.println("ECG test started");
}

void loop() {
  if (digitalRead(LO_PLUS) == HIGH || digitalRead(LO_MINUS) == HIGH) {
    Serial.println("Leads off!");
  } else {
    int ecgValue = analogRead(ECG_PIN);
    Serial.println(ecgValue);
  }

  delay(50); // ~200 samples/sec
}

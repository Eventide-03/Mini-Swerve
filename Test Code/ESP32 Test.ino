void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 booted");
  pinMode(LED_BUILTIN, OUTPUT);  // GPIO48 on DevKitC-1 = onboard RGB LED
}

uint32_t counter = 0;
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.printf("alive: %u\n", counter++);
}
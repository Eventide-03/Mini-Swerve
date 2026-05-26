#define MKS_TX 15
#define MKS_RX 18

HardwareSerial& MKSSerial = Serial1;

void setup() {
  Serial.begin(115200);
  MKSSerial.begin(115200, SERIAL_8N1, MKS_RX, MKS_TX);

  delay(2000);
  Serial.println("MKS UART test starting");

  delay(500);
  MKSSerial.println("M1.target=0.0");
  Serial.println("Sent: M1.target=0.0");
}

void loop() {
  Serial.println("Moving to 1.57 rad");
  MKSSerial.println("M1.target=1.57");
  delay(2000);

  Serial.println("Moving to 0.00 rad");
  MKSSerial.println("M1.target=0.0");
  delay(2000);

  while (MKSSerial.available()) {
    Serial.write(MKSSerial.read());
  }
}
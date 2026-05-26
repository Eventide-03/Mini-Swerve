#include <CRSFforArduino.hpp>

#define ELRS_RX 16
#define ELRS_TX 17
CRSFforArduino crsf = CRSFforArduino(&Serial2, ELRS_RX, ELRS_TX);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("ELRS test starting");

  if (!crsf.begin()) {
    Serial.println("CRSF init failed!");
    while (1) delay(100);
  }
  Serial.println("CRSF initialized");
}

void loop() {
  crsf.update();

  // Channels are 1-16 in CRSF
  // Standard mode 2: ch1=roll, ch2=pitch, ch3=throttle, ch4=yaw
  uint16_t roll = crsf.getChannel(1);
  uint16_t pitch = crsf.getChannel(2);
  uint16_t throttle = crsf.getChannel(3);
  uint16_t yaw = crsf.getChannel(4);

  Serial.printf("Roll:%4d Pitch:%4d Throttle:%4d Yaw:%4d\n",
                roll, pitch, throttle, yaw);
  delay(100);
}
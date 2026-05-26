#include <DShotRMT.h>

#define MOTOR1_PIN 4

DShotRMT motor1(MOTOR1_PIN, DSHOT600);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("DSHOT test starting");

  motor1.begin();
  delay(1000);

  Serial.println("Arming ESC...");
  for (int i = 0; i < 100; i++) {
    motor1.send_dshot_value(0);
    delay(10);
  }
}

void loop() {
  Serial.println("Ramping up...");
  for (int throttle = 48; throttle < 300; throttle += 2) {
    motor1.send_dshot_value(throttle);
    delay(50);
  }

  Serial.println("Ramping down...");
  for (int throttle = 300; throttle > 48; throttle -= 2) {
    motor1.send_dshot_value(throttle);
    delay(50);
  }

  Serial.println("Idle for 2 seconds...");
  for (int i = 0; i < 200; i++) {
    motor1.send_dshot_value(0);
    delay(10);
  }
}
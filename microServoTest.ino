#include <Servo.h>
Servo microServo;

void setup() {
  microServo.attach(9);
  // put your setup code here, to run once:
}

void loop() {
  microServo.write(0);
}

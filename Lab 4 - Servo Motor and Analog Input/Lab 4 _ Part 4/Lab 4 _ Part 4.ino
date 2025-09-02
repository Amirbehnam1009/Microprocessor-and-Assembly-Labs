#include <Servo.h>

Servo myservo;  // Create servo object to control a servo

const int potpin = A0;  // Analog pin used to connect the potentiometer
int val;    // Variable to read the value from the analog pin

void setup() {
  myservo.attach(2);  // Attaches the servo on pin 9 to the servo object
}

void loop() {
  val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 90);
  myservo.write(val);
  delay(15);
}

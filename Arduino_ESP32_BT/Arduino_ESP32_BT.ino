
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define motor_int1 23
#define motor_int2 22
#define motor_int3 21
#define motor_int4 19
#define servo_pin 18

void setup() {
  Serial.begin(115200);
  SerialBT.begin("BT test");

  Serial.println(SerialBT.getBtAddressString());

  pinMode(motor_int1, OUTPUT);
  pinMode(motor_int2, OUTPUT);
  pinMode(motor_int3, OUTPUT);
  pinMode(motor_int4, OUTPUT);

  ledcAttach(motor_int1, 1000, 8);
  ledcAttach(motor_int2, 1000, 8);
  ledcAttach(motor_int3, 1000, 8);
  ledcAttach(motor_int4, 1000, 8);

  Serial.println("Bluetooth ready!");
}

void motorControl(int pin1, int pin2, float speed) {
  int pwm = constrain((int)(abs(speed) * 255), 0, 255);

  if (speed < -0.1) {
    ledcWrite(pin1, pwm);
    ledcWrite(pin2, 0);
  }
  else if (speed > 0.1) {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, pwm);
  }
  else {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, 0);
  }
}

void loop() {
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n');

    float leftY, rightY;

    if (sscanf(data.c_str(), "%f,%f", &leftY, &rightY) == 2) {
      motorControl(motor_int1, motor_int2, leftY);
      motorControl(motor_int3, motor_int4, rightY);

      Serial.print("Left: ");
      Serial.print(leftY);
      Serial.print(" | Right: ");
      Serial.println(rightY);
    }
  }
}

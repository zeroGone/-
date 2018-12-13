#include <AFMotor.h>

AF_DCMotor motor(3, MOTOR34_64KHZ);

void setup() {
  Serial.begin(9600);
  Serial.read();
  Serial.println("test");
  motor.setSpeed(150);
}

void loop() {
  if(Serial.available()) {
    int number = Serial.parseInt();  
    Serial.println(number);

  if(number==1){
    motor.run(FORWARD);
  }
  else if(number==2){
    motor.run(BACKWARD);
  }
  
  delay(1000);
  motor.run(RELEASE);
 }
}
  

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlServo.h"

#define FREQUENCY   50
#define NB_FINGERS  5
char caractere = '0';
Servo servo;

void setup() {
  Serial.begin(9600); 
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  for(int increment = 0; increment<NB_FINGERS; increment++){
      servo.servoOut(caractere,increment);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if(!moveDone && servo.servoOut('o',4)){
    Serial.println("in if");
    moveDone = true;                            ///< If the code enters this section, the move is done.
    for(int decrement = NB_FINGERS-1; decrement>-1; decrement--){
      Serial.print("dec 1.");
      Serial.println(decrement);
      servo.reverseMove('o',decrement);
    }
    moveDone = false;  
  }
  delay(1000);
  for(int increment = 0; increment<NB_FINGERS; increment++){
      Serial.print("switch 1.");
      Serial.println(increment);
      servo.servoOut('o',increment);
  }
  delay(5000);*/
}

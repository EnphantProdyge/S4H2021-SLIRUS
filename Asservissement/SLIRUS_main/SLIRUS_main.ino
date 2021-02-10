#include <Wire.h> //pour utiliser protocol i2C, need adresse de pin
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>   

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN    650
#define SERVOMAX    2350
#define FREQUENCY   50

void setup() {
  int angle = 50; // de 4 à 180 c'est OKAY
  
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  pwm.begin();
  pwm.setPWMFreq(50);  // This is the maximum PWM frequency (between 40-1600)

  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  pwm.setPWM(0,0,analog_value);
  pwm.setPWM(1,0,analog_value);
  //Wire.begin(THIS_ADDRESS);

  
}

void loop() {
  // put your main code here, to run repeatedly:

}

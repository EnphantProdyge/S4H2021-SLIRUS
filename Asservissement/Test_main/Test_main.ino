#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlServo.h"
#include <DynamixelWorkbench.h>
#include <string>

#define FREQUENCY   50
#define NB_FINGERS  5
#define BAUDRATE  57600
#if defined(__OPENCM904__)
#define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
#define DEVICE_NAME ""
#endif  
String word1 = "cr";
uint16_t model_number = 0;
uint16_t model_number2 = 0;
char result, result2;
int32_t position = 0;
Servo servo;

void setup() {
  Serial.begin(BAUDRATE); 
  while (!Serial); // Wait for Opening Serial Monitor

  //Initialisation Dinamixel
  dxl_wb.init(DEVICE_NAME, BAUDRATE); //Initialisation du systeme qui gere les moteurs
  result = dxl_wb.ping(11, &model_number);
  result2 = dxl_wb.ping(12, &model_number2);
  dxl_wb.setExtendedPositionControlMode(12);
  dxl_wb.setPositionControlMode(11);
  result = dxl_wb.setVelocityBasedProfile(11);
  result = dxl_wb.itemWrite(11,"Profile_Velocity",150 );
  result = dxl_wb.itemWrite(11,"Profile_Acceleration",30);
  //result = dxl_wb.itemWrite(11,"Drive_Mode",0);
  dxl_wb.torqueOn(11);
  dxl_wb.torqueOn(12);

  //Initialisation Servo
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  
  //Initialisation base
  dxl_wb.itemRead(12,"Present_Position",&position);
  servo.setupBase(position);

  //Initialisation fingers and wrist
  for(int increment = 0; increment<NB_FINGERS; increment++){
      servo.servoOut('5',increment);
  }
}

void loop() {
  // string to individual caracteres
  if (word1.length()>0){
    Serial.println("in if");
    Serial.println(word1);
      char caracteres[sizeof(word1)+1];
      strcpy(caracteres, word1.c_str());
      for (int i = 0; i < (word1.length()); i++){
        Serial.print("for i = ");
        Serial.println(i);
        Serial.print("Caractere = ");
        Serial.println(caracteres[i]);
        delay(1000);
        for(int increment = 0; increment<NB_FINGERS; increment++){
            Serial.println("for increment");
            servo.servoOut(caracteres[i],increment);
        }
        delay(1000);
        for(int decrement = NB_FINGERS-1; decrement >= 0; decrement--){
            Serial.println("for decrement");
            servo.reverseMove(caracteres[i],decrement);
        }
      }
  }
}

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlServo.h"
#include <DynamixelWorkbench.h>
#include <string>
#include "Communication.h"

#define FREQUENCY   50
#define NB_FINGERS  5
#define BAUDRATE  57600
#if defined(__OPENCM904__)
#define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
#define DEVICE_NAME ""
#endif  

String msg = "";
uint16_t model_number = 0;
uint16_t model_number2 = 0;
char result, result2;
int32_t position = 0;

COM_Arduino ROB;
Servo servo;

void setup() {
  Serial.begin(BAUDRATE); 
  //while (!Serial); // Wait for Opening Serial Monitor

  //Initialisation Dinamixel
  dxl_wb.init(DEVICE_NAME, BAUDRATE); //Initialisation du systeme qui gere les moteurs
  result = dxl_wb.ping(11, &model_number);
  result2 = dxl_wb.ping(12, &model_number2);
  dxl_wb.setExtendedPositionControlMode(12);
  dxl_wb.setPositionControlMode(11);
  result = dxl_wb.setVelocityBasedProfile(11);
  result = dxl_wb.itemWrite(11,"Profile_Velocity",80 );
  result = dxl_wb.itemWrite(11,"Profile_Acceleration",20);
  result = dxl_wb.itemWrite(11,"Drive_Mode",0);
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
  if (ROB.Get_state() == 1){// waiting for input
    if(Serial.available()>0){// waiting for the serial port to receive an input 
        msg = ROB.ReadMessage();//Reading the string sent
        ROB.Get_status();// Sending the status
        ROB.SendMessage(msg);//Sending the message received
    }
  }
  if (ROB.Get_state() == 2){
    delay(500);
    char caracteres[sizeof(msg)+1];
    strcpy(caracteres, msg.c_str());
    for (int i = 0; i < (msg.length()); i++){
      delay(2000);
      Serial.print("Interface:start:"); //Message to interface : start letter
      Serial.println(caracteres[i]);
      for(int increment = 0; increment<NB_FINGERS; increment++){
        servo.servoOut(caracteres[i],increment);
      }
      delay(2000);
        for(int decrement = NB_FINGERS-1; decrement >= 0; decrement--){
          servo.reverseMove(caracteres[i],decrement);
        }
      Serial.print("Interface:done:");//Message to interface : finish letter
      Serial.println(caracteres[i]);
    }
   ROB.Set_state(3);
    
  }
  if (ROB.Get_state() == 3){  
    delay(500);
    Serial.println("Interface:exit");
    ROB.SendMessage("IM DONE");
    msg = "";
    ROB.Set_state(1);
    
  }    
  
}

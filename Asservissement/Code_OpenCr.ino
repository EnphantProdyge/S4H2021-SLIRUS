#include "Communication.h"


void setup() {
  Serial.begin(57600);
}
  COM_Arduino ROB;
  String msg = "";
void loop() {


  if (ROB.Get_state() == 1)// waiting for input
  {
    if(Serial.available()>0)// waiting for the serial port to receive an input 
    {
      msg = ROB.ReadMessage();//Reading the string sent
      ROB.Get_status();// Sending the status
      ROB.SendMessage(msg);//Sending the message received
    }
  }
  
  if (ROB.Get_state() == 2)
  {
    ROB.Set_state(3);
    delay(10000);
  }

  if (ROB.Get_state() == 3)
  {
    ROB.SendMessage("IM DONE");
  }
  
  
}

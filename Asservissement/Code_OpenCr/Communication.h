// Communication.h
//

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <Arduino.h>
#include <ArduinoJson.h>

//Class to initiate Serial communication with the Raspberry Pi 
class COM_Arduino
{
public:
  
//Constructor
  COM_Arduino()
  {
   state = state1;// initial state 
  };
  
//Methode to send information(a String) to the Raspberry Pi
  void SendMessage(String str)
  {
    Serial.println(str);
  }

//Methode to read information form the Raspberry Pi
  String ReadMessage()
  {
    String msg = Serial.readStringUntil('\n');
    if (msg == "")
    {
      msg = "Signal received but no message with it";
      return msg;
    }
    state = state2;
    return msg;
  };

//Methode to set a new state to the object
  void Set_state(int new_state)
  {
    state = new_state;
  };

//Methode to get the state of the object
  int Get_state()
  {
    return state; 
  };
  void Get_status()
  {
      if (state == state1)
    {
      Serial.println("The robot is wiating for an input");
    }
    if (state == state2)
    {
      Serial.println("Message received");
    }
    if (state == state3)
    {
      Serial.println("Robot is doing a sign");
    } 
  };  
  
private:
//Variable for the state of the object
  int state;
//type of state possible
  int state1 = 1; // Waiting for input
  int state2 = 2; // Message received
  int state3 = 3; // robot is doing a sign
};

#endif  // _COMMUNICATION_H

// Communication.h
//

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <Arduino.h>
#include <ArduinoJson.h>


class COM_Arduino
{
public:
  COM_Arduino()
  {
    //Constructor
   state = state1;// initial state 
  };

  void SendMessage(String str)
  {
    Serial.println(str);
  }

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

  void Set_state(int new_state)
  {
    state = new_state;
  };

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
  int state;
  int state1 = 1; // Waiting for input
  int state2 = 2; // Message received
  int state3 = 3; // robot is doing a sign
};

#endif  // _COMMUNICATION_H

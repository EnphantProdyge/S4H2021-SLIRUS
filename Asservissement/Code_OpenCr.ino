#include "Communication.h"


void setup() {
  Serial.begin(57600);
  COM_Arduino 
}

void loop() {
  
  if(Serial.available()>0)
  {
    msg = 
    Serial.println(msg);
  }
    
  
}

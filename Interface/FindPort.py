#This code finds what Rapsberry Pi the serial port is used for the communication with the arduino and sends information by serial port.
import sys
import glob
import serial
import serial.tools.list_ports
import json
import time

#Function that finds the serial port used by the arduino
def serial_ports():
    #if on windows:
    #Finds all ports available
    if sys.platform.startswith('win'):
        ports = list(serial.tools.list_ports.comports())

    #if on linux:
    #Finds all ports available
    elif sys.platform.startswith('linux'):
        ports = list(serial.tools.list_ports.comports())
    #Send an error if the platform is unsupported
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        #if on windows:
        if sys.platform.startswith('win'):
            if "Arduino" in port[1] or "USB" in port[1]:
                #Finds the port connected to the arduino
                result.append(port[0])
        #if on linux
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            if "ACM0" in port[1] or "AMA0" in port[1]:
                #Finds the port connected to the arduino
                result.append(port[0])
        #Send an error if the platform is unsupported
        else:
            raise EnvironmentError('Unsupported platform')
    return result


def SendStrToOpenCR(port,msg):
    #Telling the code where to send the information(string)
    ser = serial.Serial("/dev/ttyACM0", 57600, timeout=1) #try this line if the you dont receive anything form the arduino or if the arduino doesn't receive what you are sending
    #ser = serial.Serial(port, 57600, timeout=1)          #else use this line
    ser.flush()
    line = ""
    
    #Sends the string to the arduino
    ser.write(msg.encode('utf-8'))
    while line != "IM DONE" :
        #Verifies if the arduino is sending information back
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(1)



if __name__ == '__main__':
    list = serial_ports()
    Str_port = list[0]
    string = input("Quel est le string?:\t")
    SendStrToOpenCR(Str_port,string)

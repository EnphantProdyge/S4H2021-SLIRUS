# coding=utf-8
import sys
import glob
import serial
import serial.tools.list_ports
import json
import time

def serial_ports():

    if sys.platform.startswith('win'):
        ports = list(serial.tools.list_ports.comports())
        #print(ports)

    elif sys.platform.startswith('linux'):
        # this excludes your current terminal "/dev/tty"
        ports = list(serial.tools.list_ports.comports())
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        j = 0
        list2 = []
        for i in port:
            list2.insert(j,i)
            #print(i)
            #print(list2)
            #print(port[1])
            #print(list2)
            j+=1
        if sys.platform.startswith('win'):
            if "Arduino" in port[1] or "USB" in port[1]:
                #print(port)
                #print("in")
                result.append(port[0])
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            if "ACM0" in port[1]:
                result.append(port[0])
        else:
            raise EnvironmentError('Unsupported platform')
    #print(result)
    return result


def SendStrToOpenCR(port,msg):
    ser = serial.Serial(port, 57600)
    ser.write(msg.encode("utf-8"))
    print(msg)








if __name__ == '__main__':
    # print(sys.version_info)
    list = serial_ports()
    Str_port = list[0]
    print(Str_port)

    SendStrToOpenCR(Str_port,"Xd trop drole hahaha")


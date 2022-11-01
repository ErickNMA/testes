#Baixar módulo pyserial: https://pypi.org/project/pyserial/#files
#instalar módulo pyserial: https://www.youtube.com/watch?v=Pf-cGzOQmXU

import serial

port = serial.Serial('4')

while(True):
    print(port.readline())
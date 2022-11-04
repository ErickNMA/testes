import serial
import time

def check_port():
    for i in range(10): 
        try:
            return serial.Serial(('COM'+str(i)), 9600)
        except serial.SerialException:
            pass
port = check_port()

time.sleep(3)
port.timeout = 3

port.write(bytes(180, 'utf-8'))

port.close()
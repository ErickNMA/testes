import serial
import time

def check_port():
    for i in range(10): 
        try:
            return serial.Serial(('COM'+str(i)), 9600)
        except serial.SerialException:
            pass
port = check_port()

time.sleep(2)

while True:
    send = str(0)
    port.write(send.encode('utf-8'))
    listen = port.readline()
    print(listen.decode('utf-8'))

port.close()
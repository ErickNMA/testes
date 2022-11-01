from ast import Bytes
from operator import indexOf
import serial
import time
import matplotlib.pyplot as plt
import numpy as np

#Parâmetros do motor:
max_voltage = 6 #tensão máxima, em volts
max_rpm = 80 #rotação máxima, em RPM
reduction_ratio = 1/100 #razão de redução

#Identificação da porta COM:
def check_port():
    for i in range(10): 
        try:
            return serial.Serial(('COM'+str(i)), 9600) 
        except serial.SerialException:
            pass
port = check_port()

#Função para controle do sistema:
def velocity_analysis(duration, prec, u):
    tu = np.arange(0, duration, (10**(-prec)))
    ve = []
    te = []
    try:
        port.open()
    except:
        pass
    t0 = time.time()
    ct = time.time()
    while(True):
        
        #pwm = u[indexOf(tu, round((ct-t0), prec))]
        pwm = 200
        port.write(pwm.to_bytes(8, 'little'))
        data = port.readline().decode('utf-8')
        if((ct-t0) < duration):
            ve.append(float(data))
            te.append(ct-t0)
        ct = time.time()
        if((ct-t0) >= duration):
            port.close()
            return tu, te, ve,

#Função para plotar resultados:
def plot(tc, vc, te, ve, tu, u):
    plt.figure(1)
    plt.subplot(2, 1, 1)
    #plt.rcParams['xtick.labelsize'] = 20
    #plt.rcParams['ytick.labelsize'] = 20
    plt.plot(tc, vc, 'b', label='$Calculado$')#, linewidth=3)
    plt.plot(te, ve, 'r', label='$Encoder$')#, linewidth=3)
    plt.ylabel('|$\omega}$| [rad/s]')#, fontsize=28)
    plt.legend()#fontsize=18)
    plt.title('Comparação das curvas de velocidade e sinal de controle', fontsize=20)
    plt.ylim(-(((max_rpm*2*np.pi)/(60*reduction_ratio))+20), (((max_rpm*2*np.pi)/(60*reduction_ratio))+20))
    plt.grid()
    plt.subplot(2, 1, 2)
    #plt.rcParams['xtick.labelsize'] = 20
    #plt.rcParams['ytick.labelsize'] = 20
    plt.plot(tu, u, 'g', label='$PWM$')#, linewidth=3)
    plt.ylabel('$PWM$')#, fontsize=28)
    #plt.legend(fontsize=18)
    plt.xlabel('Tempo [s]')#, fontsize=28)
    plt.ylim(-270, 270)
    plt.grid()
    plt.show()





#################################################################################################################################

#Definição do sinal de controle:
u1 = 100*np.ones(200)
u2 = -180*np.ones(200)
u3 = 200*np.ones(200)
u4 = 0*np.ones(200)
u5 = -50*np.ones(200)
u = np.hstack((u1, u2, u3, u4, u5))
#u = -255*np.ones(500)

#Controle e análise do sistema:
tu, te, ve = velocity_analysis(10, 2, u)

#Cálculos e definições:
tc = tu
vc = ((max_rpm*2*np.pi/(60*reduction_ratio))*((u+1e-15)/255))

#Plotagem das respostas temporais:
plot(tc, vc, te, ve, tu, u)
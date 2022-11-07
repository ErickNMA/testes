//Bibliotecas:
#include <Arduino.h>
#include <math.h>
#include <string.h>

//Definição dos pinos:
#define A 2
#define B 3
#define M1 5
#define M2 6
#define EN 10

//Resolução do encoder:
#define resolution 11

//Razão de redução:
#define reduction_ratio 1/300 

//Definição de variáveis:
bool active=false;
int value=0;
long step=0, stepant=0, i=0;
float v=0, pos[2], time[2];

void check_A()
{
  bool ra = digitalRead(A);
  bool rb = digitalRead(B);
  
  if(ra && !rb)
  {
    step--;
  }
}

void check_B()
{
  bool ra = digitalRead(A);
  bool rb = digitalRead(B);
  if(!ra && rb)
  {
    step++;
  }
}

void update_velocity()
{
  switch(i%3)
  {
    case 0:
      stepant = step;
      i++;
    break;
    case 1:
      if(step != stepant)
      {
        pos[0] = (step*2*M_PI*reduction_ratio/resolution);
        time[0] = (micros()/1e6);
        stepant = step;
        i++;
      }      
    break;
    case 2:
      if(step != stepant)
      {
        pos[1] = (step*2*M_PI*reduction_ratio/resolution);
        time[1] = (micros()/1e6);
        v = ((pos[1]-pos[0])/(time[1]-time[0]));
        i++;
      }
    break;
  }  
}

//Inicializações:
void setup() 
{
  Serial.begin(9600);

  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(EN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(A), check_A, RISING);
  attachInterrupt(digitalPinToInterrupt(B), check_B, RISING);
}

void loop() 
{
  update_velocity();  
  if(active)
  {
    //Retorna a velocidade atual:
    Serial.println(v);
  }

  //Obtenção do comando de controle:
  if(Serial.available())
  {
    value = atoi(Serial.readString().c_str());
    Serial.flush();
    if(value == 48)
    {
      active = !active;
      value = 0;
    }else
    {
      value = round((value-150)*2.55);
    }    
  }  

  //Envio do sinal para o motor:
  if(value > 0)
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
    analogWrite(EN, abs(value));
  }
  if(value < 0)
  {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
    analogWrite(EN, abs(value));
  }
  if(value == 0)
  {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(EN, 255);
    v = 0;
  }
}
//Bibliotecas:
#include <Arduino.h>
#include <math.h>
#include <string.h>

//Definição dos pinos:
#define A 2
#define B 3
//#define M1 2
//#define M2 3
#define EN 10

//Resolução do encoder:
#define resolution 11

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
        pos[0] = (step*2*M_PI/resolution);
        time[0] = (micros()/1e6);
        stepant = step;
        i++;
      }      
    break;
    case 2:
      if(step != stepant)
      {
        pos[1] = (step*2*M_PI/resolution);
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
  //pinMode(M1, OUTPUT);
  //pinMode(M2, OUTPUT);
  pinMode(EN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(A), check_A, RISING);
  attachInterrupt(digitalPinToInterrupt(B), check_B, RISING);
}

void loop() 
{
  update_velocity();
  Serial.println(v);
}
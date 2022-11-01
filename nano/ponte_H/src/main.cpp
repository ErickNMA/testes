#include <Arduino.h>

//Definição dos pinos:
#define in_1 7
#define in_2 8
#define enable 9

void setup() 
{
  //Inicializações:
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(enable, OUTPUT);

  digitalWrite(in_1, LOW);
  digitalWrite(in_2, LOW);
  analogWrite(enable, 0);
}

void loop() 
{
  //Controle da ponte H:
  digitalWrite(in_1, LOW);
  digitalWrite(in_2, HIGH);
  //Variar PWM (0 ~ 255):
  analogWrite(enable, 255);
}
//Bibliotecas:
#include <Arduino.h>
#include <math.h>

//Definição dos pinos:
#define A 5
#define B 6

//Resolução do encoder:
#define resolution 11

//Inicializações:
void setup() 
{
  Serial.begin(9600);

  pinMode(B, INPUT);
  pinMode(A, INPUT);
}

//Definição de variáveis:
bool last_B=true, last_A=true;
long step=0;
unsigned long i=0;
double time[2], v=0, pos[2];
int mode=0;

void loop() 
{

  //Leitura do encoder:
  bool current_B = digitalRead(B);
  bool current_A = digitalRead(A);
  //Aguarda contato:
  if((last_B && last_A) && !mode)
  {
    mode = 1;
  }
  //Identifica sentido:
  if(mode == 1)
  {
    if(current_B != current_A)
    {
        if(current_B)
        {
          step--;
        }else
        {
          step++;
        }
        time[i%2] = (micros()/1e6);
        pos[i%2] = (step*2*M_PI/resolution);
        if(i%2)
        {
          v = ((pos[1]-pos[0])/(time[1]-time[0]));
          Serial.print("\n V = ");
          Serial.print(v);
          Serial.println(" rad/s");
        }
        i++;     
        mode = 2;    
    }  
  }
  //Fim do contato mecânico:
  if(mode == 2)
  {
    if(!last_B && !last_A)
    {
      mode = 0;
    }
  }
  last_B = current_B;
  last_A = current_A;
}
//Bibliotecas:
#include <Arduino.h>
#include <math.h>

//Definição dos pinos:
#define DT 5
#define CLK 6

//Resolução do encoder:
#define resolution 20

//Inicializações:
void setup() 
{
  Serial.begin(9600);

  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);
}

//Definição de variáveis:
bool last_DT=true, last_CLK=true;
long step=0;
unsigned long i=0;
double time[2], v=0, pos[2];
int mode=0;

void loop() 
{

  //Leitura do encoder:
  bool current_DT = digitalRead(DT);
  bool current_CLK = digitalRead(CLK);
  //Aguarda contato:
  if((last_DT && last_CLK) && !mode)
  {
    mode = 1;
  }
  //Identifica sentido:
  if(mode == 1)
  {
    if(current_DT != current_CLK)
    {
        if(current_DT)
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
    if(!last_DT && !last_CLK)
    {
      mode = 0;
    }
  }
  last_DT = current_DT;
  last_CLK = current_CLK;
}
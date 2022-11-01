//Bibliotecas:
#include <Arduino.h>
#include <math.h>
#include <string.h>

//Definição dos pinos:
#define A 5
#define B 6
#define M1 2
#define M2 3
#define EN 10

//Resolução do encoder:
#define resolution 11

//Inicializações:
void setup() 
{
  Serial.begin(9600);

  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(EN, OUTPUT);
}

//Definição de variáveis:
bool last_B=true, last_A=true;
long step=0;
unsigned long i=0;
double time[2], v=0, pos[2];
int mode=0, value;
char data[20];

void loop() 
{
  //Obtenção do comando de controle:
  char caractere;
  bool sinal = false;
  // Enquanto receber algo pela serial
  int l=0;
  while(Serial.available() > 0) 
  {
    sinal = true;
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n')
    {
      data[l] = caractere;
    }else
    {
      data[l] = '\0';
      break;
    }
    l++;
    // Aguarda buffer serial ler próximo caractere
    //delay(10);
  }
  if(sinal)
  {
    value = atoi(data);
  }else
  {
    value = 0;
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
    analogWrite(EN, 200);
  }

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
          Serial.println(v);
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
/*  Conectar:
  Arduino UNO - DRV8833
      5(AIN1) -   AIN1
      3(AIN2) -   AIN2
      9(BIN1) -   BIN1
     6(BIN2) -   BIN2
*/
#define AIN1 5
#define AIN2 3
#define BIN1 9
#define BIN2 6

void Motors_Init();
void MotorEsqSpeed(int Speed);
void MotorDirSpeed(int Speed);
void MotorsSpeed(int Vel_Esq, int Vel_Dir);

//motor
const int Trig_right  = 2;
const int Echo_right  = 4;
const int Trig_front  = 11;
const int Echo_front  = 12;
const int Trig_left   = A1;
const int Echo_left   = A2;

//sensor chama
int sensorPin         = A0;
int sensorValue       = 0;
int sensorValue0      = 0;
int sensorValue45     = 0;
int sensorValue90     = 0;
int sensor_left       = 0;
int sensor_center     = 0;
int sensor_right      = 0;
int contador          = 0;
int chama_enter       = 35;
int chama_stop        = 990;
boolean fita_branca1  = false;
boolean fita_branca2  = false;
boolean enter_room    = false;
boolean chama         = false;
boolean stop_rob      = false;
boolean start         = false;
boolean return_rob    = false;
boolean random_rob    = true;
boolean esquerda      = true;
boolean direita       = true;
boolean frente        = true;
boolean find_return   = false;
boolean find_start    = false;

//sensor cor
#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 color_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
int white             = 40;
int black             = 25;
int red               = 7500;
int blue              = 18000;

//start
const int buttonPin   = 8;
int buttonState       = 0;
int count             = 0;
int nrooms            = 0;       //para contar quantos quartos entrou
int return_count      = 0;       //para ir contando as linhas pelo regresso


void setup() {
  
  Motors_Init();
  MotorsSpeed(0,0);

  Serial.begin(9600);
  color_sensor.begin();

  //pin para os sensores de distancia
  pinMode(Trig_right,OUTPUT);
  pinMode(Echo_right,INPUT);
  pinMode(Trig_front,OUTPUT);
  pinMode(Echo_front,INPUT);
  pinMode(Trig_left,OUTPUT);
  pinMode(Echo_left,INPUT);

  pinMode(13,OUTPUT);         //led vermelho
  pinMode(10,OUTPUT);         //ventoinha
  pinMode(buttonPin, INPUT);  //botao start
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH){
    start=true;
  }
  
  if(start==true){
    if(return_rob==false){ // vai apagar a vela
    count=count+1;
  
    uint16_t clear, red, green, blue;
    color_sensor.getRawData(&red, &green, &blue, &clear);
    int colorTemp = color_sensor.calculateColorTemperature(red,green,blue);

    if(find_start==false){
      //right
      scan(Trig_right);
      long duracao_side=pulseIn(Echo_right, HIGH);
      long cm_side= distance(duracao_side);
      //front
      scan(Trig_front);
      long duracao_front=pulseIn(Echo_front, HIGH);
      long cm_front= distance(duracao_front);

      if(cm_side<11 || cm_front<10){
        find_start=true;
      }
    }

    if(clear>white && find_start==false){
      nrooms=nrooms+1;
      MotorsSpeed(90,-60);
      delay(1);
      MotorsSpeed(180,180);
      delay(2); 
    }
    else if(clear<black && find_start==false){
      MotorsSpeed(200,-180);
      delay(1);
      MotorsSpeed(180,180);
      delay(2); 
    }
    else if(clear>white && fita_branca1==false && count>5 && nrooms>0 && find_start==true){  // caso onde encontra a fita branca
      nrooms=nrooms+1;
      digitalWrite(13, LOW);
      MotorsSpeed(0,0);
      delay(500);
      fita_branca1=true;
      MotorsSpeed(180,180);
      delay(1000);
      MotorsSpeed(0,0);
      delay(300);
      sensorValue0 = analogRead(sensorPin); //valor do sensor para a chama quando vai direito
      Serial.print("chama   ");
      Serial.println(sensorValue);
      MotorsSpeed(0,0);
      delay(300);
      MotorsSpeed(180,-180);
      delay(162.5);
      sensorValue90 = analogRead(sensorPin); //direita
      MotorsSpeed(-180,180);
      delay(325);
      MotorsSpeed(0,0);
      delay(30); //antes era 300
      sensorValue45 = analogRead(sensorPin); //esquerda
      ///////////////////VERIFICAR INSPEÇAO DA CHAMA EM TODOS OS QUARTOS

      if(sensorValue0>chama_enter && sensorValue0>sensorValue90 && sensorValue0>sensorValue45 ){ //vai em frente
        enter_room=true;
        MotorsSpeed(180,-180);
        delay(162.5);
      }
      else if(sensorValue45>chama_enter && sensorValue45>sensorValue90 && sensorValue45>sensorValue0){//vai para a esquerda
        enter_room=true;
      }
      else if(sensorValue90>chama_enter && sensorValue90>sensorValue45 && sensorValue90>sensorValue0){//vai para a direita
        enter_room=true;
        MotorsSpeed(180,-180);
        delay(325);
      }
      else{ //rotaçao para sair do quarto TESTAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        MotorsSpeed(-180,180);
        delay(162.5);
        MotorsSpeed(-180,180);
        delay(650); 
        MotorsSpeed(180,180);
        delay(1350); // já saiu do quarto
        MotorsSpeed(120,-120);
        delay(349);
        MotorsSpeed(180,180); //se estiver descoordenado, tirar esta instruçao meter robotmove
        delay(600);
      
        fita_branca1=false;
      }
    }
    else if(clear>white && fita_branca1==false && count>5 && random_rob==true && find_start==true){
      nrooms=nrooms+1;
      MotorsSpeed(180,180);
      delay(200);
    }
    else if(clear>white && fita_branca1==false && count<=5 && find_start==true){
      move_rob(Echo_right,Trig_right);
    }
    else if(enter_room==true){ //quer dizer que entrou no quarto e tem a vela (<25 o chao é preto)
        digitalWrite(13, LOW);
    
        sensorValue = analogRead(sensorPin);
        ComeCloserToFlame();
        
        sensorValue = analogRead(sensorPin);
        if(sensorValue>chama_stop){
          MotorsSpeed(0,0);
          digitalWrite(13,HIGH);
          delay(1000);
          digitalWrite(10,HIGH);
          MotorsSpeed(30,-30);
          delay(500);
          MotorsSpeed(-30,30);
          delay(1000);
          digitalWrite(10,LOW);
          digitalWrite(13,LOW);
        
        //nao falta comando para continuar a apagar ate threshold minimo? !????????????????????????????????
          return_rob=true;
          MotorsSpeed(180,-180); //VOLTA DE 180 apos apagar chama E SE FOSSE delay(162,5)= 45graus????????????????????????????????
          delay(650);
       }
    }
    else if(clear<black && enter_room==false && find_start==true){ // caso onde o chao é preto e esta fora do quarto
      digitalWrite(13, LOW); 
      move_rob(Echo_right,Trig_right);
    }
    else{ // caso onde encontra um tapete com uma cor
      if(colorTemp>blue && find_start==true){ //é azul //////////////////////////////////////////////////////////MUDEI para piscar
        digitalWrite(13, HIGH);
        delay(5);
        digitalWrite(13, LOW);
        delay(5);
        move_rob(Echo_right,Trig_right);
      }
      else if(colorTemp<red && find_start==true){ // é vermelho
        digitalWrite(13, HIGH); 
        move_rob(Echo_right,Trig_right);
      }
    }
  }
  else{ //vai regressar a posiçao inical
      if(return_count<=nrooms){
        uint16_t clear, red, green, blue;
        color_sensor.getRawData(&red, &green, &blue, &clear);
        int colorTemp = color_sensor.calculateColorTemperature(red,green,blue);

        if(find_return==false){
          //right
          scan(Trig_left);
          long duracao_side=pulseIn(Echo_left, HIGH);
          long cm_side= distance(duracao_side);
          //front
          scan(Trig_front);
          long duracao_front=pulseIn(Echo_front, HIGH);
          long cm_front= distance(duracao_front);

          if(cm_side<11 || cm_front<10){
            find_return=true;
           }
         }

        if(clear>white && find_return==false){ //TENTA SE COLAR A PAREDE APOS APAGAR CHAMA E SAIR DO QUARTO (TENTAR MAIS RAPIDO? MAIOR NEGATIVO NA DIREITA)
          return_count=return_count+1;
          MotorsSpeed(60,-60); //MotorsSpeed(-60,60) pois tem de se colar a parede da esquerda e assim caminha mal//////////////////////////////////////////////
          delay(1);
          MotorsSpeed(180,180);
          delay(2); 
         }
        else if(clear<black && find_return==false){//TENTA SE COLAR A PAREDE APOS APAGAR CHAMA ainda dentro do quarto
          MotorsSpeed(-180,180); // PODE DAR MERDA POIS ANDA MUITO AS VOLTAS SE NAO ENCONTRAR PAREDE: DIMINUIR VALORES DO MORTORSPEED///////////////////////////////////////////////
          delay(1);
          MotorsSpeed(180,180);
          delay(2); 
        } 
        else if(return_count==(nrooms) && clear>white && find_return==true){ //NAO PÁRA POIS AINDA NAO IGUALOU NROOMS SOLUTION:TIRAR NROOMS./////////////////////////////
          MotorsSpeed(180,180);
          delay(420);
          MotorsSpeed(0,0);
          delay(5);
          //Celebration
          start=false;
        }
        else if(return_count>0 && clear>white && find_return==true){
          return_count=return_count+1;
          MotorsSpeed(100,-100); // SE ELE VOLTAR A TOCAR NA LINHA APOS A REJEITAR, AUMENTAR DELAY !!!!!!!!!!!!!!!!!!!!!!!!!
          delay(650); 
          MotorsSpeed(180,180);
          delay(300); 
          move_rob(Echo_left,Trig_left);
        }
        else if(return_count==0 && clear>white && find_return==true){
          return_count=return_count+1;
          MotorsSpeed(180,180);
          delay(150);
          move_rob(Echo_left,Trig_left);
        }   
      }
    }
  }
}

void move_rob(int Echo_side, int Trig_side){
  MotorsSpeed(250,250);
   delay(1.7);

  //right,left
  scan(Trig_side);
  long duracao_side=pulseIn(Echo_side, HIGH);
  long cm_side= distance(duracao_side);
  //front
  scan(Trig_front);
  long duracao_front=pulseIn(Echo_front, HIGH);
  long cm_front= distance(duracao_front);

   if(cm_front<11){
    if(return_rob==false){
      MotorsSpeed(-120,120);
    }
    else{
      MotorsSpeed(120,-120);
    }
    delay(349);
  }

  if(cm_side <10){
    if(return_rob==false){
      MotorsSpeed(-200,195);
    }
    else{
      MotorsSpeed(195,-200);
    }
    delay(1.5);
  }
  else if(cm_side>14){
    if(return_rob==false){
      MotorsSpeed(180,-120);
    }
    else{
      MotorsSpeed(-120,180);
    }
    delay(1.1);
  }
  else if(cm_side>11 && cm_side<=14){
    if(return_rob==false){
      MotorsSpeed(195,-200);
    }
    else{
      MotorsSpeed(-200,195);
    }
    delay(1.5);
  }
  else{
    MotorsSpeed(250,250);
    delay(1.49);
  }
}

void scan (int var){
  digitalWrite(var,LOW);
  delayMicroseconds(2);
  digitalWrite(var,HIGH);
  delayMicroseconds(5);
  digitalWrite(var,LOW);
}

long distance( long dur){
  return dur/29/2;
}

/*
  Chamem isto para configurar os pins.
*/
void Motors_Init() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

/*
  Muda a velocidade de só do motor da esquerda
*/
void MotorDirSpeed(int Speed) {
  if (Speed == 0) {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 1);
  }
  bool forwards = 1;

  if (Speed < 0) {
    Speed = Speed * (-1);
    forwards = 0;
  }
  if(Speed > 255)
    Speed = 255;
    
  Speed = 255-Speed;

  if (forwards) {
    digitalWrite(AIN1, HIGH);
    analogWrite(AIN2, Speed);
  }
  else {
    analogWrite(AIN1, Speed);
    digitalWrite(AIN2, HIGH);  
  }
}

/*
  Muda a velocidade de só do motor da direita
*/
void MotorEsqSpeed(int Speed) {
  if (Speed == 0) {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 1);
  }
  bool forwards = 1;

  if (Speed < 0) {
    Speed = Speed * (-1);
    forwards = 0;
  }
  if(Speed > 255)
    Speed = 255;
    
Speed = 255-Speed;
  if (forwards) {
    digitalWrite(BIN1, HIGH);
    analogWrite(BIN2, Speed);
  }
  else {
    analogWrite(BIN1, Speed);
    digitalWrite(BIN2, HIGH);  
  }
}

/*
  Muda a velocidade de ambos os motores
*/

void MotorsSpeed(int Vel_Esq, int Vel_Dir) {
  MotorEsqSpeed(Vel_Esq);
  MotorDirSpeed(Vel_Dir);
}

void ComeCloserToFlame(){

// I need the sensor function to read the temperature
  sensorValue = analogRead(sensorPin); //valor do sensor para a chama.
  int ant=sensorValue;

  if (contador==0){
    MotorsSpeed(-60,60);
    delay(30);
    sensorValue = analogRead(sensorPin); //valor do sensor para a chama.
    sensor_left=sensorValue;
    delay(5);
  }
  
  if(ant<sensor_left){
    esquerda=true;
  }
  else{
    esquerda=false;
  }

  contador++;
    
  if (esquerda){
    MotorsSpeed(80,80);
    delay(20);
    contador=0;
  }
  else if(!esquerda){
    MotorsSpeed(120,-120);
    delay(60);
    sensorValue = analogRead(sensorPin); //valor do sensor para a chama.
    sensor_right=sensorValue;
    delay(5);
    if(ant<sensor_right){
      direita=true;
    }

    else if(ant>sensor_right){
      direita=false;
    }
    
  }
  
  if (direita){ 
    MotorsSpeed(80,80);
    delay(20);
    contador=0;
  }
  else if(!direita){
    MotorsSpeed(-60,60);
    delay(30);
    MotorsSpeed(80,80);
    delay(20);
    
  }

  
      
}

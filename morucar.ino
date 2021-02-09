#include "IRremote.h"

int RECEIVER = 13;
int ENABLE = 5;
int DIRA = 3;
int DIRB = 4;
int POWER = 10;

IRrecv irrecv(RECEIVER); // 受信で使用するオブジェクトを作成 'irrecv'
decode_results results;  // 受信情報の格納先を作成 'results'

int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(POWER, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  pinMode(RECEIVER, INPUT);
}


void FrontMotor(){
  //前進
  digitalWrite(ENABLE,HIGH); 
  digitalWrite(DIRA,LOW); 
  digitalWrite(DIRB,HIGH);
}
void BackMotor(){
  //後進
  digitalWrite(ENABLE,HIGH); 
  digitalWrite(DIRA,HIGH); 
  digitalWrite(DIRB,LOW);
}

void StopMotor(){
  //止まる
  digitalWrite(ENABLE,LOW);
  digitalWrite(DIRA,LOW); 
  digitalWrite(DIRB,LOW);
}

void translateIR() // takes action based on IR code received
{
  switch(results.value){
    case 0xFFA25D: Serial.println("POWER"); 
      //接続確認
      count++;
      count = count % 2;
      digitalWrite(POWER, count);
      break;
    case 0xFF629D: Serial.println("VOL+"); 
      //前進
      FrontMotor();
      break;
    case 0xFF02FD: Serial.println("PAUSE"); 
      //止まる
      StopMotor();
      break;
    case 0xFFA857: Serial.println("VOL-");
      //後進
      BackMotor();
      break;
    default: 
    Serial.println(" other button   ");
  }// End Case
  delay(500);
}

void controller(){
  //リモコン
  if (irrecv.decode(&results)) 
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}




void loop() {
  controller();
}

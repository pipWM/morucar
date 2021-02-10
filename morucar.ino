#include "IRremote.h"
#include "pitches.h"
#include "NewTone.h"

int RECEIVER = 13;
int ENABLE = 5;
int DIRA = 3;
int DIRB = 4;
int POWER = 10;
int BUZZER = 8;

IRrecv irrecv(RECEIVER); // 受信で使用するオブジェクトを作成 'irrecv'
decode_results results;  // 受信情報の格納先を作成 'results'

int count = 0;

int pui[] = {NOTE_G6, NOTE_B6, NOTE_G6, NOTE_B6,0};
int op[] = {NOTE_A5, NOTE_FS5, NOTE_A5, NOTE_B5, 0, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_D5, 0, 0, NOTE_A5, NOTE_FS5, NOTE_A5, NOTE_B5, 0, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_FS5, 0, 0, NOTE_G4, NOTE_FS4, NOTE_F4, NOTE_E4, 0, NOTE_A5, NOTE_FS5, NOTE_A5, NOTE_B5, 0, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_D5, 0};
int duration = 50;

void OP(){
  //オープニングの最後のところが流れる
  int length = sizeof(op) / sizeof(*op);
  for (int thisNote = 0; thisNote < length; thisNote++) {
    NewTone(BUZZER, op[thisNote], duration);
    delay(100);
  }
}

void PUI(){
  //鳴き声が流れる
  int length = sizeof(pui) / sizeof(*pui);
  for (int thisNote = 0; thisNote < length; thisNote++) {
    NewTone(BUZZER, pui[thisNote], duration);
    delay(100);
  }
}

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
    case 0xFF30CF: Serial.println("1");
      //オープニングが流れる
      OP();
      break;
    case 0xFF18E7: Serial.println("2");
      //鳴き声が流れる
      PUI();
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

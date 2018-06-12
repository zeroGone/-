#include "pitches.h"

char *help[] = {"help","1","2","3"};
char serial_data[10];
int speakerPin = 2;
int tempPin = 1;
int button1 = 5;
int button2 = 6;
int button_count = 0;

int digitForNum[10][8] = {//7세그먼트 표시할 배열
  {1, 1, 1, 0, 1, 1, 1, 0}, //0
  {0, 0, 1, 0, 1, 0, 0, 0}, //1
  {1, 1, 0, 0, 1, 1, 0, 1}, //2
  {0, 1, 1, 0, 1, 1, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1, 0}, //4
  {1, 0, 1, 1, 0, 1, 1, 0}, //5
  {1, 0, 1, 1, 1, 1, 1, 0}, //6
  {1, 1, 1, 0, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1, 0}, //8
  {1, 1, 1, 1, 0, 1, 1, 0}  //9
};
int  segmentLEDs[] = {7,8,9,10,11,12,13,14};

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(tempPin, INPUT);

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  attachPinInterrupt(button1,callback1,HIGH); 
  attachPinInterrupt(button2,callback2,HIGH); 
  
  for (int i = 0 ; i < 8; i++) {
    pinMode(segmentLEDs[i], OUTPUT);
  }
}

// 음표 길이들 : 4 = 4분(1/4) 음표, 8 = 8분(1/8) 음표, 
unsigned int noteDurations[] = {
   4, 4, 4, 4, 4, 4, 2,
   4, 4, 4, 4, 2, 2
};

// 멜로디를 위한 음표들:
unsigned int melody[] = {
   NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4,
   NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0
};


void play_school_bell(void)
{ 
  int noteDuration = 0, thisNote = 0, pauseBetweenNotes = 0;
  for (thisNote = 0; thisNote < sizeof(noteDurations) / sizeof(int); thisNote++) {

    // 음표 길이를 계산하기 위하여, 1초를 음표 타입으로 나눕니다. 
    // 예를 들어, 4분 음표 = 1000 / 4,  8분 음표 = 1000/8 등과 같이
    // 합니다.
    noteDuration = 1000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote]);
    delay(noteDuration);

    // 음표들을 구분하기 위하여, 음표 사이에 최소한의 지연 시간을
    // 음표 길이 + 30%가 잘 동작하는 것 같습니다:
    noTone(speakerPin);
    pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
     // 음 발생하는 것을 멈춤니다:
     noTone(speakerPin);
}


void serial_input(){
  int num=0;
  char input=0;
  while(!Serial.available());//시리얼 통신에 입력된 것이 없으면 반복문에 걸려 무한 반복 
  while(Serial.available()>0){//시리얼 통신에 입력된 것이 있으면
    delay(50);
    input=Serial.read();//하나씩 읽어와서 배열에 저장
    serial_data[num]=input;
    
    if (input == '\r' || input == '\n' || input == '\a'){
      Serial.print("Blueino Received: ");
      serial_data[num] = 0;
      Serial.println(serial_data);
    }
    num=(num+1)%10;
  }
}

int cmd_interpret(char *cmd)
{
     for(int i=0; i< (sizeof(help)/sizeof( char *)); i++)
     {
         if(!strcmp(help[i], cmd)) return i;
     }
     return 0;
}

void ledOnOff(){
  digitalWrite(3,HIGH);
  delay(100);
  digitalWrite(3,LOW);
  Serial.println("led on/off");
}

void temp(){
  int reading = analogRead(tempPin);
  Serial.print((3.0*reading*100.0)/1024.0);
  Serial.println("C");
}

int callback1(uint32_t ulPin){
  button_count=(button_count+1)%4;
  Serial.print("click num:");
  Serial.println(button_count);
  switch(button_count){
         case 0: Serial.println("1. temp, 2. school bell, 3. led on/off(100ms)"); break;
         case 1: temp(); break;
         case 2: play_school_bell(); break;
         case 3: ledOnOff(); break;
   }

   for(int i=0;  i<8; i++){
    digitalWrite(segmentLEDs[i], digitForNum[button_count][i]);
   }
}

int callback2(uint32_t ulPin){
  if(button_count<=0) button_count=3;
  else button_count-=1;
  Serial.print("click num:");
  Serial.println(button_count);
  switch(button_count){
         case 0: Serial.println("1. temp, 2. school bell, 3. led on/off(100ms)"); break;
         case 1: temp(); break;
         case 2: play_school_bell(); break;
         case 3: ledOnOff(); break;
   }

   for(int i=0;  i<8; i++){
    digitalWrite(segmentLEDs[i], digitForNum[button_count][i]);
   }
}

void loop() {
  serial_input();
  long input = cmd_interpret(serial_data);
   switch(input){
         case 0: Serial.println("1. temp, 2. school bell, 3. led on/off(100ms)"); break;
         case 1: temp(); break;
         case 2: play_school_bell(); break;
         case 3: ledOnOff(); break;
   }
   button_count=input;

   for(int i=0;  i<8; i++){
    digitalWrite(segmentLEDs[i], digitForNum[button_count][i]);
   }
}

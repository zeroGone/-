#include "pitches.h"

char *help[] = {"help","1","2","3"};
char serial_data[10];
int speakerPin = 2;
int DHpin = 1;
int button1 = 5;
int button2 = 6;
int button_count = 0;
byte dat[5];
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
  pinMode(DHpin, OUTPUT);

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
  Serial.println("play school bell");
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

void DH(){
 digitalWrite (DHpin, LOW); // bus down, send start signal 
  delay (30); 
  // delay greater than 18ms, so DHT11 start signal can be detected
  digitalWrite (DHpin, HIGH); 
  delayMicroseconds (40); // Wait for DHT11 response
  pinMode (DHpin, INPUT); 
  while (digitalRead (DHpin) == HIGH); 
    delayMicroseconds (80); // DHT11 response, pulled the bus 80us 
  if (digitalRead (DHpin) == LOW); 
    delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data 
  for (int i = 0; i < 4; i ++) {
    dat[i] = read_data ();  
  }
// receive temperature and humidity data,
//  the parity bit is not considered

  pinMode (DHpin, OUTPUT); 
  digitalWrite (DHpin, HIGH); // send data once after releasing 
  //  the bus, wait for the host to open the next Start signal
  Serial.print ("Current humdity ="); 
  Serial.print (dat [0], DEC); // display the humidity-bit integer 
  Serial.print ('.'); 
  Serial.print (dat [1], DEC); // display the humidity decimal places 
  Serial.println ('%');
  Serial.print ("Current temperature ="); 
  Serial.print (dat [2], DEC); // display the temperature of integer bits; 
  Serial.print ('.'); 
  Serial.print (dat [3], DEC); // display the temperature of decimal places; 
  Serial.println ('C'); 
  delay (700); 
}

byte read_data () { 
  byte data; 
  for (int i = 0; i < 8; i ++) { 
    if (digitalRead (DHpin) == LOW) { 
      while (digitalRead (DHpin) == LOW); // wait for 50us 
       delayMicroseconds (30);  // determine the duration of the high 
      // level to determine the data is '0 'or '1' 
       if (digitalRead (DHpin) == HIGH) 
          data |= (1 << (7-i)); // high front and low in the post 
        while (digitalRead (DHpin) == HIGH); 
         // data '1 ', wait for the next one receiver 
     
    }
  }
   return data; 
}

int callback1(uint32_t ulPin){
  button_count=(button_count+1)%4;
  Serial.print("click num:");
  Serial.println(button_count);
  segmentLED();
   state();
}

int callback2(uint32_t ulPin){
  if(button_count<=0) button_count=3;
  else button_count-=1;
  Serial.print("click num:");
  Serial.println(button_count);
  segmentLED();
  state();
}

void segmentLED(){
   for(int i=0;  i<8; i++){
    digitalWrite(segmentLEDs[i], digitForNum[button_count][i]);
   }
}

void state(){
  switch(button_count){
         case 0: Serial.println("1. DH, 2. school bell, 3. led on/off(100ms)"); break;
         case 1: DH(); break;
         case 2: play_school_bell(); break;
         case 3: ledOnOff(); break;
   }
}
void loop() {
  segmentLED();
  state();
  serial_input();
  long input = cmd_interpret(serial_data);
  
   button_count=input;
}

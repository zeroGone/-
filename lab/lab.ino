char input[10];
char *choice[] = {"help","1","2","3"};
void setup() {
  Serial.begin(9600);
}

void serialInput(){
  int count=0;
  
  while(!Serial.available());
  while(Serial.available()){
    delay(50);
    char ch = Serial.read();
    input[count]=ch;

    if (ch=='\r'||ch== '\n' || ch == '\a'){
      input[count]=0;
      Serial.print("input:");
      Serial.println(input);
    }
    count=(count+1)%10;
  }
}

int inputInterpret(){
  for(int i=0; i<sizeof(choice)/sizeof(char *); i++){
    if(!strcmp(choice[i], input)){
      return i;
    }
  }
  return -1;
}

void helpOutput(){
  Serial.println("-------help-------");
  Serial.print("1.temperature measurement/");
  Serial.print("2.School bell/");
  Serial.print("3.led on/off");
  Serial.println();
}

void temp(){
  Serial.println("temperature measurement");
}

void schoolBell(){
  Serial.println("School bell");
}

void led(){
  Serial.println("led");
}
void loop() {
  serialInput();
  int choice = inputInterpret();
  switch(choice){
    case 0: helpOutput(); break; 
    case 1: temp(); break;
    case 2: schoolBell(); break;
    case 3: led(); break;
  }
}

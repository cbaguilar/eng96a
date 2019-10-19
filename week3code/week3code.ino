//Team 2: Christian Aguilar, Daniel Roji, Jason Salgado
//Defines our Wiring according to our hardware 

//wiring for left motor
#define ENA 5 
#define IN1 6 
#define IN2 7  

 
//wiring for right motor
#define ENB 3
#define IN3 2
#define IN4 4


#define Trig 6
#define Echo 5 

void forward(int speed = 128);
void backward(int speed = 128);
void turn(int speed = 128);
void printerr(String msg);

void setup() {
  // SetUp for Motor Left
  pinMode(ENA,OUTPUT); 
  pinMode(IN1,OUTPUT);  
  pinMode(IN2,OUTPUT);

  // SetUp for Motor Right
  pinMode(ENB,OUTPUT); 
  pinMode(IN3,OUTPUT);  
  pinMode(IN4,OUTPUT);

  //SetUp for Ultra-Sonic Sensor  
  pinMode(Trig,OUTPUT); 
  pinMode(Echo,INPUT); 
  
 digitalWrite(ENA,LOW);
 digitalWrite(IN1,LOW); 
 digitalWrite(IN2,LOW); 

 Serial.begin(9600);
}

void loop() {
  /*
  turn();  
  delay(1000); 
  stop1(); 
  delay(1000); 
  */
  String he = readLine();
  Serial.print(he);
  doCommand(he);
  delay(100);
} 

void printErr(String msg) {
  Serial.println(msg);
}

void doCommand(String command) {
  if (command == "forward") {
    printErr("Moving forward");
    forward(128);
  }
  else if (command == "backward") {
    backward(128);
  }
  else if (command == "turn") {
    turn(128);
  }
  else if (command == "stop") {
    stop1();
  }
  else {
    printErr("Unknown command");
  }
}

String readLine(){
  String command;
  char nextByte = 0;
  if (Serial.available() > 0) {
    while (nextByte != '\n') {
      nextByte = Serial.read();
      //delay(100);
      Serial.print(nextByte);
      if (nextByte == '\n') {break;}
      command+=nextByte;
      
    }
  }
  return command;
}

void forward(int speed = 128){ 
  analogWrite(ENA,speed); 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW); 

  analogWrite(ENB,speed); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
  }

void backward(int speed = 128){
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH); 

  digitalWrite(ENB,speed); 
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH); 
}

void turn(int speed = 128){
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH); 
  
  digitalWrite(ENB,speed); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
  
}

void stop1(){   
  digitalWrite(ENA,LOW); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW);

  digitalWrite(ENB,LOW); 
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,LOW); 
  }

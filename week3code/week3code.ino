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


#define TRIG 9
#define ECHO 11

const double FACTOR = 0.0135039;


float duration, distance;

boolean moving = false;

void forward(int speed = 128);
void backward(int speed = 128);
void turnRight(int speed = 128);
void printerr(String msg);
double pollDist();
void turnLeft(int speed = 128);

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
  pinMode(TRIG,OUTPUT); 
  pinMode(ECHO,INPUT); 
  
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
  //Serial.print(he);
  doCommand(he);
  delay(100);
} 
double pollDist() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration*FACTOR)/2;
  return distance;
}

void printErr(String msg) {
  Serial.println(msg);
}

void doCommand(String command) {
  if (command == "forward" || command=="w") {
    printErr("Moving forward");
    forward(128);
  }
  else if (command == "backward" || command=="s") {
    backward(128);
  }
  else if (command == "turnright" || command=="d"){
    turnRight(128);
  }
  else if (command == "turnleft" || command=="a") {
    turnLeft(128);
  }
  else if (command == "stop" || command=="s") {
    stop1();
  }
  else if (command == "distance" || command == "dist") {
    Serial.print("Distance: ");
    Serial.print(pollDist());
    Serial.print(" in\n");
  }
  else {
    if (pollDist()<6&&(moving)) {
      Serial.println("Encountered wall, stopping");
      stop1();
      }
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

void backward(int speed = 128){
  moving = true; 
  analogWrite(ENA,speed); 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW); 

  analogWrite(ENB,speed); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
  }

void forward(int speed = 128){
  moving = true;
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH); 

  digitalWrite(ENB,speed); 
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH); 
}

void turnRight(int speed = 128){
  moving = true;
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH); 
  
  digitalWrite(ENB,speed); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
  
}

void turnLeft(int speed = 128) {
  moving = true;
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW); 
  
  digitalWrite(ENB,speed); 
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,HIGH);
}

void stop1(){   
  digitalWrite(ENA,LOW); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW);

  digitalWrite(ENB,LOW); 
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,LOW); 
  moving = false;
  }

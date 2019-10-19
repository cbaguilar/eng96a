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
  delay(100);
} 

String readLine(){
  String command;
  char nextByte = 0;
  if (Serial.available() > 0) {
    for (int i = 0; i < 100; i++) {
      nextByte = Serial.read();
      delay(100);
      Serial.print(nextByte);
      command+=nextByte;
      if (nextByte == '\n') {break;}
    }
  }
  return command;
}

void forward(){ 
  digitalWrite(ENA,HIGH); 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW); 

  digitalWrite(ENB,HIGH); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
    
  }

void turn(){
  digitalWrite(ENA,HIGH); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,HIGH); 

  digitalWrite(ENB,HIGH); 
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

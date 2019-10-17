//Team 2: Christian Aguilar, Daniel Roji, Jason Salgado
//Defines our Wiring according to our hardware 
#define ENA A0 
#define IN1 13 
#define IN2 12   
#define Trig 6
#define Echo 5 

void setup() {
  // SetUp for Motor 
  pinMode(ENA,OUTPUT); 
  pinMode(IN1,OUTPUT);  
  pinMode(IN2,OUTPUT);

  //SetUp for Ultra-Sonic Sensor  
  pinMode(Trig,OUTPUT); 
  pinMode(Echo,INPUT); 
  
 digitalWrite(ENA,LOW);
 digitalWrite(IN1,LOW); 
 digitalWrite(IN2,LOW); 

 Serial.begin(9600);
}

void loop() {
  forward();  
  delay(1000); 
  stop1(); 
  delay(1000); 
} 

void forward(int speed){ 
  digitalWrite(ENA,speed); 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW); 
    
  }

void stop1(){   
  digitalWrite(ENA,LOW); 
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW); 
  }

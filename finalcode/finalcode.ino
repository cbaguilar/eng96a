#define ENA A0
#define IN1 13
#define IN2 12
#define TRIG 5
#define ECHO 6

float distance;


void setup() {
  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(TRIG, OUTPUT);

  pinMode(ECHO, INPUT);
  
  
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);


  Serial.begin(9600);
  

}

void loop() {

  
  approachWall();
  scanForOpening();
  turnRight();
  
}

void reportPosition() {
  //do stuff;
}

void scanForOpening() {
  backward();
  while (ultrasonic() > 20) {
    reportPosition();
    int width = 0;
    if (sideScan > 30) {
      width++;
    }
    if (width > 20) {
      break;
    }
  }
}

void turnRight() {
  //turn
}

void approachWall() {
  forward();
  while (ultrasonic() > 20) {
    reportPosition();
  }
  stop_();
}


void forward() {

  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
}


void stop_() {

  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}


float ultrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  distance = pulseIn(ECHO, HIGH)* .0133/2;
  return distance;
  
}

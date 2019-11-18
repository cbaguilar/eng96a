#define ENA A0
#define IN1 13
#define IN2 12
#define US_LEFT 5
#define US_RIGHT 7
#define US_FL
#define US_FR
#define US_BL
#define US_BR

#define QUARTER_TURN 90

#define ECHO 6

#define THRESHOLD 20


#include <Wire.h>
const int MPU = 0x68;
float yaw = 0;
float GryoZ, gyroAngleZ, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

float distance;

//here is an update

void setup() {

  //YAW CODE
  Serial.begin(19200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  calculate_IMU_error();


  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(TRIG, OUTPUT);

  pinMode(ECHO, INPUT);
  
  
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);


  

}

void loop() {

  read_yaw();
  Serial.println(yaw);

  approachWall();
  scanForOpening();
  turnRight();
  
}

float read_yaw() {
  previousTime = currentTime;

  currentTime == millis();
  elapsedTime = (currentTime - previousTime) / 1000;
  Wire.beginTransmission(MPU);
  Wire.write(0x47);
  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);
  
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  
  GyroZ = GyroZ - GyroErrorZ;

  yaw = yaw + GyroZ * elapsedTime;
  return yaw;
}

float calculate_IMU_error() {
    while (c < 200) {
        Wire.beginTransmission(MPU);
        Wire.write(0x47);
        Wire.requestFrom(MPU, 2, true);
        
        GyroZ = Wire.read() << 8 | Wire.read();

        GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);

        c++;
    }
    GyroErrorZ = GyroErrorZ / 200;
    return GyroErrorZ;
}

void reportPosition() {
  //do stuff;
}

void scanForOpening() {
  backward();
  int width = 0;
  while (ultrasonic() > 20) {
    reportPosition();
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
  float startyaw = read_yaw();
  while (readYaw-startYar > QUARTER_TURN) { //we can change this
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, HIGH);

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW); 
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
}

void turnLeft() {
  float startyaw = read_yaw();
  while (readYaw-startYar > QUARTER_TURN) { //we can change this
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, HIGH);

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW); 
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }


}

void approachWall() {
  if (ultrasonic(US_FL) > THRESHOLD) {
    forward();
  }
  while (ultrasonic(US_FL) > THRESHOLD) {
    reportPosition();
  }
  stop_();
}


void forward() {

  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
}

void backward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}


void stop_() {

  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}


float ultrasonic(int PORT) { //you should be able to specify a port, L/R/F/B
  digitalWrite(PORT, LOW);
  delayMicroseconds(2);
  digitalWrite(PORT, HIGH);
  delayMicroseconds(10);
  digitalWrite(PORT, LOW);
  distance = pulseIn(ECHO, HIGH)* .0133/2;
  return distance;
}

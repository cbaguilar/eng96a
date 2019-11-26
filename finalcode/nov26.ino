#include "Arduino.h"
#include "pins_arduino.h"
#include <Wire.h>
#include <math.h>
 
//motors
#define frontLeft 10 //left front
#define frontLeftIn1 9
#define frontLeftIn2 2
 
#define frontRight 11 //front right
#define frontRightIn1 3
#define frontRightIn2 4
 
#define backLeft 12 //back left
#define backLeftIn1 5
#define backLeftIn2 6
 
#define backRight 13 //backright
#define backRightIn1 7
#define backRightIn2 8
 
//sensors
#define frontLeftEcho 22 //front left
#define frontLeftTrig 23
 
#define frontRightEcho 24 //front right
#define frontRightTrig 25
 
#define centerLeftEcho 26 //center left
#define centerLeftTrig 27
 
#define centerRightEcho 28 //center right
#define centerRightTrig 29
 
#define backLeftEcho 30 //back left
#define backLeftTrig 31
 
#define backRightEcho 32 //back right
#define backRightTrig 33
 
const int MPU = 0x68;
float yaw = 0;
float GyroZ, gyroAngleZ, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
 
const int SPEED = 120;
const int NUMPULSES = 10;
const float TURNPRECISION = 0.1;
 
const int FRONTLEFT = 0;
const int FRONTRIGHT = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int BACKLEFT = 4;
const int BACKRIGHT = 5;
 
const double THRESHOLD = 6;
 
const bool ISLEFT = true;
const bool ISRIGHT = false;
 
void setup()
{
    pinMode(frontLeft, OUTPUT);
    pinMode(frontLeftIn1, OUTPUT);
    pinMode(frontLeftIn2, OUTPUT);
 
    pinMode(frontRight, OUTPUT);
    pinMode(frontRightIn1, OUTPUT);
    pinMode(frontRightIn2, OUTPUT);
 
    pinMode(backLeft, OUTPUT);
    pinMode(backLeftIn1, OUTPUT);
    pinMode(backLeftIn2, OUTPUT);
 
    pinMode(backRight, OUTPUT);
    pinMode(backRightIn1, OUTPUT);
    pinMode(backRightIn2, OUTPUT);
 
    digitalWrite(frontLeft, LOW);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, LOW);
    
    digitalWrite(frontRight, LOW);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, LOW);
 
    digitalWrite(backLeft, LOW);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, LOW);
    
    digitalWrite(backRight, LOW);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, LOW);
 
    pinMode(frontLeftEcho, INPUT);
    pinMode(frontLeftTrig, OUTPUT);
 
    pinMode(frontRightEcho, INPUT);
    pinMode(frontRightTrig, OUTPUT);
 
    pinMode(centerLeftEcho, INPUT);
    pinMode(centerLeftTrig, OUTPUT);
 
    pinMode(centerRightEcho, INPUT);
    pinMode(centerRightTrig, OUTPUT);
 
    pinMode(backLeftEcho, INPUT);
    pinMode(backLeftTrig, OUTPUT);
 
    pinMode(backRightEcho, INPUT);
    pinMode(backRightTrig, OUTPUT);
 
    digitalWrite(frontLeftTrig, LOW);
    digitalWrite(frontRightTrig, LOW);
    digitalWrite(centerLeftTrig, LOW);
    digitalWrite(centerRightTrig, LOW);
    digitalWrite(backLeftTrig, LOW);
    digitalWrite(backRightTrig, LOW);
 
    Serial.begin(9600);
 
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);
    calculate_IMU_error();

    forward(128);
    delay(1000); 
 
    left90(); //ALL TESTING
    delay(1000);
    right90();
    
}

void loop() {
  return;
}
 
float read_yaw() 
{
    previousTime = currentTime; // Use the time when read_yaw was last called as the previous time
    currentTime = millis(); // Update the value of the current time
    elapsedTime = (currentTime - previousTime) / 1000; // Find the elapsed time and divide by 1000 to get seconds
    
    Wire.beginTransmission(MPU); // Start Communication
    Wire.write(0x47); // Start with the first of the two registers that hold the GYRO_ZOUT data (There are GYRO_ZOUT_H and GYRO_ZOUT_L)
    Wire.endTransmission(false); // backRight the communication
    Wire.requestFrom(MPU, 2, true); // Read the values of the two bytes that contain the data we need
    GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0; // Combine the two bytes by left shifting the first 8 bits and adding on the second set of 8 bits
    // Correct the outputs with the calculated error values
    GyroZ = GyroZ - GyroErrorZ; // GyroErrorZ ~ (-0.8)
    
    // Find the angle in degrees by multiplying the angular velocity, which is in deg/s, by time to get the change in angle and then add that to the initial angular position
    yaw =  yaw + GyroZ * elapsedTime;
    
    // Print the values on the serial monitor
    return yaw;
}
 
 
float calculate_IMU_error()
{
    // Read gyro values 200 times
    while (c < 200) 
    {
        Wire.beginTransmission(MPU); // Start communication
        Wire.write(0x47); // Start with the first of the two registers that hold the GYRO_ZOUT data (There are GYRO_ZOUT_H and GYRO_ZOUT_L)
        Wire.endTransmission(false); // backRight the communication
        Wire.requestFrom(MPU, 2, true); // Read the values of the two bytes that contain the data we need
        GyroZ = Wire.read() << 8 | Wire.read(); // Combine the two bytes by left shifting the first 8 bits and adding on the second set of 8 bits
        GyroErrorZ = GyroErrorZ + (GyroZ / 131.0); // Sum the average of the readings so that they can be averaged
        c++;
    }
    GyroErrorZ = GyroErrorZ / 200;  // Average the gyro readings so that we know what the 0 point is
    return GyroErrorZ;
}
 
float readAverageYaw()
{
    float total = 0;
    for(int i = 0; i != NUMPULSES; i++)
        total += read_yaw();
    return total / NUMPULSES;
}
 
double measureDistance(int ECHO, int TRIG) //measure and return the distance of an object in front of the rover
{
  //declare local variables
  double ultraSonicDuration; 
  double distanceInInches;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
 
  digitalWrite(TRIG, HIGH); //send out pulse to be interpreted by the echo receiver
  delayMicroseconds(10); //how long the pulse is 
  digitalWrite(TRIG, LOW);
 
  ultraSonicDuration = pulseIn(ECHO, HIGH); //measure length taken for pulse to travel
  distanceInInches = ultraSonicDuration * (0.0133/2); //convert time taken for pulse to travel into inches away from object
 
  return distanceInInches; //return the distance in inches from object for use in other functions
}
 
double averageDistance(int ECHO, int TRIG)
{
    double total = 0;
    for (int i = 0; i < NUMPULSES; i++)
        total += measureDistance(ECHO,TRIG);
    return total/NUMPULSES;
}
 
float look(int sensorid)
{
    switch(sensorid)
    {
        case FRONTLEFT:
        {
            return averageDistance(frontLeftEcho, frontLeftTrig);
            break;
        }
        case FRONTRIGHT:
        {
            return averageDistance(frontRightEcho, frontLeftTrig);
            break;
        }
        case LEFT:
        {
            return averageDistance(centerLeftEcho, centerLeftTrig);
            break;
        }
        case RIGHT:
        {
            return averageDistance(centerRightEcho, centerRightTrig);
            break;
        }
        case BACKLEFT:
        {
            return averageDistance(backLeftEcho, backLeftTrig);
            break;
        }
        case BACKRIGHT:
        {
            return averageDistance(backRightEcho, backRightTrig);
            break;
        }
        default:
        {
            Serial.println("YOU SHOULDNT BE HERE");
        }
    }
}
 
void forward()
{
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontLeftIn1, HIGH);
    digitalWrite(frontLeftIn2, LOW);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, HIGH);
    digitalWrite(frontRightIn2, LOW);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, HIGH);
    digitalWrite(backLeftIn2, LOW);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, HIGH);
    digitalWrite(backRightIn2, LOW);
 
    Serial.println("Forward");
    Serial.println("Speed: FULL");
}
 
void forward(int speed)
{
    analogWrite(frontLeft, speed);
    digitalWrite(frontLeftIn1, HIGH);
    digitalWrite(frontLeftIn2, LOW);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, speed);
    digitalWrite(frontRightIn2, LOW);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, speed);
    digitalWrite(backLeftIn2, LOW);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, speed);
    digitalWrite(backRightIn2, LOW);
 
    Serial.println("Forward");
    Serial.println("Speed: " + speed);
}
 
void backward()
{
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, HIGH);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, HIGH);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, HIGH);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, HIGH);
 
    Serial.println("Backward");
    Serial.println("Speed: FULL");
}
 
void backward(int speed)
{
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, speed);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, speed);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, speed);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, speed);
 
    Serial.println("Backward");
    Serial.println("Speed: " + speed);
}
 
void stop()
{
    digitalWrite(frontLeft, LOW);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, LOW);
 
    digitalWrite(frontRight, LOW);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, LOW);
 
    digitalWrite(backLeft, LOW);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, LOW);
 
    digitalWrite(backRight, LOW);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, LOW);
    
    Serial.println("Stop");
}
 
void right()
{
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, HIGH);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, HIGH);
    digitalWrite(frontRightIn2, LOW);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, HIGH);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, HIGH);
    digitalWrite(backRightIn2, LOW);
 
    Serial.println("RIGHT");
    Serial.println("Speed: FULL");
}
 
void right(int speed)
{
    analogWrite(frontLeft, speed);
    digitalWrite(frontLeftIn1, LOW);
    digitalWrite(frontLeftIn2, HIGH);
 
    analogWrite(frontRight, speed);
    digitalWrite(frontRightIn1, HIGH);
    digitalWrite(frontRightIn2, LOW);
 
    analogWrite(backLeft, speed);
    digitalWrite(backLeftIn1, LOW);
    digitalWrite(backLeftIn2, HIGH);
 
    analogWrite(backRight, speed);
    digitalWrite(backRightIn1, HIGH);
    digitalWrite(backRightIn2, LOW);
 
    Serial.println("RIGHT");
    Serial.println("Speed " + speed);
}
 
void left()
{
    digitalWrite(frontLeft, HIGH);
    digitalWrite(frontLeftIn1, HIGH);
    digitalWrite(frontLeftIn2, LOW);
 
    digitalWrite(frontRight, HIGH);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, HIGH);
 
    digitalWrite(backLeft, HIGH);
    digitalWrite(backLeftIn1, HIGH);
    digitalWrite(backLeftIn2, LOW);
 
    digitalWrite(backRight, HIGH);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, HIGH);
 
    Serial.println("left");
    Serial.println("Speed: FULL");
}
 
void left(int speed)
{
    analogWrite(frontLeft, speed);
    digitalWrite(frontLeftIn1, HIGH);
    digitalWrite(frontLeftIn2, LOW);
 
    analogWrite(frontRight, speed);
    digitalWrite(frontRightIn1, LOW);
    digitalWrite(frontRightIn2, HIGH);
 
    analogWrite(backLeft, speed);
    digitalWrite(backLeftIn1, HIGH);
    digitalWrite(backLeftIn2, LOW);
 
    analogWrite(backRight, speed);
    digitalWrite(backRightIn1, LOW);
    digitalWrite(backRightIn2, HIGH);
    
    Serial.println("left");
    Serial.println("Speed: " + speed);
}
 
void left90()
{
    float initialYaw = readAverageYaw();
    float targetYaw = initialYaw - 90.0;
 
    while(abs(readAverageYaw() - targetYaw) > TURNPRECISION)
        left(SPEED);
    stop();
 
    Serial.println("Left90");
    Serial.print("Initial Yaw: ");
    Serial.println(initialYaw);
    Serial.print("Target Yaw: ");
    Serial.println(targetYaw);
    Serial.print("Final Yaw: ");
    Serial.println(read_yaw());
 
}
 
void right90()
{
    float initialYaw = readAverageYaw();
    float targetYaw = initialYaw + 90.0;
 
    while(abs(readAverageYaw() - targetYaw) > TURNPRECISION)
        right(SPEED);
    stop();
 
    Serial.println("Right90");
    Serial.print("Initial Yaw: ");
    Serial.println(initialYaw);
    Serial.print("Target Yaw: ");
    Serial.println(targetYaw);
    Serial.print("Final Yaw: ");
    Serial.println(read_yaw());
}
 
void moveForward()
{
 
}
 
void findOpening()
{
 
}
 
void objectSearch()
{
 
}
 



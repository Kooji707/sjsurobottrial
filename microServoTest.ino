#include<Servo.h>
#include<Wire.h>
Servo microServo;
int refAngle = 90;
double gyroToDegrees = (90/16700);
const int MPU=0x68; 
int16_t GyX,GyY,GyZ;
int Xdegrees, Ydegrees, Zdegrees; 

void setup()
{
  microServo.attach(9) ;
  microServo.write(refAngle);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop()
{ 
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  int GyX=Wire.read()<<8|Wire.read();  
  int GyY=Wire.read()<<8|Wire.read();  
  int GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Gyroscope: ");
  Serial.print("X  = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");

  int Xdegrees = (GyX*(90/16700)); 
  int Ydegrees = map(GyY, -16700, 16700, -90, 90);
  int Zdegrees = (GyZ*(90/16700));

  // Serial.print("X Degrees = "); Serial.print(Xdegrees); Serial.println("°"); 
  Serial.print("Y Degrees = "); Serial.print(Ydegrees); Serial.println("°"); 
  // Serial.print("Z Degrees = "); Serial.print(Zdegrees); Serial.println("°"); 
  // Serial.println(" ");

  delay(100);
  microServo.write(refAngle-Ydegrees);
  
}
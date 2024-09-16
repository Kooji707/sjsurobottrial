#include<Servo.h>
#include<Wire.h>
Servo microServo;
int refAngle = 90;
double gyroToDegrees = (90/16700);
const int MPU=0x68; 
int16_t GyX,GyY,GyZ;
int Xdegrees, Ydegrees, Zdegrees; 
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
int dataNumber = 0;

void setup() {
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

void loop() { 
  recvWithEndMarker();
  showNewNumber();
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

  int Xdegrees = map(GyX, -32768, 32768, -180, 180);
  int Ydegrees = map(GyY, -32768, 32768, -180, 180);
  int Zdegrees = map(GyZ, -32768, 32768, -180, 180);

  Serial.print("In degrees: ");
  Serial.print("X = "); Serial.print(Xdegrees); Serial.print("°"); 
  Serial.print(" | Y = "); Serial.print(Ydegrees); Serial.print("°"); 
  Serial.print(" | Z = "); Serial.print(Zdegrees); Serial.println("°"); 
  Serial.println(" ");

  delay(100);
  microServo.write(refAngle-dataNumber-Ydegrees);
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        Serial.print("Data as Number ... ");    // new for this version
        Serial.println(dataNumber);     // new for this version
        newData = false;
    }
}
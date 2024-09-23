#include<Servo.h>
#include<Wire.h>
Servo microServo;
int refAngle = 90;
double gyroToDegrees = (90/16700);
const int MPU=0x68; //this is the I2C address of the MPU
int16_t GyX,GyY,GyZ;
int Xdegrees, Ydegrees, Zdegrees; 
const byte numChars = 32; //max digit number user can give from serial monitor
char receivedChars[numChars]; //an array to save that data
boolean newData = false; //flag to check for new data. this is to get user input in case they want to do an offset on the servo.
int dataNumber = 0;

void setup() {
  microServo.attach(9); //servo is attached to pin 9
  microServo.write(refAngle); //set the servo to the middle of its range of motion
  Serial.begin(9600); //serial communication is with this baud rate
  Wire.begin(); //initialize I2C communication
  Wire.beginTransmission(MPU); //start communication with the mpu
  Wire.write(0x6B); //address the mpu chip that has address 0x6B
  Wire.write(0); //wake up the mpu since it is in sleep mode by default    
  Wire.endTransmission(true);
  Serial.begin(9600); //force serial communication
}

void loop() { 
  recvWithEndMarker(); //receive serial input. \n to end
  showNewNumber(); //send the recently inputted data to the serial output
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  //request data from the mpu. keep requesting until 12 bytes are sent.
  int GyX=Wire.read()<<8|Wire.read();  
  int GyY=Wire.read()<<8|Wire.read();  
  int GyZ=Wire.read()<<8|Wire.read();  //read gyroscope data for each axis
  
  Serial.print("Gyroscope: ");
  Serial.print("X  = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" "); //print data with readable formatting

  int Xdegrees = map(GyX, -32768, 32768, -180, 180);
  int Ydegrees = map(GyY, -32768, 32768, -180, 180);
  int Zdegrees = map(GyZ, -32768, 32768, -180, 180); //convert raw gyroscope data to degrees using map function

  Serial.print("In degrees: ");
  Serial.print("X = "); Serial.print(Xdegrees); Serial.print("°"); 
  Serial.print(" | Y = "); Serial.print(Ydegrees); Serial.print("°"); 
  Serial.print(" | Z = "); Serial.print(Zdegrees); Serial.println("°"); 
  Serial.println(" "); //prints degrees data to the serial monitor as well

  delay(100); //wait 100 milliseconds before next loop (0.1 seconds)
  microServo.write(refAngle-dataNumber-Ydegrees); //move servo arm to counteract the tilt detected by the mpu
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
        dataNumber = 0;             
        dataNumber = atoi(receivedChars);   
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        Serial.print("Data as Number ... ");   
        Serial.println(dataNumber);     
        newData = false;
    }
}
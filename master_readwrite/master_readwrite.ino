#include <Wire.h>


#define PAYLOAD_SIZE 2
#define NEW_NODE_ADDRESS 14
bool DEVICE_RESPONSE = true;
byte NODE_ADDRESS = 8;
byte error;


void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  delay(3000);
  Serial.println("Initialized");
  initialization();
}

void loop() {
}


void initialization(){
    while (DEVICE_RESPONSE == true) {
     Wire.beginTransmission(NODE_ADDRESS);
     error = Wire.endTransmission();

     if (error==0) {
     Wire.requestFrom(NODE_ADDRESS, 1, false);
      byte RANDOM_NUMBER  = Wire.read(); // receive a byte as characte
      //Wire.endTransmission();
      Serial.println(RANDOM_NUMBER);         // print the character
    
      byte buffer[PAYLOAD_SIZE]; // Load buffer 
      buffer[0] = RANDOM_NUMBER;
      buffer[1] = NEW_NODE_ADDRESS;
      
      Wire.beginTransmission(NODE_ADDRESS);
      Wire.write(buffer, PAYLOAD_SIZE);
      Wire.endTransmission();
     }  

    else if (error=2) {
      Serial.println("All devices registered");
      DEVICE_RESPONSE = false;
    }
    
   delay(2000);
   }
}



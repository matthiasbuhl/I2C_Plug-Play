#include <Wire.h>


#define PAYLOAD_SIZE 2
#define NEW_NODE_ADDRESS 14
byte NODE_ADDRESS = 8;


void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  delay(3000);
}

void loop() {
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

  
  delay(2000);
}

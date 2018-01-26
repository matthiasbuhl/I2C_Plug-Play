#include <Wire.h>


bool DEVICE_RESPONSE = true;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte NODE_ADDRESS = 8;
byte NEW_NODE_ADDRESS = 9;
byte error;
byte a,b;
uint16_t RANDOM_NUMBER;


void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  delay(3000);
  Serial.println("Initialized");
  initialization();
}

void loop() {
  Serial.println("workin");
  delay(2000);
}


void initialization(){
    while (DEVICE_RESPONSE == true) {
     Wire.beginTransmission(NODE_ADDRESS);
     error = Wire.endTransmission();

     if (error==0) {
      if (NEW_NODE_ADDRESS < 120) {
       Wire.requestFrom(NODE_ADDRESS, READ_PAYLOAD_SIZE, false);
        //byte RANDOM_NUMBER  = Wire.read(); // receive a byte as characte
        a = Wire.read();
        b = Wire.read();

        RANDOM_NUMBER = a;
        RANDOM_NUMBER = (RANDOM_NUMBER <<8)|b;
        
        Serial.println(RANDOM_NUMBER);         // print the character
      
        byte buffer[WRITE_PAYLOAD_SIZE]; // Load buffer 
        buffer[0] = (RANDOM_NUMBER >> 8) & 0xFF;
        buffer[1] = RANDOM_NUMBER & 0xFF;
        buffer[2] = NEW_NODE_ADDRESS;
        
        Wire.beginTransmission(NODE_ADDRESS);
        Wire.write(buffer, WRITE_PAYLOAD_SIZE);
        Wire.endTransmission();
        NEW_NODE_ADDRESS++;
      }
      else { 
      Serial.println("All possible addresses already in use!");
      DEVICE_RESPONSE = false;
      }
      
     }

    else if (error=2) {
      Serial.println("All devices registered");
      DEVICE_RESPONSE = false;
    }
    
   delay(2000);
   }
}



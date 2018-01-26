#include <Wire.h>


bool DEVICE_RESPONSE = true;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte NODE_ADDRESS = 9;
byte NEW_NODE_ADDRESS = 14;
byte error;
byte a,b;
uint16_t RANDOM_NUMBER;
uint16_t fake = 49;


void setup() {
  Wire.begin(8);        // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  // start serial for output
  delay(200);
  Serial.println("Initialized");
}

void loop() {
  Serial.println("workin");
  enumeration();
  delay(2000);
}


void receiveEvent(int howMany) {
 if (howMany == 2){
      a = Wire.read();
      b = Wire.read();
      RANDOM_NUMBER = a;
      RANDOM_NUMBER = (RANDOM_NUMBER <<8)|b;  
      Serial.println(RANDOM_NUMBER);
    }  
}


void enumeration() {
  Wire.beginTransmission(NODE_ADDRESS);
  error = Wire.endTransmission();
  
  if ((error == 0) && (NEW_NODE_ADDRESS < 120)) {
      byte buffer[WRITE_PAYLOAD_SIZE]; // Load buffer 
      buffer[0] = (RANDOM_NUMBER >> 8) & 0xFF;
      buffer[1] = RANDOM_NUMBER & 0xFF;
      buffer[2] = NEW_NODE_ADDRESS;
          
      Wire.beginTransmission(NODE_ADDRESS);
      Wire.write(buffer, WRITE_PAYLOAD_SIZE);
      Wire.endTransmission();
      NEW_NODE_ADDRESS++;
      Serial.println("New address assigned");
  }
  else if (error == 2) {
        Serial.println("All devices on bus assigned");      
     } 
  else if (error ==4) {   
    Serial.println("All addresses already in use!");
  }

}


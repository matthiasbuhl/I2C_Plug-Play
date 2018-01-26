
#include <Wire.h>

#define PAYLOAD_SIZE 2


bool LED_STATE = false;
uint16_t RANDOM_NUMBER;
uint16_t x; 
byte MASTER_ADDRESS = 8;
byte error,a,b;



void setup() {
  Wire.begin(9); // join i2c bus (address optional for master)
  Serial.begin(9600);
  randomSeed(analogRead(13));
  RANDOM_NUMBER = random(65535);
  pinMode(LED1, OUTPUT);
  for (int i=0; i<=3; i++) {
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  delay(200);
  initialization();
  }
  Serial.println("Starting initialization");
  Wire.onReceive(receiveEvent);
  
  
}
 

void loop() {
  Serial.println("workin");
  delay(100);
}

void initialization() {
   Wire.beginTransmission(MASTER_ADDRESS);
   error = Wire.endTransmission();

   if (error == 0) {
    byte buffer[PAYLOAD_SIZE]; // Load buffer 
    buffer[0] = (RANDOM_NUMBER >> 8) & 0xFF;
    buffer[1] = RANDOM_NUMBER & 0xFF;
    
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buffer, PAYLOAD_SIZE);              // sends two byte
    Wire.endTransmission();    // stop transmitting
    }
   else if (error == 4) {
    byte buffer[PAYLOAD_SIZE]; // Load buffer 
    buffer[0] = (RANDOM_NUMBER >> 8) & 0xFF;
    buffer[1] = RANDOM_NUMBER & 0xFF;
    
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buffer, PAYLOAD_SIZE);              // sends two byte
    Wire.endTransmission();    // stop transmitting
   } 
  else {
    return;
  }
}

void receiveEvent(int howMany) {
    if (howMany == 3){
    a = Wire.read();
    b = Wire.read();
    x = a;
    x = (x <<8)|b;  
    Serial.println(x);
    byte NEW_NODE_ADDRESS = Wire.read();
    Serial.println(NEW_NODE_ADDRESS);
     if ((x == RANDOM_NUMBER) && (LED_STATE == false)) {
        LED_STATE = true;
        Wire.begin(NEW_NODE_ADDRESS);
        Serial.println("New address assigned");
        digitalWrite(LED1, HIGH);
      }
     else if ((x != RANDOM_NUMBER) && (LED_STATE == false)) {
        Serial.println("wrong number");
        digitalWrite(LED1, LOW);
        delay(200);
        digitalWrite(LED1, HIGH);
        delay(200);
        digitalWrite(LED1, LOW);
        initialization();
      }  
    }
}


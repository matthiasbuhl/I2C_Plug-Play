#include <Wire.h>

#define NODE_ADDRESS 8
#define PAYLOAD_SIZE 2


bool LED_STATE = false;
byte a,b;
uint16_t RANDOM_NUMBER;
uint16_t x; 

void setup() {
  Wire.begin(NODE_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  randomSeed(analogRead(12));
  pinMode(LED1, OUTPUT);
  for (int i=0; i<=3; i++) {
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  delay(200);
  }
}

void loop() {
  //Serial.println(RANDOM_NUMBER);
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  RANDOM_NUMBER = random(65535);
  byte buffer[PAYLOAD_SIZE];
  buffer[0] = (RANDOM_NUMBER >> 8) & 0xFF;
  buffer[1] = RANDOM_NUMBER & 0xFF;
  Wire.write(buffer, PAYLOAD_SIZE);
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
        digitalWrite(LED1, HIGH);
        LED_STATE = true;
        Wire.begin(NEW_NODE_ADDRESS);
      }
      else {
        digitalWrite(LED1, LOW);
        LED_STATE = false;
      } 
    }
}

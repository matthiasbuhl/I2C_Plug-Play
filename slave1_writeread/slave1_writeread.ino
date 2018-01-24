#include <Wire.h>

#define RANDOM_NUMBER 220

bool LED_STATE = false;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  for (int i=0; i<=3; i++) {
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  delay(200);
  }
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(RANDOM_NUMBER);
}

void receiveEvent(int howMany) {
    if (howMany == 2){
    byte x = Wire.read();
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

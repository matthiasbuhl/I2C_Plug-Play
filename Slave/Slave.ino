
#include <Wire.h>

#define PAYLOAD_SIZE 2


bool LED_STATE = false;
int RANDOM_NUMBER;
int x;
byte MASTER_ADDRESS = 8;
byte DUPLICATE_ERROR = 4;
byte error, a, b;



void setup() {
  Wire.begin(9); // join i2c bus (address optional for master)
  Serial.begin(9600);
  randomSeed(analogRead(13));
  //RANDOM_NUMBER = random(65535);
  RANDOM_NUMBER = 12500;
  pinMode(LED1, OUTPUT);
  for (int i = 0; i <= 3; i++) {
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
  }
  initialization();
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
    Serial.println(RANDOM_NUMBER);
  }
  else {
    return;
  }
}

void receiveEvent(int howMany) {
  if (howMany == 3) {

    byte a = Wire.read();
    byte b = Wire.read();
    x = a;
    x = (x << 8) | b;
    Serial.println(x);
    byte NEW_NODE_ADDRESS = Wire.read();
    Serial.println(NEW_NODE_ADDRESS);
    if ((x == RANDOM_NUMBER) && (NEW_NODE_ADDRESS != DUPLICATE_ERROR) && (LED_STATE == false)) {
      LED_STATE = true;
      Wire.begin(NEW_NODE_ADDRESS);
      Serial.println("New address assigned");
      digitalWrite(LED1, HIGH);
    }
    if ((x == RANDOM_NUMBER) && (NEW_NODE_ADDRESS == DUPLICATE_ERROR) && (LED_STATE == false)) {
      RANDOM_NUMBER = random(65535);
      Serial.println("Duplicate error! Created new random number!");
      digitalWrite(LED1, LOW);
      delay(200);
      digitalWrite(LED1, HIGH);
      delay(200);
      digitalWrite(LED1, LOW);
      initialization();
    }
    else if ((x != RANDOM_NUMBER) && (LED_STATE == false)) {
      Serial.println("wrong number");
      digitalWrite(LED1, LOW);
      delay(200);
      digitalWrite(LED1, HIGH);
      delay(200);
      digitalWrite(LED1, LOW);
      //initialization();
    }
  }
  if (howMany == 1) {
     
  }
}



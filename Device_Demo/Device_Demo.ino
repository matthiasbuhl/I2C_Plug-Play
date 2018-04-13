#include <Wire.h>

#define PAYLOAD_SIZE 2


bool LED_STATE = false;
bool SEND_DESCRIPTION = false;
int DEVICE_ID = 12510;
int x;
byte MASTER_ADDRESS = 8;
byte DUPLICATE_ERROR = 4;
byte error, a, b;
byte DESCRIPTION_CMD = 101;
byte DESCRIPTION_CODE = 101;



void setup() {
  Wire.begin(9); // join i2c bus (address optional for master)
  Serial.begin(9600);
  randomSeed(analogRead(13));
  //RANDOM_NUMBER = random(65535);
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
  send_description();
}

void initialization() {
  Wire.beginTransmission(MASTER_ADDRESS);
  error = Wire.endTransmission();

  if (error == 0) {
    byte buffer[PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (DEVICE_ID >> 8) & 0xFF;
    buffer[1] = DEVICE_ID & 0xFF;

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buffer, PAYLOAD_SIZE);              // sends two byte
    Wire.endTransmission();    // stop transmitting
    Serial.println(DEVICE_ID);
  }
  else {
    delay(7000);
    byte buffer[PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (DEVICE_ID >> 8) & 0xFF;
    buffer[1] = DEVICE_ID & 0xFF;

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buffer, PAYLOAD_SIZE);              // sends two byte
    Wire.endTransmission();    // stop transmitting
    Serial.println(DEVICE_ID);
    Serial.println("no response");
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
    if ((x == DEVICE_ID) && (NEW_NODE_ADDRESS != DUPLICATE_ERROR) && (LED_STATE == false)) {
      LED_STATE = true;
      Wire.begin(NEW_NODE_ADDRESS);
      Serial.println("New address assigned");
      digitalWrite(LED1, HIGH);
    }
    if ((x == DEVICE_ID) && (NEW_NODE_ADDRESS == DUPLICATE_ERROR) && (LED_STATE == false)) {
      DEVICE_ID = random(65535);
      Serial.println("Duplicate error! Created new random number!");
      digitalWrite(LED1, LOW);
      delay(200);
      digitalWrite(LED1, HIGH);
      delay(200);
      digitalWrite(LED1, LOW);
      initialization();
    }
    else if ((x != DEVICE_ID) && (LED_STATE == false)) {
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
     SEND_DESCRIPTION = true;
     /*byte a = Wire.read();
     Serial.println(a);

     if (a = DESCRIPTION_CMD) {
      SEND_DESCRIPTION = true;
     } */
  }
}

void send_description() {
  if (SEND_DESCRIPTION == true) {
    /*char json[] = "{\"id\":60106401,\"name\":\"DPS330\",\"category\":[5,10],\"functions\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1,2],\"data3ength\":[16,16]}";
      char sub[] = {};
      for (byte k=0; k<10; k++) {
      sub[k] = json[k];
      }
      //Serial.println(sub);
    */
    /* const int BUFFER_SIZE = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0);
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    Serial.println(BUFFER_SIZE);

    JsonObject& root = jsonBuffer.createObject();
    root["id"] = 60106401;
    root["name"] = "DPS310";
    JsonArray& category = root.createNestedArray("category");
    category.add(5);
    category.add(10);
    JsonArray& fnct = root.createNestedArray("fnct");
    fnct.add("getPressure_Pa");
    fnct.add("getTemp_C");
    JsonArray& cmd = root.createNestedArray("cmd");
    cmd.add(1);
    cmd.add(2);    
    JsonArray& datalength = root.createNestedArray("datalength");
    datalength.add(16);
    datalength.add(16);
    
    char description[32];
    root.printTo(description, 32);
    Serial.print("Created json: ");
    Serial.println(description); */
    Wire.beginTransmission(8);
    Wire.write(DESCRIPTION_CODE);
    Wire.endTransmission();
    Serial.println("Description sent");
    SEND_DESCRIPTION = false;
  }
  else {
    return;
  }
}






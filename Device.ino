
#include <Wire.h>
#include <Dps310.h>
#include <ArduinoJson.h>

#define PAYLOAD_SIZE 2

Dps310 Dps310PressureSensor = Dps310();   //DPS310 Object

TwoWire onboard_bus = Wire;
TwoWire network_bus = Wire1;

bool LED_STATE = false;
bool SEND_DESCRIPTION = false;
int GENERATED_TICKET;
int RECEIVED_TICKET;
byte NODE_ADDRESS = 8;
byte DUPLICATE_ERROR = 4;
byte DESCRIPTION_CMD = 101;
byte error, a, b;



void setup() {
  network_bus.begin(9); // join i2c bus (address optional for master)
  Serial.begin(9600);
  randomSeed(analogRead(13));
  GENERATED_TICKET = random(65535);
  Dps310PressureSensor.begin(onboard_bus);   // Call begin to initialize DPS310Pressure Sensor
  pinMode(LED1, OUTPUT);
  for (int i = 0; i <= 3; i++) {
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
  }
  initialization();
  Serial.println("Starting initialization");
  network_bus.onReceive(receiveEvent);


}


void loop() {
  Serial.println("workin");
  delay(100);
  send_description();
}


void initialization() {
  network_bus.beginTransmission(NODE_ADDRESS);
  error = network_bus.endTransmission();

  if (error == 0) {
    byte buffer[PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (GENERATED_TICKET >> 8) & 0xFF;
    buffer[1] = GENERATED_TICKET & 0xFF;

    network_bus.beginTransmission(8); // transmit to device #8
    network_bus.write(buffer, PAYLOAD_SIZE);              // sends two byte
    network_bus.endTransmission();    // stop transmitting
    Serial.println(GENERATED_TICKET);
  }
  else {
    return;
  }
}


void receiveEvent(int howMany) {
  if (howMany == 3) {

    byte a = network_bus.read();
    byte b = network_bus.read();
    RECEIVED_TICKET = a;
    RECEIVED_TICKET = (RECEIVED_TICKET << 8) | b;
    Serial.println(RECEIVED_TICKET);
    byte NEW_NODE_ADDRESS = network_bus.read();
    Serial.println(NEW_NODE_ADDRESS);
    if ((RECEIVED_TICKET == GENERATED_TICKET) && (NEW_NODE_ADDRESS != DUPLICATE_ERROR) && (LED_STATE == false)) {
      LED_STATE = true;
      network_bus.begin(NEW_NODE_ADDRESS);
      Serial.println("New address assigned");
      digitalWrite(LED1, HIGH);
    }
    if ((RECEIVED_TICKET == GENERATED_TICKET) && (NEW_NODE_ADDRESS == DUPLICATE_ERROR) && (LED_STATE == false)) {
      GENERATED_TICKET = random(65535);
      Serial.println("Duplicate error! Created new random number!");
      digitalWrite(LED1, LOW);
      delay(200);
      digitalWrite(LED1, HIGH);
      delay(200);
      digitalWrite(LED1, LOW);
      initialization();
    }
    else if ((RECEIVED_TICKET != GENERATED_TICKET) && (LED_STATE == false)) {
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
     byte a = network_bus.read();
     Serial.println(a);

     if (a = DESCRIPTION_CMD) {
      SEND_DESCRIPTION = true;
     }
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
    const int BUFFER_SIZE = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0);
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
    Serial.println(description);
    network_bus.beginTransmission(8);
    network_bus.print(description);
    network_bus.endTransmission();
    Serial.println("Json sent");
    SEND_DESCRIPTION = false;
  }
  else {
    return;
  }
}


void readCommand() {
}

int resolveCommand(int cmd) {
  if (cmd == 1) {
   int32_t temperature;
   int16_t oversampling = 7;
   
   Dps310PressureSensor.measureTempOnce(temperature, oversampling);
  
   return temperature;
  }
  if (cmd == 2) {
   int32_t pressure;
   int16_t oversampling = 7;
   
   Dps310PressureSensor.measurePressureOnce(pressure, oversampling);
  
   return pressure;
 }
}


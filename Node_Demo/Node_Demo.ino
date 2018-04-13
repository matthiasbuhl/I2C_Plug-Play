#include <Wire.h>
#include <ArduinoJson.h>

bool DESCRIPTION_RECEIVED = false;
bool DESCRIPTION_NEEDED = false;
bool DEVICE_RESPONSE = true;
bool DUPLICATE = false;
bool NEW_DEVICE_DETECTED = false;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte NODE_ADDRESS = 9;
byte NEW_NODE_ADDRESS = 14;
byte a, b;
byte i = 0;
byte r = 0;
byte DUPLICATE_ERROR = 4;
byte DESCRIPTION_CMD = 101;

int k = 0;

int const ARRAY_LENGTH = 112;
uint16_t DEVICE_ID;
uint16_t DUPLICATE_NUMBER;

uint16_t NUM_MEMORY[ARRAY_LENGTH];
int ADDRESS_MEMORY[ARRAY_LENGTH];


void setup() {
  Wire.begin(8);        // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  // start serial for output
  delay(200);
  Serial.println("Initialized");
}

void loop() {
  Serial.println("workin");
  resolve_duplicate();
  delay(500);
  request_description();
  enumeration();
  delay(2000);
}


void receiveEvent(int howMany) {
  byte error;
  if (howMany == 2) { 
    a = Wire.read();
    b = Wire.read();
    DEVICE_ID = a;
    DEVICE_ID = (DEVICE_ID << 8) | b;

    error = search_duplicates(NUM_MEMORY, DEVICE_ID, ARRAY_LENGTH);


    if (i < 112) {
      if (error == 0) {
        NUM_MEMORY[i] = DEVICE_ID;
        Serial.print("Received new ID: ");
        Serial.println(DEVICE_ID);
        i++;
        NEW_DEVICE_DETECTED = true;
      }
      else if (error == 1) {
        DUPLICATE_NUMBER = DEVICE_ID;
        Serial.println("Duplicate detected!");
        i--;
        DUPLICATE = true;
      }

      else if (i >= 112) {
        Serial.println("All addresses already in use!");
      } 
    }
  }
  if (howMany == 1) {
   byte s = Wire.read();
   if (s == 101) {
   Serial.println("Description received");
   delay(600);
    StaticJsonBuffer<200> jsonBuffer;
    char json[] = 
    "{\"id\":12510,\"name\":\"DPS310\",\"category\":[5,10],\"Service\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1010,1011],\"datalength\":[16,16]}";
  
   
    Serial.print("Received description: ");
    Serial.println(json);
    delay(500);
    Serial.println("Parsing description");
    delay(500);
    JsonObject& root = jsonBuffer.parseObject(json);
    
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }
    const char* name = root["name"];
    long id = root["id"];
    int category = root["category"][0];
    const char* srv1 = root["Service"][0];
    const char* srv2 = root["Service"][1];
    int cmd1 = root["commands"][0];
    int cmd2 = root["commands"][1];
    int datalength1 = root["datalength"][0];
    int datalength2 = root["datalength"][1];

    Serial.println(""); 
    Serial.print("Device: ");
    Serial.println(name);
    //Serial.print("ID: ");
    //Serial.println(id);
    Serial.print("Category: ");
    Serial.println(category);
    Serial.print("Service: ");
    Serial.println(srv1);
    Serial.print("Service: ");
    Serial.println(srv2);
    Serial.print("Command for first service: ");
    Serial.println(cmd1);
    Serial.print("Command for second service: ");
    Serial.println(cmd2);
    Serial.print("Data length first service: ");
    Serial.println(datalength1);
    Serial.print("Data length second service: ");
    Serial.println(datalength2);
    Serial.println();
    } 
   if (s == 102) {
   Serial.println("Description received");
   delay(600);
    StaticJsonBuffer<200> jsonBuffer;
    char json[] = 
    "{\"id\":12510,\"name\":\"TLV493D\",\"category\":[3],\"Service\":[\"getX\",\"getY\",\"getZ\"],\"commands\":[1010,1011,1012],\"datalength\":[8,8,8]}";
  
   
    Serial.print("Received description: ");
    Serial.println(json);
    delay(500);
    Serial.println("Parsing description");
    delay(500);
    JsonObject& root = jsonBuffer.parseObject(json);
    
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }
    const char* name = root["name"];
    long id = root["id"];
    int category = root["category"][0];
    const char* srv1 = root["Service"][0];
    const char* srv2 = root["Service"][1];
    const char* srv3 = root["Service"][2];
    int cmd1 = root["commands"][0];
    int cmd2 = root["commands"][1];
    int cmd3 = root["commands"][2];
    int datalength1 = root["datalength"][0];
    int datalength2 = root["datalength"][1];
    int datalength3 = root["datalength"][2];

    Serial.println(); 
    Serial.print("Device: ");
    Serial.println(name);
    //Serial.print("ID: ");
    //Serial.println(id);
    Serial.print("Category: ");
    Serial.println(category);
    Serial.print("Service: ");
    Serial.println(srv1);
    Serial.print("Service: ");
    Serial.println(srv2);
    Serial.print("Service: ");
    Serial.println(srv3);
    Serial.print("Command for first service: ");
    Serial.println(cmd1);
    Serial.print("Command for second service: ");
    Serial.println(cmd2);
    Serial.print("Command for second service: ");
    Serial.println(cmd3);
    Serial.print("Data length first service: ");
    Serial.println(datalength1);
    Serial.print("Data length second service: ");
    Serial.println(datalength2);
    Serial.print("Data length second service: ");
    Serial.println(datalength3);
    Serial.println();
    } 
  }
  
}

int search_duplicates(uint16_t arr[], const int value, const int step) {
  bool detected = false;
  for (int j = 0; j < 112; j++) {
    if (arr[j] == value) {
      detected = true;
    }
    else {
    }
  }
  if (detected == true) {
    return 1;
  }
  else {
    return 0;
  }
  Serial.println("No duplicates detected");
}


void enumeration() {
  if (NEW_DEVICE_DETECTED == true) {
  byte error;
  Wire.beginTransmission(NODE_ADDRESS);
  error = Wire.endTransmission();
  

    if ((error == 0) && (NEW_NODE_ADDRESS < 120)) {
      for (k; k < i; k++) {
      byte buffer[WRITE_PAYLOAD_SIZE]; 
      buffer[0] = (NUM_MEMORY[k] >> 8) & 0xFF;
      buffer[1] = NUM_MEMORY[k] & 0xFF;
      buffer[2] = NEW_NODE_ADDRESS;
      ADDRESS_MEMORY[k] = NEW_NODE_ADDRESS;
      Wire.beginTransmission(NODE_ADDRESS);
      Wire.write(buffer, WRITE_PAYLOAD_SIZE);
      Wire.endTransmission();
      NEW_NODE_ADDRESS++;
      Serial.print("New address assigned to Device with ID: ");
      Serial.println(NUM_MEMORY[k]);
      NUM_MEMORY[k] = 0;
      delay(500);
      }
    }
    else if (error == 2) {
      Serial.println("All devices on bus assigned");
    }
    else if (error == 4) {
      Serial.println("All addresses already in use!");
    }
    else {
      return;
    }
   NEW_DEVICE_DETECTED = false; 
  }
  else {
    return;
  }
  print_memory();
  DESCRIPTION_NEEDED = true;
}

void resolve_duplicate() {
  if (DUPLICATE == true) {
    byte buffer[WRITE_PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (DUPLICATE_NUMBER >> 8) & 0xFF;
    buffer[1] = DUPLICATE_NUMBER & 0xFF;
    buffer[2] = DUPLICATE_ERROR;

    Wire.beginTransmission(NODE_ADDRESS);
    Wire.write(buffer, WRITE_PAYLOAD_SIZE);
    Wire.endTransmission();

    Serial.println("Requested new number");
    delay(500);
    DUPLICATE = false;
  }
}


void print_memory() {
    for (int k = 0; k < i; k++) {
      String Str1 = "Device";
      String Str2 = " connected at: 0x";
      Serial.print(Str1);
      Serial.print(k+1);
      Serial.print(Str2);
      Serial.println(ADDRESS_MEMORY[k]);
    }
}

void request_description() {
  if (DESCRIPTION_NEEDED == true) {
    Wire.beginTransmission(ADDRESS_MEMORY[r]);
    Wire.write(DESCRIPTION_CMD);
    Wire.endTransmission();
    Serial.print("Request for Description sent to 0x");
    Serial.println(ADDRESS_MEMORY[r]);
    r++;
    }
  DESCRIPTION_NEEDED = false;
}





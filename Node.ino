#include <Wire.h>
#include <ArduinoJson.h>

TwoWire network_bus = Wire;

bool DEVICE_RESPONSE = true;
bool DUPLICATE = false;
bool NEW_DEVICE_DETECTED = false;
bool DESCRIPTION_RECEIVED = false;
bool DESCRIPTION_NEEDED = false;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte INIT_DEVICE_ADDRESS = 9;
byte NEW_DEVICE_ADDRESS = 14;
byte DUPLICATE_ERROR = 4;
byte DESCRIPTION_CMD = 101;
byte a, b;
byte i = 0;
int k = 0;

int const ARRAY_LENGTH = 112;
uint16_t GENERATED_TICKET;
uint16_t DUPLICATE_NUMBER;

uint16_t NUM_MEMORY[ARRAY_LENGTH];
int ADDRESS_MEMORY[ARRAY_LENGTH];

char json[] = {};


void setup() {
  network_bus.begin(8);        // join i2c bus (address optional for master)
  network_bus.onReceive(receiveEvent);
  Serial.begin(9600);  // start serial for output
  delay(200);
  Serial.println("Initialized");
}


void loop() {
  Serial.println("workin");
  resolve_duplicate();
  delay(500);
  assign_address();
  print_memory();
  delay(500);
  request_description();
  delay(500);
  parse_description();
}


void receiveEvent(int howMany) {
  byte error;
  if (howMany == 2) {
    a = network_bus.read();
    b = network_bus.read();
    GENERATED_TICKET = a;
    GENERATED_TICKET = (GENERATED_TICKET << 8) | b;

    error = search_duplicates(NUM_MEMORY, GENERATED_TICKET, ARRAY_LENGTH);


    if (i < 112) {
      if (error == 0) {
        NUM_MEMORY[i] = GENERATED_TICKET;
        Serial.println(GENERATED_TICKET);
        i++;
        NEW_DEVICE_DETECTED = true;
      }
      else if (error == 1) {
        DUPLICATE_NUMBER = GENERATED_TICKET;
        Serial.println("Duplicate detected!");
        i--;
        DUPLICATE = true;
      }

      else if (i >= 112) {
        Serial.println("All addresses already in use!");
      }
    }
  }
  if (howMany >= 10) {
    Serial.println(howMany);
    String inString = "";
    while (network_bus.available()) {
      inString += char(network_bus.read());
    }
    Serial.println(inString);
    inString.toCharArray(json, 32);
    DESCRIPTION_RECEIVED = true;
  }
}


int search_duplicates(uint16_t arr[], const uint16_t value, const int step) {
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


void assign_address() {
  if (NEW_DEVICE_DETECTED == true) {
    byte error;
    network_bus.beginTransmission(INIT_DEVICE_ADDRESS);
    error = network_bus.endTransmission();


    if ((error == 0) && (NEW_DEVICE_ADDRESS < 120)) {
      for (k = k; k < i; k++) {
        byte buffer[WRITE_PAYLOAD_SIZE];
        buffer[0] = (NUM_MEMORY[k] >> 8) & 0xFF;
        buffer[1] = NUM_MEMORY[k] & 0xFF;
        buffer[2] = NEW_DEVICE_ADDRESS;
        ADDRESS_MEMORY[k] = NEW_DEVICE_ADDRESS;
        network_bus.beginTransmission(INIT_DEVICE_ADDRESS);
        network_bus.write(buffer, WRITE_PAYLOAD_SIZE);
        network_bus.endTransmission();
        NEW_DEVICE_ADDRESS++;
        Serial.print("New address assigned to ");
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
    DESCRIPTION_NEEDED = true;
  }
  else {
    return;
  }
}


void resolve_duplicate() {
  if (DUPLICATE == true) {
    byte buffer[WRITE_PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (DUPLICATE_NUMBER >> 8) & 0xFF;
    buffer[1] = DUPLICATE_NUMBER & 0xFF;
    buffer[2] = DUPLICATE_ERROR;

    network_bus.beginTransmission(INIT_DEVICE_ADDRESS);
    network_bus.write(buffer, WRITE_PAYLOAD_SIZE);
    network_bus.endTransmission();

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
    Serial.print(k + 1);
    Serial.print(Str2);
    Serial.println(ADDRESS_MEMORY[k]);
  }
}


void request_description () {
  if (DESCRIPTION_NEEDED == true && DESCRIPTION_RECEIVED == false) {
  network_bus.beginTransmission(ADDRESS_MEMORY[0]);
  network_bus.write(DESCRIPTION_CMD);
  network_bus.endTransmission();
  Serial.print("Request for Description sent to 0x");
  Serial.println(ADDRESS_MEMORY[0]);
  }
}


void parse_description() {
  if (DESCRIPTION_RECEIVED == true) {
    const int BUFFER_SIZE = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0);
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success()) {
      Serial.println("parseObject() failed");
      DESCRIPTION_RECEIVED = false;
      return;
    }

    const char* name = root["name"];
    long id = root["id"];
    int category = root["category"][0];
    const char* fnct1 = root["fnct"][0];
    const char* fnct2 = root["fnct"][1];
    int cmd1 = root["cmd"][0];
    int datalength1 = root["datalength"][0];
    int datalength2 = root["datalength"][1];

     
    Serial.print("Device: ");
    Serial.println(name);
    Serial.print("ID: ");
    Serial.println(id);
    Serial.print("Category: ");
    Serial.println(category);
    Serial.print("Function: ");
    Serial.println(fnct1);
    Serial.print("Function: ");
    Serial.println(fnct2);
    Serial.print("Command for first function: ");
    Serial.println(cmd1);
    Serial.print("Data length first function: ");
    Serial.println(datalength1); 
  }
  
}


void publish_description() {
  //todo
}



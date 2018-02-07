#include <Wire.h>


bool DEVICE_RESPONSE = true;
bool DUPLICATE = false;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte NODE_ADDRESS = 9;
byte NEW_NODE_ADDRESS = 14;
byte a, b;
byte i = 0;
byte DUPLICATE_ERROR = 4;

int k = 0;

int const ARRAY_LENGTH = 112;
uint16_t RANDOM_NUMBER;
uint16_t DUPLICATE_NUMBER;

uint16_t fake = 49;

uint16_t NUM_MEMORY[ARRAY_LENGTH];


void setup() {
  Wire.begin(8);        // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  // start serial for output
  delay(200);
  Serial.println("Initialized");
}

void loop() {
  Serial.println("workin");
  
  print_memory();
  resolve_duplicate();
  enumeration();
  

  delay(2000);
}


void receiveEvent(int howMany) {
  byte error;
  if (howMany == 2) {
    a = Wire.read();
    b = Wire.read();
    RANDOM_NUMBER = a;
    RANDOM_NUMBER = (RANDOM_NUMBER << 8) | b;

    error = search_duplicates(NUM_MEMORY, RANDOM_NUMBER, ARRAY_LENGTH);


    if (i < 112) {
      if (error == 0) {
        NUM_MEMORY[i] = RANDOM_NUMBER;
        Serial.println(RANDOM_NUMBER);
        i++;
      }
      else if (error == 1) {
        DUPLICATE_NUMBER = RANDOM_NUMBER;
        Serial.println("Duplicate detected!");
        DUPLICATE = true;
      }

      else if (i >= 112) {
        Serial.println("All addresses already in use!");
      }

      
    }
  }
}

int search_duplicates(int arr[], const int value, const int step) {
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
}


void enumeration() {
  for (k = 0; k < i; k++) {
  byte error;
  Wire.beginTransmission(NODE_ADDRESS);
  error = Wire.endTransmission();
  

  if ((error == 0) && (NEW_NODE_ADDRESS < 120)) {
    byte buffer[WRITE_PAYLOAD_SIZE]; 
    buffer[0] = (NUM_MEMORY[k] >> 8) & 0xFF;
    buffer[1] = NUM_MEMORY[k] & 0xFF;
    buffer[2] = NEW_NODE_ADDRESS;
    
    Wire.beginTransmission(NODE_ADDRESS);
    Wire.write(buffer, WRITE_PAYLOAD_SIZE);
    Wire.endTransmission();
    NEW_NODE_ADDRESS++;
    Serial.print("New address assigned to ");
    Serial.println(NUM_MEMORY[k]);
    delay(500);
  }
  else if (error == 2) {
    Serial.println("All devices on bus assigned");
  }
  else if (error == 4) {
    Serial.println("All addresses already in use!");
  }

  }
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
    Serial.println(NUM_MEMORY[k]);
  }
}


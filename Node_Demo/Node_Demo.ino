#include <Wire.h>
#include <ArduinoJson.h>


bool DESCRIPTION_RECEIVED = false;
bool DESCRIPTION_NEEDED = false;
bool DEVICE_RESPONSE = true;
bool DUPLICATE = false;
bool NEW_DEVICE_DETECTED = false;
byte READ_PAYLOAD_SIZE = 2;
byte WRITE_PAYLOAD_SIZE = 3;
byte NODE_ADDRESS = 8;
byte INITIAL_DEVICE_ADDRESS = 9; //initial device with which all new devices enter the bus at the beginning
byte NEW_DEVICE_ADDRESS = 10;   //start number from which the node assigns new addreses to the connected devices and counts up
byte MAXIMUM_I2CADDRESS = 110;    //assignable i2c addresses = 112 - (master address and inital node address)
byte a, b;
byte i = 0;
byte r = 0;
byte DUPLICATE_ERROR = 4;
byte DESCRIPTION_CMD = 1010;   //randomly assigned number to identify which command has been sent
byte DESCRIPTION_TLV493D = 102;   //randomly assigned number to identify which command has been sent
byte DESCRIPTION_DPS310 = 103;    //randomly assigned number to identify which command has been sent

int DESCRIPTION = 0;

int k = 0;

int const ARRAY_LENGTH = 110;   // set the array length to the maximum of available i2c-addresses
uint16_t DEVICE_ID;
uint16_t DUPLICATE_NUMBER;

uint16_t NUM_MEMORY[ARRAY_LENGTH];
int ADDRESS_MEMORY[ARRAY_LENGTH];


void setup() {
  delay(3000);
  Wire.begin(NODE_ADDRESS);        // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent);   //setup handler for incoming data
  Serial.begin(9600);  // start serial for output
  delay(200);
  Serial.println("Initialized");
}

void loop() {
  Serial.println("workin");   //print something to indicate on serial monitor, that the node is going through the loop function as long as no data is received
  delay(500);
  //request_description();    //if a new device was registered, this function triggers in the next loop and sends a request to the new device for the device description
  description();
  enumeration();    //if a new device was registered, this function assigns a new i2c-address to the device
  delay(1000);
}


void receiveEvent(int howMany) {
  if (howMany == 2) {   //size of IDs are two bytes
    a = Wire.read();    //read out first part of ID
    b = Wire.read();    //read out second part of ID
    DEVICE_ID = a;
    DEVICE_ID = (DEVICE_ID << 8) | b;   //merge both parts of the ID together

    if (i < MAXIMUM_I2CADDRESS) {    //check if there are still addresses available
      NUM_MEMORY[i] = DEVICE_ID;    //save received device id for later assignement of a new address
      Serial.print("Received new ID: ");
      Serial.println(DEVICE_ID);
      i++;
      NEW_DEVICE_DETECTED = true;   //triggers the enumeration function in the next loop
    }
    else if (i > MAXIMUM_I2CADDRESS) {
      Serial.println("All addresses already in use!");    //if i is bigger than the maximum of available i2c-addresses, no futher addresses should be assigned
    }

  }
  if (howMany == 1) {   //read out the description of the connected device. Originally the devices should transmitt the entire json, but due to buffer limitations of the arduino uno, this segment demonstrates the read out process of a received json
    byte s = Wire.read();
    if (s == DESCRIPTION_DPS310) {
      Serial.println("Description received");
      DESCRIPTION = 1;
      /*
      DynamicJsonBuffer jsonBuffer;
      char json[] =
      "{\"id\":12510,\"name\":\"DPS310\",\"category\":[5,10],\"Service\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1010,1011],\"datalength\":[16,16]}";    //this data should be sent by the device. Here it is added in for demonstration purpose


      Serial.print("Received description: ");
      Serial.println(json);
      delay(1000);
      Serial.println("Parsing description");
      delay(1000);
      JsonObject& root = jsonBuffer.parseObject(json);
      /*
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
        if (s == DESCRIPTION_TLV493D) {
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
        Serial.println(); */
    }
    if (s == DESCRIPTION_TLV493D) {
      Serial.println("Description received");
      DESCRIPTION = 2;
    }
  }
}




void enumeration() {
  if (NEW_DEVICE_DETECTED == true) {
    byte error;
    Wire.beginTransmission(INITIAL_DEVICE_ADDRESS);
    error = Wire.endTransmission();   //verifies that devices are still registered under the initial device address


    if ((error == 0) && (NEW_DEVICE_ADDRESS < MAXIMUM_I2CADDRESS)) {
      for (k; k < i; k++) {
        byte buffer[WRITE_PAYLOAD_SIZE];
        buffer[0] = (NUM_MEMORY[k] >> 8) & 0xFF;    //load the buffer with the first part of the received ID of the device
        buffer[1] = NUM_MEMORY[k] & 0xFF;   //load the buffer with the first part of the received ID of the device
        buffer[2] = NEW_DEVICE_ADDRESS;   //load the buffer with the new address the device should take on the bus
        ADDRESS_MEMORY[k] = NEW_DEVICE_ADDRESS;   //save the assigned address of the new device
        Wire.beginTransmission(INITIAL_DEVICE_ADDRESS);   //send the device ID with the new address to the initial device address
        Wire.write(buffer, WRITE_PAYLOAD_SIZE);
        Wire.endTransmission();
        NEW_DEVICE_ADDRESS++;
        Serial.print("New address assigned to Device with ID: ");
        Serial.println(NUM_MEMORY[k]);
        NUM_MEMORY[k] = 0;
        delay(1000);
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
  print_memory();   //prints out all the already assigned devices
  DESCRIPTION_NEEDED = true;  //triggers the request_description function in the next loop
}


void print_memory() {   //prints out all the already assigned devices
  for (int k = 0; k < i; k++) {
    String Str1 = "Device";
    String Str2 = " connected at: 0x";
    Serial.print(Str1);
    Serial.print(k + 1);
    Serial.print(Str2);
    Serial.println(ADDRESS_MEMORY[k]);
  }
}


void request_description() {    //sends the command that triggers the device to send the description data
  if (DESCRIPTION_NEEDED == true) {
    /*
      Serial.println("Checking wire");
      Wire.beginTransmission(ADDRESS_MEMORY[r]);
      error = Wire.endTransmission();   //verifies that devices are still registered under the initial device address
      Serial.println("Wire checked"); */


    for (int k = 0; k < i; k++) {
      Wire.begin();
      delay(1000);
      Serial.print("Sending Request for Description to 0x");
      byte buffer[2];
      buffer[0] = (DESCRIPTION_CMD >> 8) & 0xFF;    //load the buffer with the first part of the received ID of the device
      buffer[1] = DESCRIPTION_CMD & 0xFF;   //load the buffer with the first part of the received ID of the device
      Serial.println(ADDRESS_MEMORY[k]);
      Wire.beginTransmission(ADDRESS_MEMORY[k]);
      Wire.write(buffer, 2);
      Wire.endTransmission();
      Serial.print("Request for Description sent to 0x");
      Serial.println(ADDRESS_MEMORY[k]);
      DESCRIPTION_NEEDED = false;
      delay(500);
    }
  }
}

void description() 
{ 

  //GLOBALE Variable wird geprüft, die zuvor im receiveEvent (Interrupt) gesetzt wurde
  if(DESCRIPTION==1)
  {
    Serial.println("Description received");
    DynamicJsonBuffer jsonBuffer;
    Serial.println("\n");
    char json[] = "{\"id\":12510,\"name\":\"DPS310\",\"category\":[5,10],\"Service\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1010,1011],\"datalength\":[16,16]}";

    Serial.print("Received description: ");
    Serial.println(json);
    Serial.println("Parsing description");
    JsonObject& root = jsonBuffer.parseObject(json);    //Es kommt Parsing fail Information - musst du mal schauen was hier noch nicht stimmt?

    if (!root.success()) {
      Serial.println("parseObject() failed");
      DESCRIPTION=0;
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
    DESCRIPTION =0;   //Zurücksetzen der globalen Variable
    return;
   }
   
   if(DESCRIPTION==2)
   {
    Serial.println("Description received");
    DynamicJsonBuffer jsonBuffer;
    char json[] = "{\"id\":\"12510\",\"name\":\"TLV493D\",\"category\":\"[3]\",\"Service\":[\"getX\",\"getY\",\"getZ\"],\"commands\":\"[1010,1011,1012]\",\"datalength\":[8,8,8]}";
    
    Serial.print("Received description: ");
    Serial.println(json);
    delay(500);
    Serial.println("Parsing description");
    delay(500);
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success()) {
      Serial.println("parseObject() failed");
      DESCRIPTION=0;
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
    DESCRIPTION =0;     //Zurücksetzen der globalen Variable
    return; 
   }
}



#include <Wire.h>

#define PAYLOAD_SIZE 2


bool LED_STATE = false;
bool SEND_DESCRIPTION = false;
bool INIT_COMPLETE = false;
int DEVICE_ID = 12700;    //unique device ID. Needs to be changed for every device flashed with this code.
int x;
byte NODE_ADDRESS = 8;    //address to reach the node of the network
byte INITIAL_DEVICE_ADDRESS = 9; //initial device with which all new devices enter the bus at the beginning
byte NEW_DEVICE_ADDRESS;
byte a, b, p;
byte DESCRIPTION_CMD = 1010;
byte DESCRIPTION_CODE = 102; //description code for a TLV493D. Comment in or out according to the used device.
//byte DESCRIPTION_CODE = 103;    ////description code for a DPS310. Comment in or out according to the used device.

uint8_t Buffer[10];
int initialisation = 0;
int error;
int retry = 0;

void setup() {
  delay(4000);
  Wire.begin(INITIAL_DEVICE_ADDRESS); // join i2c bus (address optional for master)
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  for (int i = 0; i <= 3; i++) { //flash led to demonstrate setup process
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
  }
  Serial.println("Starting initialization");
  Wire.onReceive(receiveEvent);   //setup handler for incoming data
}


void loop() {
  Serial.println("workin");   ////print something to indicate on serial monitor, that the node is going through the loop function as long as no data is received.
  if(initialisation == 0) {
  initialization();   //sends message to the node of the network containing the device ID. This enables the node to discover that a new device has entered the bus
  }
  delay(700);
  send_description();   //sends the description of the device, if SEND_DESCRIPTION has been set true by receiveEvent handler
}


void initialization() {
  Wire.beginTransmission(NODE_ADDRESS);
  error = Wire.endTransmission();

  if (error == 0) {
    if (retry == 0) {
    byte buffer[PAYLOAD_SIZE]; // Load buffer
    buffer[0] = (DEVICE_ID >> 8) & 0xFF;
    buffer[1] = DEVICE_ID & 0xFF;

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buffer, PAYLOAD_SIZE);              // sends two byte
    Wire.endTransmission();    // stop transmitting
    Serial.println(DEVICE_ID);
    delay(9000);
    //initialisation = 1;
    } 
    else if (retry == 1) {
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
    delay(9000);
    retry = 0;  
    }
  }
  else if (error > 0) {
    digitalWrite(LED2, HIGH);
    delay(200);
    digitalWrite(LED2, LOW);
    delay(200);
    digitalWrite(LED2, HIGH);
    delay(200);
    digitalWrite(LED2, LOW);
    delay(8000);
    return;
  }
}

void receiveEvent(int howMany) {
  if (howMany == 3) {

    byte a = Wire.read();
    byte b = Wire.read();
    byte c = Wire.read();    //read out the new address which should be assumed on the bus
    x = a;
    x = (x << 8) | b;   //merge first and second part of received ID together
    NEW_DEVICE_ADDRESS = c;
    Serial.println(x);
    Serial.println(NEW_DEVICE_ADDRESS);
    if ((x == DEVICE_ID) && (LED_STATE == false)) {    //if the received ID matches the own ID the new received address will be assumed
      LED_STATE = true;      
      Serial.println("New address assigned");
      SEND_DESCRIPTION = true; //sets the trigger to send the description in the next loop
      initialisation = 1;
      digitalWrite(LED1, HIGH);
      
    }
    else if ((x != DEVICE_ID) && (LED_STATE == false)) {    //if the received ID doesn't match the own ID, blink led and wait for further messages
      Serial.println("wrong number");
      initialisation = 0;
      retry = 1;
    }
  }
  if (howMany == 2) {   //handles the received command to send the description
    byte c = Wire.read();
    byte d = Wire.read();

    byte y = c;
    y = (y << 8) | d;
    if (y == DESCRIPTION_CMD) {
      digitalWrite(LED2, HIGH);
      //SEND_DESCRIPTION = true; //sets the trigger to send the description in the next loop
    }
    else {
      return;
    }
  }
}

void send_description() {
  if (SEND_DESCRIPTION == true) {   //final version should contain the transmission of the device description as json. For demonstration purpose, this was commented out and simplified with a fixed command.
    digitalWrite(LED2, LOW);
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

    byte error;
    Wire.beginTransmission(NODE_ADDRESS);
    error = Wire.endTransmission();   //verifies that devices are still registered under the initial device address

    if (error == 0) {
      Wire.beginTransmission(NODE_ADDRESS);   //send the fixed command to the node to allow for a correct description
      Wire.write(DESCRIPTION_CODE);   //send the according description code. can be set in the declaration of the variables
      error = Wire.endTransmission();
      Serial.println("Description sent");
      digitalWrite(LED2, HIGH);
      delay(500);
      digitalWrite(LED2, LOW);
      delay(500);
      digitalWrite(LED2, HIGH);
      SEND_DESCRIPTION = false;   //cancel trigger to send the description
      Wire.begin(NEW_DEVICE_ADDRESS);
      delay(500);
    }
    else {
      return;
    }
  }
}







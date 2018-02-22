

//#include <Wire.h>


// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
#include <Wire.h>
#include <ArduinoJson.h>


char json[] = {};
bool jsonreceived = false;


void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  //Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);

}

void loop () {
  delay(1000);
  Serial.println("workin");
  Serial.println(json);
  parseJson();

}


// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
/*
  void requestEvent() {
  if (firsttime == true) {
  Wire.write(7);
  firsttime = false;
  }
  else {
  Wire.write(message);
  }
  } */

void receiveEvent(int howMany) {
  Serial.println(howMany);
  String inString = "";
  while (Wire.available()) {
    inString += char(Wire.read());
  }
  inString.toCharArray(json, 128);
  jsonreceived = true;
}


void parseJson() {
  if (jsonreceived == true) {
    const int BUFFER_SIZE = JSON_OBJECT_SIZE(10) + JSON_ARRAY_SIZE(10);
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success()) {
      Serial.println("parseObject() failed");
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
  jsonreceived = true;
}




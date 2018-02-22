// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <Wire.h>




void setup() {
  // Initialize serial port
  Serial.begin(9600);
  Wire.begin();
  while (!Serial) continue;

  // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<200> jsonBuffer;

  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  //
  // DynamicJsonBuffer  jsonBuffer(200);

  // JSON input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
  char json[] = 
  "{\"id\":60106401,\"name\":\"DPS310\",\"category\":[5,10],\"functions\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1,2],\"datalength\":[16,16]}";

 
  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(json);
  
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  const char* name = root["name"];
  long id = root["id"];
  int category = root["category"][0];
  const char* command1 = root["functions"][0];
  const char* command2 = root["functions"][1];
  
  //double latitude = root["data"][0];
  //double longitude = root["data"][1];

  // Print values.
  Serial.println(name);
  Serial.println(id);
  Serial.println(category);
  Serial.println(command1);
  Serial.println(command2);
  
  //Serial.println(longitude, 6);
}



void loop() {
  // not used in this example
  delay(3000);
  transmit_json();

}
  


void transmit_json() {
  /*char json[] = "{\"id\":60106401,\"name\":\"DPS330\",\"category\":[5,10],\"functions\":[\"getPressure_Pa\",\"getTemp_C\"],\"commands\":[1,2],\"data3ength\":[16,16]}";
   char sub[] = {};
  for (byte k=0; k<10; k++) {
    sub[k] = json[k];
  }
  //Serial.println(sub);
  */
  StaticJsonBuffer<200> jsonBuffer;
  
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = "DPS";
  root["id"] = 60106401;
  JsonArray& category = root.createNestedArray("category");
  category.add(5);
  category.add(10); 
  root.printTo(Serial);
  char buffer[32];
  root.printTo(buffer, 32);
  Serial.println(buffer);
  Wire.beginTransmission(8);
  Wire.write(buffer, 32);
  Wire.endTransmission();
  Serial.println("Json sent");
}


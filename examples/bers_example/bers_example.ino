/*
    BersControl -----
    Made by Robotika Politeknik Harapan Bersama
    Source code: https://github.com/Faizyee/BersControl
    Using WiFi ESP-32/ESP-8266, WebSocketsServer, ArduinoJson
*/


#include <bersControlV1.h>

bersControlV1 mycontrol;

void onEventBersControl(const BersData& data) {
  // For get client id
  if (data.getClientID == 0) {
    // For send message to client
    mycontrol.sendClient(data.getClientID, "Hello first client!");
    // For get IP client
    mycontrol.sendClient(data.getClientID, "your IP is " + data.getIPClient);
  } else {
    // For disconnect client
    mycontrol.clientDisconnect(data.getClientID);
  }

  // For get status code
  Serial.print("statusCode : ");
  Serial.println(String(data.statusCode));

  // For get status message
  Serial.print("statusMessage : ");
  Serial.println(data.statusMessage);

  if (data.statusCode == 0) {
    // For get Json data
    JsonDocument jsonData = data.out.typeJson;
    String output = jsonData["data"]["output"]["other"].as<String>();
    Serial.print("typeJson : ");
    Serial.println(output);
  }

  // For get type char from data
  Serial.print("typeChar : ");
  Serial.println(String(data.out.typeChar));

  // For get type string from data
  Serial.print("typeString : ");
  Serial.println(data.out.typeString);

  // For get type int from data if data is numerical
  Serial.print("typeInt : ");
  Serial.println(String(data.out.typeInt));

  // For get type double from data if data is numerical
  Serial.print("typeDouble : ");
  Serial.println(String(data.out.typeDouble));
}

void setup() {
    Serial.begin(115200);
    // Set your WiFi SSID and Password
    mycontrol.begin("RobotikaBers", "robotikabers");
    // Default port is 81, you can change port with mycontrol.setPort(port);
    
    // For get IP Address
    Serial.print("IP Address : ");
    Serial.println(mycontrol.getIPAddress());

    // For set event
    mycontrol.onEvent(onEventBersControl);
}

void loop() {
    mycontrol.loop();
}

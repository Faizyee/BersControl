/*
    BersControl -----
    Made by Robotika Politeknik Harapan Bersama
    Source code: https://github.com/Faizyee/BersControl
    Using WiFi ESP-32/ESP-8266, WebSocketsServer, ArduinoJson
*/


#include <bersControlV1.h>

bersControlV1 mycontrol;

void onEventBersControl(const BersSignal& data) {
  // For get client id
  if (data.client.id == 0) {
    // For send message to client
    mycontrol.sendClient(data.client.id, "Hello first client!");
    // For get IP client
    mycontrol.sendClient(data.client.id, "your IP is " + data.client.ip);
  } else {
    // For disconnect client
    mycontrol.clientDisconnect(data.client.id);
  }

  // For get status code
  Serial.print("statusCode : ");
  Serial.println(String(data.status.code));

  // For get status message
  Serial.print("statusMessage : ");
  Serial.println(data.status.message);

  if (data.status.code == 0) {
    // For get Json data
    JsonDocument jsonData = data.output.json;
    String output = jsonData["data"]["output"]["other"].as<String>();
    Serial.print("typeJson : ");
    Serial.println(output);
  }

  // For get type char from data
  Serial.print("typeChar : ");
  Serial.println(String(data.output.char));

  // For get type string from data
  Serial.print("typeString : ");
  Serial.println(data.output.string);

  // For get type int from data if data is numerical
  Serial.print("typeInt : ");
  Serial.println(String(data.output.int));

  // For get type double from data if data is numerical
  Serial.print("typeDouble : ");
  Serial.println(String(data.output.double));
}

void setup() {
    Serial.begin(115200);
    // Set your WiFi SSID and Password
    mycontrol.begin("RobotikaBers", "robotikabers", true);
    // Default port is 81, you can change port with mycontrol.setPort(port);
    // You can set max connection with mycontrol.setMaxConnect(1); for set max connect to 1
    
    // For get IP Address
    Serial.print("IP Address : ");
    Serial.println(mycontrol.getIPAddress());

    // For set event
    mycontrol.onEvent(onEventBersControl);
}

void loop() {
    mycontrol.loop();
}

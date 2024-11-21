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
  if (data.client.ID == 0) {
    // For send message to client
    mycontrol.sendClient(data.client.ID, "Hello first client!");
    // For get IP client
    mycontrol.sendClient(data.client.ID, "your IP is " + data.client.IP);
  } else {
    // For disconnect client
    mycontrol.clientDisconnect(data.client.ID);
  }

  // For get status code
  Serial.print("statusCode : ");
  Serial.println(String(data.status.Code));

  // For get status message
  Serial.print("statusMessage : ");
  Serial.println(data.status.Message);

  if (data.status.code == 0) {
    // For get Json data
    JsonDocument jsonData = data.output.Json;
    String output = jsonData["data"]["output"]["other"].as<String>();
    Serial.print("typeJson : ");
    Serial.println(output);
  }

  // For get type char from data
  Serial.print("typeChar : ");
  Serial.println(String(data.output.Char));

  // For get type string from data
  Serial.print("typeString : ");
  Serial.println(data.output.Str);

  // For get type int from data if data is numerical
  Serial.print("typeInt : ");
  Serial.println(String(data.output.Int));

  // For get type double from data if data is numerical
  Serial.print("typeDouble : ");
  Serial.println(String(data.output.Dec));
}

void setup() {
    Serial.begin(115200);
    // Set your WiFi SSID, Password, and Mode Access Point (true) or Station (false)
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

#ifndef BERS_CONTROL_V1_H
#define BERS_CONTROL_V1_H

#if !defined(ESP32) && !defined(ESP8266)
#error "Platform not supported. Please define either ESP32 or ESP8266."
#endif

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WebSocketsServer.h>
#include <ArduinoJson.h>

struct Output {
    JsonDocument typeJson;
    const char* typeChar;
    String typeString;
    int typeInt;
    double typeDouble;
};

struct BersData {
    Output out;
    int statusCode;
    String statusMessage;
    int getClientID;
    String getIPClient;
};

typedef void (*EventCallback)(const BersData& data);

class bersControlV1 {
public:
    bersControlV1();
    void begin(const char* ssid, const char* password);
    void setPort(const int port);
    voidssetMaxConnect(const int maxc);
    void onEvent(EventCallback callback);
    void sendClient(int clientID, String message);
    void clientDisconnect(int clientID);
    void loop();
    const char* getIPAddress();
private:
    WebSocketsServer webSocket;
    EventCallback eventCallback;
    int maxC, curC;
    IPAddress localIP;
    void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif

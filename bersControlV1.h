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

struct Client {
    int id;
    string ip;
}

struct Output {
    JsonDocument json;
    char char;
    String string;
    int int;
    double double;
};

struct Status {
    int code;
    String message;
};

struct BersData {
    Client client;
    Output output;
    Status status;
};

typedef void (*EventCallback)(const BersSignal& data);

class bersControlV1 {
public:
    bersControlV1();
    void begin(const char* ssid, const char* password);
    void setPort(const int port);
    void setMaxConnect(const int maxc);
    void onEvent(EventCallback callback);
    void sendClient(int clientID, String message);
    void clientDisconnect(int clientID);
    void loop();
    const char* getIPAddress();
private:
    WebSocketsServer webSocket;
    EventCallback eventCallback;
    int maxC, curC;
    bool modeAP;
    IPAddress localIP;
    void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif

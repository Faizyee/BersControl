#include "bersControlV1.h"

bersControlV1::bersControlV1() : webSocket(81), eventCallback(nullptr) {}

void bersControlV1::begin(const char* ssid, const char* password) {
    WiFi.softAP(ssid, password);
    webSocket.begin();
    webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        handleWebSocketMessage(num, type, payload, length);
    });
}

void bersControlV1::setPort(const int port) {
    webSocket = WebSocketsServer(port);
}

void bersControlV1::setMaxConnect(const int maxc) {
    maxC = maxc;
}

void bersControlV1::onEvent(EventCallback callback) {
    eventCallback = callback;
}

void bersControlV1::sendClient(int clientID, String message) {
    webSocket.sendTXT(clientID, message);
}

void bersControlV1::clientDisconnect(int clientID) {
    webSocket.disconnect(clientID);
}

void bersControlV1::loop() {
    webSocket.loop();
}

const char* bersControlV1::getIPAddress() {
    localIP = WiFi.softAPIP();
    return localIP.toString().c_str();
}

void bersControlV1::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_CONNECTED) {
        if (maxC) {

        }
    }
    if (type == WStype_TEXT) {
        BersSignal data;
        data.statusCode = 0;
        data.statusMessage = "Json format is true, you can use .out.typeJson or .out.typeChar , .out.typeString (if want to use char or string) or .out.typeInt for Int";
        data.getClientID = num;
        data.getIPClient = webSocket.remoteIP(num).toString();
        JsonDocument doc;
        const char* message = (const char*)payload;
        DeserializationError error = deserializeJson(doc, message);
        data.out.typeChar = message;
        data.out.typeString = String(message);
        data.out.typeInt = String(message).toInt();
        data.out.typeDouble = String(message).toDouble();

        if (error) {
            data.statusCode = 1;
            data.statusMessage = "Json format is false, you can use .out.typeChar , .out.typeString (if want to use char or string) or .out.typeInt for Int";
            if (eventCallback) {
                eventCallback(data);
            }
            return;
        }

        data.out.typeJson = doc;
        if (eventCallback) {
            eventCallback(data);
        }
    }
}

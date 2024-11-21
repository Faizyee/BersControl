#include "bersControlV1.h"

bersControlV1::bersControlV1() : webSocket(81), eventCallback(nullptr) {}

void bersControlV1::begin(const char* ssid, const char* password, const bool ap) {
    modeAP = ap;
    if (modeAP) {
        WiFi.softAP(ssid, password);
    } else {
        WiFi.begin(ssid, password);
        pinMode(LED_BUILTIN, OUTPUT);
        while(WiFi.status() != WL_CONNECTED) {
           digitalWrite(LED_BUILTIN, HIGH);
           delay(200);
           digitalWrite(LED_BUILTIN, LOW);
           delay(200);
        }
        pinMode(LED_BUILTIN, INPUT);
    }
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
    if (modeAP) {
        localIP = WiFi.softAPIP();
    } else {
        localIP = WiFi.localIP();
    }
    return localIP.toString().c_str();
}

void bersControlV1::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_DISCONNECTED) {
        if (maxC > 0) {
            curC--;
        }
    }
    if (type == WStype_CONNECTED) {
        if (maxC > 0) {
            curC++;
            if (curC > maxC) {
                webSocket.disconnect(num);
            }
        }
    }
    if (type == WStype_TEXT) {
        BersSignal data;
        data.status.Code = 0;
        data.status.Message = "Json format is true, you can use .out.typeJson or .out.typeChar , .out.typeString (if want to use char or string) or .out.typeInt for Int";
        data.client.ID = (int)num;
        data.client.IP = webSocket.remoteIP(num).toString();

        const char* message = (const char*)payload;
        data.output.Char = (char*)message;
        data.output.Str = String(message);
        data.output.Int = String(message).toInt();
        data.output.Dec = String(message).toDouble();

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, message);

        if (error) {
            data.status.Code = 1;
            data.status.Message = "Json format is false, you can use .out.typeChar , .out.typeString (if want to use char or string) or .out.typeInt for Int";
            if (eventCallback) {
                eventCallback(data);
            }
            return;
        }


        data.output.Json = doc;
        if (eventCallback) {
            eventCallback(data);
        }
    }
}

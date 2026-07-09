#include "web_server.h"
#include "config.h"
#include "dashboard.h"
#include "wifi_manager.h"
#include <ArduinoJson.h>

WebServer WebServerManager::_server(80);

void WebServerManager::init() {
    _server.on("/",            _handleRoot);
    _server.on("/api/status",  _handleAPI);
    _server.onNotFound(_handleNotFound);
    _server.begin();
    Serial.println("[HTTP] Servidor iniciado en http://192.168.4.1");
}

void WebServerManager::handleClient() {
    _server.handleClient();
}

void WebServerManager::_handleRoot() {
    _server.send_P(200, "text/html; charset=utf-8", DASHBOARD_HTML);
}

void WebServerManager::_handleAPI() {
    StaticJsonDocument<256> doc;

    doc["rssi"]            = WiFiManager::getRSSI();
    doc["clients"]         = WiFiManager::getClientCount();
    doc["distance"]        = WiFiManager::getDistanceLabel();
    doc["distance_meters"] = WiFiManager::getDistanceMeters();
    doc["uptime"]          = WiFiManager::getUptime();
    doc["ssid"]            = WIFI_SSID;

    String response;
    serializeJson(doc, response);
    _server.send(200, "application/json; charset=utf-8", response);
}

void WebServerManager::_handleNotFound() {
    _server.send(404, "text/plain", "404 - Ruta no encontrada");
}

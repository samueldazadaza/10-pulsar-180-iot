#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>
#include <esp_wifi.h>

int8_t   WiFiManager::_rssi       = -100;
uint8_t  WiFiManager::_clients    = 0;
uint32_t WiFiManager::_startTime  = 0;

void WiFiManager::initAP() {
    _startTime = millis();

    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, 0, WIFI_MAX_CLIENTS);

    // Reducir consumo energético
    esp_wifi_set_ps(WIFI_PS_MIN_MODEM);          // Modem sleep entre tramas
    esp_wifi_set_max_tx_power(40);               // Potencia TX reducida (~10 dBm)

    Serial.println("[WiFi] AP iniciado: " + String(WIFI_SSID));
    Serial.println("[WiFi] IP: " + WiFi.softAPIP().toString());
}

void WiFiManager::updateRSSI() {
    _clients = WiFi.softAPgetStationNum();

    if (_clients > 0) {
        wifi_sta_list_t stationList;
        esp_wifi_ap_get_sta_list(&stationList);

        if (stationList.num > 0) {
            for (int i = 0; i < stationList.num; i++) {
                _rssi = stationList.sta[i].rssi;
            }
        }
    } else {
        _rssi = -100;
    }
}

int8_t WiFiManager::getRSSI() {
    return _rssi;
}

uint8_t WiFiManager::getClientCount() {
    return _clients;
}

const char* WiFiManager::getDistanceLabel() {
    if (_clients == 0)           return "Sin conexión";
    if (_rssi >= RSSI_MUY_CERCA) return "Muy cerca";
    if (_rssi >= RSSI_CERCA)     return "Cerca";
    if (_rssi >= RSSI_MEDIO)     return "Medio";
    if (_rssi >= RSSI_LEJOS)     return "Lejos";
    return "Sin señal";
}

uint32_t WiFiManager::getUptime() {
    return (millis() - _startTime) / 1000;
}

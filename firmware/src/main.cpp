#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "web_server.h"

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println();
    Serial.println("=== PULSAR 180 GT - Sistema de Proximidad ===");
    Serial.println("Iniciando...");

    setCpuFrequencyMhz(80);

    WiFiManager::initAP();
    WebServerManager::init();

    Serial.println("Sistema listo.");
    Serial.print("Conectate a la red WiFi: ");
    Serial.println(WIFI_SSID);
    Serial.print("Abre el navegador en: ");
    Serial.println("http://192.168.4.1");
}

void loop() {
    static uint32_t lastUpdate = 0;
    uint32_t now = millis();

    if (now - lastUpdate >= UPDATE_INTERVAL_MS) {
        lastUpdate = now;
        WiFiManager::updateRSSI();

        Serial.print("[Lectura] RSSI: ");
        Serial.print(WiFiManager::getRSSI());
        Serial.print(" dBm | Clientes: ");
        Serial.print(WiFiManager::getClientCount());
        Serial.print(" | Distancia: ");
        Serial.println(WiFiManager::getDistanceLabel());
    }

    WebServerManager::handleClient();

    delay(50);
}

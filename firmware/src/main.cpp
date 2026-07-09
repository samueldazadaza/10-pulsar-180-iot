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

    pinMode(PIN_LED_BUILTIN, OUTPUT);
    digitalWrite(PIN_LED_BUILTIN, HIGH); // Apagado por defecto

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
    static uint32_t lastBlink = 0;
    uint32_t now = millis();

    if (now - lastBlink >= LED_INTERVAL_MS) {
        lastBlink = now;
        digitalWrite(PIN_LED_BUILTIN, LOW);
    }
    if (now - lastBlink >= LED_BLINK_MS) {
        digitalWrite(PIN_LED_BUILTIN, HIGH);
    }

    if (now - lastUpdate >= UPDATE_INTERVAL_MS) {
        lastUpdate = now;
        WiFiManager::updateRSSI();

        float dist = WiFiManager::getDistanceMeters();

        Serial.print("[Lectura] RSSI: ");
        Serial.print(WiFiManager::getRSSI());
        Serial.print(" dBm | Clientes: ");
        Serial.print(WiFiManager::getClientCount());
        Serial.print(" | Distancia: ");
        if (dist < 0) {
            Serial.print("--");
        } else {
            Serial.print(dist, 1);
        }
        Serial.print(" m (");
        Serial.print(WiFiManager::getDistanceLabel());
        Serial.println(")");
    }

    WebServerManager::handleClient();

    delay(50);
}

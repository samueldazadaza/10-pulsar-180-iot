#pragma once

// ============================================================
//  CONFIGURACIÓN DEL SISTEMA PULSAR 180 IoT
//  Modifica estos valores según tus necesidades
// ============================================================

// --- RED WIFI (Access Point) ---
constexpr const char* WIFI_SSID       = "Pulsar-180";
constexpr const char* WIFI_PASSWORD   = "pulsar180";
constexpr uint8_t     WIFI_CHANNEL    = 6;
constexpr uint8_t     WIFI_MAX_CLIENTS = 4;

// --- UMBRALES DE RSSI PARA DISTANCIA (dBm) ---
// Cuanto más cerca de 0, mejor señal
constexpr int8_t RSSI_MUY_CERCA  = -50;   // < -50  → "Muy cerca"
constexpr int8_t RSSI_CERCA      = -65;   // -50 a -65 → "Cerca"
constexpr int8_t RSSI_MEDIO      = -75;   // -65 a -75 → "Medio"
constexpr int8_t RSSI_LEJOS      = -85;   // -75 a -85 → "Lejos"
                                          // < -85  → "Sin señal"

// --- TEMPORIZACIÓN ---
constexpr uint32_t UPDATE_INTERVAL_MS = 10000; // 10 seg entre lecturas

// --- PINES ESP32 ---
// (Reservados para futuros sensores — v2+)
constexpr uint8_t PIN_FUEL_SENSOR  = 34;  // ADC1_CH6 — sensor gasolina
constexpr uint8_t PIN_RELAY_ALARM  = 32;  // Relé alarma
constexpr uint8_t PIN_BUZZER       = 33;  // Buzzer / sirena
constexpr uint8_t PIN_VIBRATION    = 35;  // Sensor de vibración

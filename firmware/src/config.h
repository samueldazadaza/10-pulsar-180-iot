#pragma once

// ============================================================
//  CONFIGURACIÓN DEL SISTEMA PULSAR 180 IoT
//  Modifica estos valores según tus necesidades
// ============================================================

// --- RED WIFI (Access Point) ---
constexpr const char* WIFI_SSID       = "180gtpul";
constexpr const char* WIFI_PASSWORD   = "bogota2066";
constexpr uint8_t     WIFI_CHANNEL    = 6;
constexpr uint8_t     WIFI_MAX_CLIENTS = 4;

// --- UMBRALES DE RSSI PARA DISTANCIA (dBm) ---
// Cuanto más cerca de 0, mejor señal
constexpr int8_t RSSI_MUY_CERCA  = -50;   // < -50  → "Muy cerca"
constexpr int8_t RSSI_CERCA      = -65;   // -50 a -65 → "Cerca"
constexpr int8_t RSSI_MEDIO      = -75;   // -65 a -75 → "Medio"
constexpr int8_t RSSI_LEJOS      = -85;   // -75 a -85 → "Lejos"
                                          // < -85  → "Sin señal"

// --- PARÁMETROS DE DISTANCIA ---
// Fórmula: distancia = 10 ^ ((RSSI - RSSI_AT_1M) / (-10 * PATH_LOSS_N))
constexpr float RSSI_AT_1M   = -40.0f;   // RSSI medido experimentalmente a 1 metro
constexpr float PATH_LOSS_N  = 2.5f;      // Factor de atenuación (2.0 espacio libre, 2.5-4.0 exterior)

// --- TEMPORIZACIÓN ---
constexpr uint32_t UPDATE_INTERVAL_MS = 10000; // 10 seg entre lecturas

// --- PINES ESP32 ---
// (Reservados para futuros sensores — v2+)
constexpr uint8_t PIN_FUEL_SENSOR  = 34;  // ADC1_CH6 — sensor gasolina
constexpr uint8_t PIN_RELAY_ALARM  = 32;  // Relé alarma
constexpr uint8_t PIN_BUZZER       = 33;  // Buzzer / sirena
constexpr uint8_t PIN_VIBRATION    = 35;  // Sensor de vibración

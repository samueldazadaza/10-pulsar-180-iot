#pragma once
#include <Arduino.h>

class WiFiManager {
public:
    static void initAP();
    static void updateRSSI();

    static int8_t    getRSSI();
    static uint8_t   getClientCount();
    static const char* getDistanceLabel();
    static uint32_t  getUptime();

private:
    static int8_t    _rssi;
    static uint8_t   _clients;
    static uint32_t  _startTime;
};

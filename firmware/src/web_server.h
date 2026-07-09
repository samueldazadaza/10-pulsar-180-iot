#pragma once
#include <WebServer.h>

class WebServerManager {
public:
    static void init();
    static void handleClient();

private:
    static WebServer _server;
    static void _handleRoot();
    static void _handleAPI();
    static void _handleNotFound();
};

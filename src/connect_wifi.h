#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "settings.h"

namespace wifi
{
    void connect()
    {
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected");
    }
}
/*
 CrazySafe - "course.h" - part of the code for get course of USD and EUR in RUB.
 Copyright (C) 2025 CrazyMax

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <Arduino.h>
#include <StringN.h>
#include <GSON.h>
#include <WiFiClientSecure.h>
#include <GyverHTTP.h>

#include "display.h"

WiFiClientSecure client;
ghttp::Client http(client, "www.cbr-xml-daily.ru", 443);

// Currency parser
namespace course_parser
{
    struct
    {
        float USD = 79.55;
        float EUR = 93.35;
    } courses;

    void get()
    {
        Serial.println("get");
        client.setInsecure();
        http.request("/daily_json.js");
        http.onResponse([](ghttp::Client::Response &resp) {
            String s = resp.body().readString();
            gson::Parser json;
            if (json.parse(s))
            {
                // json.stringify(Serial);
                courses.USD = json["Valute"]["USD"]["Value"].toFloat();
                courses.EUR = json["Valute"]["EUR"]["Value"].toFloat();
                Serial.println(courses.USD);
                Serial.println(courses.EUR);
            }
            else
            {
              Serial.println("Parse error");
            }
        });
    }

    void tick()
    {
        static long timer = 0;
        if (millis() - timer > 600000)
        {
            timer = millis();
            course_parser::get();
        }
        http.tick();
    }
}
/*
 CrazySafe - is an simulator of safe on ESP32 with control via Telegram-bot, and balance output to an LED matrix.
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

#include <Arduino.h>

#include "connect_wifi.h"
#include "display.h"
#include "course.h"
#include "safe.h"
#include "tg.h"

void setup()
{
    Serial.begin(115200);
    Serial.println();

    display::begin();
    wifi::connect();
    tg::begin();
    course_parser::get();
}

void loop()
{
    tg::tick();
    display::tick();
    course_parser::tick();
}
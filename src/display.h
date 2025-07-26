/*
 CrazySafe - "display.h" - part of the code for working with LED matrix.
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
#include "CrazyMatrix.h"

#include "safe.h"

#include "settings.h"

namespace display
{
    runningMatrix<LED_WIDTH, LED_HEIGHT, LED_PIN> led;

    String text = "";

    void begin()
    {
        led.color = CRGB::Green;
        led.setSpeed(13);
        led.start();
        update();
    }

    void tick()
    {
        led.tick();
    }

    void update()
    {
        text = safe::getValueFormatted1line().c_str();
        Serial.println(text);
        led.setText(text);
        led.start();
    }
}
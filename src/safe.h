/*
 CrazySafe - "safe.h" - part of the code for simulating a safe.
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
#include <StringUtils.h>
#include <StringN.h>
#include "dec2places.h"

namespace display
{
  void update();
}
#include "course.h"

namespace safe
{
  dec2places value;

  // Пополнить
  void replenish(dec2places _value)
  {
    value += _value;
    display::update();
  }

  // Снять
  void withdraw(dec2places _value)
  {
    value -= _value;
    display::update();
  }

  // Снять всё
  void withdraw_all()
  {
    value = 0;
    display::update();
  }

  su::Text getValueFormatted()
  {
    return su::Text(String(String128("Ваш баланс:\n") +
                           value.toString().c_str() + " рублей\n" +
                           (value / course_parser::courses.USD).toString().c_str() + " долларов\n" +
                           (value / course_parser::courses.EUR).toString().c_str() + " евро"))
        .decodeUnicode();
  }

  su::Text getValueFormatted1line()
  {
    return su::Text(String(String128("Ваш баланс: ") +
                           value.toString().c_str() + " рублей " +
                           (value / course_parser::courses.USD).toString().c_str() + " долларов " +
                           (value / course_parser::courses.EUR).toString().c_str() + " евро"))
        .decodeUnicode();
  }
}
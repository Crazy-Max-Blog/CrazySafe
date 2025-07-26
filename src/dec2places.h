/*
 CrazySafe - "dec2places.h" - library for working with large numbers with two digits after the decimal point
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

/*
  64-битный тип данных для хранения чисел с двумя знаками после запятой
  Максимальное число - 18 цифр до запятой, точнее
  184 467 440 737 095 516.15 ~ 184.4 квадриллиона
  При использовании умножения и/или деления максимальное число - 16 цифр до запятой, точнее
  1 844 674 407 370 955.15 ~ 1.8 квадриллиона, отсюда максимальное значение будет занимать 62 байта
  (Apple, самая дорогая компания в мире по информации на 2:22 (Саратов (UTC+3)), 26.07.2025,
  стоит меньше (3.548 триллионов доллраров - 282.2434 триллиона (282 243 400 000 000) рублей (15 цифр))
  В расчётах выше использовалась короткая шкала наименования чисел
*/

#pragma once

#include <Arduino.h>
#include <StringN.h>
#include <StringUtils.h>

class dec2places
{
public:
    void set(su::Value value)
    {
        byte c = value.count('.') - 1;
        if (c == 0)
            _value = su::strToInt<uint64_t>(value.c_str()) * 100;
        else if (c == 1)
            _value = su::strToInt<uint64_t>(value.getSub(0, '.').c_str()) * 100 + value.substring(value.indexOf('.') + 1, min(uint16_t(value.indexOf('.') + 3), value.length())).toInt();
    }

    dec2places() {}
    dec2places(Value value) { set(value); }

    dec2places &operator=(const su::Value &v)
    {
        set(v);
        return *this;
    }

    su::Text toString()
    {
        StringN<21> _val = "";
        su::Value value1 = _value;
        for (byte i = 0; i < byte(max(int(3 - value1.length()), 0)); i++)
            _val += "0";
        _val += _value;
        su::Value value = _val.c_str();
        return su::Text(StringN<21>(value.substring(0, value.length() - 2).c_str()) + "." + value.substring(value.length() - 2, value.length()).c_str());
    }

    operator su::Text()
    {
        return toString();
    }

    // operator +
    dec2places &operator+=(const su::Value &v)
    {
        _value += dec2places(v)._value;
        return *this;
    }

    dec2places &operator+=(const dec2places &v)
    {
        _value += v._value;
        return *this;
    }

    dec2places &operator+(const su::Value &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o += v;
        return *o;
    }

    dec2places &operator+(const dec2places &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o += v;
        return *o;
    }

    // operator -
    dec2places &operator-=(const su::Value &v)
    {
        _value -= dec2places(v)._value;
        return *this;
    }

    dec2places &operator-=(const dec2places &v)
    {
        _value -= v._value;
        return *this;
    }

    dec2places &operator-(const su::Value &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o -= v;
        return *o;
    }

    dec2places &operator-(const dec2places &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o -= v;
        return *o;
    }

    // operator *
    dec2places &operator*=(const su::Value &v)
    {
        _value = _value * uint64_t(v.toFloat() * 100) / 100;
        return *this;
    }

    dec2places &operator*=(const dec2places &v)
    {
        _value = _value * uint64_t(v._value) / 100;
        return *this;
    }

    dec2places &operator*(const su::Value &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o *= v;
        return *o;
    }

    dec2places &operator*(const dec2places &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o *= v;
        return *o;
    }

    // operator /
    dec2places &operator/=(const su::Value &v)
    {
        _value = _value * 100 / dec2places(v)._value;
        return *this;
    }

    dec2places &operator/=(const dec2places &v)
    {
        _value = _value * 100 / v._value;
        return *this;
    }

    dec2places &operator/(const su::Value &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o /= v;
        return *o;
    }

    dec2places &operator/(const dec2places &v)
    {
        dec2places *o = new dec2places();
        o->_value = _value;
        *o /= v;
        return *o;
    }

    // private:
    uint64_t _value = 0;
};
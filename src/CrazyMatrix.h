/*
 CrazySafe - "CrazyMatrix.h" - library for an LED matrix and a running line on it.
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

#include <FastLED.h>
#include <GyverGFX.h>
#include <RunningGFX.h>

class Buffer
{
public:
  Buffer(byte width, byte height) : Buffer(new uint8_t[width * height / 8], width, height) {}

  Buffer(uint8_t *buf, byte width, byte height)
  {
    _buf = buf;
    Buffer::width = width;
    Buffer::height = height;
    for (int i = 0; i < width * height; i++)
      set(i, 0);
  }

  int xy(byte x, byte y)
  {
    return x + y * width;
  }

  bool get(int index)
  {
    return bitRead(_buf[index >> 3], (index & 0b111));
  }

  bool get(byte x, byte y)
  {
    return get(xy(x, y));
  }

  void set(int index, bool fill)
  {
    bitWrite(_buf[index >> 3], (index & 0b111), fill);
  }

  void set(byte x, byte y, bool fill)
  {
    set(xy(x, y), fill);
  }

  uint8_t *_buf;
  byte width, height;
};

template <int WIDTH, int HEIGHT, int pin>
class Matrix : public GyverGFX
{
public:
  Matrix() : GyverGFX(WIDTH, HEIGHT), buf(WIDTH, HEIGHT)
  {
    FastLED.addLeds<WS2812, pin, GRB>(leds, WIDTH * HEIGHT);
    FastLED.setBrightness(50);
  }

  void dot(int x, int y, uint8_t fill = 1)
  {
    buf.set(x, y, fill);
  }

  void set(byte x, byte y, CRGB c)
  {
    leds[x * buf.height + ((x % 2) ? 7 - y : y)].setRGB(c.r, c.g, c.b);
  }

  void update()
  {
    for (int x = 0; x < WIDTH; x++)
      for (int y = 0; y < HEIGHT; y++)
        set(x, y, buf.get(x, y) ? color : CRGB::Black);
    FastLED.show();
  }

  Buffer buf;
  CRGB color = CRGB::Green;
  CRGB leds[WIDTH * HEIGHT];
};

template <int WIDTH, int HEIGHT, int pin>
class runningMatrix : public Matrix<WIDTH, HEIGHT, pin>, public RunningGFX
{
public:
  runningMatrix() : RunningGFX(this) { setWindow(0, WIDTH - 1, 0); }
};

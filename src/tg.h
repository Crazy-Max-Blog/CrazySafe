/*
 CrazySafe - "tg.h" - part of the code for working with Telegram Bot.
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
#include <FastBot2.h>
#include "dec2places.h"

#include "settings.h"

namespace tg
{
  enum states
  {
    FREE,
    REPLENISH,
    WITHDRAW
  } state;

  FastBot2 bot;

  su::Text helloText =
      "Здрасте\\. Я бот для управления сейфом\\.\n"
      "*Ограничения*\n"
      "Максимальная сумма в сейфе \\- 1 844 674 407 370 955\\.15 \\~ 1\\.8 квадриллиона рублей\\.";

  void update(fb::Update &u);
  void begin()
  {
    bot.attachUpdate(update);
    bot.setToken(F(BOT_TOKEN));
    bot.setPollMode(fb::Poll::Long, 60000);
  }

  void tick() { bot.tick(); }

  void start(su::Text chat_id)
  {
    fb::Message msg(helloText, chat_id);
    msg.mode = fb::Message::Mode::MarkdownV2;

    fb::Menu menu;
    menu.addButton("Пополнить").addButton("Снять").addButton("Снять всё").newRow();
    menu.addButton("Узнать баланс");
    msg.setMenu(menu);

    bot.sendMessage(msg);
  }

  void replenish(su::Text chat_id)
  {
    bot.sendMessage(fb::Message("Укажите значение для пополнения (в рублях)", chat_id));
    state = states::REPLENISH;
  }

  void withdraw(su::Text chat_id)
  {
    bot.sendMessage(fb::Message("Укажите значение для снятия (в рублях)", chat_id));
    state = states::WITHDRAW;
  }

  void withdraw_all(su::Text chat_id)
  {
    safe::withdraw_all();
    bot.sendMessage(fb::Message("Ваш баланс: 0 рублей.", chat_id));
  }

  void getBalance(su::Text chat_id)
  {
    Serial.println(safe::getValueFormatted());
    bot.sendMessage(fb::Message(safe::getValueFormatted(), chat_id));
  }

  void update_on_state(su::Text chat_id, su::Text msg)
  {
    if (msg.toInt64() == 0 && msg != "0")
    {
      bot.sendMessage(fb::Message("В значении есть буквы, но оно должно состоять только из цифр.", chat_id));
      state = states::FREE;
      return;
    }
    switch (state)
    {
    case states::REPLENISH:
      bot.sendMessage(fb::Message(String(StringN<50>("Пополнение на ") + msg.c_str() + " RUB"), chat_id));
      safe::replenish((dec2places)msg);
      state = states::FREE;
      break;

    case states::WITHDRAW:
      bot.sendMessage(fb::Message(String(StringN<44>("Снятие на ") + msg.c_str() + " RUB"), chat_id));
      safe::withdraw((dec2places)msg);
      state = states::FREE;
      break;

    default:
      break;
    }
  }

  void update(fb::Update &u)
  {
    if (u.isMessage())
    {
      su::Text chat_id = u.message().chat().id();

      Serial.println(u.message().text());
      if (state != states::FREE)
      {
        update_on_state(chat_id, u.message().text());
        return;
      }
      switch (u.message().text().hash())
      {
      case "/start"_h:
        start(chat_id);
        break;

      case "Пополнить"_h:
        replenish(chat_id);
        break;

      case "Снять"_h:
        withdraw(chat_id);
        break;

      case "Снять всё"_h:
        withdraw_all(chat_id);
        break;

      case "Узнать баланс"_h:
        getBalance(chat_id);
        break;

      default:
        break;
      }
    }
  }
};
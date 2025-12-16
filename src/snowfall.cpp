/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            snowfall.h
 *
 *  Sat Dec 13 12:00:00 CEST 2025
 *  Copyright 2025 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of Leddy.
 *
 *  Leddy is free software; you can redistribute it and/or modify
 led *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Leddy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Leddy; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "snowfall.h"
#include "globaldefs.h"

#include <QPainter>
#include <QRandomGenerator>

constexpr std::size_t TABLE_SIZE = 32;
constexpr double PI = 3.14159265358979323846;

consteval auto make_sine_table() {
  std::array<double, TABLE_SIZE> table{};

  for (std::size_t i = 0; i < TABLE_SIZE; ++i) {
    table[i] = std::sin(2.0 * PI * i / TABLE_SIZE);
  }

  return table;
}

constexpr auto sine_table = make_sine_table();

Snowfall::Snowfall(Settings &settings,
                   const QString &duration,
                   const QString &background,
                   const QString &bgColor,
                   const QString &fps)
  : Scene(settings, SCENE::SNOWFALL, duration, background, bgColor)
{
  frameTimer.setInterval(1000 / (fps.toInt() <= 0 || fps.toInt() > 120?5:fps.toInt()));
}

void Snowfall::start()
{
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&ground);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    ground.fill(bgColor);
  }

  nextFrame();
}

void Snowfall::nextFrame()
{
  for(quint32 a = 0; a < QRandomGenerator::global()->generate() % 3; ++a) {
    Snowflake sf;
    sf.x = QRandomGenerator::global()->generate() % settings.width;
    snowFlakes.append(sf);
  }

  buffer = ground;

  // Progress sine wave for 'wind'
  //sineIdx += (QRandomGenerator::global()->generate() % 2) + 1;
  sineIdx++;
  if(sineIdx >= TABLE_SIZE) {
    sineIdx = 0;
  }

  for(int a = snowFlakes.length() - 1; a >= 0; --a) {
    buffer.setPixelColor(snowFlakes[a].x, snowFlakes[a].y, fgColor);

    // Don't move downwards vertically always
    if(QRandomGenerator::global()->generate() % 10 > 1) {
      snowFlakes[a].y += 1.0;
    }

    // Random 1 pixel horizontal movement
    if(QRandomGenerator::global()->generate() % 10 == 0) {
      if(QRandomGenerator::global()->generate() % 2 == 0) {
        snowFlakes[a].x -= 1.0;
      } else {
        snowFlakes[a].x += 1.0;
      }
    }
    
    // Apply sine 'wind' movement
    snowFlakes[a].x += sine_table[sineIdx] * (settings.width / 128.0);

    // Restrict inside boundaries
    if(snowFlakes[a].x < 0) {
      snowFlakes[a].x = 0;
    }
    if(snowFlakes[a].x > settings.width - 1) {
      snowFlakes[a].x = settings.width - 1;
    }
    if(snowFlakes[a].y > settings.height - 1) {
      snowFlakes[a].y = settings.height - 1;
    }
    
    // Settling checks
    if(ground.pixelColor(snowFlakes[a].x, snowFlakes[a].y) != bgColor) { // Check if non-air is below snowflake
      int clearAt = 0;
      if(snowFlakes[a].x - 1 >= 0 && ground.pixelColor(snowFlakes[a].x - 1, snowFlakes[a].y) == bgColor) { // Are we clear to the left?
        clearAt = 1; // Left side clear
      }
      if(snowFlakes[a].x + 1 < settings.width && ground.pixelColor(snowFlakes[a].x + 1, snowFlakes[a].y) == bgColor) { // Are we clear to the right?
        if(clearAt == 1) {
          clearAt = 3; // Both sides clear
        } else {
          clearAt = 2; // Only right side clear
        }
      }

      if(clearAt == 1) {
        snowFlakes[a].x = std::clamp((int)snowFlakes[a].x - 1, 0, settings.width - 1);
      } else if(clearAt == 2) {
        snowFlakes[a].x = std::clamp((int)snowFlakes[a].x + 1, 0, settings.width - 1);
      } else if(clearAt == 3) {
        if(QRandomGenerator::global()->generate() % 2) {
          snowFlakes[a].x = std::clamp((int)snowFlakes[a].x + 1, 0, settings.width - 1);
        } else {
          snowFlakes[a].x = std::clamp((int)snowFlakes[a].x - 1, 0, settings.width - 1);
        }
      } else if(snowFlakes[a].y - 1 > 0) {
        ground.setPixelColor(snowFlakes[a].x, snowFlakes[a].y - 1, fgColor);
        snowFlakes.removeAt(a);
      }
    } else if(snowFlakes[a].y >= settings.height - 1) { // Landing at the very bottom
      ground.setPixelColor(snowFlakes[a].x, snowFlakes[a].y, fgColor);
      snowFlakes.removeAt(a);
    }
  }

  /*
  if(!background.isNull()) {
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    buffer.fill(bgColor);
  }

  nextGen.fill(Qt::transparent);
  for(int y = 0; y < prevGen.height(); ++y) {
    for(int x = 0; x < prevGen.width(); ++x) {
      int adjacentAlive = 0;
      for(int b = -1; b <= 1; ++b) {
        for(int a = -1; a <= 1; ++a) {
          if(a == 0 && b == 0) {
            continue;
          }
          if(prevGen.pixelColor([&]() -> int {int c = x + a; if(c > settings.width - 1) return 0; if(c < 0) return settings.width - 1; return c;}(),
                                [&]() -> int {int c = y + b; if(c > settings.height - 1) return 0; if(c < 0) return settings.height - 1; return c;}()) == QColor(Qt::white)) {
            //if(prevGen.pixelColor(finalX, finalY) == QColor(Qt::white)) {
            adjacentAlive++;
          }
        }
      }
      if(prevGen.pixelColor(x, y) == QColor(Qt::white)) { // Cell is currently alive
        if(adjacentAlive == 2 || adjacentAlive == 3) {
          nextGen.setPixelColor(x, y, QColor(Qt::white));
          buffer.setPixelColor(x, y, fgColor);
        }
      } else { // Cell is currently dead
        if(adjacentAlive == 3) {
          nextGen.setPixelColor(x, y, QColor(Qt::white));
          buffer.setPixelColor(x, y, fgColor);
        }
      }
    }
  }
  
  currentFrame++;
  if(currentFrame >= 100 || prevGen.createAlphaMask() == nextGen.createAlphaMask()) {
    currentFrame = 0;
    start();
    return;
  }

  prevGen = nextGen;

  */
  frameTimer.start();
}

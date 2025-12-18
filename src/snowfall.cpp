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

  sfTotal = 0;

  nextFrame();
}

void Snowfall::nextFrame()
{
  for(quint32 a = 0; a < QRandomGenerator::global()->generate() % 4; ++a) {
    Snowflake sf;
    sf.x = QRandomGenerator::global()->generate() % settings.width;
    snowflakes.append(sf);
    sfTotal++;
  }

  buffer = ground;

  // Progress sine wave for 'wind'
  sineIdx++;
  if(sineIdx >= TABLE_SIZE) {
    sineIdx = 0;
  }

  double windDelta = sine_table[sineIdx] * (settings.width / 128.0);

  for(int a = snowflakes.length() - 1; a >= 0; --a) {
    // Move snowflake downwards but not always (but always set to 0 if it's a new snowflake)
    if(QRandomGenerator::global()->generate() % 10 > 1) {
      snowflakes[a].y += 1;
    } else if(snowflakes[a].y == -1) {
      snowflakes[a].y = 0;
    }

    // We're at the bottom so settle it and move on
    if(snowflakes[a].y >= settings.height - 1) {
      buffer.setPixelColor((int)snowflakes[a].x, snowflakes[a].y, fgColor);
      ground.setPixelColor((int)snowflakes[a].x, snowflakes[a].y, fgColor);
      snowflakes.removeAt(a);
      continue;
    }

    // Find expected x delta movement
    double xDelta = 0.0;
    
    // Find sine 'wind' horizontal delta movement
    xDelta += windDelta;

    // Random -1.0 to 1.0 pixel horizontal delta movement 1/10th of the time
    if(QRandomGenerator::global()->generate() % 10 == 0) {
      xDelta += ((QRandomGenerator::global()->generate() % 21) * 0.1) - 1.0;
    }

    // Check for walls / solids horizontally before moving x
    int xDeltaInt = (int)fabs(xDelta);
    bool wallHit = false;
    if(xDelta < 0.0) {
      for(int b = 0; b <= xDeltaInt; ++b) {
        //buffer.setPixelColor((int)snowflakes[a].x - (b + 1), snowflakes[a].y, QColor(Qt::green)); // For debugging
        if((int)snowflakes[a].x - (b + 1) >= 0 && ground.pixelColor((int)snowflakes[a].x - (b + 1), snowflakes[a].y) != bgColor) {
          snowflakes[a].x -= (double)b;
          wallHit = true;
          break;
        }
      }
    } else if(xDelta > 0.0) {
      for(int b = 0; b <= xDeltaInt; ++b) {
        //buffer.setPixelColor((int)snowflakes[a].x + (b + 1), snowflakes[a].y, QColor(Qt::green)); // For debugging
        if((int)snowflakes[a].x + (b + 1) <= settings.width - 1 && ground.pixelColor((int)snowflakes[a].x + (b + 1), snowflakes[a].y) != bgColor) {
          snowflakes[a].x += (double)b;
          wallHit = true;
          break;
        }
      }
    }

    if(!wallHit) {
      snowflakes[a].x += xDelta;
    }

    // Restrict inside boundaries
    if((int)snowflakes[a].x < 0) {
      snowflakes[a].x = 0.0;
    }
    if((int)snowflakes[a].x > settings.width - 1) {
      snowflakes[a].x = (double)settings.width - 1.0;
    }

    buffer.setPixelColor((int)snowflakes[a].x, snowflakes[a].y, fgColor);

    // Now check for ground underneath snowflake
    if(ground.pixelColor((int)snowflakes[a].x, snowflakes[a].y + 1) != bgColor) {
      int freeDir = 0; // No directions free
      if((int)snowflakes[a].x - 1 >= 0 && ground.pixelColor((int)snowflakes[a].x - 1, snowflakes[a].y + 1) == bgColor) {
        freeDir = 1; // Left free
      }
      if((int)snowflakes[a].x + 1 <= settings.width - 1 && ground.pixelColor((int)snowflakes[a].x + 1, snowflakes[a].y + 1) == bgColor) {
        if(freeDir == 1) {
          freeDir = 3; // Both left and right free
        } else {
          freeDir = 2; // Right free
        }
      }
      bool roll = QRandomGenerator::global()->generate() % 20 > 1;
      if(freeDir == 1 && roll) {
        snowflakes[a].x -= 1.0;
      } else if(freeDir == 2 && roll) {
        snowflakes[a].x += 1.0;
      } else if(freeDir == 3) {
        flipper = !flipper;
        if(flipper) {
          snowflakes[a].x -= 1.0;
        } else {
          snowflakes[a].x += 1.0;
        }
      } else {
        // Settle snowflake where it is
        ground.setPixelColor((int)snowflakes[a].x, snowflakes[a].y, fgColor);
        snowflakes.removeAt(a);
      }
    }
  }

  if(sfTotal >= (settings.width * settings.height) / 5) {
    running = false;
    emit sceneEnded();
  }

  frameTimer.start();
}

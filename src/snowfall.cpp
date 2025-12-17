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
    snowflakes.append(sf);
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
      ground.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);
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
    int xDeltaInt = fabs(xDelta);
    bool wallHit = false;
    if(xDelta < 0.0) {
      for(int b = 0; b <= xDeltaInt; ++b) {
        if(snowflakes[a].x - (b + 1) >= 0 && ground.pixelColor(snowflakes[a].x - (b + 1), snowflakes[a].y) != bgColor) {
          snowflakes[a].x -= b;
          wallHit = true;
        }
      }
    } else if(xDelta > 0.0) {
      for(int b = 0; b <= xDeltaInt; ++b) {
        if(snowflakes[a].x + (b + 1) <= settings.width - 1 && ground.pixelColor(snowflakes[a].x + (b + 1), snowflakes[a].y) != bgColor) {
          snowflakes[a].x += b;
          wallHit = true;
        }
      }
    }

    if(!wallHit) {
      snowflakes[a].x += xDelta;
    }

    // Restrict inside boundaries
    if(snowflakes[a].x < 0) {
      snowflakes[a].x = 0;
    }
    if(snowflakes[a].x > settings.width - 1) {
      snowflakes[a].x = settings.width - 1;
    }

    buffer.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);

    // Now check for ground underneath snowflake
    if(ground.pixelColor(snowflakes[a].x, snowflakes[a].y + 1) != bgColor) {
      int freeDir = 0;
      if(snowflakes[a].x - 1.0 >= 0.0 && ground.pixelColor(snowflakes[a].x - 1.0, snowflakes[a].y + 1) == bgColor) {
        freeDir = 1; // Left free
      } else if(snowflakes[a].x + 1.0 <= (double)settings.width - 1 && ground.pixelColor(snowflakes[a].x + 1.0, snowflakes[a].y + 1) == bgColor) {
        if(freeDir == 1) {
          freeDir = 3; // Both left and right free
        } else {
          freeDir = 2; // Right free
        }
      } else {
        ground.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);
        snowflakes.removeAt(a);
      }
      if(freeDir == 1) {
        snowflakes[a].x -= 1.0;
      } else if(freeDir == 2) {
        snowflakes[a].x += 1.0;
      } else if(freeDir == 3) {
        if(QRandomGenerator::global()->generate() % 2) {
          snowflakes[a].x -= 1.0;
        } else {
          snowflakes[a].x += 1.0;
        }
      }
    }
    /*

    if(snowflakes[a].y > settings.height - 1) { // We're at the bottom
      snowflakes[a].y = settings.height - 1.0;
      ground.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);
      snowflakes.removeAt(a);
    } else {
      // Now check for ground underneath snowflake
      if(ground.pixelColor(snowflakes[a].x, snowflakes[a].y + 1) != bgColor) {
        if(windDelta > 0.0) {
          if(ground.pixelColor(snowflakes[a].x + 1.0, snowflakes[a].y + 1) == bgColor) {
            snowflakes[a].x += 1.0;
          }
        } else if(windDelta < 0.0) {
          if(ground.pixelColor(snowflakes[a].x - 1.0, snowflakes[a].y + 1) == bgColor) {
            snowflakes[a].x -= 1.0;
          }
        } else {
          ground.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);
          snowflakes.removeAt(a);
        }
      }
    }
    */
  }

  frameTimer.start();
}

/*
void Snowfall::nextFrame()
{
  for(quint32 a = 0; a < QRandomGenerator::global()->generate() % 3; ++a) {
    Snowflake sf;
    sf.x = QRandomGenerator::global()->generate() % settings.width;
    snowflakes.append(sf);
    //sfTotal++;
  }

  buffer = ground;

  // Progress sine wave for 'wind'
  //sineIdx += (QRandomGenerator::global()->generate() % 2) + 1;
  sineIdx++;
  if(sineIdx >= TABLE_SIZE) {
    sineIdx = 0;
  }

  for(int a = snowflakes.length() - 1; a >= 0; --a) {
    // Draw snowflake to this-frame-only buffer
    buffer.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);

    // Don't move downwards vertically always
    if(QRandomGenerator::global()->generate() % 10 > 1) {
      snowflakes[a].y += 1.0;
    }

    // Random 1 pixel horizontal movement
    if(QRandomGenerator::global()->generate() % 10 == 0) {
      if(QRandomGenerator::global()->generate() % 2 == 0) {
        snowflakes[a].x -= 1.0;
      } else {
        snowflakes[a].x += 1.0;
      }
    }

    // Apply sine 'wind' movement
    snowflakes[a].x += sine_table[sineIdx] * (settings.width / 128.0);

    // Restrict inside boundaries
    if(snowflakes[a].x < 0) {
      snowflakes[a].x = 0;
    }
    if(snowflakes[a].x > settings.width - 1) {
      snowflakes[a].x = settings.width - 1;
    }
    if(snowflakes[a].y > settings.height - 1) {
      snowflakes[a].y = settings.height - 1;
    }
    
    // Settling checks
    if(ground.pixelColor(snowflakes[a].x, snowflakes[a].y) != bgColor) { // Check if non-air is below snowflake
      int clearAt = 0;
      if(snowflakes[a].x - 1 >= 0 && ground.pixelColor(snowflakes[a].x - 1, snowflakes[a].y) == bgColor) { // Are we clear to the left?
        clearAt = 1; // Left side clear
      }
      if(snowflakes[a].x + 1 < settings.width && ground.pixelColor(snowflakes[a].x + 1, snowflakes[a].y) == bgColor) { // Are we clear to the right?
        if(clearAt == 1) {
          clearAt = 3; // Both sides clear
        } else {
          clearAt = 2; // Only right side clear
        }
      }

      if(clearAt == 1) {
        snowflakes[a].x = std::clamp((int)snowflakes[a].x - 1, 0, settings.width - 1);
      } else if(clearAt == 2) {
        snowflakes[a].x = std::clamp((int)snowflakes[a].x + 1, 0, settings.width - 1);
      } else if(clearAt == 3) {
        if(QRandomGenerator::global()->generate() % 2) {
          snowflakes[a].x = std::clamp((int)snowflakes[a].x + 1, 0, settings.width - 1);
        } else {
          snowflakes[a].x = std::clamp((int)snowflakes[a].x - 1, 0, settings.width - 1);
        }
      } else if(snowflakes[a].y - 1 > 0) {
        ground.setPixelColor(snowflakes[a].x, snowflakes[a].y - 1, fgColor);
        snowflakes.removeAt(a);
      }
    } else if(snowflakes[a].y >= settings.height - 1) { // Landing at the very bottom
      ground.setPixelColor(snowflakes[a].x, snowflakes[a].y, fgColor);
      snowflakes.removeAt(a);
    }
  }
  
  frameTimer.start();
}
*/

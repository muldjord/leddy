/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            gameoflife.h
 *
 *  Mon Feb 8 12:00:00 CEST 2021
 *  Copyright 2021 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of Leddy.
 *
 *  Leddy is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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

#include "gameoflife.h"
#include "globaldefs.h"

#include <QPainter>
#include <QRandomGenerator>

GameOfLife::GameOfLife(Settings &settings,
                       const QString &duration,
                       const QString &background,
                       const QString &bgColor,
                       const QString &fgColor,
                       const QString &fps)
: Scene(settings, SCENE::GAMEOFLIFE, duration, background, bgColor, fgColor)
{
  frameTimer.setInterval(1000 / (fps.toInt() <= 0 || fps.toInt() > 120?5:fps.toInt()));
}

void GameOfLife::start()
{
  if(bgColorType == COLOR::RANDOM) {
    bgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                              255,
                              50);
  }
  if(fgColorType == COLOR::RANDOM) {
    fgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                              255,
                              200);
  } else if(fgColorType == COLOR::COMPLIMENTARY) {
    fgColor = QColor::fromHsl((bgColor.hslHue() + 180 > 359?180 + bgColor.hslHue() - 359:bgColor.hslHue() + 180),
                              bgColor.hslSaturation(),
                              200).toRgb();
  }
  
  prevGen.fill(Qt::transparent);
  nextGen.fill(Qt::transparent);
  quint32 pixelChance = QRandomGenerator::global()->generate() % 100;
  for(int y = 0; y < prevGen.height(); ++y) {
    for(int x = 0; x < prevGen.width(); ++x) {
      if((QRandomGenerator::global()->generate() % 100) > pixelChance) {
        prevGen.setPixelColor(x, y, QColor(Qt::white));
      }
    }
  }
  nextFrame();
}

void GameOfLife::nextFrame()
{
  QPainter painter;
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

  frameTimer.start();
}

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            runcommand.h
 *
 *  Mon Feb 15 12:00:00 CEST 2021
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

#include "runcommand.h"

#include <cmath>
#include <QPainter>
#include <QRandomGenerator>
#include <QRegularExpression>

RunCommand::RunCommand(Settings &settings,
                       const QString &background,
                       const QString &bgColor,
                       const QString &font,
                       const QString &fontColor,
                       const QString &fontShadowColor,
                       const QString &textY,
                       const QString &waveHeight,
                       const QString &waveLength,
                       const QString &command,
                       const QString &interval,
                       const QString &fps)
  : Scene(settings, SCENE::RUNCOMMAND, QString(), background, bgColor, fontColor), command(command)
{
  if(!font.isNull() && settings.fonts.contains(font)) {
    this->font = font;
  }
  // Font color is handled with fgColor
  if(!fontShadowColor.isNull()) {
    if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(fontShadowColor).hasMatch()) {
      this->fontShadowColor = QColor(fontShadowColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(5, 2).toInt(Q_NULLPTR, 16));
    } else if(QRegularExpression("^#[0-9a-fA-F]{8}$").match(fontShadowColor).hasMatch()) {
      this->fontShadowColor = QColor(fontShadowColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(5, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(7, 2).toInt(Q_NULLPTR, 16));
    }
  }
  if(!textY.isNull()) {
    this->textY = textY.toInt();
  }
  if(!waveHeight.isNull() &&
     waveHeight.toInt() > 0 &&
     waveHeight.toInt() <= 20) {
    this->waveHeight = waveHeight.toInt();
  }
  if(!waveLength.isNull() &&
     waveLength.toInt() > 0 &&
     waveLength.toInt() <= 200) {
    this->waveLength = 2.0 / waveLength.toDouble();
  }
  if(!interval.isNull() &&
     interval.toInt() >= 10 &&
     interval.toInt() <= 86400) { // More than 10 seconds and less than 24 hours
    runInterval = interval.toInt() * 1000;
  }
  frameTimer.setInterval(1000 / (fps.toInt() <= 0 || fps.toInt() > 120?30:fps.toInt()));

  runCommandTimer.setInterval(runInterval);
  runCommandTimer.setSingleShot(true);
  connect(&runCommandTimer, &QTimer::timeout, this, &RunCommand::runCommand);

  settings.commandQueue->addEntry(command);
}

void RunCommand::start()
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
                              200);
  }
  currentX = 64;

  if(commandResult.isEmpty()) {
    commandResult = "Command returned no output.";
  }
  nextFrame();
}

void RunCommand::nextFrame()
{
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    buffer.fill(bgColor);
  }

  if(waveHeight > 0) {
    wavePhase += waveLength;
    if(wavePhase > 2.0) {
      wavePhase = 0.0;
    }
  }

  QRect textRect = drawText(currentX, textY - (settings.fonts[font].getHeight() / 2) + (sin(wavePhase * 3.14) * waveHeight), font, commandResult, fgColor, fontShadowColor);

  if(currentX < -1 * textRect.width()) {
    running = false;
    emit sceneEnded();
    return;
  }
  currentX--;
  frameTimer.start();
}

void RunCommand::runCommand()
{
  settings.commandQueue->addEntry(command);
}

void RunCommand::checkResult(const QString command, const QString result)
{
  if(this->command == command) {
    commandResult = result;
    runCommandTimer.start();
  }
}

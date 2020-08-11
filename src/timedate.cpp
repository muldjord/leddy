/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            timedate.h
 *
 *  Sun Aug 2 12:00:00 CEST 2020
 *  Copyright 2020 Lars Muldjord
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

#include "timedate.h"
#include "globaldefs.h"

#include <QTime>
#include <QDateTime>
#include <QPainter>
#include <QRegularExpression>

TimeDate::TimeDate(Settings &settings,
                   const QString &duration,
                   const QString &background,
                   const QString &bgColor,
                   const QString &fontColor,
                   const QString &timeFont,
                   const QString &timeFormat,
                   const QString &timeX,
                   const QString &timeY,
                   const QString &timeSpacing,
                   const QString &dateFont,
                   const QString &dateFormat,
                   const QString &dateX,
                   const QString &dateY,
                   const QString &dateSpacing)
: Scene(settings, SCENE::TIMEDATE, duration, background, bgColor, fontColor)
{
  if(!timeFont.isNull() && settings.fonts.contains(timeFont)) {
    this->timeFont = timeFont;
  }
  if(!timeFormat.isNull()) {
    this->timeFormat = timeFormat;
  }
  if(!timeX.isNull()) {
    this->timeX = timeX.toInt();
  }
  if(!timeY.isNull()) {
    this->timeY = timeY.toInt();
  }
  if(!timeSpacing.isNull()) {
    this->timeSpacing.clear();
    for(const auto &spacing: timeSpacing.simplified().split(",")) {
      this->timeSpacing.append(spacing.toInt());
    }
  }
  if(!dateFont.isNull() && settings.fonts.contains(dateFont)) {
    this->dateFont = dateFont;
  }
  if(!dateFormat.isNull()) {
    this->dateFormat = dateFormat;
  }
  if(!dateX.isNull()) {
    this->dateX = dateX.toInt();
  }
  if(!dateY.isNull()) {
    this->dateY = dateY.toInt();
  }
  if(!dateSpacing.isNull()) {
    this->dateSpacing.clear();
    for(const auto &spacing: dateSpacing.simplified().split(",")) {
      this->dateSpacing.append(spacing.toInt());
    }
  }
}

void TimeDate::start()
{
  frameTimer.setInterval(60000);
  nextFrame();
}

void TimeDate::nextFrame()
{
  // Change the bg and fg color each minute

  if(bgColorType == COLOR::RANDOM) {
    bgColor.setHsl(qrand() % 256,
                   (qrand() % 100) + 156,
                   50);
  }
  if(fgColorType == COLOR::RANDOM) {
    fgColor.setHsl(qrand() % 256,
                   (qrand() % 100) + 156,
                   200);
  } else if(fgColorType == COLOR::COMPLIMENTARY) {
    fgColor.setHsl(bgColor.hslHue() + 127,
                   bgColor.hslSaturation(),
                   200);
  }
  
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    buffer.fill(bgColor);
  }
  drawText(timeX, timeY,
           timeFont, QTime::currentTime().toString(timeFormat), fgColor, timeSpacing);
  drawText(dateX, dateY,
           dateFont, QDate::currentDate().toString(dateFormat), fgColor, dateSpacing);

  frameTimer.start();
}

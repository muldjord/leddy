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
#include <QRandomGenerator>

TimeDate::TimeDate(Settings &settings,
                   const QString &duration,
                   const QString &background,
                   const QString &bgColor,
                   const QString &fontColor,
                   const QString &fontShadowColor,
                   const QString &timeFont,
                   const QString &timeFormat,
                   const QString &timeX,
                   const QString &timeY,
                   const QString &timeSpacing,
                   const QString &timeAlign,
                   const QString &dateFont,
                   const QString &dateFormat,
                   const QString &dateX,
                   const QString &dateY,
                   const QString &dateSpacing,
                   const QString &dateAlign)
: Scene(settings, SCENE::TIMEDATE, duration, background, bgColor, fontColor)
{
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
  if(!timeAlign.isNull()) {
    // Left is default
    if(timeAlign == "center") {
      this->timeAlign = VALIGN::CENTER;
    } else if(timeAlign == "right") {
      this->timeAlign = VALIGN::RIGHT;
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
  if(!dateAlign.isNull()) {
    // Left is default
    if(dateAlign == "center") {
      this->dateAlign = VALIGN::CENTER;
    } else if(dateAlign == "right") {
      this->dateAlign = VALIGN::RIGHT;
    }
  }
}

void TimeDate::start()
{
  frameTimer.setInterval(timeFormat.contains("ss")?1000:60000);
  nextFrame();
}

void TimeDate::nextFrame()
{
  // Change the bg and fg color each minute

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
  
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    buffer.fill(bgColor);
  }
  drawText(timeX, timeY, timeFont, QTime::currentTime().toString(timeFormat), fgColor, fontShadowColor, timeAlign, timeSpacing);
  drawText(dateX, dateY, dateFont, QDate::currentDate().toString(dateFormat), fgColor, fontShadowColor, dateAlign, dateSpacing);

  frameTimer.start();
}

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
                   const QString &timeFont,
                   const QString &timeColor,
                   const QString &timeFormat,
                   const QString &timeX,
                   const QString &timeY,
                   const QString &timeSpacing,
                   const QString &dateFont,
                   const QString &dateColor,
                   const QString &dateFormat,
                   const QString &dateX,
                   const QString &dateY,
                   const QString &dateSpacing)
: Scene(settings, SCENE::TIMEDATE, duration, background)
{
  if(!timeFont.isNull() && settings.fonts.contains(timeFont)) {
    this->timeFont = timeFont;
  }
  if(!timeColor.isNull() &&
     QRegularExpression("^#[0-9a-fA-F]{6}$").match(timeColor).hasMatch()) {
    this->timeColor = QColor(timeColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                             timeColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                             timeColor.mid(5, 2).toInt(Q_NULLPTR, 16));
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
    for(const auto &spacing: timeSpacing.simplified().split(",")) {
      this->timeSpacing.append(spacing.toInt());
    }
  }
  if(!dateFont.isNull() && settings.fonts.contains(dateFont)) {
    this->dateFont = dateFont;
  }
  if(!dateColor.isNull() &&
     QRegularExpression("^#[0-9a-fA-F]{6}$").match(dateColor).hasMatch()) {
    this->dateColor = QColor(dateColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                             dateColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                             dateColor.mid(5, 2).toInt(Q_NULLPTR, 16));
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

  if(randBgColor) {
    bgColor = QColor(qrand() % 100,
                     qrand() % 100,
                     qrand() % 100);
  }
  
  if(background.isNull()) {
    buffer.fill(bgColor);
  } else {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  }
  drawText(timeX, timeY,
           timeFont, QTime::currentTime().toString(timeFormat), timeColor, timeSpacing);
  drawText(dateX, dateY,
           dateFont, QDate::currentDate().toString(dateFormat), dateColor, dateSpacing);

  frameTimer.start();
}

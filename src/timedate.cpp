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

TimeDate::TimeDate(Settings &settings, const QImage &background, const int &duration)
  : Scene(settings, SCENE::TIMEDATE, duration), background(background)
{
}

void TimeDate::start()
{
  frameTimer.setInterval(60000);
  nextFrame();
}

void TimeDate::nextFrame()
{
  bgColor = QColor(qrand() % 127, qrand() % 127, qrand() % 127);
  if(background.isNull()) {
    buffer.fill(bgColor);
  } else {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  }
  QColor fgColor = QColor((qrand() % 127) + 127, (qrand() % 127) + 127, (qrand() % 127) + 127);
  QString timeStr = QTime::currentTime().toString("HHmm");
  drawText(0, 2, "tiny", timeStr.left(2), fgColor, 0);
  drawText(7, 2, "tiny", ":", fgColor, 0);
  drawText(9, 2, "tiny", timeStr.right(2), fgColor, 0);
  QString dateStr = QDate(QDate::currentDate()).toString("ddMM");
  drawText(1, 9, "tiny", dateStr.left(2), fgColor, 0);
  drawText(8, 9, "tiny", ".", fgColor, 0);
  drawText(10, 9, "tiny", dateStr.right(2), fgColor, 0);

  frameTimer.start();
}

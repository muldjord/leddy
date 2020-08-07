/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            weather.h
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

#include "weather.h"
#include "globaldefs.h"

#include <QTime>
#include <QPainter>

Weather::Weather(Settings &settings) : Scene(settings, SCENE::WEATHER)
{
}

void Weather::start()
{
  frameTimer.setInterval(60000);
  nextFrame();
}

void Weather::nextFrame()
{
  QPainter painter;
  painter.begin(&buffer);
  painter.drawImage(0, 0, QImage(":" + settings.weatherType + ".png"));
  painter.end();

  QColor tempColor(Qt::white);
  if(settings.temperature < 0) {
    tempColor = QColor(0, 0, 255);
  } else if(settings.temperature < 5) {
    tempColor = QColor(0, 210, 255);
  } else if(settings.temperature < 10) {
    tempColor = QColor(0, 255, 204);
  } else if(settings.temperature < 15) {
    tempColor = QColor(0, 255, 145);
  } else if(settings.temperature < 20) {
    tempColor = QColor(0, 255, 69);
  } else if(settings.temperature < 25) {
    tempColor = QColor(143, 255, 0);
  } else if(settings.temperature < 30) {
    tempColor = QColor(255, 248, 0);
  } else if(settings.temperature < 35) {
    tempColor = QColor(255, 159, 0);
  } else if(settings.temperature < 40) {
    tempColor = QColor(255, 65, 0);
  }
  drawText(1, 9, "small", QString::number((int)settings.temperature) + "C", tempColor, 1);

  frameTimer.start();
}

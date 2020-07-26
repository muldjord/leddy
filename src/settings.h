/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            settings.h
 *
 *  Fri Jul 24 12:00:00 CEST 2020
 *  Copyright 2020 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of leddy.
 *
 *  leddy is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  leddy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with leddy; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QList>

struct Settings {
  // LEDs
  int rotation = 180; // 0-360 degrees
  int brightness = 50; // 0-100
  QByteArray device = "/dev/spidev0.0";
  
  // Weather
  bool forceWeatherType = false;
  QString weatherType = "11d";
  bool forceTemperature = false;
  double temperature = -42;
  bool forceWindDirection = false;
  QString windDirection = "N";
  bool forceWindSpeed = false;
  double windSpeed = 0.0;
  QString city = "Copenhagen";
  QString key = "fe9fe6cf47c03d2640d5063fbfa053a2";

  // RSS rss
  QList<QString> rssLines;
  QString rssUrl = "http://rss.slashdot.org/Slashdot/slashdotMain";
};

#endif // SETTINGS_H

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            settings.h
 *
 *  Fri Jul 24 12:00:00 CEST 2020
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

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "pixelfont.h"

#include <QString>
#include <QList>

struct Settings {
  // General
  bool clear = false;

  // Data paths
  QString fontPath = "data/fonts";
  QString animationPath = "data/animations";
  QString transitionPath = "data/transitions";

  // Fonts
  QMap<QString, PixelFont> fonts;
  
  // SPI / Unicorn Hat HD
  int framerate = 50;
  int rotation = 180; // 0-360 degrees
  int brightness = 50; // 0-100
  QByteArray device = "/dev/spidev0.0";
  uint32_t speed = 9000000;
  uint8_t mode = 0;
  uint8_t bits = 8;
  
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

#endif // _SETTINGS_H

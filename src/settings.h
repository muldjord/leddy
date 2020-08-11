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
#include "backgrounds.h"

#include <QString>
#include <QList>

struct Settings {
  // General
  bool clear = false;
  QString themePath = "themes/default";
  // Data paths
  QString fontPath = "fonts";
  QString animationPath = "animations";
  QString transitionPath = "transitions";
  QString backgroundPath = "backgrounds";
  QString weatherIconPath = "weather";

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
  QMap<QString, QImage> icons;

  // Backgrounds
  Backgrounds backgrounds;
};

#endif // _SETTINGS_H

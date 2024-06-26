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
#include "commandqueue.h"

#include <QSharedPointer>
#include <QString>
#include <QList>

struct Settings {
  // General
  bool clear = false; // If set to true it will clear the matrix and exit
  QString themePath = "themes/default";
  QString themeXmlFile = "theme.xml";

  // Data paths
  QString fontPath = "fonts";
  QString animationPath = "animations";
  QString transitionPath = "transitions";
  QString backgroundPath = "backgrounds";
  QString weatherIconPath = "weather";

  // Fonts
  QMap<QString, PixelFont> fonts;
  
  // General matrix settings
  int width = 16;
  int height = 16;
  int framerate = 50;
  int rotation = 180; // 0-360 degrees
  int brightness = 50; // 0-100
  
  // Unicorn Hat HD / Ubercorn specific settings
  QByteArray uniSpiDevice = "/dev/spidev0.0";
  uint32_t uniSpiSpeed = 9000000;
  uint8_t uniSpiMode = 0;
  uint8_t uniSpiBits = 8;

  // Adafruit Matrix Bonnet specific settings
  int adaChainLength = 1;
  int adaParallel = 1;
  bool adaShowRefreshRate = true;
  int adaGpioSlowdown = 4;
  
  // Weather
  QMap<QString, QImage> icons;

  // Backgrounds
  Backgrounds backgrounds;

  // A queue of 'runcommand' commands that are polled and handled in a separate thread
  QSharedPointer<CommandQueue> commandQueue;
};

#endif // _SETTINGS_H

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            leddy.cpp
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

#include "leddy.h"
#include "netcomm.h"
#include "loader.h"

#include <QImage>
#include <QSettings>

QMap<QChar, QImage> pfont;

Leddy::Leddy(const QCommandLineParser &parser)
{
  qsrand((uint)QTime::currentTime().msec());

  QSettings iniSettings("config.ini", QSettings::IniFormat);

  if(!iniSettings.contains("feed_url")) {
    iniSettings.setValue("feed_url", "http://rss.slashdot.org/Slashdot/slashdotMain");
  }
  settings.feedUrl = iniSettings.value("feed_url").toString();

  if(!iniSettings.contains("weather_city")) {
    iniSettings.setValue("weather_city", "Copenhagen");
  }
  settings.city = iniSettings.value("weather_city").toString();

  if(!iniSettings.contains("weather_key")) {
    iniSettings.setValue("weather_key", "fe9fe6cf47c03d2640d5063fbfa053a2");
  }
  settings.key = iniSettings.value("weather_key").toString();

  if(Loader::loadFont(pfont)) {
    qInfo("Font loaded ok... :)\n");
  } else {
    qInfo("Error when loading font...\n");
  }

  connect(&eventTimer, &QTimer::timeout, this, &Leddy::nextEvent);
  eventTimer.setInterval(10000);
  eventTimer.setSingleShot(true);

  netComm = new NetComm(&settings);
}

Leddy::~Leddy()
{
}

void Leddy::run()
{
  QByteArray devNode = "/dev/spidev0.0";
  spiDev = new SPIConn(devNode, 9000000, 0, 8, 50, 180); // dev, speed, mode, bits, brightness, rotation
  if(spiDev->init()) {
    eventTimer.start();
  } else {
    printf("ERROR: Couldn't init SPI device '%s'\n", devNode.data());
    emit finished();
  }
}

void Leddy::nextEvent()
{
  printf("Time for next event!\n");
  switch(eventIdx) {
  case 0:
    spiDev->clear();
    spiDev->drawText(0, 0, QTime::currentTime().toString("HH:mm"), -1);
    spiDev->drawText(0, 7, QString::number((int)settings.temperature) + "C");
    spiDev->refresh();
    break;
  case 1:
    spiDev->clear();
    spiDev->drawText(0, 0, QTime::currentTime().toString(""));
    spiDev->refresh();
    break;
  default:
    break;
  }
  eventIdx++;
  if(eventIdx > 0) {
    eventIdx = 0;
  }
  eventTimer.setInterval((qrand() % 1000) + 50);
  eventTimer.start();
}

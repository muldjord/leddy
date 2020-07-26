/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            leddy.cpp
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

  if(!iniSettings.contains("rss/url")) {
    iniSettings.setValue("rss/url", "http://rss.slashdot.org/Slashdot/slashdotMain");
  }
  settings.rssUrl = iniSettings.value("rss/url").toString();

  if(!iniSettings.contains("weather/city")) {
    iniSettings.setValue("weather/city", "Copenhagen");
  }
  settings.city = iniSettings.value("weather/city").toString();

  if(!iniSettings.contains("weather/key")) {
    iniSettings.setValue("weather/key", "fe9fe6cf47c03d2640d5063fbfa053a2");
  }
  settings.key = iniSettings.value("weather/key").toString();

  if(!iniSettings.contains("unicorn_hd/device")) {
    iniSettings.setValue("unicorn_hd/device", "/dev/spidev0.0");
  }
  settings.device = iniSettings.value("unicorn_hd/device").toByteArray();

  if(!iniSettings.contains("unicorn_hd/rotation")) {
    iniSettings.setValue("unicorn_hd/rotation", 180);
  }
  settings.rotation = iniSettings.value("unicorn_hd/rotation").toInt();

  if(!iniSettings.contains("unicorn_hd/brightness")) {
    iniSettings.setValue("unicorn_hd/brightness", 50);
  }
  settings.brightness = iniSettings.value("unicorn_hd/brightness").toInt();

  if(Loader::loadFont(pfont)) {
    qInfo("Font loaded ok... :)\n");
  } else {
    qInfo("Error when loading font...\n");
  }

  connect(&eventTimer, &QTimer::timeout, this, &Leddy::nextEvent);
  eventTimer.setInterval(100);
  eventTimer.setSingleShot(true);

  netComm = new NetComm(&settings);
}

Leddy::~Leddy()
{
}

void Leddy::run()
{
  spiDev = new SPIConn(settings.device,
                       9000000,
                       0,
                       8,
                       settings.brightness,
                       settings.rotation); // dev, speed, mode, bits, brightness, rotation
  if(spiDev->init()) {
    eventTimer.start();
  } else {
    printf("ERROR: Couldn't init SPI device '%s'\n", settings.device.data());
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

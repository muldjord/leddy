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

  if(!iniSettings.contains("unicorn_hd/rotation")) {
    iniSettings.setValue("unicorn_hd/rotation", 180);
  }
  if(parser.isSet("r") && !parser.value("r").isEmpty()) {
    settings.rotation = parser.value("r").toInt();
  } else {
    settings.rotation = iniSettings.value("unicorn_hd/rotation").toInt();
  }

  if(!iniSettings.contains("unicorn_hd/brightness")) {
    iniSettings.setValue("unicorn_hd/brightness", 50);
  }
  if(parser.isSet("B") && !parser.value("B").isEmpty()) {
    settings.brightness = parser.value("B").toInt();
  } else {
    settings.brightness = iniSettings.value("unicorn_hd/brightness").toInt();
  }

  if(!iniSettings.contains("spi/device")) {
    iniSettings.setValue("spi/device", "/dev/spidev0.0");
  }
  if(parser.isSet("d") && !parser.value("d").isEmpty()) {
    settings.device = parser.value("d").toUtf8();
  } else {
    settings.device = iniSettings.value("spi/device").toByteArray();
  }

  if(!iniSettings.contains("spi/speed")) {
    iniSettings.setValue("spi/speed", 9000);
  }
  if(parser.isSet("s") && !parser.value("s").isEmpty()) {
    settings.speed = parser.value("s").toInt() * 1000;
  } else {
    settings.speed = iniSettings.value("spi/speed").toInt() * 1000;
  }

  if(!iniSettings.contains("spi/bits_per_word")) {
    iniSettings.setValue("spi/bits_per_word", 8);
  }
  if(parser.isSet("b") && !parser.value("b").isEmpty()) {
    settings.bits = parser.value("b").toInt();
  } else {
    settings.bits = iniSettings.value("spi/bits_per_word").toInt();
  }

  if(!iniSettings.contains("spi/mode")) {
    iniSettings.setValue("spi/mode", 0);
  }
  if(parser.isSet("m") && !parser.value("m").isEmpty()) {
    settings.mode = parser.value("m").toInt();
  } else {
    settings.mode = iniSettings.value("spi/mode").toInt();
  }

  if(parser.isSet("clear")) {
    settings.clear = true;
  }

  if(!Loader::loadFont(pfont)) {
    printf("ERROR: Couldn't load font!\n");
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
                       settings.speed,
                       settings.mode,
                       settings.bits,
                       settings.brightness,
                       settings.rotation); // dev, speed, mode, bits, brightness, rotation
  if(spiDev->init()) {
    if(settings.clear) {
      spiDev->clear();
      spiDev->refresh();
      emit finished();
    }
    eventTimer.start();
  } else {
    emit finished();
  }
}

void Leddy::nextEvent()
{
  printf("Time for next event!\n");
  if(eventIdx == 0) {
    spiDev->clear();
    spiDev->drawText(0, 2, QTime::currentTime().toString("HH:mm"), -1, QColor(Qt::white));
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
    spiDev->drawText(0, 8, QString::number((int)settings.temperature) + "C", 0, tempColor);
    spiDev->refresh();
    eventTimer.setInterval(10000);
  } else if(eventIdx == 1) {
    spiDev->clear();
    spiDev->setFromImage(QImage(":" + settings.weatherType + ".png"));
    spiDev->refresh();
    eventTimer.setInterval(10000);
  }
  eventIdx++;
  if(eventIdx > 1) {
    eventIdx = 0;
  }
  eventTimer.start();
}

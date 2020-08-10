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
#include "netcomm.h"

#include <QTime>
#include <QPainter>
#include <QDomDocument>

extern NetComm *netComm;

Weather::Weather(Settings &settings,
                 const QString &duration,
                 const QString &city,
                 const QString &key)
  : Scene(settings, SCENE::WEATHER, duration)
{
  if(!city.isNull()) {
    this->weatherCity = city;
  }
  if(!key.isNull()) {
    this->weatherKey = city;
  }
  weatherTimer.setInterval(60 * 30 * 1000); // Every half hour
  weatherTimer.setSingleShot(true);
  connect(&weatherTimer, &QTimer::timeout, this, &Weather::weatherUpdate);
  weatherUpdate();
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
  painter.drawImage(0, 0, settings.icons[weatherType]);
  painter.end();

  QColor tempColor(0, 40, 255);
  int newHue = tempColor.hsvHue() - ((150.0 / 30.0) * (temperature + 10));
  if(newHue < 0) {
    newHue = 0;
  } else if(newHue > 255) {
    newHue = 255;
  }
  tempColor.setHsv(newHue, 255, 255);
  drawText(1, 9, "small", QString::number((int)temperature) + "C",
           tempColor, QList<int>({1}));

  frameTimer.start();
}

void Weather::weatherUpdate()
{
  weatherReply = netComm->get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/weather?q=" + weatherCity + "&mode=xml&units=metric&appid=" + weatherKey)));
  connect(weatherReply, &QNetworkReply::finished, this, &Weather::weatherReady);
}

void Weather::weatherReady()
{
  printf("Weather updated for city '%s' (OpenWeatherMap):\n", weatherCity.toStdString().c_str());
  QDomDocument doc;
  doc.setContent(weatherReply->readAll());

  weatherType = doc.elementsByTagName("weather").at(0).toElement().attribute("icon");
  windSpeed = doc.elementsByTagName("speed").at(0).toElement().attribute("value").toDouble();
  windDirection = doc.elementsByTagName("direction").at(0).toElement().attribute("code");
  temperature = doc.elementsByTagName("temperature").at(0).toElement().attribute("value").toDouble();

  if(weatherType.isEmpty()) {
    weatherType = "11d";
  }
  if(temperature == 0.0) {
    temperature = -42;
  }
  if(windDirection.isEmpty()) {
    windDirection = "E";
  }
    
  //printf("%s\n", rawData.data());
  printf("  Icon: %s\n", weatherType.toStdString().c_str());
  printf("  Temp: %f\n", temperature);
  printf("  Wind: %f m/s from %s\n", windSpeed, windDirection.toStdString().c_str());

  weatherReply->deleteLater();
  weatherTimer.start();
}

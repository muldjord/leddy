/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            netcomm.cpp
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

#include "netcomm.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QFile>
#include <QDomDocument>

NetComm::NetComm(Settings &settings) : settings(settings)
{
  //connect(this, &NetComm::finished, this, &NetComm::netReply);

  weatherTimer.setInterval(1000 * 60 * 10); // 10 minutes between updates
  weatherTimer.setSingleShot(true);
  connect(&weatherTimer, &QTimer::timeout, this, &NetComm::weatherUpdate);

  weatherUpdate();
}

NetComm::~NetComm()
{
}

void NetComm::weatherUpdate()
{
  weatherReply = get(QNetworkRequest(QUrl("http://api.openweathermap.org/data/2.5/weather?q=" + settings.city + "&mode=xml&units=metric&appid=" + settings.key)));
  connect(weatherReply, &QNetworkReply::finished, this, &NetComm::weatherReady);
}
   
void NetComm::weatherReady()
{
  QDomDocument doc;
  doc.setContent(weatherReply->readAll());
  weatherReply->close();
  printf("Weather updated from city '%s' (openweathermap.org):\n", settings.city.toStdString().c_str());
  if(!settings.forceWeatherType) {
    settings.weatherType = doc.elementsByTagName("weather").at(0).toElement().attribute("icon");
  }
  if(!settings.forceWindSpeed) {
    settings.windSpeed = doc.elementsByTagName("speed").at(0).toElement().attribute("value").toDouble();
  }
  if(!settings.forceWindDirection) {
    settings.windDirection = doc.elementsByTagName("direction").at(0).toElement().attribute("code");
  }
  if(!settings.forceTemperature) {
    settings.temperature = doc.elementsByTagName("temperature").at(0).toElement().attribute("value").toDouble();
  }

  if(settings.weatherType.isEmpty()) {
    settings.weatherType = "11d";
  }
  if(settings.temperature == 0.0) {
    settings.temperature = -42;
  }
  if(settings.windDirection.isEmpty()) {
    settings.windDirection = "E";
  }
    
  //printf("%s\n", rawData.data());
  printf("  Icon: %s\n", settings.weatherType.toStdString().c_str());
  printf("  Temp: %f\n", settings.temperature);
  printf("  Wind: %f m/s from %s\n", settings.windSpeed, settings.windDirection.toStdString().c_str());
  weatherReply->deleteLater();
  weatherTimer.start();
}

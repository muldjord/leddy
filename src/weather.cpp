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
#include <QRegularExpression>

extern NetComm *netComm;

Weather::Weather(Settings &settings,
                 const QString &duration,
                 const QString &background,
                 const QString &bgColor,
                 const QString &city,
                 const QString &key,
                 const QString &cityFont,
                 const QString &cityColor,
                 const QString &cityX,
                 const QString &cityY,
                 const QString &citySpacing,
                 const QString &tempFont,
                 const QString &tempColor,
                 const QString &tempX,
                 const QString &tempY,
                 const QString &tempSpacing)
: Scene(settings, SCENE::WEATHER, duration, background, bgColor, cityColor)
{
  if(!city.isNull()) {
    this->weatherCity = city;
  }
  if(!key.isNull()) {
    this->weatherKey = key;
  }
  if(!cityFont.isNull() && settings.fonts.contains(cityFont)) {
    this->cityFont = cityFont;
  }
  // cityColor is handled with fgColor
  if(!cityX.isNull()) {
    this->cityX = cityX.toInt();
  }
  if(!cityY.isNull()) {
    this->cityY = cityY.toInt();
  }
  if(!citySpacing.isNull()) {
    this->citySpacing.clear();
    for(const auto &spacing: citySpacing.simplified().split(",")) {
      this->citySpacing.append(spacing.toInt());
    }
  }
  if(!tempFont.isNull() && settings.fonts.contains(tempFont)) {
    this->tempFont = tempFont;
  }
  if(!tempColor.isNull()) {
    if(tempColor == "random") {
      tempColorType = COLOR::RANDOM;
    } else if(tempColor == "complimentary") {
      tempColorType = COLOR::COMPLIMENTARY;
    } else if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(tempColor).hasMatch()) {
      tempColorType = COLOR::STATIC;
      this->tempColor = QColor(tempColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                               tempColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                               tempColor.mid(5, 2).toInt(Q_NULLPTR, 16));
    }
  }
  if(!tempX.isNull()) {
    this->tempX = tempX.toInt();
  }
  if(!tempY.isNull()) {
    this->tempY = tempY.toInt();
  }
  if(!tempSpacing.isNull()) {
    this->tempSpacing.clear();
    for(const auto &spacing: tempSpacing.simplified().split(",")) {
      this->tempSpacing.append(spacing.toInt());
    }
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
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else if(bgColorType != COLOR::UNSET) {
    if(bgColorType == COLOR::RANDOM) {
      bgColor.setHsl(qrand() % 256,
                     (qrand() % 100) + 156,
                     50);
    }
    buffer.fill(bgColor);
  } else {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, settings.icons[weatherType]);
    painter.end();
  }

  QColor heatColor(0, 40, 255);
  int newHue = heatColor.hsvHue() - ((150.0 / 30.0) * (temperature + 10));
  if(newHue < 0) {
    newHue = 0;
  } else if(newHue > 255) {
    newHue = 255;
  }
  heatColor.setHsv(newHue, 255, 255);

  if(fgColorType == COLOR::UNSET) {
    fgColor = heatColor;
  } else if(fgColorType == COLOR::RANDOM) {
    fgColor.setHsl(qrand() % 256,
                   (qrand() % 100) + 156,
                   200);
  } else if(fgColorType == COLOR::COMPLIMENTARY) {
    fgColor.setHsl(bgColor.hslHue() + 127,
                   bgColor.hslSaturation(),
                   200);
  }
  if(tempColorType == COLOR::UNSET) {
    tempColor = heatColor;
  } else if(tempColorType == COLOR::RANDOM) {
    tempColor.setHsl(qrand() % 256,
                   (qrand() % 100) + 156,
                   200);
  } else if(tempColorType == COLOR::COMPLIMENTARY) {
    tempColor.setHsl(bgColor.hslHue() + 127,
                   bgColor.hslSaturation(),
                   200);
  }

  drawText(cityX, cityY, cityFont, weatherCity,
           fgColor, citySpacing);
  drawText(tempX, tempY, tempFont, QString::number((int)temperature) + "C",
           tempColor, tempSpacing);

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

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

#ifndef _WEATHER_H
#define _WEATHER_H

#include "scene.h"
#include "settings.h"

#include <QNetworkReply>

class Weather : public Scene
{
  Q_OBJECT

public:
  Weather(Settings &settings,
          const QString &duration = QString(),
          const QString &city = QString(),
          const QString &key = QString(),
          const QString &cityFont = QString(),
          const QString &cityColor = QString(),
          const QString &cityX = QString(),
          const QString &cityY = QString(),
          const QString &citySpacing = QString(),
          const QString &tempFont = QString(),
          const QString &tempColor = QString(),
          const QString &tempX = QString(),
          const QString &tempY = QString(),
          const QString &tempSpacing = QString());
  void start() override;
                             
public slots:
  void nextFrame() override;

private slots:
  void weatherUpdate();
  void weatherReady();

private:
  QNetworkReply *weatherReply = nullptr;
  QTimer weatherTimer;

  QString font = "medium";

  QString cityFont = "tiny";
  QColor cityColor = QColor();
  int cityX = 0;
  int cityY = 2;
  QList<int> citySpacing = QList<int>({0});
  QString tempFont = "small";
  QColor tempColor = QColor();
  int tempX = 0;
  int tempY = 9;
  QList<int> tempSpacing = QList<int>({1});

  QString weatherCity = "Copenhagen";
  QString weatherKey = "fe9fe6cf47c03d2640d5063fbfa053a2";
  QString weatherType = "11d";
  double temperature = 0.0;
  double windSpeed = 0.0;
  QString windDirection = "E";
  
};

#endif // _WEATHER_H

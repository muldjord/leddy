/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            timedate.h
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

#ifndef _TIMEDATE_H
#define _TIMEDATE_H

#include "scene.h"
#include "settings.h"

class TimeDate : public Scene
{
  Q_OBJECT

public:
  TimeDate(Settings &settings,
           const QString &duration = QString(),
           const QString &background = QString(),
           const QString &bgColor = QString(),
           const QString &fontColor = QString(),
           const QString &fontShadowColor = QString(),
           const QString &timeFont = QString(),
           const QString &timeFormat = QString(),
           const QString &timeX = QString(),
           const QString &timeY = QString(),
           const QString &timeSpacing = QString(),
           const QString &timeAlign = QString(),
           const QString &dateFont = QString(),
           const QString &dateFormat = QString(),
           const QString &dateX = QString(),
           const QString &dateY = QString(),
           const QString &dateSpacing = QString(),
           const QString &dateAlign = QString());
  void start() override;

public slots:
  void nextFrame() override;

private:
  QColor fontShadowColor = QColor(0, 0, 0, 0);
  QString timeFont = "tiny";
  QString timeFormat = "HH:mm";
  int timeX = 0;
  int timeY = 9;
  QList<int> timeSpacing = QList<int>({0, 1, 1, 0});
  int timeAlign = VALIGN::LEFT;
  QString dateFont = "tiny";
  QString dateFormat = "ddMMM";
  int dateX = 0;
  int dateY = 2;
  QList<int> dateSpacing = QList<int>({0, 1, 0});
  int dateAlign = VALIGN::LEFT;
};

#endif // _TIMEDATE_H

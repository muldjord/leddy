/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            snowfall.h
 *
 *  Sat Dec 13 12:00:00 CEST 2025
 *  Copyright 2025 Lars Muldjord
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

#pragma once

#include "scene.h"
#include "settings.h"
#include "globaldefs.h"

#include <algorithm>

struct Snowflake {
  double x = 0.0;
  double y = 0.0;
};

class Snowfall : public Scene
{
  Q_OBJECT

public:
  Snowfall(Settings &settings,
           const QString &duration,
           const QString &background,
           const QString &bgColor,
           const QString &fps);
  void start() override;
                       
public slots:
  void nextFrame() override;

private:
  QImage ground = QImage(settings.width, settings.height, QImage::Format_ARGB32);
  QList<Snowflake> snowFlakes;
  bool flipper = false;
  int sineIdx = 0;
};

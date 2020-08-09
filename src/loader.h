/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            loader.h
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

#ifndef _LOADER_H
#define _LOADER_H

#include "pixelfont.h"
#include "animation.h"
#include "transition.h"

#include <QObject>
#include <QMap>
#include <QImage>

class Loader: public QObject
{
  Q_OBJECT;
  
public:
  static bool loadFonts(Settings &settings);
  static bool loadAnimations(Settings &settings, QMap<QString, Animation *> &animations);
  static bool loadTransitions(Settings &settings, QMap<QString, Transition *> &transitions);
  static bool loadBackgrounds(Settings &settings);
  static bool loadWeatherIcons(Settings &settings);
};

#endif // _LOADER_H

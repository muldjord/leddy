/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            backgrounds.cpp
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

#include "backgrounds.h"

#include <QRandomGenerator>

Backgrounds::Backgrounds()
{
}

Backgrounds::~Backgrounds()
{
}

bool Backgrounds::setBackground(const QString &name, const QImage &background)
{
  if(backgrounds.contains(name)) {
    return false;
  }
  backgrounds[name] = background;
  return true;
}

bool Backgrounds::contains(const QString &name)
{
  if(backgrounds.contains(name)) {
    return true;
  }
  return false;
}

QImage Backgrounds::getBackground(const QString &name)
{
  if(backgrounds.contains(name)) {
    return backgrounds[name];
  }
  if(name == "random") {
    int random = QRandomGenerator::global()->bounded(backgrounds.count());
    return backgrounds[backgrounds.keys().at(random)];
  }
  return QImage();
}

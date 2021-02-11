/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            gameoflife.h
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

#ifndef _GAMEOFLIFE_H
#define _GAMEOFLIFE_H

#include "scene.h"
#include "settings.h"

class GameOfLife : public Scene
{
  Q_OBJECT

public:
  GameOfLife(Settings &settings,
             const QString &duration = QString(),
             const QString &background = QString(),
             const QString &bgColor = QString(),
             const QString &fgColor = QString(),
             const QString &fps = QString());
  void start() override;

public slots:
  void nextFrame() override;

private:
  QImage prevGen = QImage(16, 16, QImage::Format_ARGB32);
  QImage nextGen = QImage(16, 16, QImage::Format_ARGB32);
};

#endif // _GAMEOFLIFE_H

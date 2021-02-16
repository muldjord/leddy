/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            runcommand.h
 *
 *  Mon Feb 15 12:00:00 CEST 2021
 *  Copyright 2021 Lars Muldjord
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

#ifndef _RUNCOMMAND_H
#define _RUNCOMMAND_H

#include "scene.h"
#include "settings.h"

#include <QTimer>

class RunCommand : public Scene
{
  Q_OBJECT

public:
  RunCommand(Settings &settings,
             const QString &background = QString(),
             const QString &bgColor = QString(),
             const QString &font = QString(),
             const QString &fontColor = QString(),
             const QString &waveHeight = QString(),
             const QString &waveLength = QString(),
             const QString &command = QString(),
             const QString &interval = QString(),
             const QString &fps = QString());
  void start() override;
                             
public slots:
  void nextFrame() override;

private slots:
  void runCommand();
  
private:
  int runInterval = 60000;
  QTimer runCommandTimer;
  QString command = QString();
  QString commandResult = QString();
  QString font = "medium";
  double wavePhase = 0.0;
  double waveHeight = -1;
  double waveLength = 0;
  int currentX = 17;

};

#endif // _RUNCOMMAND_H

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            leddy.h
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

#ifndef _LEDDY_H
#define _LEDDY_H

#include "uniconn.h"
#include "settings.h"
#include "netcomm.h"
#include "scene.h"

#include <QObject>
#include <QCommandLineParser>

class Leddy : public QObject
{
  Q_OBJECT

public:
  Leddy(const QCommandLineParser &parser);
  ~Leddy();
  void run();

private slots:
  void nextScene();

private:
  Scene *getNextScenePtr();

  QMap<QString, Scene> scenes;
  QList<QString> sceneRotation;

  Scene *scene;
  Scene *transition;

  NetComm *netComm;
  Settings settings;
  UniConn *uniConn;
};

#endif // _LEDDY_H

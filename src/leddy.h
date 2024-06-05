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

#include "matrixabstract.h"

#include "settings.h"
#include "netcomm.h"
#include "animation.h"
#include "transition.h"
#include "scenedesc.h"
#include "commandhandler.h"
#include "globaldefs.h"

#include <QObject>
#include <QThread>
#include <QCommandLineParser>
#include <QTime>
#include <QBasicTimer>

struct Action {
  QString time = "23:00";
  QString parameter = "brightness";
  int value = 50;
};

class Leddy : public QObject
{
  Q_OBJECT

public:
  Leddy(const QCommandLineParser &parser);
  ~Leddy();
  void run();

protected:
  void timerEvent(QTimerEvent *);

private slots:
  //void pushBuffer();
  void sceneChange();

private:
  void loadTheme();
  Scene *getTransition(const QString &name);
  Scene *getAnimation(const QString &name);
  void checkActions(const bool &init = false);
  Scene *getNextScene();
  void drawToBuffer();

  QMap<QString, Animation *> animations;
  QMap<QString, Transition *> transitions;

  QList<Action> actions;
  QString prevTime = QTime::currentTime().toString("HH:mm");
  
  QList<SceneDesc *> sceneRotation;
  int rotationIdx = -1; // Will be 0 when it's first used

  QImage prevBuffer = QImage(MATRIX::WIDTH, MATRIX::HEIGHT, QImage::Format_ARGB32);

  QTimer sceneTimer;
  QBasicTimer uniTimer;

  Scene *previousScene = nullptr;
  Scene *currentScene = nullptr;
  Scene *nextScene = nullptr;

  Settings settings;

  MatrixAbstract *matrix = nullptr;

  QThread workerThread;
  CommandHandler *commandHandler = nullptr;
};

#endif // _LEDDY_H
